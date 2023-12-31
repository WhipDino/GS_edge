# Bracelete Inteligente para Gerenciamento de Medicamentos

### O projeto wokwi está na pasta arduino e o video está na pasta video.

## Descrição do Problema

A adesão ao tratamento medicamentoso é um aspecto crítico da gestão da saúde, especialmente para indivíduos com condições crônicas que requerem medicação regular. O esquecimento de tomar medicamentos no horário correto ou a perda de doses pode levar a resultados de saúde subótimos e complicações adicionais. A necessidade de uma solução discreta e conveniente que possa auxiliar os usuários a manterem a regularidade de sua medicação é evidente.

## Visão Geral da Solução

Este projeto visa desenvolver um bracelete inteligente que funcione como um dispensador de medicamentos portátil e lembrete para o usuário. O bracelete contém compartimentos para armazenar doses individuais de medicamentos e é equipado com um mecanismo de alerta que notifica o usuário quando é hora de tomar o medicamento. Com esta solução, os usuários terão um método mais fácil e confiável para gerenciar sua medicação, melhorando a adesão ao tratamento e promovendo melhores resultados de saúde.

O bracelete será projetado para ser confortável e esteticamente agradável, incentivando o uso contínuo e discreto. A funcionalidade inteligente do bracelete é alcançada através da integração com tecnologia IoT, permitindo que ele se comunique com um aplicativo móvel para rastreamento e gerenciamento de adesão à medicação.

### Características Principais

- Compartimentos seguros e discretos para armazenamento de medicamentos.
- Alertas vibratórios e visuais para lembretes de medicação.
- Conectividade com aplicativo móvel para acompanhamento de adesão e ajustes de programação.
- Design ergonômico e personalizável para atender a diferentes preferências de usuários.

## Recursos

- Controle de dispensação de medicamentos via MQTT
- Integração com Node-RED para processamento de mensagens
- Notificações automáticas enviadas para o WhatsApp
- Simulação de hardware no Wokwi

## Componentes

- ESP32
- Servomotor
- Sensor Ultrassônico HC-SR04
- LEDs indicadores
- Buzzer
- LCD Display 16x2

## Configuração

### Requisitos de Hardware

- ESP32 Dev Module
- Servomotor compatível com ESP32
- Sensor Ultrassônico HC-SR04
- LEDs (Vermelho, Amarelo e Azul)
- Buzzer
- LCD Display 16x2
- Fonte de alimentação adequada para o ESP32 e o servomotor

### Configuração de Software

- Ambiente de Desenvolvimento: Arduino IDE ou PlatformIO
- Bibliotecas necessárias:
  - `ESP32Servo.h` para controle do servomotor
  - `LiquidCrystal.h` para o display LCD
  - `WiFi.h` para conexão Wi-Fi
  - `PubSubClient.h` para a comunicação MQTT

### Configuração do MQTT

- Broker MQTT: `broker.hivemq.com`
- Porta: `1883`
- Tópicos MQTT: `fiap/iot/dispenser` para controle do dispensador

### Configuração do Node-RED

- Instalar o Node-RED em um dispositivo que possa permanecer sempre ligado, como um Raspberry Pi ou um servidor.
- Instalar nodes de contribuição para MQTT e WhatsApp (por exemplo, `node-red-contrib-whatsappbot`).

## Uso

### Operação do Dispensador

- Inicie o ESP32 e certifique-se de que ele está conectado à rede Wi-Fi.
- O ESP32 irá se conectar automaticamente ao broker MQTT configurado.
- Use um cliente MQTT para enviar comandos para o tópico `fiap/iot/dispenser` para controlar o dispensador.

### Comandos MQTT

- Publicar `"dispensar"` para o tópico `fiap/iot/dispenser` irá ativar o servomotor e dispensar o medicamento.
- Publicar `"status"` para o tópico `fiap/iot/dispenser` solicitará uma atualização do status do dispensador.

### Integração com Node-RED e WhatsApp

- O Node-RED escuta mensagens do ESP32 via MQTT e processa os dados recebidos.
- As mensagens são formatadas e enviadas para um número específico de WhatsApp usando o serviço de mensagens configurado.
- Respostas e alertas podem ser recebidos no WhatsApp de acordo com as ações do dispensador.

### Monitoramento e Notificações

- O status do dispensador é monitorado continuamente.
- Notificações são enviadas automaticamente para o WhatsApp quando o medicamento é dispensado ou quando ocorre um erro.
