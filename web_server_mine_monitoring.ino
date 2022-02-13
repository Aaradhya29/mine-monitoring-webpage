#if ESP32
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#else
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif

// Replace with your network credentials
const char* ssid = "Aaru";
const char* password = "aaru12345";

//const int PulseWire = 4; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0 //D3
//const int LED13 = 16; // The on-board Arduino LED, close to PIN 13.//D0
//int Threshold = 550; // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.

const int TouchTemp = 0;
const int Temperature = 1;
const int MQ9 = 2;
const int MQ4 = 3;
const int Buzzer = 4;

AsyncWebServer server(80);

String TouchTemp() {
  int TouchTemp = random(60,100); // Calls function on our pulseSensor object that returns BPM as an "int".
  Serial.println("♥ A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
  Serial.print("BPM: "); // Print phrase "BPM: "
  Serial.println(TouchTemp); // Print the value inside of myBPM.
  return String(TouchTemp);
}

String TemperatureSensor() {

  int TempSensor = random(10,40);
  Serial.print("Temperature Level :");
  Serial.print(TempSensor);
  Serial.println("Celcius");
  delay(10000);

  return String(TempSensor);
}

String MQ9() {
  int MQ9 = int(random(10, 80));
  return String(MQ9);
}

String MQ4() {
  int MQ4 = random(10,25);
  return String(MQ4);
}

// Replaces placeholder with DHT values
String processor(const String& var) {
  //Serial.println(var);
  if (var == "Speed") {
    return readspeedlimit();
  }
  else if (var == "myBPM") {
    return myBPM();
  }
  else if (var == "state") {
    return readseatbeltstatus();
  }
  else if (var == "SpeedC") {
    return readspeedC();
  }

  else if (var == "pirstate") {
    return pir();
  }
  else if (var == "Tyre1") {
    return tyre1pressure();
  }
  else if (var == "Tyre2") {
    return tyre2pressure();
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/Speed", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readspeedlimit().c_str());
  });
  server.on("/myBPM", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", myBPM().c_str());
  });
  server.on("/state", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readseatbeltstatus().c_str());
  });
  server.on("/SpeedC", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readspeedC().c_str());
  });
  server.on("/pirstate", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", pir().c_str());
  });
   server.on("/Tyre1", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", tyre1pressure().c_str());
  });
   server.on("/Tyre2", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", tyre2pressure().c_str());
  });
  // Start server
  server.begin();
}

void loop() {

}
