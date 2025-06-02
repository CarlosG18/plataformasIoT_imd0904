#include <wifi.h>
#include "DHTesp.h"
#include <PubSubClient>

const int DHT_PIN = 15;
const int PIN_PRESENCA = 2;

DHTesp dhtSensor;

const char* ssid = "npiti-iot";

const char* password = "npiti-iot";

const char* mqtt server = "10.7.236.23";

const int mqtt_port = 1883;

const char* pw = "phiot";

const char* id = "esp-iot";

WiFiClient espClient;

PubSubClient client(espClient);

/*void reconnect(){
  unsigned long iniciotentativa = millis();
  bool mqttconectado = false;

  do{
    if(mqtt.Conectado && millis() - iniciotentativa > 1000){
      mqttconectado 
    }
  }
}*/

void reconnect(){
   while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect(client_id, mqtt_user, mqtt_pass)) {
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
  client.publish ("esp32-iot", payload.c_str());
  delay(2000); // Wait for a new reading from the sensor (DHT22 has ~0.5Hz sample rate)

  //leitura do sensor de presença
  int value = digitalRead(PIN_PRESENCA);
  if (value == HIGH){
    Serial.println("foi identificado algum corno!");
    client.publish("esp32-iot", "Presença detectada!")
  }else{
    Serial.println("Nenhum chifre a vista!");
    client.publish("esp32-iot", "ninguém agora");
  }

  client.loop();
}
