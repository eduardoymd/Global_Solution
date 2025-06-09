# Global-Solution-Disruptive-Architectures-Iot-Iob-Generative-Ia

## 💧 Projeto FloodGuardian: Sistema Inteligente de Prevenção e Resposta a Enchentes
### Objetivo
Monitorar áreas de risco em tempo real usando IoT e satélites (como a Carta Internacional), prever 
enchentes e alertar moradores via app mobile, além de permitir atuação da Defesa Civil com apoio de 
painéis e APIs. 

## Descrição do Projeto
Este projeto monitora o nível e o volume de um reservatório usando um sensor ultrassônico HC-SR04, ESP32, display LCD I2C, LEDs indicadores e um módulo RTC DS3231 para registro da data e hora. Os dados são enviados para duas plataformas diferentes:
- 📡 ThingSpeak para visualização remota em nuvem
- ☁️ MQTT + Node-RED para monitoramento via dashboard local

## ⚙️ Funcionalidades
- Leitura da altura da água com sensor ultrassônico HC-SR04
- Cálculo do volume (baseado no formato cilíndrico do reservatório)
- Exibição de dados em LCD 16x2
- Indicação de níveis com LEDs (Verde, Amarelo, Vermelho)
- Envio de dados para ThingSpeak (via HTTP) ou para Node-RED (via MQTT)
- Registro do horário de cada medição com o RTC DS3231

## Simulação no Wokwi
O Wokwi permite simular projetos com ESP32 de forma realista. Ambos os códigos podem ser testados diretamente na plataforma.
### Hardware
| Componente                      | Função Principal                 |
| ------------------------------- | -------------------------------- |
| ESP32                           | Controlador principal            |
| HC-SR04                         | Leitura da distância da água     |
| LCD 16x2 com I2C                | Exibição do nível e volume       |
| LEDs (verde, amarelo, vermelho) | Indicação visual do nível        |
| RTC DS3231                      | Registro do horário das medições |

![image](https://github.com/user-attachments/assets/3dbe4626-52d9-4bcb-b4a3-86c8c1ed90f7)

## 📡 Wokwi com ThingSpeak (via HTTP)
O ThingSpeak é uma plataforma na nuvem para armazenamento, visualização e análise de dados IoT. Ele permite que você crie "canais" para receber dados de sensores em tempo real, sem necessidade de um servidor próprio.
### Como o projeto funciona com o ThingSpeak no Wokwi:
#### Simulação no Wokwi:
- O ESP32 é simulado com sensores como HC-SR04 (ultrassônico), RTC DS3231 e display LCD.
- O código coleta o nível da água, calcula o volume e obtém o horário atual.
#### Conexão com Wi-Fi:
- O ESP32 (virtual) se conecta à rede simulada do Wokwi (Wokwi-GUEST), que não exige senha.
- Essa rede permite conexões reais de internet, inclusive para serviços externos como ThingSpeak.
#### Envio de dados via HTTP GET:
- O código monta uma URL com os valores coletados (nível, volume, horário etc.).
- Usa a biblioteca HTTPClient para enviar uma requisição HTTP GET para o endpoint da ThingSpeak.
  
![image](https://github.com/user-attachments/assets/3a95386c-4dde-4851-ae51-c6e4faa60018)

## ☁️ Wokwi com Node-RED via MQTT
O MQTT (Message Queuing Telemetry Transport) é um protocolo leve de mensagens usado em IoT. Ele é baseado no modelo publicador/assinante:
- O dispositivo (ESP32) publica mensagens em um tópico.
- Node-RED (ou outro cliente) assina esse tópico e recebe os dados.
### Como funciona a comunicação Wokwi → Node-RED (via HiveMQ):
#### ESP32 simulado no Wokwi:
- Coleta os dados como na versão anterior.
- Em vez de enviar via HTTP, ele publica via MQTT no tópico:
> globalsolution/medidor/nivel
#### Broker MQTT (HiveMQ):
- Utiliza o broker público da HiveMQ (broker.hivemq.com) na porta 1883.
- Serve como ponto de encontro entre o ESP32 e o Node-RED.
#### Node-RED:
- Conecta-se ao mesmo broker MQTT.
- Usa um nó mqtt in para assinar o tópico globalsolution/medidor/nivel.
- Recebe mensagens no formato JSON, como:
  
```json
{
  "nivel_cm": 150,
  "volume_litros": 11780,
  "distancia_cm": 150,
  "horario": "13:15:20"
}
```
#### Dashboard no Node-RED:
- Um nó function separa o JSON em 3 fluxos.
- Os dados são exibidos em widgets:
  - Gauge (volume)
  - Textos (nível e horário)

### Fluxo Node-RED
![image](https://github.com/user-attachments/assets/78af34a9-e58f-4dfd-ad16-33cce06ad230)
### Dashboard
![image](https://github.com/user-attachments/assets/743804f1-669d-4db2-ac3a-459c539ad1dc)

## Código-fonte
Este projeto utiliza um **ESP32**, um **sensor ultrassônico**, um **display LCD**, **LEDs indicadores**, **RTC** e conexão **Wi-Fi** para monitorar o nível de água em um reservatório, exibindo os dados localmente e enviando-os para a plataforma **ThingSpeak**.
---
## ⚙️ Componentes Utilizados

- ESP32
- Sensor Ultrassônico (HC-SR04)
- Display LCD 16x2 com interface I2C
- RTC (Real Time Clock) DS3231
- LEDs (verde, amarelo, vermelho)
- Conexão Wi-Fi
- Plataforma ThingSpeak
---
## ⚙️ Bibliotecas Utilizadas

- `Wire.h` - Comunicação I2C
- `LiquidCrystal_I2C.h` - Controle do display LCD
- `RTClib.h` - Controle do RTC
- `WiFi.h` - Conexão com a rede Wi-Fi
- `HTTPClient.h` - Envio de dados via HTTP
---
## ⚙️ Funcionamento

1. O **sensor ultrassônico** mede a distância da água até o topo do reservatório.
2. A altura da água é usada para calcular o **volume** em litros.
3. Os dados são:
   - Exibidos em tempo real no **display LCD**;
   - Indicados por **LEDs**:
     - Verde: até 200 cm
     - Amarelo: até 300 cm
     - Vermelho: acima de 300 cm
   - Enviados para o **ThingSpeak** com horário atual via RTC.
---
## ⚙️ ThingSpeak
O projeto envia os seguintes dados para o ThingSpeak:
- `field1`: Nível da água (cm)
- `field2`: Volume calculado (litros)
- `field3`: Horário da medição
- `field4`: Repetição do nível (pode ser usado como gauge)
**API Key:** `N18H1NFNM1SSJCND`
---
## ⚙️ Fórmulas utilizadas
### Volume do reservatório (cilindro):
```cpp
volume_cm3 = π * raio² * altura
volume_litros = volume_cm3 / 1000
```










