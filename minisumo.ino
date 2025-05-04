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
int speedA  = 255 // 0-255, 255 max
int speedB = 255
const int inf_thresold = 700; // example While: 400, Black: 900, good thresold: 650
string enemyPosition;
sensors = [0, 0, 0, 0, 0]  # inf back, inf front, sonar left, sonar front, sonar right

NewPing sonar1(TRIG_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIG_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonar3(TRIG_PIN_3, ECHO_PIN_3, MAX_DISTANCE);



void setup(){
  Serial.begin(9600);
  pinMode(INF_FRONT, INPUT);
  pinMode(INF_BACK, INPUT);
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
  } else if enemyPosition == "right_front" {
    moveForwardRight();
  } else if enemyPosition == "right" {
    turnRight();
  } else if enemyPosition == "none" {
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
