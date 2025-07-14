#include <WiFi.h>
#include <WiFiClient.h>
#include "DHTesp.h"
#include <PubSubClient.h>

#define LED_ALARME_1 12
#define LED_ALARME_2 14
#define LED_ALARME_3 27

#define BUTTON_INCREMENTA  4
#define BUTTON_RESETA     5

const int DHT_PIN = 15;
int buttonState = 0;

DHTesp dhtSensor;

const char* ssid = "A05s de Carlos";

const char* password = "carlos123";

const char* mqtt_server = "192.168.1.190";

const int mqtt_port = 1883;

//const char* mqtt_user = "PHIoT";

//const char* mqtt_pass = "phiot";

const char* id = "HumidGuard_ESP32";

WiFiClient espClient;
volatile bool ledState_verde = false;
volatile bool ledState_amarelo = false;
volatile bool ledState_vermelho = false;

volatile unsigned long lastInterruptTime = 0;
volatile unsigned long lastInterruptTimeReseta = 0;
volatile unsigned long lastInterruptTimeIncrementa = 0;

const unsigned long debounceDelay = 200; // em milissegundos

int contador = 0;

// sensor de umidade capacitivo
const int sensor_umi_cap = 34; // Pino onde o sensor está conectado
int sensorValue = 0;

PubSubClient client(espClient);

void IRAM_ATTR handleIncrementa() {
  unsigned long tempoAgora = millis();
  if (tempoAgora - lastInterruptTimeIncrementa > debounceDelay) {
    contador += 1;
    lastInterruptTimeIncrementa = tempoAgora;
  }
}

void IRAM_ATTR handleReseta() {
  Serial.print("acionou o reset");
  unsigned long tempoAgora = millis();
  if (tempoAgora - lastInterruptTimeReseta > debounceDelay) {
    contador = 0;
    lastInterruptTimeReseta = tempoAgora;
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String msg;

  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  /*if (String(topic) == "/esp-iot/gas") {
    if (msg == "true") {
      gas_on = true;
    } else if (msg == "false") {
      gas_on = false;
    }
  }*/
}


void reconnect(){
   while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect(id)) {
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
  // definindo os leds
  pinMode(LED_ALARME_1, OUTPUT);
  pinMode(LED_ALARME_2, OUTPUT);
  pinMode(LED_ALARME_3, OUTPUT);

  //definindo os botões
  pinMode(BUTTON_INCREMENTA, INPUT);
  pinMode(BUTTON_RESETA, INPUT);

  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  // cadastrando a função de callback
  client.setCallback(callback);

  // Configura a interrupção no botão
  attachInterrupt(digitalPinToInterrupt(BUTTON_INCREMENTA), handleIncrementa, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_RESETA), handleReseta, RISING);
}

void pisca_led(){
  digitalWrite(LED_ALARME_3, HIGH);
  delay(500);
  digitalWrite(LED_ALARME_3, LOW);
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  Serial.println(contador);
  if(contador > 0 && contador <= 2){
    digitalWrite(LED_ALARME_1, HIGH);  
    client.publish ("HumidGuard/botao", "3");
  }else if(contador > 2 && contador <= 4){
    digitalWrite(LED_ALARME_2, HIGH);
    digitalWrite(LED_ALARME_1, LOW);
    client.publish ("HumidGuard/botao", "2");
  }else if(contador > 5){
    digitalWrite(LED_ALARME_3, HIGH);
    digitalWrite(LED_ALARME_2, LOW);
    digitalWrite(LED_ALARME_1, LOW);
    client.publish ("HumidGuard/botao", "1");
  }/*else if(contador == 0){
    digitalWrite(LED_ALARME_3, LOW);
    digitalWrite(LED_ALARME_2, LOW);
    digitalWrite(LED_ALARME_1, LOW);
  }*/

  //leitura do sensor DHT22
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();

  String Temp = String(data.temperature, 2);
  String Humidity = String(data.humidity, 1);

  Serial.println("LEITURA SENSOR DHT22:");
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
  
  client.publish ("HumidGuard/dht11/temp", Temp.c_str());
  client.publish ("HumidGuard/dht11/hum", Humidity.c_str());
  delay(2000); // Wait for a new reading from the sensor (DHT22 has ~0.5Hz sample rate)

  //leitura de sensor de umidade capacitivo
  sensorValue = analogRead(sensor_umi_cap); // Lê o valor analógico
  Serial.print("Valor do sensor: ");
  Serial.println(sensorValue);

  // Opcional: converter o valor para percentual (0–100%)
  int porcentagemUmidade = map(sensorValue, 2650, 1000, 0, 100); // ajuste conforme o sensor
  porcentagemUmidade = constrain(porcentagemUmidade, 0, 100);

  Serial.println("LEITURA SENSOR DE UMIDADE CAPACITIVO:");
  Serial.print("Umidade estimada: ");
  Serial.print(porcentagemUmidade);
  Serial.println(" %");
  client.publish ("HumidGuard/sensor_cap/umi", String(porcentagemUmidade).c_str());

  client.loop();
}
