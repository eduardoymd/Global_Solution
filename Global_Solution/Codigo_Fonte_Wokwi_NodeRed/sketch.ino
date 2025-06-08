#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define TRIG_PIN 5
#define ECHO_PIN 18

#define LED_VERDE 13
#define LED_AMARELO 12
#define LED_VERMELHO 14

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "globalsolution/medidor/nivel";

WiFiClient espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

const float altura_total_cm = 450.0;
const float raio_cm = 50.0;

float distanciaAnterior = -1;

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32Client1234")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando de novo em 2s");
      delay(2000);
    }
  }
}

float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH, 60000);
  if (duracao == 0) return -1;
  return duracao * 0.0343 / 2;
}

void atualizarLEDs(float nivel) {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  if (nivel <= 200) digitalWrite(LED_VERDE, HIGH);
  else if (nivel <= 300) digitalWrite(LED_AMARELO, HIGH);
  else digitalWrite(LED_VERMELHO, HIGH);
}

void atualizarLCD(float nivel, float volume) {
  lcd.setCursor(7, 0);
  lcd.print("     ");
  lcd.setCursor(7, 0);
  lcd.print(nivel, 0);
  lcd.setCursor(8, 1);
  lcd.print("     ");
  lcd.setCursor(8, 1);
  lcd.print(volume, 0);
}

void enviarMQTT(float nivel, float volume, String horario, float distancia) {
  StaticJsonDocument<256> doc;
  doc["nivel_cm"] = nivel;
  doc["volume_litros"] = volume;
  doc["distancia_cm"] = distancia;
  doc["horario"] = horario;

  char buffer[256];
  serializeJson(doc, buffer);

  if (!client.publish(mqtt_topic, buffer)) {
    Serial.println("Erro ao publicar MQTT");
  } else {
    Serial.println("MQTT publicado:");
    Serial.println(buffer);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nivel:       cm");
  lcd.setCursor(0, 1);
  lcd.print("Volume:      L");

  if (!rtc.begin()) {
    Serial.println("Erro no RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float distancia = medirDistancia();
  if (distancia < 0 || distancia > altura_total_cm) {
    lcd.setCursor(0, 0);
    lcd.print("Erro leitura     ");
    Serial.println("Erro de leitura");
    delay(1000);
    return;
  }

  float nivel_agua = distancia;
  float volume_cm3 = 3.1416 * pow(raio_cm, 2) * nivel_agua;
  float volume_litros = volume_cm3 / 1000.0;

  atualizarLEDs(nivel_agua);

  if (abs(distancia - distanciaAnterior) > 0.5) {
    atualizarLCD(nivel_agua, volume_litros);

    DateTime now = rtc.now();
    String horario = String(now.hour()) + ":" +
                     String(now.minute()) + ":" +
                     String(now.second());

    Serial.printf("[%s] Nivel: %.2f cm | Volume: %.2f L\n", horario.c_str(), nivel_agua, volume_litros);

    enviarMQTT(nivel_agua, volume_litros, horario, distancia);

    distanciaAnterior = distancia;
  }

  delay(3000);
}
