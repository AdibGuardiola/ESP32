#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"
//MOVISTAR_67A8 777C6190FEDA3ABE7616
// Reemplazar con los datos de tu red wifi
#define WIFI_SSID "TP-Link_76E3"
#define WIFI_PASSWORD "13193348"

// Token de Telegram BOT se obtienen desde Botfather en telegram
#define BOT_TOKEN "6075391597:AAFi28sadDJmqOrgvKGlbMnMK5hk8A1JFQY"

const unsigned long tiempo = 1000; // tiempo medio entre mensajes de escaneo
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long tiempoAnterior; // última vez que se realizó el análisis de mensajes

const int led12 = 25; // Primer relé
const int led13 = 33; // Segundo relé
const int led15 = 27; // Tercer relé
const int led16 = 26; // Cuarto relé
const int led17 = 32; // Quinto relé
const int led18 = 15; // Sexto relé
const int ldrPin = 34; // Pin del sensor LDR
int estadoLed12 = 0;
int estadoLed13 = 0;
int estadoLed15 = 0;
int estadoLed16 = 0;
int estadoLed17 = 0;
int estadoLed18 = 0;
int inicio = 1;

// Definir el pin y el tipo del sensor DHT
#define DHT_PIN 4
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

String chat_id;
#define ID_Chat "909954663" // ID_Chat se obtiene de telegram

void mensajesNuevos(int numerosMensajes) //Es una bucle
{
  for (int i = 0; i < numerosMensajes; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    // Control de relés
    if (text == "R1on")
    {
      estadoLed12 = 1;
      digitalWrite(led12, LOW);      
      bot.sendMessage(chat_id, "Relé1 encendido", "");
    }

    if (text == "R1off")
    {
      estadoLed12 = 0;
      digitalWrite(led12, HIGH);
      bot.sendMessage(chat_id, "Relé1 apagado", "");
    }

    if (text == "R2on")
    {
      estadoLed13 = 1;
      digitalWrite(led13, LOW);      
      bot.sendMessage(chat_id, "Relé2 encendido", "");
    }

    if (text == "R2off")
    {
      estadoLed13 = 0;
      digitalWrite(led13, HIGH);
      bot.sendMessage(chat_id, "Relé2 apagado", "");
    }

    if (text == "R3on")
    {
      estadoLed15 = 1;
      digitalWrite(led15, LOW);      
      bot.sendMessage(chat_id, "Relé3 encendido", "");
    }

    if (text == "R3off")
    {
      estadoLed15 = 0;
      digitalWrite(led15, HIGH);
      bot.sendMessage(chat_id, "Relé3 apagado", "");
    }
    
    if (text == "R4on")
    {
      estadoLed16 = 1;
      digitalWrite(led16, LOW);      
      bot.sendMessage(chat_id, "Relé4 encendido", "");
    }

    if (text == "R4off")
    {
      estadoLed16 = 0;
      digitalWrite(led16, HIGH);
      bot.sendMessage(chat_id, "Relé4 apagado", "");
    }
    
    if (text == "R5on")
    {
      estadoLed17 = 1;
      digitalWrite(led17, LOW);      
      bot.sendMessage(chat_id, "Relé5 encendido", "");
    }

    if (text == "R5off")
    {
      estadoLed17 = 0;
      digitalWrite(led17, HIGH);
      bot.sendMessage(chat_id, "Relé5 apagado", "");
    }
    
    if (text == "R6on")
    {
      estadoLed18 = 1;
      digitalWrite(led18, LOW);      
      bot.sendMessage(chat_id, "Relé6 encendido", "");
    }

    if (text == "R6off")
    {
      estadoLed18 = 0;
      digitalWrite(led18, HIGH);
      bot.sendMessage(chat_id, "Relé6 apagado", "");
    }

    // Comando para obtener la temperatura y la humedad
    if (text == "Datos")
    {
      float temperatura = dht.readTemperature();
      float humedad = dht.readHumidity();

      String respuesta = "Datos del sensor DHT22:\n";
      respuesta += "Temperatura: " + String(temperatura, 1) + " °C\n";
      respuesta += "Humedad: " + String(humedad, 1) + " %";
      bot.sendMessage(chat_id, respuesta, "");
    }

    // Verificar estado del sensor LDR
    if (text == "EstadoLDR")
    {
      int estadoLDR = digitalRead(ldrPin);
      if (estadoLDR == HIGH)
      {
        bot.sendMessage(chat_id, "El sensor LDR detecta luz.", "");
      }
      else
      {
        bot.sendMessage(chat_id, "El sensor LDR no detecta luz.", "");
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(led12, OUTPUT);
  pinMode(led13, OUTPUT);
  pinMode(led15, OUTPUT);
  pinMode(led16, OUTPUT);
  pinMode(led17, OUTPUT);
  pinMode(led18, OUTPUT);
  pinMode(ldrPin, INPUT);

  digitalWrite(led12, HIGH);
  digitalWrite(led13, HIGH);
  digitalWrite(led15, HIGH);
  digitalWrite(led16, HIGH);
  digitalWrite(led17, HIGH);
  digitalWrite(led18, HIGH);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  if (inicio == 1)
  {
    bot.sendMessage(ID_Chat, "Sistema preparado!!!, escribe Ayuda para ver las opciones", "");
    inicio = 0;
  }

  dht.begin();
}

void loop()
{
  if (millis() - tiempoAnterior > tiempo)
  {
    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1);

    while (numerosMensajes)
    {
      mensajesNuevos(numerosMensajes);
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    }

    tiempoAnterior = millis();
  }
}
