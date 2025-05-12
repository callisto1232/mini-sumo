#include <NewPing.h>

#define TRIG_PIN_1 6
#define ECHO_PIN_1 7
#define TRIG_PIN_2 8
#define ECHO_PIN_2 9
#define TRIG_PIN_3 10
#define ECHO_PIN_3 11
#define MAX_DISTANCE 200
#define INF_FRONT A0
#define INF_BACK A1
#define A1A 4
#define A1B 5
#define B1A 2
#define B1B 3
#define ENA A2 //to control speed
#define ENB A3 // so
int speedA  = 255 // 0-255, 255 max
int speedB = 255
const int inf_thresold = 700; // example While: 400, Black: 900, good thresold: 650
string enemyPosition;
sensors = [0, 0, 0, 0, 0]  // inf back, inf front, sonar left, sonar front, sonar right
unsigned long previousMillis = 0;
const long interval = 50; // 50 ms between sensor readings

NewPing sonar1(TRIG_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIG_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonar3(TRIG_PIN_3, ECHO_PIN_3, MAX_DISTANCE);



void setup(){
  Serial.begin(9600);
  pinMode(INF_FRONT, INPUT);
  pinMode(INF_BACK, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(B1A, OUTPUT);
  pinMode(B1B, OUTPUT);      
}

void loop() {
  inf_sensors()
  if enemyPosition == "left" {
    moveLeft();
  } else if enemyPosition == "left_front" {
    moveForwardLeft();
  } else if enemyPosition == "front" {
    moveForward();
    // we need to use delay here but we won't. last comment 
  } else if enemyPosition == "right_front" {
    moveForwardRight();
  } else if enemyPosition == "right" {
    turnRight();
  } else if enemyPosition == "none" {
    //check again then turn around (left)
    turnLeft();
  }
  
}

void moveForward() {
  analogWrite(A1A, 0);
  analogWrite(A1B, speedA);
  analogWrite(B1A, 0);
  analogWrite(B1B, speedB);
}

void moveBackward() {
  analogWrite(A1A, speedA);
  analogWrite(A1B, 0);
  analogWrite(B1A, speedB);
  analogWrite(B1B, 0);
}

void moveForwardRight() {
  analogWrite(A1A, 0);
  analogWrite(A1B, speedA);  
  analogWrite(B1A, 0);
  analogWrite(B1B, speedB/2);
}

void turnLeft() {
  analogWrite(A1A, 0);
  analogWrite(A1B, speedA); 
  analogWrite(B1A, speedB);
  analogWrite(B1B, 0);   
}

void turnRight() {
  analogWrite(A1A, speedA);
  analogWrite(A1B, 0);  
  analogWrite(B1A, 0);
  analogWrite(B1B, speedB); 
}

void stopMotors() {
  analogWrite(A1A, 0);
  analogWrite(A1B, 0);
  analogWrite(B1A, 0);
  analogWrite(B1B, 0);
}


void distance_far(int distance) { // is it 255 when no object? or blocked view
  if (0 < distance <= 20 ) {
    return 1;
  } else if (distance > 20 && distance <= 40){
    return 2; 
  } else if (distance > 40){
    return 3;
  } else {
    return 0;
  }

}

void inf_sensors() {
  bool frontWhite = analogRead(INF_FRONT) < threshold;
  bool backWhite = analogRead(INF_BACK) < threshold;
  sensors[0] = frontWhite;
  sensors[1] = backWhite;
  sensors[2] = distance_far(sonarLeft.ping_cm());
  sensors[3] = distance_far(sonarFront.ping_cm());
  sensors[4] = distance_far(sonarRight.ping_cm());
}

// sensors[2] left distance, sensors[3] front distance, sensors[4] right distance
void enemyPosition(){
  if sensors[2] == 1 && sensors[3] == 3 && sensors[4] == 3{
    enemyPosition = "left";
  } else if sensors[2] == 1 && sensors[3] == 2 && sensors[4] == 3{
    enemyPosition = "left_front";
  } else if sensors[2] == 3 && sensors[3] == 1 && sensors[4] == 3{
    enemyPosition = "front";
  } else if sensors[2] == 3 && sensors[3] == 1 && sensors[4] == 2{
    enemyPosition = "right_front";
  } else if sensors[2] == 3 && sensors[3] == 3 && sensors[4] == 1{
    enemyPosition = "right";
  } else {
    enemyPosition = "none";
  }

}
// add if else to control at the beginning of the loop function to check if infrared sensors are white not to go out of the dohyo
// qwertyuiop
// we don't actually need to check infrared ones when distance is detected, unless the enemy escapes and we go out of the dohyo
// in moving functions, if we just move the robot a little bit then check for the infrared again, we may reduce the possibility to exit the dohyo
// we can collect the data to an array then check the average as in the github mini sumo repo by ethanphunter

/*
else if (lineR == LOW && lineL == HIGH) { // Right line sensor hits the white line and triggers low
  motors(-100, -100);           // Back up
  delay(200);                  // Only back up for .2 seconds
  end_time = millis() + 400;   // Find the time 400 miliseconds into the future
  while(millis() < end_time) { // Spin away from edge untill the time limit is hit
    motors(-100, 100);
    sensors();
    if(midL > threshold || midR > threshold || prox == LOW){  // If you see a target, break loop
      break;
    }
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, LOW);
  }
}

it is using time to check when to move, where to move rather than delay, this way you can both check and move at the same time
*/
