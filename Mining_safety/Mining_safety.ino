#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#include <Wire.h>
#include <Adafruit_BMP085.h>

#include <SoftwareSerial.h> 
#include <TinyGPS.h> 

//Need to define the Pin that we attach from sensor's Data pin
#define DHT_Pin 16 //D0
#define MQ9_Pin  2 //D4
#define MQ4_Pin 0 //D3
#define Buzzer_Pin 14 //D5

#define DHTTYPE DHT11 // DHT 11
#define seaLevelPressure_hPa 1013.25

#define BMP_SDA 4 //D2 SDA
#define BMP_SCL 5 //D1 SCL

float th_limit_mq9 = 300.00;
float th_limit_mq4 = 200.00;

float lat = 28.5458,lon = 77.1703; // create variable for latitude and longitude object  

// Replace with your network credentials
const char* ssid = "Mihir";
const char* password = "9167276705";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

DHT dht(DHT_Pin, DHTTYPE);

Adafruit_BMP085 bmp;

SoftwareSerial gpsSerial(3,1);//rx,tx | rx -> TX of ESP8266 and tx-> RX of ESP8266
TinyGPS gps;

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;

unsigned int buzzer_status = 0;

long latitude,longitude;
float dht_humid;
float dht_temp;
float mq4_read;
float mq9_read;
float bmp_pres_read;
float bmp_temp_read;

void SenseHumid(){
  
  dht_humid = dht.readHumidity(); //use random function if sensor is not available
  dht_temp = dht.readTemperature(); // use random function if sensor is not available, or dht.readTemperature(true) for Fahrenheit

  if (isnan(dht_humid) || isnan(dht_temp)) {
  Serial.println("Failed to read from DHT sensor!");
  }
}

//void SenseBTemp() {
//  float reading = analogRead(LM35_Pin);
//  // Convert that reading into voltage
//  float voltage = reading * (5.0 / 1024.0);
//  // Convert the voltage into the temperature in Celsius
//  float BTemp = voltage * 100;
//
//return BTemp;
//}

//Sense the Methane Gas
void SenseMQ4() {
  mq4_read = digitalRead(MQ4_Pin);
}

void SenseMQ9() {
  mq9_read = digitalRead(MQ9_Pin);
}

void SensePressure() {
  
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }

  bmp_temp_read = bmp.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(bmp_temp_read);
  Serial.println(" *C");

  bmp_pres_read = bmp.readPressure();
  Serial.print("Pressure = ");
  Serial.print(bmp_pres_read);
  Serial.println(" Pa");

return;
}

void Sensegps() {
  while(gpsSerial.available()){ // check for gps data 
  if(gps.encode(gpsSerial.read()))// encode gps data 
  {  
  gps.f_get_position(&lat,&lon); // get latitude and longitude 
  }

  String latitude = String(lat,6); 
  String longitude = String(lon,6); 
  Serial.println(latitude+";"+longitude);
  }
return;
}

void CheckBuzzer() {
  
  if((mq4_read >= th_limit_mq4) || (mq9_read >= th_limit_mq9)) {
    Serial.println("Limit Crossed, Buzzing on");
    digitalWrite(Buzzer_Pin,HIGH);
    buzzer_status = 1;
  }
  else {
    Serial.println("GAS PARAMETERS ARE OK");
    digitalWrite(Buzzer_Pin,LOW);
    buzzer_status = 0;
  }
  
return;
}

// Get Sensor Readings and return JSON object
String getSensorReadings(){

//  SenseHumid(); //dht_humid
//  SenseMQ4(); //mq4_read
//  SenseMQ9(); //mq9_read
//  SensePressure(); //bmp_pres_read, bmp_temp_read
//  Sensegps(); //latitude,longitude
//  CheckBuzzer(); //buzzer_status;
  
  readings["temperature"] = String(random(0,25));
  //readings["temperature"] = String(bmp_temp_read);
  readings["humidity"] =  String(dht_humid);
  readings["mq4"] = String(mq4_read);
  readings["mq9"] = String(mq9_read);
  readings["pressure"] = String(bmp_pres_read);
  readings["latitude"] = latitude;
  readings["longitude"] = longitude;
  
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}


void setup() {
  
  Serial.begin(9600);
  gpsSerial.begin(9600);

  pinMode(DHT_Pin,INPUT);
  pinMode(MQ9_Pin,INPUT);
  pinMode(MQ4_Pin,INPUT);
  pinMode(Buzzer_Pin,OUTPUT);

  initWiFi();
  initFS();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");
  
  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 30 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    lastTime = millis();
  }
}
