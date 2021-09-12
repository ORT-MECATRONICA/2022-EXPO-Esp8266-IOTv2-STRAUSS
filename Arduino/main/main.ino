#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>
#include <FastLED.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER

#define REL_PINA 13
#define REL_PINB 14
#define LED_PIN 12

// Cambiar por personales
#define FIREBASE_HOST "https://esp8266-iot-947ea-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "V79839Ek0y82X8rK7tnZMEGlE6fCJNSmTcy2rK9p"
#define WIFI_SSID "CABLEVISION-eec1"
#define WIFI_PASSWORD "1805BMISBQHA"

#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LEDS 1

#define DHTPIN 15
#define DHTTYPE DHT11

FirebaseData firebaseData;

DHT dht(DHTPIN, DHTTYPE);

uint8_t max_bright = 100;
struct CRGB leds[NUM_LEDS];

int estadoLed, inputVal;
float valorAnterior;
float temp;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(REL_PINA, OUTPUT);
  pinMode(REL_PINB, OUTPUT);

  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("_");
  Serial.println("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Firebase.setInt(firebaseData, "led", 0);
  Firebase.setInt(firebaseData, "barra", 0);
}

void loop() {
  if (Firebase.getInt(firebaseData, "led/")) { // Si pongo muchos de estos se bugea mucho
    estadoLed = firebaseData.intData();
  }

  delay(10);

  if (estadoLed == 0) {
    rainbowOff();
  }
  else if (estadoLed == 1) {
    rainbowOn(1, 1);
    FastLED.show();
  }
  else if (estadoLed == 2) {
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
  }
  else if (estadoLed == 3) {
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.show();
  }
  else if (estadoLed == 4) {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    FastLED.show();
  }
  else if (estadoLed == 5) {
    digitalWrite(REL_PINA, HIGH);
  }
  else if (estadoLed == 6) {
    digitalWrite(REL_PINA, LOW);
  }
  else if (estadoLed == 7) {
    digitalWrite(REL_PINB, HIGH);
  }
  else if (estadoLed == 8) {
    digitalWrite(REL_PINB, LOW);
  }
  else if (estadoLed == 9) {
    poteLed();
  }
  // digitalWrite(REL_PINA, LOW);
  // digitalWrite(REL_PINB, LOW);

  temperature();
}

void poteLed() {
  Firebase.getInt(firebaseData, "barra/");
  inputVal = firebaseData.intData(); // De 0 a 255

  uint8_t hue = inputVal / 4;
  CHSV color = CHSV(hue, 255, 255);
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

void temperature() {
  if (millis() - valorAnterior >= 10000) {
    valorAnterior = millis();
    temp = dht.readTemperature();
    // Serial.println(String(temp) + " " + String(millis()));

    Firebase.setFloat(firebaseData, "Temperatura", temp);
  }
}

void rainbowOn(uint8_t thisSpeed, uint8_t deltaHue) {
  // uint8_t thisHue = beatsin8(thisSpeed,0,255);
  uint8_t thisHue = beat8(thisSpeed, 255);
  fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);
}

void rainbowOff() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}
