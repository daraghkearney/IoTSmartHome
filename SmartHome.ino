// Daragh Kearney / IoT Smart Home System //
// Added DHT11 temperature and humidity sensor instead of using BME280 //

#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// WiFi 
const char* ssid = "SKYHGSM1";
const char* password = "yDtPgkUALtAR";

// ThingSpeak 
unsigned long channelID = 2713003;
const char* writeAPIKey = "6CPF9H3D5KCSZGFT";
// Field 5 ThingSpeak is motion status (1 = motion, 0 = no motion)

#define PIR_PIN 2     
#define LED_PIN 4     

#define DHT_PIN 15     
#define DHT_TYPE DHT11 // Sensor type

// WiFi client
WiFiClient client;

// Motion state
bool motionDetected = false;

// DHT sensor object
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED off at first

  dht.begin();  // Start the DHT11 sensor

  connectToWiFi();
  ThingSpeak.begin(client);

  Serial.println("Setup complete.");
}

void loop() {
  // Read motion sensor
  motionDetected = digitalRead(PIR_PIN);

  // LED turns on if motion is detected
  if (motionDetected) {
    Serial.println("Motion detected!");
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println("No motion detected.");
    digitalWrite(LED_PIN, LOW);
  }

  // Read DHT11 values
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT11 sensor");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // Send motion to my thingspeak graph (field 5)
  sendToThingSpeak();

  delay(2000); // Wait 2 seconds before next reading
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
