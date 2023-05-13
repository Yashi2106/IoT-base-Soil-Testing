#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define WIFI_SSID "SB"
#define WIFI_PASS "sushil9876"
#define THINGSPEAK_CHANNEL_ID  2102197
#define THINGSPEAK_API_KEY "MWDI6ZS7606WMYMB"

#define DHTPIN D3          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11      // DHT 11

#define RAIN_PIN D0        // Digital pin connected to the rain sensor
#define MOISTURE_PIN D1    // Digital pin connected to the moisture sensor

float ph = random(59,71)/10;   // Set default pH value
float N_SOIL = random(40.0,70.0); // Set default nitrogen value
float P_SOIL = random(50.0,71.0); // Set default phosphorous value
float K_SOIL = random(59,71); // Set default potassium value

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10L * 1000L; // post data every 10 seconds

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  ThingSpeak.begin(client);

  pinMode(RAIN_PIN, INPUT);
  pinMode(MOISTURE_PIN, INPUT);

  dht.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    ThingSpeak.begin(client);
  }

  unsigned long currentTime = millis();
  if (currentTime - lastConnectionTime >= postingInterval) {
    float TEMPERATURE = dht.readTemperature();
    float HUMIDITY = dht.readHumidity();
    int RAINFALL = digitalRead(RAIN_PIN);
    int MOISTURE = analogRead(MOISTURE_PIN);

    Serial.print("Nitrogen: ");
    Serial.print(N_SOIL);
     Serial.print("    ");
    Serial.print("Phosphorous: ");
    Serial.print(P_SOIL);
    Serial.print("    ");
    Serial.print("Potassium: ");
    Serial.print(K_SOIL);
    Serial.print("    ");
    Serial.print("Temperature: ");
    Serial.print(TEMPERATURE);
    Serial.print("°C    ");
    Serial.print("Humidity: ");
    Serial.print(HUMIDITY);
    Serial.print("%    ");
     Serial.print("pH: ");
    Serial.print(ph);
     Serial.print("    ");
    Serial.print("Rain: ");
    Serial.print(RAINFALL);
    Serial.print("    ");
    Serial.print("Moisture: ");
    Serial.print(MOISTURE);
    

    ThingSpeak.setField(1, N_SOIL);
    ThingSpeak.setField(2, P_SOIL );
    ThingSpeak.setField(3, K_SOIL);
    ThingSpeak.setField(4, TEMPERATURE);
    ThingSpeak.setField(5, HUMIDITY);
    ThingSpeak.setField(6, ph);
    ThingSpeak.setField(7, RAINFALL);
    ThingSpeak.setField(8, MOISTURE);

    int responseCode = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
    if (responseCode == 200) {
      Serial.println("Data sent to ThingSpeak successfully.");
    } else {
      Serial.print("Error sending data to ThingSpeak. Response code: ");
      Serial.println(responseCode);
    }

    lastConnectionTime = currentTime;
  }

  delay(1000);
}

