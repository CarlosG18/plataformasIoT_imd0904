#include <WiFi.h>
#include <WiFiClient.h>
#include "DHTesp.h"
#include <PubSubClient.h>

#define LED_ALARME 5
#define LED_GAS 19

const int DHT_PIN = 15;
const int PIN_DHT11

bool gas_on = 1;

DHTesp dhtSensor;

const char* mqtt_server = "";

const int mqtt_port = 1883;

const char* mqtt_user = "PHIoT";

const char* mqtt_pass = "phiot";

const char* id = "esp-iot";

WiFiClient espClient;

PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length) {
  String msg;

  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (String(topic) == "/esp-iot/gas") {
    if (msg == "true") {
      gas_on = true;
    } else if (msg == "false") {
      gas_on = false;
    }
  }
}

void reconnect(){
   while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect(id, mqtt_user, mqtt_pass)) {
      Serial.println("Conectado ao broker MQTT!");
      client.subscribe("/esp-iot/gas");
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
  // definindo a pinagem do servo
  myservo.attach(18); // attach servo on GPIO 18

  // definindo os leds
  pinMode(LED_ALARME, OUTPUT);
  pinMode(LED_GAS, OUTPUT);

  Serial.begin(115200);
  pinMode(PIN_PRESENCA, INPUT);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  // cadastrando a função de callback
  client.setCallback(callback);
}

void pisca_led(){
  digitalWrite(LED_ALARME, HIGH);
  delay(500);
  digitalWrite(LED_ALARME, LOW);
}

void loop() {
  if(gas_on){
    pisca_led();
    digitalWrite(LED_GAS, HIGH);
    myservo.write(0);  // move to 90 degrees
  }else{
    digitalWrite(LED_GAS, LOW);
    myservo.write(90);  // move to 90 degrees
  }

  Serial.println(gas_on);
  
  if(!client.connected()){
    reconnect();
  }

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  String Temp = String(data.temperature, 2);
  String Humidity = String(data.humidity, 1);
  //client.publish ("/esp-iot", payload.c_str());
  client.publish ("/esp-iot/temp", Temp.c_str());
  client.publish ("/esp-iot/hum", Humidity.c_str());
  delay(2000); // Wait for a new reading from the sensor (DHT22 has ~0.5Hz sample rate)
  
  //leitura do sensor de presença
  int value = digitalRead(PIN_PRESENCA);
  if (value == HIGH){
    client.publish("/esp-iot/pres", "1");
    delay(2000);
  }else{
    client.publish("/esp-iot/pres", "0");
    delay(2000);
  }

  client.loop();
}
