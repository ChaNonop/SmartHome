#include <WiFi.h>
// #include <PubSubClient.h>
// #include "DHT.h"
// #include "Arduino_LED_Matrix.h"

// Pin definitions
const int ldrPin = A1; // Pin for LDR
#define LED_PIN 11 // Pin for LED
#define DHTPIN 2   // Pin for DHT11 sensor
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor
ArduinoLEDMatrix matrix;  // Initialize LED matrix

// WiFi credentials
const char* ssid = "Seek";
const char* password = "12345679";

// MQTT broker settings
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "47eb28e0-0fe7-4eed-b90f-b86cd49baa38";
const char* mqtt_username = "tVgNN3VMTFJzBEYoyj5pVEj23aM9Pc3c";
const char* mqtt_password = "x6pR5J5fFmRHLDAonsigbBWozLd33oZpH";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[100];

// Predefined frames for LED matrix
const uint32_t on_off[][3] = {
  {0x39, 0x245a4564, 0x52392000}, // ON 
  {0x67, 0x79449779, 0x44644000}  // OFF
};

const char* lux_topic = "@shadow/data/lux";

// Function to connect to WiFi
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to reconnect to MQTT broker
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/operator");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

// Callback function for received MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);
  
  if (String(topic) == "@msg/operator") {
    if (message == "ON") {
      matrix.loadFrame(on_off[0]); // ON frame
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON");
    } else if (message == "OFF") {
      matrix.loadFrame(on_off[1]); // OFF frame
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF");
    }
  }
}

// Function to publish sensor data to MQTT
void publishSensorData(float temp, float humidity, float Lux) {
  snprintf(msg, sizeof(msg), "{\"data\":{\"temperature\":%.2f,\"humidity\":%.2f,\"lux\":%.2f}}", temp, humidity, Lux);
  Serial.print("Publishing message: ");
  Serial.println(msg);
  client.publish("@shadow/data/update", msg);
}

// Function to calculate Lux from resistance value
float calculateLux(float resistance) {
  if (resistance <= 0) return 0; // Prevent division by zero
  return (resistance > 20000) ? 20000 : (102200.0 / resistance); // Cap lux at 20000
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  dht.begin(); // Initialize DHT sensor
  matrix.begin(); // Start Matrix Display
  
  connectToWiFi(); // Connect to WiFi
  
  client.setServer(mqtt_server, mqtt_port); // Set MQTT server
  client.setCallback(callback); // Set MQTT callback
}

void loop() { 
  if (!client.connected()) {
    reconnectMQTT(); // Reconnect to MQTT broker if necessary
  }
  client.loop(); // Maintain MQTT connection
  
  long now = millis();
  if (now - lastMsg > 5000) { // Publish data every 5 seconds
    lastMsg = now;

    // Read sensor data
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int ldrValue = analogRead(ldrPin); // Read from LDR
    if(ldrValue != 0) {
      float resistance = ((1023.0 / ldrValue) - 1) * 1000; // Calculate LDR resistance
      float Lux = calculateLux(resistance); // Calculate lux
    
      // Error handling for DHT sensor
      if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
      } else {
        Serial.print("Lux: ");
        Serial.println(Lux);
      
        publishSensorData(t, h, Lux); // Publish data to MQTT
      }
    }
  }
}