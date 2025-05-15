#include <NewPing.h>

// Pin definitions
#define TRIG_PIN_1 4
#define ECHO_PIN_1 5
#define TRIG_PIN_2 2
#define ECHO_PIN_2 3
#define TRIG_PIN_3 6
#define ECHO_PIN_3 7
#define MAX_DISTANCE 200

#define INF_FRONT A0
#define INF_BACK A1

#define A1A 8
#define A1B 10
#define B1A 12
#define B1B 14
#define ENA A2 // Speed control for motor A
#define ENB A3 // Speed control for motor B

int speedA = 255; // Max speed (0-255)
int speedB = 255;

const int threshold = 700; // Infrared sensor threshold

// Sensors array: [inf_back, inf_front, sonar_left, sonar_front, sonar_right]
int sensors[5] = {0, 0, 0, 0, 0};

String enemyPosition = "none";

unsigned long previousMillis = 0;
const long interval = 50; // 50 ms between sensor readings

// Initialize sonar sensors
NewPing sonarLeft(TRIG_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonarFront(TRIG_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonarRight(TRIG_PIN_3, ECHO_PIN_3, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);

  pinMode(INF_FRONT, INPUT);
  pinMode(INF_BACK, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(B1A, OUTPUT);
  pinMode(B1B, OUTPUT);

  stopMotors();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    readSensors();
    determineEnemyPosition();

    // Avoid leaving dohyo
    if (sensors[0] == 1) { // Infrared detects white line
      stopMotors();
      moveBackwardShort();
      turnLeftShort();
      stopMotors();
      return;
    }
    if (sensors[1] == 1){ // öndeki de arkadikde aynısını yapamaz
      stopMotors();
      moveForwardShort();
      turnLeftShort();
      stopMotors();
      return;
    }

    // Movement based on enemy position
    if (enemyPosition == "left") {
      moveLeft();
    } else if (enemyPosition == "left_front") {
      moveForwardLeft();
    } else if (enemyPosition == "front") {
      moveForward();
    } else if (enemyPosition == "right_front") {
      moveForwardRight();
    } else if (enemyPosition == "right") {
      turnRight();
    } else {
      // No enemy detected, search by turning left
      turnLeft();
    }
  }
}

// Movement functions
void moveForward() {
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  analogWrite(A1A, 0);
  analogWrite(A1B, speedA);

  analogWrite(B1A, 0);
  analogWrite(B1B, speedB);
}

void moveBackward() {
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  analogWrite(A1A, speedA);
  analogWrite(A1B, 0);

  analogWrite(B1A, speedB);
  analogWrite(B1B, 0);
}

void moveBackwardShort() {
  moveBackward();
  delay(200);
  stopMotors();
}

void moveForwardShort() {
  moveForward();
  delay(200);
  stopMotors();
}

void moveLeft() {
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  analogWrite(A1A, 0);
  analogWrite(A1B, speedA);

  analogWrite(B1A, speedB);
  analogWrite(B1B, 0);
}

void moveForwardLeft() {
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  analogWrite(A1A, 0);
  analogWrite(A1B, speedA);

  analogWrite(B1A, 0);
  analogWrite(B1B, speedB / 2);
}

void moveForwardRight() {
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  analogWrite(A1A, 0);
  analogWrite(A1B, speedA);

  analogWrite(B1A, 0);
  analogWrite(B1B, speedB / 2);
}

void turnLeft() {
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  analogWrite(A1A, 0);
  analogWrite(A1B, speedA);

  analogWrite(B1A, speedB);
  analogWrite(B1B, 0);
}

void turnLeftShort() {
  turnLeft();
  delay(300);
  stopMotors();
}

void turnRight() {
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  analogWrite(A1A, speedA);
  analogWrite(A1B, 0);

  analogWrite(B1A, 0);
  analogWrite(B1B, speedB);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  analogWrite(A1A, 0);
  analogWrite(A1B, 0);

  analogWrite(B1A, 0);
  analogWrite(B1B, 0);
}

// Convert distance to category
int distance_far(int distance) {
  if (distance > 0 && distance <= 20) return 1;
  else if (distance > 20 && distance <= 40) return 2;
  else if (distance > 40) return 3;
  else return 0;
}

// Read sensors
void readSensors() {
  sensors[0] = (analogRead(INF_BACK) > threshold) ? 1 : 0;
  sensors[1] = (analogRead(INF_FRONT) > threshold) ? 1 : 0;

  sensors[2] = distance_far(sonarLeft.ping_cm());
  sensors[3] = distance_far(sonarFront.ping_cm());
  sensors[4] = distance_far(sonarRight.ping_cm());

  // Debug print
  Serial.print("IR Back: "); Serial.print(sensors[0]);
  Serial.print(", IR Front: "); Serial.print(sensors[1]);
  Serial.print(", Sonar Left: "); Serial.print(sensors[2]);
  Serial.print(", Sonar Front: "); Serial.print(sensors[3]);
  Serial.print(", Sonar Right: "); Serial.println(sensors[4]);
}

// Determine enemy position
void determineEnemyPosition() {
  if (sensors[2] == 1 && sensors[3] == 3 && sensors[4] == 3) {
    enemyPosition = "left";
  } else if (sensors[2] == 1 && sensors[3] == 2 && sensors[4] == 3) {
    enemyPosition = "left_front";
  } else if (sensors[2] == 3 && sensors[3] == 1 && sensors[4] == 3) {
    enemyPosition = "front";
  } else if (sensors[2] == 3 && sensors[3] == 1 && sensors[4] == 2) {
    enemyPosition = "right_front";
  } else if (sensors[2] == 3 && sensors[3] == 3 && sensors[4] == 1) {
    enemyPosition = "right";
  } else {
    enemyPosition = "none";
  }

  Serial.print("Enemy Position: ");
  Serial.println(enemyPosition);
}
