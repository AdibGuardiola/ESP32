#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"
#include <WebServer.h>
// MOVISTAR_67A8 777C6190FEDA3ABE7616
#define WIFI_SSID "MOVISTAR_67A8"
#define WIFI_PASSWORD "777C6190FEDA3ABE7616"
#define BOT_TOKEN "6075391597:AAFi28sadDJmqOrgvKGlbMnMK5hk8A1JFQY"

const unsigned long tiempo = 1000;
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long tiempoAnterior;
const int led12 = 25; // Primer relé
const int led13 = 33; // Segundo relé
const int led15 = 27; // Tercer relé
const int led16 = 26; // Cuarto relé
const int led17 = 32; // Quinto relé
const int led18 = 15; // Sexto relé

const int ldrPin = 34; // Asegúrate de que este pin no esté en conflicto
int estadoLed12 = 0;
int estadoLed13 = 0;
int estadoLed15 = 0;
int estadoLed16 = 0;
int estadoLed17 = 0;
int estadoLed18 = 0;
// Considera añadir estados para los relés adicionales si es necesario
int inicio = 1;

#define DHT_PIN 4
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

#define TRIG_PIN 19
#define ECHO_PIN 18

WebServer server(80);
String chat_id;
#define ID_Chat "909954663"

void mensajesNuevos(int numerosMensajes) {
    String ipMensaje = " IP actual: " + WiFi.localIP().toString();
    for (int i = 0; i < numerosMensajes; i++) {
        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;

        if (text == "R1on") {
            digitalWrite(led12, HIGH);
            estadoLed12 = 1;
            bot.sendMessage(chat_id, "Relé 1 encendido." + ipMensaje, "");
        } else if (text == "R1off") {
            digitalWrite(led12, LOW);
            estadoLed12 = 0;
            bot.sendMessage(chat_id, "Relé 1 apagado." + ipMensaje, "");
        } else if (text == "R2on") {
            digitalWrite(led13, HIGH);
            estadoLed13 = 1;
            bot.sendMessage(chat_id, "Relé 2 encendido." + ipMensaje, "");
        } else if (text == "R2off") {
            digitalWrite(led13, LOW);
            estadoLed13 = 0;
            bot.sendMessage(chat_id, "Relé 2 apagado." + ipMensaje, "");
        } else if (text == "R3on") {
            digitalWrite(led15, HIGH);
            estadoLed15 = 1;
            bot.sendMessage(chat_id, "Relé 3 encendido." + ipMensaje, "");
        } else if (text == "R3off") {
            digitalWrite(led15, LOW);
            estadoLed15 = 0;
            bot.sendMessage(chat_id, "Relé 3 apagado." + ipMensaje, "");
        } else if (text == "R4on") {
            digitalWrite(led16, HIGH);
            estadoLed16 = 1;
            bot.sendMessage(chat_id, "Relé 4 encendido." + ipMensaje, "");
        } else if (text == "R4off") {
            digitalWrite(led16, LOW);
            estadoLed16 = 0;
            bot.sendMessage(chat_id, "Relé 4 apagado." + ipMensaje, "");
        } else if (text == "R5on") {
            digitalWrite(led17, HIGH);
            estadoLed17 = 1;
            bot.sendMessage(chat_id, "Relé 5 encendido." + ipMensaje, "");
        } else if (text == "R5off") {
            digitalWrite(led17, LOW);
            estadoLed17 = 0;
            bot.sendMessage(chat_id, "Relé 5 apagado." + ipMensaje, "");
        } else if (text == "R6on") {
            digitalWrite(led18, HIGH);
            estadoLed18 = 1;
            bot.sendMessage(chat_id, "Relé 6 encendido." + ipMensaje, "");
        } else if (text == "R6off") {
            digitalWrite(led18, LOW);
            estadoLed18 = 0;
            bot.sendMessage(chat_id, "Relé 6 apagado." + ipMensaje, "");
        }
    }
}

