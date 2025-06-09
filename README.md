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
### 📡 Wokwi com ThingSpeak
---
### ⚙️ Componentes Utilizados

- ESP32
- Sensor Ultrassônico (HC-SR04)
- Display LCD 16x2 com interface I2C
- RTC (Real Time Clock) DS3231
- LEDs (verde, amarelo, vermelho)
- Conexão Wi-Fi
- Plataforma ThingSpeak
---
### ⚙️ Bibliotecas Utilizadas

- `Wire.h` - Comunicação I2C
- `LiquidCrystal_I2C.h` - Controle do display LCD
- `RTClib.h` - Controle do RTC
- `WiFi.h` - Conexão com a rede Wi-Fi
- `HTTPClient.h` - Envio de dados via HTTP
---
### ⚙️ Funcionamento

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
### ⚙️ ThingSpeak
O projeto envia os seguintes dados para o ThingSpeak:
- `field1`: Nível da água (cm)
- `field2`: Volume calculado (litros)
- `field3`: Horário da medição
- `field4`: Repetição do nível (pode ser usado como gauge)
**API Key:** `N18H1NFNM1SSJCND`
---
### ⚙️ Fórmulas utilizadas
##### Volume do reservatório (cilindro):
```cpp
volume_cm3 = π * raio² * altura
volume_litros = volume_cm3 / 1000
```
## Código-fonte
### 📡 Wokwi com ThingSpeak (via HTTP)
```cpp
// Bibliotecas para comunicação I2C, RTC, Wi-Fi e requisições HTTP
#include <Wire.h>                    // Comunicação I2C (RTC e LCD)
#include <LiquidCrystal_I2C.h>      // Controle de display LCD 16x2 via I2C
#include <RTClib.h>                 // Relógio de tempo real DS3231
#include <WiFi.h>                   // Wi-Fi para ESP32
#include <HTTPClient.h>             // Cliente HTTP para envio via ThingSpeak

// Pinos do sensor ultrassônico
#define TRIG_PIN 5
#define ECHO_PIN 18

// LEDs para indicar o nível da água
#define LED_VERDE 13
#define LED_AMARELO 12
#define LED_VERMELHO 14

// Configurações da rede Wi-Fi e API do ThingSpeak
const char* ssid = "Wokwi-GUEST";          // Rede Wi-Fi simulada no Wokwi
const char* password = "";                 // Sem senha no Wokwi-GUEST
const char* server = "http://api.thingspeak.com/update";  // Endpoint de envio
String apiKey = "N18H1NFNM1SSJCND";        // Chave da API do canal ThingSpeak

// LCD no endereço I2C 0x27 com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RTC DS3231 para registrar horário da medição
RTC_DS3231 rtc;

// Parâmetros do reservatório
const float altura_total_cm = 400.0; // Altura total do tanque
const float raio_cm = 50.0;          // Raio da base do tanque

float distanciaAnterior = -1;  // Última distância medida (evita envio repetido)

// Função de inicialização
void setup() {
  Serial.begin(115200);  // Inicializa a comunicação serial

  // Configura os pinos dos sensores e LEDs
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // Inicializa comunicação I2C e o LCD
  Wire.begin(21, 22);      // Pinos padrão I2C do ESP32
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nivel:       cm");
  lcd.setCursor(0, 1);
  lcd.print("Volume:      L");

  // Inicializa o RTC
  if (!rtc.begin()) while (1); // Para tudo se RTC falhar
  if (rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Ajusta horário

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
}

// Função para medir a distância da água usando o sensor ultrassônico
float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH, 60000);  // Tempo do eco (timeout: 60ms)
  if (duracao == 0) return -1;                    // Retorna erro se não houver retorno
  return duracao * 0.0343 / 2;                    // Converte para centímetros
}

// Função para acender LEDs conforme nível da água
void atualizarLEDs(float nivel) {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  if (nivel <= 200) digitalWrite(LED_VERDE, HIGH);         // Nível bom
  else if (nivel <= 300) digitalWrite(LED_AMARELO, HIGH);  // Alerta
  else digitalWrite(LED_VERMELHO, HIGH);                   // Crítico
}

// Função para exibir dados no LCD
void atualizarLCD(float nivel, float volume) {
  lcd.setCursor(7, 0);
  lcd.print("     ");       // Limpa antes de imprimir
  lcd.setCursor(7, 0);
  lcd.print(nivel, 0);      // Nível da água sem casas decimais

  lcd.setCursor(8, 1);
  lcd.print("     ");
  lcd.setCursor(8, 1);
  lcd.print(volume, 0);     // Volume da água sem casas decimais
}

// Função para enviar dados ao ThingSpeak via HTTP
void enviarParaThingSpeak(float nivel, float volume, String horario, float gauge) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Monta URL com parâmetros
    String url = server;
    url += "?api_key=" + apiKey;
    url += "&field1=" + String(nivel, 2);     // campo 1: nível da água
    url += "&field2=" + String(volume, 2);    // campo 2: volume
    url += "&field3=" + horario;              // campo 3: horário
    url += "&field4=" + String(gauge);        // campo 4: redundância da medição

    http.begin(url);                // Inicia a requisição
    int httpCode = http.GET();      // Envia GET para o ThingSpeak

    if (httpCode > 0) {
      Serial.println("Enviado para ThingSpeak!");
    } else {
      Serial.print("Erro ao enviar: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end(); // Finaliza a conexão
  } else {
    Serial.println("WiFi desconectado");
  }
}

// Loop principal do código
void loop() {
  float distancia = medirDistancia();

  // Verifica se a leitura é válida
  if (distancia < 0 || distancia > altura_total_cm) {
    lcd.setCursor(0, 0);
    lcd.print("Erro leitura     ");
    Serial.println("Erro de leitura");
    delay(1000);
    return;
  }

  // Calcula o nível da água e volume do cilindro
  float nivel_agua = distancia;
  float volume_cm3 = 3.1416 * pow(raio_cm, 2) * nivel_agua;
  float volume_litros = volume_cm3 / 1000.0;

  atualizarLEDs(nivel_agua);  // Mostra estado via LEDs

  // Verifica se houve variação significativa antes de enviar
  if (abs(distancia - distanciaAnterior) > 0.5) {
    atualizarLCD(nivel_agua, volume_litros);

    // Captura horário atual do RTC
    DateTime now = rtc.now();
    String horarioFormatado = String(now.hour()) + ":" +
                              String(now.minute()) + ":" +
                              String(now.second());

    // Exibe informações no monitor serial
    Serial.print("[");
    Serial.print(horarioFormatado);
    Serial.print("] Nivel: ");
    Serial.print(nivel_agua, 2);
    Serial.print(" cm | Volume: ");
    Serial.print(volume_litros, 2);
    Serial.println(" L");

    // Envia para ThingSpeak
    enviarParaThingSpeak(nivel_agua, volume_litros, horarioFormatado, nivel_agua);

    // Atualiza a última leitura válida
    distanciaAnterior = distancia;
  }

  delay(5000); // Aguarda 5 segundos entre leituras
}

```

