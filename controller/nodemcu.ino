/*
 *  Author: Wanghley Soares Martins (@wanghley)
 *
 */
 
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager​
#include <ArduinoJson.h>
#include <Stepper.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

// Define variables to be used 

 
ESP8266WebServer server(80);
SoftwareSerial s(3,1);
 
// Serving Hello world
void getHelloWord() {
    DynamicJsonDocument doc(512);
    doc["name"] = "Hello world";
    String buf;
    serializeJson(doc, buf);
    server.send(200, "application/json", buf);
}
// Serving Hello world
void getSettings() {
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  //  StaticJsonDocument<512> doc;
  // You can use DynamicJsonDocument as well
  DynamicJsonDocument doc(512);

  doc["ip"] = WiFi.localIP().toString();
  doc["gw"] = WiFi.gatewayIP().toString();
  doc["nm"] = WiFi.subnetMask().toString();

  if (server.arg("signalStrength")== "true"){
      doc["signalStrengh"] = WiFi.RSSI();
  }

  if (server.arg("chipInfo")== "true"){
      doc["chipId"] = ESP.getChipId();
      doc["flashChipId"] = ESP.getFlashChipId();
      doc["flashChipSize"] = ESP.getFlashChipSize();
      doc["flashChipRealSize"] = ESP.getFlashChipRealSize();
  }
  if (server.arg("freeHeap")== "true"){
      doc["freeHeap"] = ESP.getFreeHeap();
  }

  String buf;
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);
}

void setPosition() {
  Serial.println();
  if (server.hasArg("plain")== false){ //Check if body received
    server.send(200, "text/plain", "Body not received");
    return;
  }

  // StaticJsonBuffer<200> jsonBuffer;
  // JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
  
  // String message = "Body received:\n";
  //         message += server.arg("plain");
  //         message += "\n";

  const char* json = server.arg("plain").c_str();
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  String id = doc["id"];
  int pos = doc["pos"];

  if(id=="usa"){
    if(pos==1){
      s.println('u');
      String resp = "{id:"+id+",pos:"+pos+"}";
      server.send(201, "text/json", resp);
    }
    else if(pos==-1){
      s.println('d');
      String resp = "{id:"+id+",pos:"+pos+"}";
      server.send(201, "text/json", resp);
    }
    else if(pos==0){
      s.println('m');
      String resp = "{id:"+id+",pos:"+pos+"}";
      server.send(201, "text/json", resp);
    }else{
      String resp = "{\"error\": \"invalid option\"}";
      server.send(500, "text/json", resp);
    }
  }
  //Serial.println(message);
}
 
// Define routing
// Implement data json filtering
// https://github.com/esp8266/Arduino/issues/1670
void restServerRouting() {
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    server.on(F("/helloWorld"), HTTP_GET, getHelloWord);
    server.on(F("/settings"), HTTP_GET, getSettings);
    server.on(F("/set-position"), HTTP_POST, setPosition);
}
 
// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void) {
  Serial.begin(115200);
  s.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  //wifiManager.resetSettings();
  wifiManager.autoConnect("HalfMaster USA","1234567890");
  // Serial.println("connected :)");
 
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
 
  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  // if (MDNS.begin("esp8266")) {
  //   Serial.println("MDNS responder started");
  // }
 
  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
  // Serial.println("HTTP server started");
}
 
void loop(void) {
  server.handleClient();
}