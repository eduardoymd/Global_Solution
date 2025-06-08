#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define TRIG_PIN 5
#define ECHO_PIN 18

#define LED_VERDE 12
#define LED_AMARELO 13
#define LED_VERMELHO 14

const char* ssid = "Wokwi-GUEST";        
const char* password = "";               
const char* server = "http://api.thingspeak.com/update";
String apiKey = "N18H1NFNM1SSJCND";     
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

const float altura_total_cm = 400.0;
const float raio_cm = 50.0;

float distanciaAnterior = -1;

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

  if (!rtc.begin()) while (1);
  if (rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
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

void enviarParaThingSpeak(float nivel, float volume, String horario, float gauge) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = server;
    url += "?api_key=" + apiKey;
    url += "&field1=" + String(nivel, 2);
    url += "&field2=" + String(volume, 2);
    url += "&field3=" + horario;
    url += "&field4=" + String(gauge);

    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("Enviado para ThingSpeak!");
    } else {
      Serial.print("Erro ao enviar: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi desconectado");
  }
}

void loop() {
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

    String horarioFormatado = String(now.hour()) + ":" +
                              String(now.minute()) + ":" +
                              String(now.second());

    Serial.print("[");
    Serial.print(horarioFormatado);
    Serial.print("] Nivel: ");
    Serial.print(nivel_agua, 2);
    Serial.print(" cm | Volume: ");
    Serial.print(volume_litros, 2);
    Serial.println(" L");

    enviarParaThingSpeak(nivel_agua, volume_litros, horarioFormatado, nivel_agua);
    distanciaAnterior = distancia;
  }

  delay(5000); 
}
