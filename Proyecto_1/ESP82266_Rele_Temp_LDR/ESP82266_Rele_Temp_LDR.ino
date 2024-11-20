#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"

// Datos de tu red WiFi
#define WIFI_SSID "TP-Link_76E3"
#define WIFI_PASSWORD "13193348"

// Token de Telegram BOT obtenido desde Botfather
#define BOT_TOKEN "6075391597:AAFi28sadDJmqOrgvKGlbMnMK5hk8A1JFQY"
#define CHAT_ID "909954663" // ID_Chat se obtiene de telegram

const unsigned long tiempo = 1000;  // Intervalo entre chequeos de mensajes
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long tiempoAnterior = 0;

// Configuración de pines para los relés y sensor LDR
const int ledPins[] = {5, 4, 0, 2, 14, 12}; // Pines de los relés
const int ldrPin = 13;  // Pin del sensor LDR
int estadoLeds[6] = {0};  // Estado de los relés

// Configuración del sensor DHT
#define DHT_PIN 16  // GPIO16 para ESP8266
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    // Control de relés
    for (int j = 0; j < 6; j++) {
      if (text == "R" + String(j+1) + "on") {
        digitalWrite(ledPins[j], LOW);
        bot.sendMessage(chat_id, "Relé " + String(j+1) + " encendido", "");
        estadoLeds[j] = 1;
      } else if (text == "R" + String(j+1) + "off") {
        digitalWrite(ledPins[j], HIGH);
        bot.sendMessage(chat_id, "Relé " + String(j+1) + " apagado", "");
        estadoLeds[j] = 0;
      }
    }

    // Comando para obtener la temperatura y la humedad
    if (text == "Datos") {
      float temperatura = dht.readTemperature();
      float humedad = dht.readHumidity();
      if (isnan(temperatura) || isnan(humedad)) {
        bot.sendMessage(chat_id, "Error al leer del sensor DHT", "");
      } else {
        String mensaje = "Temperatura: " + String(temperatura, 1) + " °C\n";
        mensaje += "Humedad: " + String(humedad, 1) + " %";
        bot.sendMessage(chat_id, mensaje, "");
      }
    }

    // Estado del sensor LDR
    if (text == "EstadoLDR") {
      int estadoLDR = digitalRead(ldrPin);
      bot.sendMessage(chat_id, estadoLDR == HIGH ? "El sensor LDR detecta luz." : "El sensor LDR no detecta luz.", "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH);
  }
  pinMode(ldrPin, INPUT);
  dht.begin();

  // Conexión WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configuración para evitar la verificación del certificado (eliminar en producción)
  secured_client.setInsecure();
}

void loop() {
  if (millis() - tiempoAnterior > tiempo) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    if (numNewMessages) {
      handleNewMessages(numNewMessages);
    }
    tiempoAnterior = millis();
  }
}

