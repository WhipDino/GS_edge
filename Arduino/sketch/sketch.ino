#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <LiquidCrystal.h>

WiFiClient espClient;
PubSubClient MQTT(espClient);
Servo servoMedicamento;

void publishMessage(const char* topic, const char* message) {
  if (MQTT.connected()) {
    MQTT.publish(topic, message);
  }
}

const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";  

const char* BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char* ID_MQTT = "esp32_mqtt";
const char* TOPIC_DISPENSE = "fiap/iot/dispenser";

#define PIN_SERVO 18 
#define TRIG_PIN 17
#define ECHO_PIN 16
#define LED_BLUE_PIN 19
#define LED_YELLOW_PIN 21
#define LED_RED_PIN 22
#define BUZZER_PIN 23

LiquidCrystal lcd(14, 27, 26, 25, 33, 32);


const unsigned long DISPENSE_INTERVAL = 20000; 
const int DETECTION_DISTANCE = 10; 
unsigned long lastDispenseTime = 0; 

void initWiFi() {
    Serial.print("Conectando com a rede: ");
    Serial.println(SSID);
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("Conectado com sucesso: ");
    Serial.println(SSID);
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
    String msg = String((char *)payload).substring(0, length);
    Serial.print("Mensagem MQTT recebida: ");
    Serial.println(msg);

}

void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("Tentando conectar ao broker MQTT: ");
        Serial.println(BROKER_MQTT);

        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao broker MQTT!");
            MQTT.subscribe(TOPIC_DISPENSE);
        } else {
            Serial.println("Falha na conexão com o broker MQTT. Tentando novamente em 2 segundos.");
            delay(2000);
        }
    }
}

void checkWiFIAndMQTT() {
    if (WiFi.status() != WL_CONNECTED) {
        reconnectWiFi();
    }
    if (!MQTT.connected()) {
        reconnectMQTT();
    }
}

void reconnectWiFi() {
    if (WiFi.status() == WL_CONNECTED) return;

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("WiFi reconectado com sucesso, IP: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);
    initWiFi();
    initMQTT();

    servoMedicamento.attach(PIN_SERVO);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    servoMedicamento.attach(PIN_SERVO);
    servoMedicamento.write(0);

    lcd.begin(16, 2);
    lcd.print("Dispensador Med");
}

void loop() {
    checkWiFIAndMQTT();
    MQTT.loop();

    unsigned long currentTime = millis();
    unsigned long timeSinceLastDispense = currentTime - lastDispenseTime;

    int secondsLeft = max(0, (int)(DISPENSE_INTERVAL - timeSinceLastDispense) / 1000);
    lcd.setCursor(0, 1);
    lcd.print("Prox. dose: ");
    lcd.print(secondsLeft);
    lcd.print(" seg    "); 

    if (secondsLeft <= 10 && secondsLeft > 0) {
        digitalWrite(LED_BLUE_PIN, LOW); 
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(LED_YELLOW_PIN, (currentTime % 1000) > 500 ? HIGH : LOW); 
    } else if (secondsLeft == 0) {
        digitalWrite(LED_BLUE_PIN, LOW); 
        digitalWrite(LED_YELLOW_PIN, LOW); 
        digitalWrite(LED_RED_PIN, HIGH); 
        digitalWrite(BUZZER_PIN, HIGH); 
    } else {
        digitalWrite(LED_BLUE_PIN, HIGH); 
        digitalWrite(LED_YELLOW_PIN, LOW); 
        digitalWrite(BUZZER_PIN, LOW); 
        digitalWrite(LED_RED_PIN, LOW); 
    }

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    int distance = duration * 0.034 / 2;

    if (secondsLeft <= 0 && distance <= DETECTION_DISTANCE) {
        servoMedicamento.write(90); 
        delay(2000);       
        servoMedicamento.write(0);  

        publishMessage("fiap/iot/dispenser", "Medicamento dispensado");

        lastDispenseTime = currentTime;  
        digitalWrite(LED_RED_PIN, LOW);  
        digitalWrite(LED_BLUE_PIN, HIGH); 
    }
}
