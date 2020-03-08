// Define stepper motor connections and steps per revolution:
#define dirPin 4
#define stepPin 5
#define stepsPerRevolution 200


//define end position, start position and variables to change directions
#define pos1 0; 
#define pos2 100; 
#define rotationsToEnd 10
bool travelingLeft = true; 
bool travelingRight = false; 
int currentRotations = 0; 
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
  
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

//delay to plug in motor 
for(int i=0; i<5; i++){
     delay(1000);
     Serial.print("."); 
}



}
void loop() {
//    inputVal = analogRead(analog_ip);
//    String valueToString = String(inputVal);
//    Serial.println("Velostat Value= " + valueToString); 
  // Set the away from motor 
  // Traveling to the left

while(hit ==false){
//  Serial.println("In main loop!");  
//      digitalWrite(stepPin, HIGH);
  if(currentSteps < stepsFullRange && travelingLeft ==true){
  digitalWrite(dirPin, HIGH);
  fullMovement(); 
  currentRotations++; 
  //if you've reached the pulley end of the platform
if(currentSteps == stepsFullRange){
  travelingLeft =false; 
  currentSteps=0;
}
  }
  // traveling right
  else if(currentSteps < stepsFullRange && travelingLeft ==false){
        digitalWrite(dirPin, LOW);
  // Spin the stepper motor 1 revolution slowly:
  fullMovement(); 
    currentRotations++; 
   //if you've reached the pulley end of the platform
if(currentSteps == stepsFullRange){
  travelingLeft =true; 
  currentSteps=0;
}
  }
     yield();
}
// Serial.println(String(currentRotations)); 
 Serial.println(hit); 
  delay(1000);

}



  void fullMovement(){
      if(currentSteps% 5 == 0 && analogRead(analog_ip)>= 100.0){
            String valueToString = String(analogRead(analog_ip));
Serial.println("velostat hit value= " + valueToString); 
            hit==true; 
            //record hit position
            hitPosition = currentSteps; 
             Serial.println("hitPostion " + String(hitPosition)); 
             stop(); 
            //send position to data base 
            
           //stop motors 
           //exit from current loop 
           currentSteps = hitPosition;
           Serial.println("current Steps= " + String(currentSteps)); 
           Serial.println("hit status= " + String(hit)); 
                                           
            //move servo to remove fallen item 
            //set hit to false again and start moving again 
      }
else{
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
    delay(1);
    currentSteps++; 
}
 
}
void stop(){
    digitalWrite(stepPin, LOW);
    Serial.println("stop bc of hit"); 
      delay(5000);
      hit == false; 

}
