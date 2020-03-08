
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

  // Set the away from motor 


  // Traveling to the left
  if(currentRotations <rotationsToEnd && travelingLeft ==true){
  digitalWrite(dirPin, HIGH);
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
//    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  currentRotations++; 
  //if you've reached the pulley end of the platform
if(currentRotations == rotationsToEnd){
  travelingLeft =false; 
  currentRotations=0;
}
  }
  // traveling right
  else if(currentRotations < rotationsToEnd && travelingLeft ==false){
        digitalWrite(dirPin, LOW);
  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
//    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
    currentRotations++; 
   //if you've reached the pulley end of the platform
if(currentRotations == rotationsToEnd){
  travelingLeft =true; 
  currentRotations=0;
}
  }
 Serial.println(String(currentRotations)); 
  delay(200);
}
