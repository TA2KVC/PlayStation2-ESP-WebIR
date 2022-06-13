#include <Arduino.h>
#include <IRremoteESP8266.h>
//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <IRsend.h>


// replace the ssid and password
const char* ssid = "xxxxxx";
const char* password = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const int port = 80;

// replace staticIP and gateway address based on your home router settings
String request;
DynamicJsonDocument doc(200);
AsyncWebServer server(port);

byte SEND_PIN = 4;
IRsend irsend(SEND_PIN);
unsigned long infraRedCode[] = {
  0x9EB92,  //up
  0x5EB92,  //down
  0xDEB92,  //left
  0x3EB92,  //right
  0x7AB5B,  //X-cross
  0xBAB5B,  //O-circle
  0x3AB5B,  //triangle
  0xFAB5B,  //square
  0xCAB5B,  //start
  0xAB5B,   //select
  0xA8B5B   //POWER
};


void connectToWiFi() {
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //Print your WiFi's SSID (might be insecure)
  Serial.println(ssid);
  Serial.print("IP address: ");
  //Print your local IP address (needed for browsing the app)
  Serial.println(WiFi.localIP());
}



void onRequest(AsyncWebServerRequest *request) {
    // dummy callback function for handling params, etc.
}


void onFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
    // dummy callback function signature, not in used in our code
}


void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    DeserializationError error = deserializeJson(doc, (char*)data);
    if (!error) {
      const char* command = doc["command"];
      uint8_t code = atoi(doc["code"]);
      Serial.printf("command=%s, code=0x%lX, protocol=", command, infraRedCode[code]);
          irsend.sendSony(infraRedCode[code], 20);
          Serial.println("Sony PS2");
             }
      request->send(200, "text/plain", "Ok");
    }

void setup() {

    Serial.begin(115200);
    WiFi.begin(ssid, password);
    bool success = SPIFFS.begin();
    //SPIFFS.begin(true);
    server.serveStatic("/", SPIFFS, "/")
          .setDefaultFile("index.html")
          .setCacheControl("max-age=86400");

    server.on("/api", HTTP_POST, onRequest, onFileUpload, onBody);

    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
    });

    server.begin();
    irsend.begin();
}


void loop(){

}
