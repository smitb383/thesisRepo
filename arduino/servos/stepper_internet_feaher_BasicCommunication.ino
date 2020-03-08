//libraries for internet
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

//initalize wifi 
ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial
#define WIFI_SSID       "darkmatter"
#define WIFI_PASSWORD   "gofuckyourself"


// Define stepper motor connections and steps per revolution:
#define dirPin 4
#define stepPin 5
#define stepsPerRevolution 200


//define end position, start position and variables to change directions
bool travelingLeft = true; 
bool travelingRight = false; 
//steps to move for one full movement
int currentSteps = 0; 
int stepsFullRange = 2000; 

//velostat
const int analog_ip = A0;
float inputVal = 0.;
bool hit = false; 

//record positon where platform was when hit occurs 
int hitPosition; 

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
  
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

//five second delay to power motor 
Serial.println("plug in power"); 
for(int i=0; i<5; i++){
     delay(1000);
     Serial.print(String(5-i)); 
}
}
void loop() {
  //while you are connected to the internet run 
  if ((WiFiMulti.run() == WL_CONNECTED)) {   
     HTTPClient http;
    int httpCode;

    
//get the analog value of velostat 
String velostatValue = String(analogRead(analog_ip)); 
String currentStepsString = String(currentSteps);   
//http.begin("http://192.168.1.5:8000/data/?currentSteps=" + currentStepsString +"");
//http.begin("http://192.168.1.5:8000/data/?velostatValue=" + velostatValue +"");
Serial.println("velostat serial reading= " + velostatValue); 


while(hit ==false){
  
//determine direction to move 
if(travelingLeft ==true){ //traveling right
    digitalWrite(dirPin, HIGH);
}
else{ //traveling left
   digitalWrite(dirPin, LOW);
}
  if(currentSteps% 5 == 0 && analogRead(analog_ip)>= 100.0){
    String currentStepsString = String(currentSteps);  
    stop(); 
     
   http.begin("http://192.168.1.5:8000/data/?currentSteps=" + currentStepsString +"");
    httpCode = http.GET();
 // file found at server 
      if (httpCode == HTTP_CODE_OK) {
        //getting data back from server 
        String payload = http.getString();
        //checking data sent back from server

        Serial.println(payload); 
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
 if(currentSteps < stepsFullRange ){ 
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
    delay(1);
    currentSteps++; 

if(currentSteps == stepsFullRange){  //change directions
  if(travelingLeft ==true){
  travelingLeft =false; 
  }
  else{
    travelingLeft = true; 
  }
  //reset current Pos to 0 
  currentSteps=0;
}
  }//end of movement loop


  
} //end of main while loop 


    
  }//end of if wifi connected loop 

 
  //otherwise you cannot connect to wifi
  else{
      // send if connection failed
      Serial.printf("\[HTTP] Unable to connect\n");
  }
  delay(1000);
}//end of main arduino loop 

void stop(){
 digitalWrite(stepPin, LOW);
 Serial.println("stop bc of hit"); 
 delay(1000);
 String valueToString = String(analogRead(analog_ip));
 Serial.println("velostat hit value= " + valueToString); 
 Serial.println("hitPostion " + String(currentSteps));
 delay(3000);
}
