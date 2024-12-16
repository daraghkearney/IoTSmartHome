

// Daragh Kearney / IoT Smart Home System //
// December 2024 //

// This code currently contains my motion sensor with led activation to simulate a light turning on when motion is detected in a room //
// Awaiting arrival of LCD and BME280 //
// Code for remaining sensors will be written once received //

#include <WiFi.h>
#include <ThingSpeak.h>

// WiFi 
const char* ssid = "SKYHGSM1";
const char* password = "yDtPgkUALtAR";

// ThingSpeak 
unsigned long channelID = 2713003;
const char* writeAPIKey = "6CPF9H3D5KCSZGFT";

#define PIR_PIN 2     // PIR sensor GPIO pin
#define LED_PIN 4      // LED GPIO pin

// WiFi client
WiFiClient client;

// Motion state
bool motionDetected = false;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Setup PIR sensor pin
  pinMode(PIR_PIN, INPUT);

  // Setup LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED off initially

  // Connect to WiFi
  connectToWiFi();

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  Serial.println("Setup complete.");
}

void loop() {
  // Read motion sensor
  motionDetected = digitalRead(PIR_PIN);

  // LED turns on when motion is detected
  if (motionDetected) {
    Serial.println("Motion detected!");
    digitalWrite(LED_PIN, HIGH); // Turn on LED
  } else {
    Serial.println("No motion detected.");
    digitalWrite(LED_PIN, LOW); // Turn off LED
  }

  // Send data from the motion sensor to my ThingSpeak
  sendToThingSpeak();

  delay(2000); // Wait 2 seconds before next update
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
  int status = ThingSpeak.writeField(channelID, 5, motionDetected ? 1 : 0, writeAPIKey);
  if (status == 200) {
    Serial.println("Motion status sent to ThingSpeak");
  } else {
    Serial.printf("Failed to send data to ThingSpeak. HTTP Error: %d\n", status);
  }
}
