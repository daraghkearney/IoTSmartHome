// Daragh Kearney / IoT Smart Home System //
// Added OLED display for displaying data //
// Improved Comments and Comment structure //

#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi 
const char* ssid = "SKYHGSM1";
const char* password = "yDtPgkUALtAR";

// ThingSpeak 
unsigned long channelID = 2713003;
const char* writeAPIKey = "6CPF9H3D5KCSZGFT";
// Field 1 = temperature, Field 2 = humidity, Field 5 = motion

#define PIR_PIN 2
#define LED_PIN 4
#define DHT_PIN 15
#define DHT_TYPE DHT11

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

WiFiClient client;
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool motionDetected = false;
float temperature = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();
  connectToWiFi();
  ThingSpeak.begin(client);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true); // Stop if OLED fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 25);
  display.println("System Starting...");
  display.display();
  delay(2000);

  Serial.println("Setup complete.");
}

void loop() {
  motionDetected = digitalRead(PIR_PIN);
  if (motionDetected) {
    Serial.println("Motion detected!");
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println("No motion detected.");
    digitalWrite(LED_PIN, LOW);
  }

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT11 sensor");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    updateOLED();
    sendToThingSpeak();
  }

  delay(2000);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP Address: " + WiFi.localIP().toString());
}

void sendToThingSpeak() {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(5, motionDetected ? 1 : 0);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.printf("ThingSpeak error: %d\n", status);
  }
}

void updateOLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Smart Home Status");

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 28);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 40);
  display.print("Motion: ");
  display.println(motionDetected ? "Yes" : "No");

  display.setCursor(0, 52);
  display.print("Light: ");
  display.println(motionDetected ? "ON" : "OFF");

  display.display();
}
