#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <FastLED.h>

#define REL_PINA 13
#define REL_PINB 14
#define LED_DT 12

// Cambiar por personales
#define FIREBASE_HOST "https://pythondht11-4ce74-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "5Ir4SU0d8EFs2hZv3imkQszchcJxCDG8v7TzEYpa"
#define WIFI_SSID "CABLEVISION-eec1"
#define WIFI_PASSWORD "1805BMISBQHA"

#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LEDS 1

FirebaseData firebaseData;

uint8_t max_bright = 255;
struct CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);

  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
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

  Firebase.setBool(firebaseData, "led", false);
}

void loop() {
  Firebase.getBool(firebaseData, "led");
  bool estado = firebaseData.boolData();
  if (estado == true) {
    rainbow();
  }
  else {
    leds[0] = CRGB::Black;
    FastLED.show();
  }
}

void rainbow() {
  uint8_t thisSpeed = 10;
  uint8_t deltaHue = 10;
  uint8_t thisHue = beat8(thisSpeed, 255);
  fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);
  FastLED.show();
}
