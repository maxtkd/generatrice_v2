#include <WiFi.h>
#include <WiFiClient.h>

// WiFi credentials
const char* ssid = "RP4Pelletier";
const char* password = "jp1234567890";

// Raspberry Pi IP address and port
const char* raspberryPiIP = "10.42.0.1";  // Replace with Raspberry Pi's IP
const int raspberryPiPort = 12345;

/*
// Wifi
WiFiClient client;
IPAddress server(10,42,0,1);
int wifiStatus = WL_IDLE_STATUS;
const char ssid[] = "RP4Pelletier";
const char password[] = "jp1234567890";
*/

WiFiClient client;

void setup() {
  Serial.begin(9600);  // ESP32 <-> Arduino communication on Serial

  // Start WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Connect to Raspberry Pi
  while (!client.connect(raspberryPiIP, raspberryPiPort)) {
    Serial.println("Connecting to Raspberry Pi...");
    delay(1000);
  }
  Serial.println("Connected to Raspberry Pi!");
}

void loop() {
  // Check for messages from Raspberry Pi
  if (client.available()) {
    String commandFromPi = client.readStringUntil('\n');

    // Relay command to Arduino
    Serial.println(commandFromPi);
  }

  // Check for messages from Arduino
  if (Serial2.available()) {
    String commandFromArduino = Serial2.readStringUntil('\n');

    // Relay the command to Raspberry Pi
    client.print(commandFromArduino + "\n");
  }
}
