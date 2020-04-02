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

//array of middlePoints
const int numMidPts = 5; 
int midPts[numMidPts] ={1900, 2500, 1400, 700, 2400};
int midPtsCnt = 0; 
int middlePt = halfRail; 
int midToLft = stepsFullRange/2.0; 
int midtoRgt= stepsFullRange/2.0; 
int railPos = 0; 


 bool check; 
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

while(hit ==false){
//get the midpoint 
//need to change every time an end position is reached 

  
//determine how far the midpoint is from each of the sides 
//add this to part only when new mid point is received from server 
//set the distance for traveling left and traveling right distance 
midToLft = middlePt-0; 
midtoRgt= stepsFullRange-middlePt;

////////////////////CHECK PRESSURE SENSOR AND COMMUNICATE WITH SERVER/////////////////////////
//evry 5 loop iterations chekc the value of the velostat 
  if(railPos% 5 == 0 && analogRead(analog_ip)>= 100.0){
    String railPosString = String(railPos);  
    stop(); 
     
   http.begin("http://192.168.50.119:8000/data/?railPosHit=" + railPosString +"");
   httpCode = http.GET();
 // file found at server 
      if (httpCode == HTTP_CODE_OK) {
        //getting data back from server 
        String payload = http.getString();
        //checking data sent back from server
        if (payload == "noChange"){
          Serial.println("WAITING ON MORE DATA!");
        }
        else{
          float newCenter = payload.toInt(); 
//          Serial.println(newCenter); 
           Serial.println("NEW CENTER VALUE " + String(newCenter)); 
            Serial.println("CURRENT RAIL POSITION " + String(railPos)); 
           //move the slider to the new center position
            middlePt =newCenter; 
           Serial.println("Delaying"); 
           delay(4000); 
        }
         
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
///////////////////////////MOVEMENT OF STEPPER/////////////////////////
////////////ONLY MOVE SHORTER DISTANCE FROM NEW MIDDLE TO END IN EACH DIRECTION////////////
if(midToLft > midtoRgt){
  midToLft =midtoRgt; 
}
if(midtoRgt > midToLft){
  midtoRgt =midToLft; 
}
///////////////////////////MOVE SHORT LENGTH OF 1/2 of rail from new middle///////////////////////
//if(midToLft > halfRail){
//  midToLft =halfRail; 
//}
//if(midtoRgt > halfRail){
//  midtoRgt =halfRail; 
//}

//determine direction to move 
if(travelingLeft ==true){ //traveling left
    digitalWrite(dirPin, HIGH);
}
else{ //traveling right
   digitalWrite(dirPin, LOW);
}




//edge case, when you increase the number beyond half way point need to increase
//make keep traveling right 
if(railPos <= middlePt && railPos < middlePt-midToLft && middlePt-midToLft!=0){
   digitalWrite(stepPin, HIGH);
   digitalWrite(stepPin, LOW);
    railPos++;
//    Serial.println(railPos); 
       delay(1);

}

if(railPos <= middlePt && railPos >= middlePt-midToLft){
//    Serial.println("on the left side"); 
//    delay(1); 
  if(railPos ==middlePt){
     Serial.println("AT MIDDLE= " + String(middlePt)); 
    delay(250); 
  }
  //reached the left end (0 pos)
  //CHANGE TO NEW MID POINT 
if (railPos == middlePt-midToLft){
   Serial.println("reached LEFT"); 
   Serial.println(railPos); 
   
    middlePt =midPts[midPtsCnt]; 
    midPtsCnt ++; 
       //go back to first middle point if at last 
   if(midPtsCnt >= numMidPts){
  midPtsCnt =0; 
//        delay(1);
   }
    Serial.println("NEW MIDDLE POINT @: " + String(middlePt)); 
    Serial.println("MidPt index: " + String(midPtsCnt)); 
//      delay(1);
  travelingLeft =false; 
  travelingRight = true; 
  railPos++; 
        delay(1);
 
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
//       delay(1);
  }
  
  //moving back towards center
  else{
//    Serial.println("traveling from LEFT to MIDDLE"); 
    railPos++;
//         delay(1);
//    Serial.println(railPos); 
  }
}
} ////end of traveling left side


//traveling to the right now 
/////SOMETHING EXTRA HERE ///////////////////////////////////////////////////
if(railPos > middlePt && railPos >middlePt+ midtoRgt){
     digitalWrite(stepPin, HIGH);
   digitalWrite(stepPin, LOW);
   delay(1);
//  Serial.println("middlePt + midtoRight" + String(middlePt+ midtoRgt)); 
//    Serial.println("railPos" + String(railPos)); 
    railPos--; 

}

if(railPos > middlePt && railPos <=middlePt+ midtoRgt){
  
  //reached the far end of rail
if (railPos == middlePt+ midtoRgt){
  Serial.println("reached RIGHT"); 
  travelingLeft =true; 
  travelingRight = false; 
//  Serial.println(railPos);
//         delay(1);
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
//       delay(1);
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