void setup() {
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
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.print("Conectando a la red ");
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("\nConectado a la red wifi. Dirección IP: ");
    Serial.println(WiFi.localIP());
    secured_client.setInsecure();// AÑADida ESTA LÍNEA AQUÍ
    if (inicio == 1) {
        bot.sendMessage(ID_Chat, "Sistema preparado!!!, escribe Ayuda para ver las opciones. IP: " + WiFi.localIP().toString(), "");
        inicio = 0;
    }

    dht.begin();



    server.on("/reles/1/encender", HTTP_GET, []() {
        digitalWrite(led12, HIGH);
        server.send(200, "text/plain", "Relé 1 encendido");
    });

    server.on("/reles/1/apagar", HTTP_GET, []() {
        digitalWrite(led12, LOW);
        server.send(200, "text/plain", "Relé 1 apagado");
    });

    server.on("/reles/2/encender", HTTP_GET, []() {
        digitalWrite(led13, HIGH);
        server.send(200, "text/plain", "Relé 1 encendido");
    });

    server.on("/reles/2/apagar", HTTP_GET, []() {
        digitalWrite(led13, LOW);
        server.send(200, "text/plain", "Relé 1 apagado");
    });

    server.on("/reles/3/encender", HTTP_GET, []() {
        digitalWrite(led15, HIGH);
        server.send(200, "text/plain", "Relé 1 encendido");
    });

    server.on("/reles/3/apagar", HTTP_GET, []() {
        digitalWrite(led15, LOW);
        server.send(200, "text/plain", "Relé 1 apagado");
    });

        server.on("/reles/4/encender", HTTP_GET, []() {
        digitalWrite(led16, HIGH);
        server.send(200, "text/plain", "Relé 1 encendido");
    });

    server.on("/reles/4/apagar", HTTP_GET, []() {
        digitalWrite(led16, LOW);
        server.send(200, "text/plain", "Relé 1 apagado");
    });
            server.on("/reles/5/encender", HTTP_GET, []() {
        digitalWrite(led17, HIGH);
        server.send(200, "text/plain", "Relé 1 encendido");
    });

    server.on("/reles/5/apagar", HTTP_GET, []() {
        digitalWrite(led17, LOW);
        server.send(200, "text/plain", "Relé 1 apagado");
    });
                server.on("/reles/6/encender", HTTP_GET, []() {
        digitalWrite(led18, HIGH);
        server.send(200, "text/plain", "Relé 1 encendido");
    });

    server.on("/reles/6/apagar", HTTP_GET, []() {
        digitalWrite(led18, LOW);
        server.send(200, "text/plain", "Relé 1 apagado");
    });




    // Añadir lógica similar para los otros relés y rutas

        server.on("/datos", HTTP_GET, []() {
        float temperatura = dht.readTemperature();
        float humedad = dht.readHumidity();

        if (isnan(temperatura) || isnan(humedad)) {
            server.send(500, "text/plain", "Error leyendo el sensor DHT");
            return;
        }

        // Construye el JSON con los datos
    String jsonResponse = "{\"temperatura\": " + String(temperatura) + ", \"humedad\": " + String(humedad) + "}";

    // Configura el encabezado CORS para permitir solicitudes desde http://localhost:8000
    server.sendHeader("Access-Control-Allow-Origin", "http://localhost:8000");
    server.send(200, "application/json", jsonResponse);
});

server.on("/distancia", HTTP_GET, []() {
    // Mide la distancia
    float distancia = medirDistancia();
    
    // Convierte la distancia a una cadena JSON
    String jsonResponse = "{\"distancia\": " + String(distancia) + "}";
    
    // Envía la respuesta JSON
    server.send(200, "application/json", jsonResponse);
});

    
    server.on("/sensor", HTTP_GET, []() {
        int valorLDR = analogRead(ldrPin);
        String respuesta = "Valor del sensor LDR: "+String(valorLDR);
        server.send(200, "text/plain", respuesta);
    });

    




    server.begin();
}

void loop() {


    if (millis() - tiempoAnterior > tiempo) {
        int numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
        while (numerosMensajes) {
            mensajesNuevos(numerosMensajes);
            numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
        }
        tiempoAnterior = millis();
    }

    server.handleClient();
}

float medirDistancia() {
    // Limpia el pin TRIG
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    // Genera un pulso de 10 microsegundos en TRIG
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  
    // Lee el tiempo del pulso en ECHO
    long duracion = pulseIn(ECHO_PIN, HIGH);
  
    // Calcula la distancia en centímetros
    float distancia = duracion * 0.034 / 2;
  
    return distancia;
}


