#include <WiFi.h>
#include <ThingSpeak.h>
#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "homepage.h"
#include "secrets.h"

// WiFi
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// ThingSpeak
unsigned long channelID = SECRET_CH_ID;
const char* writeAPIKey = SECRET_WRITE_APIKEY;

// Pins
#define PIR_PIN 2
#define LED_PIN 4
#define DHT_PIN 15
#define RELAY3_PIN 26
#define DHT_TYPE DHT11
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

WiFiClient client;
WebServer server(80);
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Data
bool motionDetected = false;
float temperature = 0.0;
float humidity = 0.0;
bool manualLEDOverride = false;
bool ledOn = false;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 16000;
const float tempThreshold = 25.0;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY3_PIN, LOW);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }

  displayStartupMessage();
  connectToWiFi();
  ThingSpeak.begin(client);

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", homepageHTML);
  });

  server.on("/led", HTTP_GET, []() {
    String state = server.arg("state");
    if (state == "on") {
      manualLEDOverride = true;
      ledOn = true;
      Serial.println("LED forced ON by fetch()");
    } else if (state == "off") {
      manualLEDOverride = false;
      ledOn = false;
      Serial.println("LED forced OFF by fetch()");
    }
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    server.send(200, "text/plain", "LED state changed");
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  motionDetected = digitalRead(PIR_PIN);

  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateInterval) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      updateOLED();
      sendToThingSpeak();

      if (temperature > tempThreshold) {
        digitalWrite(RELAY3_PIN, HIGH);
        Serial.println("Relay ON: High temperature");
      } else {
        digitalWrite(RELAY3_PIN, LOW);
        Serial.println("Relay OFF: Temp below threshold");
      }
    }

    lastUpdateTime = currentTime;
  }

  if (manualLEDOverride) {
    // Keep LED as per manual state
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  } else {
    // LED follows logic: on if motion or high temp
    ledOn = motionDetected || (temperature > tempThreshold);
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  }

  server.handleClient();
  delay(50);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("ESP32 IP: " + WiFi.localIP().toString());
}

void sendToThingSpeak() {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(5, motionDetected ? 1 : 0);
  int status = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (status == 200) Serial.println("Data sent to ThingSpeak");
  else Serial.printf("ThingSpeak error: %d\n", status);
}

void updateOLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Smart Home Status");
  display.setCursor(0, 16);
  display.print("Temp: "); display.print(temperature); display.println(" C");
  display.setCursor(0, 28);
  display.print("Humidity: "); display.print(humidity); display.println(" %");
  display.setCursor(0, 40);
  display.print("Motion: "); display.println(motionDetected ? "Yes" : "No");
  display.setCursor(0, 52);
  display.print("Light: "); display.println(ledOn ? "ON" : "OFF");
  display.display();
}

void displayStartupMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 25);
  display.println("System Starting...");
  display.display();
  delay(2000);
}
