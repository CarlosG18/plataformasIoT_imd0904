# 💡 Projeto: Monitoramento com Sensor de Presença e Sensor de Temperatura e Umidade

Este projeto combina o uso de um **sensor de presença PIR (HC-SR501)** com um **sensor de temperatura e umidade (DHT11 ou DHT22)** para monitoramento de ambientes. Pode ser utilizado em aplicações como automação residencial, segurança ou sistemas de alerta ambiental.

---

## 🔌 Hardware Utilizado

* **ESP32** (microcontrolador principal)
* **Sensor PIR HC-SR501** (detecção de movimento)
* **Sensor DHT11 ou DHT22** (temperatura e umidade)
* **Resistor de 10kΩ** (pull-up no pino de dados do DHT)
* **Protoboard e jumpers**

---

## 🧪 Simulação
<p align="center">
  <img src="https://github.com/CarlosG18/plataformasIoT_imd0904/blob/main/uni2/simulacao_workiwi.PNG" alt="simulacao">
</p>
* A simulação do circuito e funcionamento está disponível na plataforma Wokwi:

👉 [Acesse a simulação aqui](https://wokwi.com/projects/431564958342974465)

---

## 🔧 Montagem Física

O circuito foi montado em uma protoboard com base na simulação. A ligação dos componentes foi feita conforme o seguinte:

* O **sensor PIR** foi conectado ao ESP32 para detectar movimentos.
* O **sensor DHT11/DHT22** foi ligado com resistor pull-up no pino de dados, garantindo estabilidade na comunicação.
* O ESP32 foi alimentado via USB (5V), com os componentes respeitando seus limites de tensão.

---

## 🔗 Comunicação

- MQTT
broker - assinantes e publicadores: [mosquitto online e publico](https://test.mosquitto.org/)
client: [mqttx](https://mqttx.app/web-client#/recent_connections) 


---

## 🚀 Aplicação

Este projeto pode ser usado em diversas situações:

* **Automação residencial**: Acionamento de luzes ou alarmes ao detectar presença.
* **Monitoramento climático**: Coleta de dados ambientais como temperatura e umidade.
* **Sistemas de segurança**: Detecção de movimentação em ambientes restritos.
