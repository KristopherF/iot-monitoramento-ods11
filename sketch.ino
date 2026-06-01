#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// Configurações de Wi-Fi e MQTT (Broker EMQX - Mais estável)
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.emqx.io"; 

WiFiClient espClient;
PubSubClient client(espClient);

// Definição dos Pinos
const int trigPin = 5;
const int echoPin = 18;
const int ledPin = 19;
const int servoPin = 21;

Servo travaServo;
long duration;
float distance;
int percentual;

const int profundidadeTotal = 100; 

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT no EMQX...");
    String clientId = "MackenzieESP32-";
    clientId += String(random(0, 1000));
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao Broker MQTT!");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  travaServo.attach(servoPin);
  travaServo.write(0);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  percentual = 100 - ((distance / profundidadeTotal) * 100);
  if (percentual < 0) percentual = 0;
  if (percentual > 100) percentual = 100;

  Serial.print("Nível: ");
  Serial.print(percentual);
  Serial.println("%");

  String estadoTrava = "Aberta";
  
  if (percentual >= 80 && percentual < 100) {
    digitalWrite(ledPin, HIGH); 
    travaServo.write(0);
  } else if (percentual >= 100) {
    digitalWrite(ledPin, HIGH);
    travaServo.write(90); 
    estadoTrava = "Fechada";
  } else {
    digitalWrite(ledPin, LOW);
    travaServo.write(0);
  }

  String payload = "{\"percentual\":" + String(percentual) + ", \"trava\":\"" + estadoTrava + "\"}";
  
  client.publish("mackenzie/ods11/bueiro01", payload.c_str());
  Serial.println("Dado publicado no MQTT: " + payload);
  
  delay(2000); 
}