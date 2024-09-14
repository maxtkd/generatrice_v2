/* Wi-Fi STA Connect and Disconnect Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
   

#include <WiFi.h>
#include <HardwareSerial.h>

// Wifi
WiFiClient client;
IPAddress server(10,42,0,1);
int wifiStatus = WL_IDLE_STATUS;
const char ssid[] = "RP4Pelletier";
const char password[] = "jp1234567890";

// Sensor
#define RXD2 16
#define TXD2 17
long duration, distance;

HardwareSerial SerialPort(2); // use UART2


String get_wifi_status(int status){
    switch(status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
}

void setup2(){
    Serial.begin(115200);
    SerialPort.begin(9600, SERIAL_8N1, RXD2, TXD2);
    delay(1000);   

    // wifiScan();
    connectToWifi();
}

void loop2(){
  int dist = readSensor();
  float inchFactor = 0.0393701;

  float pourcent = (1 - (dist * inchFactor) / 27 ) * 100;

   char buff[10];
  
  if (client.connect(server, 5000)) {
      Serial.println("connected");
      // Make a HTTP request:
      String str = "GET /waterlevel/";
      client.print(str);
      client.println(pourcent);
      client.println();
    }
  
  delay(1000);
}

int readSensor(){
  int high, low, checksum, header;
  while (SerialPort.available()) {
    header = SerialPort.read();
    if (header == 0xff) {
      high = SerialPort.read();
      low = SerialPort.read();
      checksum = SerialPort.read();
      SerialPort.flush();
      distance = ((high<<8)+low);
    }
  }
  Serial.print("Distance mm: ");
  Serial.println(distance);
  return distance;
}

void connectToWifi(){
  WiFi.begin("RP4Pelletier");
  while(wifiStatus != WL_CONNECTED){
      delay(500);
      wifiStatus = WiFi.status();
      Serial.println(get_wifi_status(wifiStatus));
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void wifiScan() {
    Serial.println("Scan start");

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");

    if (n == 0) {
        Serial.println("no networks found");
    } else {
        for (int i = 0; i < n; ++i) {
          Serial.println(WiFi.SSID(i).c_str());
          if(WiFi.SSID(i) == ssid){
            Serial.println("\nConnecting");
            Serial.println(get_wifi_status(wifiStatus));
            break;
          }
          
        }
    }
    Serial.println("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();
}*/