// Daragh Kearney / IoT Smart Home System //
// temperature and humidity to ThingSpeak graphs //

#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// WiFi 
const char* ssid = "SKYHGSM1";
const char* password = "yDtPgkUALtAR";

// ThingSpeak 
unsigned long channelID = 2713003;
const char* writeAPIKey = "6CPF9H3D5KCSZGFT";
// field 1 = temperature, field 2 = humidity, field 5 = motion

#define PIR_PIN 2
#define LED_PIN 4
#define DHT_PIN 15
#define DHT_TYPE DHT11 // Sensor type

WiFiClient client;
DHT dht(DHT_PIN, DHT_TYPE);

bool motionDetected = false; //Motion state
float temperature = 0.0; //starting values
float humidity = 0.0;

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();
  connectToWiFi();
  ThingSpeak.begin(client);

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
