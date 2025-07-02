const int sensorPin = 34; // Pino onde o sensor está conectado
int sensorValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000); // Aguarda o Serial iniciar
  Serial.println("Iniciando leitura do sensor de umidade...");
}

void loop() {
  sensorValue = analogRead(sensorPin); // Lê o valor analógico
  Serial.print("Valor do sensor: ");
  Serial.println(sensorValue);

  // Opcional: converter o valor para percentual (0–100%)
  int porcentagemUmidade = map(sensorValue, 2650, 1000, 0, 100); // ajuste conforme o sensor
  porcentagemUmidade = constrain(porcentagemUmidade, 0, 100);

  Serial.print("Umidade estimada: ");
  Serial.print(porcentagemUmidade);
  Serial.println(" %");

  delay(1000); // Aguarda 1 segundo entre as leituras
}
