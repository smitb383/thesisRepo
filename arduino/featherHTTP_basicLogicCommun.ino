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
  
}


bool movingLeft =true; 
int testValue = 1; 

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {   
        HTTPClient http;
        int httpCode;
        
        if(testValue <200 && movingLeft==true){
          testValue ++; 
        }
       
        String valueToString = String(testValue);
//        Serial.println("testing " + valueToString); 
        //cant put a value to the URL
       http.begin("http://192.168.1.5:8000/data/?position=" + valueToString +"");

    // start connection and send HTTP header
     httpCode = http.GET();

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        //getting data back from server 
        String payload = http.getString();
        //checking data sent back from server

Serial.print(payload); 
        if(payload =="1"){
           Serial.println("number over 50");
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
  delay(1000);
  

}
