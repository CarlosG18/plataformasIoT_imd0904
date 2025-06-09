#include <WiFi.h>
#include <WiFiClient.h>
#include "DHTesp.h"
#include <PubSubClient.h>

const int DHT_PIN = 15;
const int PIN_PRESENCA = 4;

DHTesp dhtSensor;

const char* ssid = "NPITI-IoT";

const char* password = "NPITI-IoT";

const char* mqtt_server = "10.7.236.23";

const int mqtt_port = 1883;

const char* mqtt_user = "PHIoT";

const char* mqtt_pass = "phiot";

const char* id = "esp-iot";

WiFiClient espClient;

PubSubClient client(espClient);

void reconnect(){
   while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect(id, mqtt_user, mqtt_pass)) {
      Serial.println("Conectado ao broker MQTT!");
    } else {
      Serial.print("Falha na conexão, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PRESENCA, INPUT);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);

}

void loop() {
  
  if(!client.connected()){
    reconnect();
  }

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
  String payload = "Temp: " + String(data.temperature, 2) + "°C" + "Humidity: " + String(data.humidity, 1) + "%" ;
  client.publish ("/esp-iot", payload.c_str());
  delay(2000); // Wait for a new reading from the sensor (DHT22 has ~0.5Hz sample rate)
  
  //leitura do sensor de presença
  
  int value = digitalRead(PIN_PRESENCA);
  if (value == HIGH){
    Serial.println("foi identificado algum corno!");
    client.publish("/esp-iot", "Presença detectada!");
    delay(2000);
  }else{
    Serial.println("Nenhum chifre a vista!");
    client.publish("/esp-iot", "ninguém agora");
    delay(2000);
  }

  client.loop();
}
