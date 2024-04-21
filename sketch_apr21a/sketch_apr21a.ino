#include <NewPing.h>

#define SPEED_1      5 
#define DIR_1        4 
#define SPEED_2      6 
#define DIR_2        7 

#define TRIG_PIN_RIGHT 10
#define ECHO_PIN_RIGHT 11

#define TRIG_PIN_FORWARD 6
#define ECHO_PIN_FORWARD 7

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters)

#define CAR_SPEED 255 // Speed for moving forward
#define TURN_SPEED 128 // Speed for turning

#define DISTANCE_THRESHOLD 15 // Threshold distance for obstacle detection

NewPing rightSensor(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE); // Right sensor
NewPing forwardSensor(TRIG_PIN_FORWARD, ECHO_PIN_FORWARD, MAX_DISTANCE); // Forward sensor

void setup() {
  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }
  // Serial.begin(9600); // Uncomment if you need serial communication
}

void move_forward() {
  digitalWrite(DIR_1, HIGH);  
  digitalWrite(DIR_2, HIGH);
  analogWrite(SPEED_1, CAR_SPEED);  
  analogWrite(SPEED_2, CAR_SPEED);
}

void rotate_left() {
  digitalWrite(DIR_1, HIGH);  
  digitalWrite(DIR_2, LOW);
  analogWrite(SPEED_1, TURN_SPEED);  
  analogWrite(SPEED_2, TURN_SPEED);
}

void rotate_right() {
  digitalWrite(DIR_1, LOW);  
  digitalWrite(DIR_2, HIGH);
  analogWrite(SPEED_1, TURN_SPEED);  
  analogWrite(SPEED_2, TURN_SPEED);
}

void stop() {
  analogWrite(SPEED_1, 0);  
  analogWrite(SPEED_2, 0);
}

void loop() {
  int rightDistance = rightSensor.ping_cm(); // Distance from the wall on the right
  int forwardDistance = forwardSensor.ping_cm(); // Distance from the wall in front

  move_forward();

  if (rightDistance < DISTANCE_THRESHOLD) {
    rotate_left();
    delay(600);
    stop();
  }

  if (forwardDistance < DISTANCE_THRESHOLD) {
    rotate_right();
    delay(600);
    stop();
  }
}
