// libraries to include:
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial

//wifi variables 
#define WIFI_SSID       "darkmatter"
#define WIFI_PASSWORD   "gofuckyourself"

//testing potentiometer
const int analog_ip = 12;
const int LED = 5;
float inputVal = 0.;

void setup() {

  Serial.begin(115200);

 
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.println();

  
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

    pinMode (LED, OUTPUT);

}

//testing passing variables 
bool movingLeft =true; 
int testValue = 1; 

void loop() {

  
  if ((WiFiMulti.run() == WL_CONNECTED)) {   
        HTTPClient http;
        int httpCode;

        //testing potentiometer
        inputVal = analogRead(analog_ip);

        String valueToString = String(inputVal);
            Serial.println(inputVal); 
        //cant put a value to the URL
       http.begin("http://192.168.1.5:8000/data/?potentiometer=" + valueToString +"");

    // start connection and send HTTP header
     httpCode = http.GET();

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        //getting data back from server 
        String payload = http.getString();
        //checking data sent back from server

        Serial.print(payload); 
        if(payload =="1"){
           Serial.println("potentiometer over 400");        
            analogWrite (LED, inputVal/4);
        }
        else{
            analogWrite (LED, LOW);
        }
       
      }
       if (httpCode > 0) {
        //connection working 
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
       }
       // httpCode will be negative on error
      else {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] PUT... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
       http.end();
     

  }
else{
      // send if connection failed
      Serial.printf("\[HTTP] Unable to connect\n");
  }
  delay(500);
  

}
