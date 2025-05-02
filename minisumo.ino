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

NewPing sonar1(TRIG_PIN_1, ECHO_PIN_1, MAX_DISTANCE);

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
  bool frontWhite = analogRead(INF_FRONT) < threshold;
  bool backWhite = analogRead(INF_BACK) < threshold;

  if (frontWhite) {
    Serial.println("Front white - reversing");
    moveBackward();
    delay(500);
    return;
  }
  // this code won't work if it goes out of the line while driving, need to figure that out
  if (backWhite) {
    Serial.println("Back white - moving forward");
    moveForward();
    delay(500);
    return;
  }

  unsigned int frontDist = sonarFront.ping_cm();
  unsigned int leftDist = sonarLeft.ping_cm();
  unsigned int rightDist = sonarRight.ping_cm();

  if (frontDist == 0 && leftDist == 0 && rightDist == 0) {
    Serial.println("Enemy behind - moving forward-right");
    moveForwardRight();
  } else {
    // Find closest enemy direction
    unsigned int minDist = MAX_DISTANCE;
    byte direction = 0; // 1=front, 2=left, 3=right
    
    if (frontDist > 0 && frontDist < minDist) {
      minDist = frontDist;
      direction = 1;
    }
    if (leftDist > 0 && leftDist < minDist) {
      minDist = leftDist;
      direction = 2;
    } // detects whether the enemy is left, right, or front (1, 2, 3)
    if (rightDist > 0 && rightDist < minDist) {
      minDist = rightDist;
      direction = 3;
    }

    switch (direction) {
      case 1:
        Serial.println("Enemy front - approaching");
        moveForward();
        break;
      case 2:
        Serial.println("Enemy left - turning left");
        turnLeft();
        break;
      case 3:
        Serial.println("Enemy right - turning right");
        turnRight();
        break;
      default:
        Serial.println("No enemy detected - stopping");
        stopMotors();
        break;
    }
  }
  delay(100);

    }
  }

  delay(100); 
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
