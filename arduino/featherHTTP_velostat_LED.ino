// libraries to include:
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <Servo.h>    

ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial

//wifi variables 
#define WIFI_SSID       "darkmatter"
#define WIFI_PASSWORD   "gofuckyourself"

//potentiometer + LED
const int analog_ip = A0;
const int LED = 5;
float inputVal = 0.;

//servo
#define  SERVO_OFF 10   
#define  SERVO_ON 180   
int position_last_set = SERVO_OFF; 
Servo Servo1;       
const int servoInput = 4; 

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


//servo + LED + potentiometer
pinMode (LED, OUTPUT);
//pinMode (analog_ip, INPUT);
Servo1.attach(servoInput);
position_last_set = SERVO_OFF; 
Servo1.write(position_last_set);


}



void loop() {
  //start at 0 
//Servo1.write(0);
  
  if ((WiFiMulti.run() == WL_CONNECTED)) {   
        HTTPClient http;
        int httpCode;

        //testing potentiometer
        inputVal = analogRead(analog_ip);

        String valueToString = String(inputVal);
            Serial.println("Potentiometer Value= " + valueToString); 
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
//           Serial.println("potentiometer over 400"); 
//
            if(SERVO_ON != position_last_set) {
              Servo1.write(SERVO_ON);
              position_last_set = SERVO_ON;
            } 
     
            digitalWrite (LED, HIGH);
        }
        else{
            digitalWrite (LED, LOW);
           if(SERVO_OFF != position_last_set) {
              Servo1.write(SERVO_OFF);
              position_last_set = SERVO_OFF;
            }          
        
       }
//       Serial.println( position_last_set); 
       
      }
       if (httpCode > 0) {
        //connection working 
//          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
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