## Código-fonte
### ☁️ Wokwi com Node-RED via MQTT
---
### ⚙️ Componentes Utilizados
- ESP32
- Sensor Ultrassônico (HC-SR04)
- RTC (DS3231)
- Display LCD 16x2 com I2C
- LEDs (verde, amarelo, vermelho)
- Broker MQTT público: [HiveMQ](http://broker.hivemq.com)
- Wi-Fi
  
### ⚙️ Bibliotecas Utilizadas
- `Wire.h` – Comunicação I2C
- `LiquidCrystal_I2C.h` – Controle do LCD
- `RTClib.h` – Relógio de tempo real (RTC)
- `WiFi.h` – Conexão com rede Wi-Fi
- `PubSubClient.h` – Comunicação com servidor MQTT
- `ArduinoJson.h` – Criação e envio de objetos JSON
---
### ⚙️ Fluxo de Funcionamento

1. O **sensor ultrassônico** mede a distância da água até o topo.
2. A distância é convertida em:
   - **Nível da água** (em cm)
   - **Volume** (em litros)
3. Os dados são:
   - Exibidos no **LCD**
   - Indicados por **LEDs**
   - Enviados via **MQTT** no formato JSON
---
### ⚙️ Tópico MQTT
- **Broker:** `broker.hivemq.com`
- **Tópico:** `globalsolution/medidor/nivel`
- **Porta:** 1883 (padrão MQTT)
---
### ⚙️ Formato da mensagem JSON enviada:
```json
{
  "nivel_cm": 182.34,
  "volume_litros": 14344.67,
  "distancia_cm": 182.34,
  "horario": "14:26:05"
}
```
### ☁️ Wokwi com Node-RED via MQTT :
```cpp
// Bibliotecas necessárias para comunicação, sensores e MQTT
#include <Wire.h>                    // Comunicação I2C
#include <LiquidCrystal_I2C.h>      // Controle do display LCD 16x2 via I2C
#include <RTClib.h>                 // RTC DS3231 para data e hora
#include <WiFi.h>                   // Conexão Wi-Fi (ESP32)
#include <PubSubClient.h>          // Comunicação MQTT
#include <ArduinoJson.h>           // Para formatar os dados em JSON

// Pinos do sensor ultrassônico
#define TRIG_PIN 5
#define ECHO_PIN 18

// Pinos dos LEDs indicadores de nível
#define LED_VERDE 13
#define LED_AMARELO 12
#define LED_VERMELHO 14

// Dados da rede Wi-Fi simulada no Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Broker MQTT (HiveMQ público) e tópico onde os dados serão publicados
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "globalsolution/medidor/nivel";

// Criação dos objetos de rede
WiFiClient espClient;
PubSubClient client(espClient);

// LCD I2C no endereço 0x27, 16 colunas x 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Objeto para acessar o RTC
RTC_DS3231 rtc;

// Medidas do reservatório
const float altura_total_cm = 450.0;  // Altura máxima do reservatório
const float raio_cm = 50.0;           // Raio do reservatório

float distanciaAnterior = -1;  // Usado para evitar envio repetido de dados semelhantes

// Função para conectar ao Wi-Fi
void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado!");
}

// Função para reconectar ao MQTT caso a conexão caia
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32Client1234")) {  // Nome do cliente MQTT
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando de novo em 2s");
      delay(2000);
    }
  }
}

// Mede a distância da água usando o sensor ultrassônico
float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 60000);  // Tempo do eco (timeout 60ms)
  if (duracao == 0) return -1;                    // Retorna -1 em caso de falha
  return duracao * 0.0343 / 2;                    // Converte para centímetros
}

// Atualiza os LEDs conforme o nível da água
void atualizarLEDs(float nivel) {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  if (nivel <= 200) digitalWrite(LED_VERDE, HIGH);        // Nível bom
  else if (nivel <= 300) digitalWrite(LED_AMARELO, HIGH); // Alerta
  else digitalWrite(LED_VERMELHO, HIGH);                  // Crítico
}

// Atualiza o display LCD com nível e volume
void atualizarLCD(float nivel, float volume) {
  lcd.setCursor(7, 0);    // Posição para mostrar nível
  lcd.print("     ");     // Limpa área
  lcd.setCursor(7, 0);
  lcd.print(nivel, 0);    // Mostra nível (sem decimais)

  lcd.setCursor(8, 1);    // Posição para mostrar volume
  lcd.print("     ");
  lcd.setCursor(8, 1);
  lcd.print(volume, 0);   // Mostra volume (sem decimais)
}

// Envia os dados via MQTT no formato JSON
void enviarMQTT(float nivel, float volume, String horario, float distancia) {
  StaticJsonDocument<256> doc;
  doc["nivel_cm"] = nivel;
  doc["volume_litros"] = volume;
  doc["distancia_cm"] = distancia;
  doc["horario"] = horario;

  char buffer[256];
  serializeJson(doc, buffer);  // Converte para string JSON

  if (!client.publish(mqtt_topic, buffer)) {
    Serial.println("Erro ao publicar MQTT");
  } else {
    Serial.println("MQTT publicado:");
    Serial.println(buffer);  // Mostra o JSON enviado
  }
}

// Configurações iniciais
void setup() {
  Serial.begin(115200);  // Inicia porta serial para debug

  // Configura os pinos
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // Inicializa o barramento I2C com pinos padrão do ESP32 (GPIO21 = SDA, GPIO22 = SCL)
  Wire.begin(21, 22);

  // Inicia o LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nivel:       cm");
  lcd.setCursor(0, 1);
  lcd.print("Volume:      L");

  // Inicia o RTC
  if (!rtc.begin()) {
    Serial.println("Erro no RTC");
    while (1);  // Para o programa se falhar
  }

  // Ajusta o RTC se estiver sem energia
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  setup_wifi();                 // Conecta ao Wi-Fi
  client.setServer(mqtt_server, 1883);  // Configura o broker MQTT
}

// Loop principal do programa
void loop() {
  if (!client.connected()) {
    reconnect();  // Reconecta MQTT se cair
  }
  client.loop();  // Mantém a comunicação ativa

  float distancia = medirDistancia();

  // Verifica se leitura da distância é válida
  if (distancia < 0 || distancia > altura_total_cm) {
    lcd.setCursor(0, 0);
    lcd.print("Erro leitura     ");
    Serial.println("Erro de leitura");
    delay(1000);
    return;
  }

  // Calcula o nível da água e o volume do reservatório
  float nivel_agua = distancia;
  float volume_cm3 = 3.1416 * pow(raio_cm, 2) * nivel_agua;
  float volume_litros = volume_cm3 / 1000.0;

  atualizarLEDs(nivel_agua);  // Atualiza LEDs conforme o nível

  // Evita envio se não houver alteração significativa
  if (abs(distancia - distanciaAnterior) > 0.5) {
    atualizarLCD(nivel_agua, volume_litros);  // Atualiza display LCD

    DateTime now = rtc.now();
    String horario = String(now.hour()) + ":" +
                     String(now.minute()) + ":" +
                     String(now.second());

    // Exibe no monitor serial
    Serial.printf("[%s] Nivel: %.2f cm | Volume: %.2f L\n", horario.c_str(), nivel_agua, volume_litros);

    // Envia dados via MQTT
    enviarMQTT(nivel_agua, volume_litros, horario, distancia);

    // Armazena a última distância registrada
    distanciaAnterior = distancia;
  }

  delay(3000);  // Aguarda 3 segundos antes da próxima medição
}
```
## Integrantes
- Eduardo Eiki de Sousa Lopes Yamada RM 554921
- Nicollas Frei Eleoterio RM 557647
- Heitor Pereira Duarte RM 558208







