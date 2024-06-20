#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <BH1750.h>
#include <Wire.h>
#include<ArduinoJson.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your wifi password";

unsigned long channelID = YOUR_channelID;
const char* apiKey = "YOUR_apiKey";
const char* readapiKey = "YOUR_read_apiKey";

#define DHTPIN 26
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
WiFiClient client;

const int ledPin1 = 25; //device 1 at GPIO 25 of ESP32
const int ledPin2 = 27; //device 2 at GPIO 27 of ESP32
const int ledPin3 = 33; //device 3 at GPIO 33 of ESP32

void setup() {

  Serial.begin(115200);
  delay(10);

//code for wifi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  dht.begin();
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
}

void loop() {

  // Read sensor values
  float gas = analogRead(A0);  //READ GAS SENSOR VALUES
  float temperature = dht.readTemperature();  //READ TEMPERATURE VALUES WITH DHT SENSOR
  float humidity = dht.readHumidity();  //READ HUMIDITY VALUES WITH DHT SENSOR
  float lux = lightMeter.readLightLevel();   //READ LUX VALUES WITH BH1750 SENSOR
      
   if (WiFi.status() == WL_CONNECTED) {

    // esp will Read data from ThingSpeak and will do controlling of each device accordingly, values in these fields are sent from mit app.
    int ledValue1 = ThingSpeak.readIntField(channelID, 5, readapiKey);
    int ledValue2 = ThingSpeak.readIntField(channelID, 6, readapiKey);
    int ledValue3 = ThingSpeak.readIntField(channelID, 7, readapiKey);

    Serial.print("Response Code: ");
    Serial.println(ThingSpeak.getLastReadStatus());
    Serial.print("LED Value 1: ");
    Serial.println(ledValue1);

    Serial.print("LED Value 2: ");
    Serial.println(ledValue2);

    Serial.print("LED Value 3: ");
    Serial.println(ledValue3);

    // Check the value and control the LED
    if (ledValue1 == 1) {
      digitalWrite(ledPin1, HIGH); // Turn on the LED
      Serial.println("LED 1 ON");
    } else {
      digitalWrite(ledPin1, LOW); // Turn off the LED
      Serial.println("LED 1 OFF");
    }

    if (ledValue2 == 1) {
      digitalWrite(ledPin2, HIGH); // Turn on the LED
      Serial.println("LED 2 ON");
    } else {
      digitalWrite(ledPin2, LOW); // Turn off the LED
      Serial.println("LED 2 OFF");
    }
    if (ledValue3 == 1) {
      digitalWrite(ledPin3, HIGH); // Turn on the LED
      Serial.println("LED 3 ON");
    } else {
      digitalWrite(ledPin3, LOW); // Turn off the LED
      Serial.println("LED 3 OFF");
    }
    delay(10000); // Check ThingSpeak channel every 10 seconds
  }  

  // Check if readings are valid
  if (isnan(gas) || isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read sensor data!");
    return;
  }

  // Send sensor data to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, lux);
  ThingSpeak.setField(4, gas);

  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.print("Error sending data to ThingSpeak. Response code: ");
    Serial.println(response);
  }
  delay(3000); 
}
