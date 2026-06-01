# Sistema IoT para Monitoramento de Transbordo (ODS 11)

## 📌 Descrição do Projeto
Este repositório contém o código-fonte e a documentação de um sistema IoT desenvolvido para monitorar em tempo real o nível de preenchimento de bueiros e caçambas urbanas. O projeto está alinhado ao Objetivo de Desenvolvimento Sustentável 11 (Cidades e Comunidades Sustentáveis) da ONU. Para reproduzir este projeto, basta compilar o código em um ESP32 e conectar os componentes conforme o esquema elétrico descrito no artigo.

## 💻 Software e Código-fonte
O software foi desenvolvido em C++ utilizando a IDE do Arduino. O código realiza as seguintes funções:
1. Conecta-se a uma rede Wi-Fi local.
2. Estabelece conexão com um Broker MQTT público (`test.mosquitto.org`).
3. Lê os dados de distância do sensor HC-SR04.
4. Calcula o percentual de preenchimento do recipiente.
5. Aciona o LED de alerta (80%) e trava o Servomotor (100%).
6. Publica os dados em formato JSON no tópico MQTT.

## ⚙️ Hardware Utilizado
* **Microcontrolador:** ESP32 (Módulo Wi-Fi e Bluetooth integrados).
* **Sensor:** Ultrassônico HC-SR04 (leitura de distância via pulsos de 40 kHz).
* **Atuador 1:** Micro Servomotor SG90 (simulação de trava mecânica).
* **Atuador 2:** LED 5mm e Resistor de 220Ω (alerta visual).

## 📡 Comunicação e Protocolos
* **Rede:** A comunicação é feita via internet (Protocolo TCP/IP) utilizando a antena Wi-Fi nativa do ESP32 (IEEE 802.11 b/g/n).
* **Mensageria (MQTT):** O sistema utiliza o protocolo MQTT (porta 1883) sob o modelo Publish/Subscribe. O ESP32 atua como *Publisher*, enviando a telemetria em formato de payload JSON para o tópico `mackenzie/ods11/bueiro01`.