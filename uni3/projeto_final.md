# 🩺 Projeto Final - Monitoramento de Pacientes

Este projeto tem como objetivo implementar um sistema de monitoramento remoto de pacientes utilizando sensores dde umidade conectados a uma plataforma IoT. A comunicação entre os dispositivos é realizada via protocolo MQTT, com visualização dos dados em tempo real através do Node-RED.

---

## ⚙️ Montagem do Circuito

foi desenvolvido a montagem do circuito online pela plataforma do wokiwi para realizar a simulação e um teste previo antes de realizar a montagem do circuito de fato.

![imagem da simulação no workiwi]()

> [!IMPORTANT]\
> É aconselhaveu elaborar o projeto primeiro em plataformas onlines como o [wokiwi](https://wokwi.com/) para realizar simulações e observar o comportamento dos elementos para evitar queima de componentes e mal uso deles.

### 🔧 Sensores e Materiais Utilizados

* **Sensor de umidade do solo:** CS12
* **Sensor de temperatura e umidade do ar:** DHT11
* **Microcontrolador:** ESP32 (ou ESP8266, conforme o caso)
* **Jumpers e protoboard**
* **Fonte de alimentação USB ou bateria**

![Imagem do circuito montado]() 

---

## 🔄 Comunicação via MQTT - Mosquitto Broker

Para permitir a comunicação entre o ESP e a interface de visualização (Node-RED), utilizamos o protocolo **MQTT (Message Queuing Telemetry Transport)**, um protocolo leve e ideal para dispositivos IoT.

### 📌 Conceitos-chave do MQTT:

* **Publicador:** dispositivo que envia (publica) dados para um tópico específico.
* **Assinante:** dispositivo ou aplicação que recebe (assina) os dados de um tópico.
* **Broker:** servidor central que recebe todas as mensagens publicadas e encaminha para os assinantes corretos.

  > Neste projeto, utilizamos o **Mosquitto** como broker MQTT.

### Passo a passo para configuração do broker Mosquitto

1. instalar o mosquitto na sua maquina:

utilizei o linux então pode ser que para o windows possua alguma etapa diferente, porém a ideia é a mesma.

```bash
$ 
```

2. Atualizar o arquivo de configuração do mosquitto `mosquitto.conf`

- acessar o diretorio onde esta esse arquivo;
- falar como colocar a senha no broker;
- mostrar qual configuração básica deve ser inserida para rodar o serviço do mosquitto;
- rodar o serviço.

3. mostrar no codigo como é feito o envio dos topicos
---

## 🌐 Node-RED

O **Node-RED** é uma ferramenta baseada em fluxo para conectar dispositivos de hardware, APIs e serviços online de forma visual.

### Comunicação Node-RED -> broker

explicar como o node-red se comunica com o broker.

### 🖥️ Dashboard

Criamos um painel de controle no Node-RED para visualizar em tempo real:

* Temperatura e umidade do ambiente
* Umidade do solo (representando, por exemplo, níveis de hidratação)

A dashboard foi configurada com elementos como:

* Gráficos em tempo real
* Indicadores numéricos
* Alertas visuais para limites críticos

![imagem do dashboard]()

---

## 📦 Estrutura do Projeto

```plaintext
📁 monitoramento-pacientes/
├── circuitos/
│   └── esquema_circuito.png
├── codigo_esp/
│   └── main.ino
├── node-red/
│   └── fluxo.json
├── README.md
```

## Como rodar o projeto na sua máquina?

- elaborar etapas para que qualquer pessoa possa rodar o projeto em seu ambiente.