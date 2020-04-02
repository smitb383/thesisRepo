//libraries for internet
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

//initalize wifi 
ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial
#define WIFI_SSID       "brookeandjosh"
#define WIFI_PASSWORD   "FundamentallyFingerless"


// Define stepper motor connections and steps per revolution:
#define dirPin 4
#define stepPin 5
#define stepsPerRevolution 200


//define end position, start position and variables to change directions
bool travelingLeft = true; 
bool travelingRight = false; 
//10 full revolutions in each dierction 
int stepsFullRange = stepsPerRevolution*14; 
//steps to move for one full movement
int currentSteps = stepsFullRange/2.0; 
//where ever the slider is starting (might end up being somewhere in the middle 
int halfRail = stepsFullRange/2.0; 
int middlePt = halfRail; 
int midToLft = stepsFullRange/2.0; 
int midtoRgt= stepsFullRange/2.0; 
int railPos = 0; 


 
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

while(hit ==false){
//determine how far the midpoint is from each of the sides 
//add this to part only when new mid point is received from server 
//set the distance for traveling left and traveling right distance 
middlePt = 1090; 
midToLft = middlePt-0; 
midtoRgt= stepsFullRange-middlePt;

if(midToLft > halfRail){
  midToLft =halfRail; 
}
if(midtoRgt > halfRail){
  midtoRgt =halfRail; 
}

//determine direction to move 
if(travelingLeft ==true){ //traveling left
    digitalWrite(dirPin, HIGH);
}
else{ //traveling right
   digitalWrite(dirPin, LOW);
}


//moving left side of the middle point
if(railPos <= middlePt && railPos >= middlePt-midToLft){

  if(railPos ==middlePt){
    Serial.println("AT THE MIDDLE"); 
    delay(1000); 
  }
  //reached the left end (0 pos)
if (railPos == middlePt-midToLft){
//   Serial.println("reached LEFT"); 
//   Serial.println(railPos); 
  travelingLeft =false; 
  travelingRight = true; 
  railPos++; 
 
}
else{
   digitalWrite(stepPin, HIGH);
   digitalWrite(stepPin, LOW);
   delay(1);

   //moving towards left end 
  if(travelingLeft ==true){
//    Serial.println("traveling from MIDDLE to LEFT"); 
    railPos--;
//    Serial.println(railPos); 
  }
  
  //moving back towards center
  else{
//    Serial.println("traveling from LEFT to MIDDLE"); 
    railPos++;
//    Serial.println(railPos); 
  }
}
} ////end of traveling left side




//traveling to the right now 
if(railPos > middlePt && railPos <=middlePt+ midtoRgt){
  
  //reached the far end of rail
if (railPos == middlePt+ midtoRgt){
   Serial.println("midToRight: " + String(midtoRgt)); 
      Serial.println("middlePt + midToRight: " + String(middlePt+ midtoRgt)); 
  Serial.println("reached RIGHT"); 
  travelingLeft =true; 
  travelingRight = false; 
  Serial.println(railPos);
  railPos--; 
}
else{
  digitalWrite(stepPin, HIGH);
   digitalWrite(stepPin, LOW);
   delay(1);

   //moving towards right end 
  if(travelingRight ==true){
//     Serial.println("traveling from MIDDLE to RIGHT"); 
      railPos++;
//      Serial.println(railPos);
  }
  //moving back towards center
  else{
//    Serial.println("traveling from RIGHT to MIDDLE"); 
    railPos--;
//    Serial.println(railPos);
  }
}
} //end of traveling right side


  
  
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
// delay(1000);
 String valueToString = String(analogRead(analog_ip));
 Serial.println("velostat hit value= " + valueToString); 
 Serial.println("hitPostion " + String(railPos));
 delay(2000);
}
