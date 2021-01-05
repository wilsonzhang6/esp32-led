#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "FastLED.h"

#define NUM_LEDS 30
#define DATA_PIN 12
CRGB leds[NUM_LEDS];

AsyncWebServer server(80);

int mode = 0;
int r = 255;
int g = 0;
int b = 0;
int brightness = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Begin setup");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  if(!SPIFFS.begin(true)){
    Serial.println("Error mounting SPIFFS");
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int status = WiFi.begin();
  while(status != WL_CONNECTED){
    Serial.println("attempting to connect to wifi...");
    status = WiFi.begin();
    delay(1000);
  }
  Serial.println("connected");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.js");
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/led.jpg");
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    int params = request->params();
    for(int i = 0; i < params; i++){
      AsyncWebParameter* p = request->getParam(i);
//      Serial.printf("%s: %s\n", p->name().c_str(), p->value());
      if(p->name() == "mode"){
        mode = (p->value()).toInt();
      }
      if(p->name() == "r"){
        r = (p->value()).toInt();
      }
      if(p->name() == "g"){
        g = (p->value()).toInt();
      }
      if(p->name() == "b"){
        b = (p->value()).toInt();
      }
      if(p->name() == "brightness"){
        brightness = (p->value()).toInt();
        Serial.print("received brightness value: ");
        Serial.println(brightness);
      }
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("server listening");
  Serial.print("server IP: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("Setup done");
}

void loop() {
  FastLED.setBrightness(brightness);
  if(mode == 0){
      for(int i = 0; i < NUM_LEDS; i++){
        leds[i].setRGB(0,0,0);
        FastLED.show();
      }
  }
  else if(mode == 1){
    solidColor();
  }
  if(mode == 2){
    rainbow();
  }
}

void solidColor(){
     for(int i = 0; i < NUM_LEDS; i++){
      leds[i].setRGB(r,g,b);
      FastLED.show();
    } 
}

void rainbow(){
  uint8_t hue = beat8(30, 255);
  fill_rainbow(leds, NUM_LEDS, hue, 255/NUM_LEDS);
  FastLED.show();
}
