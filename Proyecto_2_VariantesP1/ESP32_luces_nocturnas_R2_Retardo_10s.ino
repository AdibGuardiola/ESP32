#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"

// Datos de la red WiFi
#define WIFI_SSID "FG_AULA2"
#define WIFI_PASSWORD "Aula2fg2"

// Token del BOT de Telegram (obtenido de Botfather en Telegram)
#define BOT_TOKEN "7857395235:AAEoXThRVg1J1hJ4DFIMRyYj5W6NU6oq8-8"

const unsigned long tiempo = 1000; // tiempo entre escaneo de mensajes
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long tiempoAnterior; // última vez que se escanearon mensajes
unsigned long tiempoEncendidoR2 = 0; // tiempo en que se encendió el relé 2

// Pines de los relés
const int led12 = 25; // Primer relé
const int led13 = 33; // Segundo relé (controlado por LDR)
const int led15 = 27; // Tercer relé
const int led16 = 26; // Cuarto relé
const int led17 = 32; // Quinto relé
const int led18 = 15; // Sexto relé
const int ldrPin = 34; // Pin del sensor LDR

int estadoLed12 = 0;
int estadoLed13 = 0; // Estado del relé 2
int estadoLed15 = 0;
int estadoLed16 = 0;
int estadoLed17 = 0;
int estadoLed18 = 0;
int inicio = 1;

// Definir el pin y el tipo del sensor DHT
#define DHT_PIN 16
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

String chat_id;
#define ID_Chat "909954663" // ID del chat obtenido desde Telegram

void mensajesNuevos(int numerosMensajes)
{
  for (int i = 0; i < numerosMensajes; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    // Control de relés mediante mensajes de Telegram
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

    // Comando para obtener la temperatura y la humedad del sensor DHT22
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
        bot.sendMessage(chat_id, "El sensor LDR no detecta luz.", "");
      }
      else
      {
        bot.sendMessage(chat_id, "El sensor LDR detecta luz.", "");
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

  // Inicializamos los relés en estado apagado (HIGH)
  digitalWrite(led12, HIGH);
  digitalWrite(led13, HIGH);
  digitalWrite(led15, HIGH);
  digitalWrite(led16, HIGH);
  digitalWrite(led17, HIGH);
  digitalWrite(led18, HIGH);

  // Conectarse a la red WiFi
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

  dht.begin(); // Iniciar sensor DHT22
}

void loop()
{
  if (millis() - tiempoAnterior > tiempo)
  {
    // Revisar estado del LDR y controlar el relé 2 en consecuencia
    int estadoLDR = digitalRead(ldrPin);
    
    if (estadoLDR == HIGH && estadoLed13 == 0) // Si el LDR no detecta luz y el relé 2 está apagado
    {
      estadoLed13 = 1; // Actualizar el estado del relé 2 a encendido
      digitalWrite(led13, LOW); // Encender el relé 2
      tiempoEncendidoR2 = millis(); // Registrar el momento en que se enciende
      bot.sendMessage(ID_Chat, "LDR apagado, encendiendo Relé 2", "");
    }
    else if (estadoLDR == LOW && estadoLed13 == 1) // Si el LDR detecta luz y el relé 2 está encendido
    {
      estadoLed13 = 0; // Actualizar el estado del relé 2 a apagado
      digitalWrite(led13, HIGH); // Apagar el relé 2
      bot.sendMessage(ID_Chat, "LDR encendido, apagando Relé 2", "");
    }

    // Verificar si han pasado 10 segundos (10000 ms) desde que se encendió el relé 2
    if (estadoLed13 == 1 && (millis() - tiempoEncendidoR2 >= 10000)) // 10 segundos = 10000 ms
    {
      estadoLed13 = 0; // Actualizar el estado del relé 2 a apagado
      digitalWrite(led13, HIGH); // Apagar el relé 2
      bot.sendMessage(ID_Chat, "El relé 2 se ha apagado automáticamente después de 10 segundos.", "");
    }

    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    while (numerosMensajes)
    {
      mensajesNuevos(numerosMensajes);
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    }
    tiempoAnterior = millis();
  }
}
