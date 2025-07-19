#include <Arduino.h>
#include "SteeringWheel.h"
#include "config.h"

#define STEERING_SERVO_PIN 3
  #define STEERING_SERVO_ANGLE_MIDDLE     90    // 90 middle // 120
  #define STEERING_SERVO_ANGLE_MAX_RIGHT  0    // 0 max right // 30
  #define STEERING_SERVO_ANGLE_MAX_LEFT   180   // 180 max left // 210

SteeringWheel steeringWheel(STEERING_SERVO_ANGLE_MAX_LEFT, STEERING_SERVO_ANGLE_MIDDLE, STEERING_SERVO_ANGLE_MAX_RIGHT, (unsigned int)0);

float increment = 1.0f;
float potValue = 0.0f;

void setup(){
    Serial.begin(115200);
    
    while (!Serial){
      delay(100);
    }

    steeringWheel.attach(STEERING_SERVO_PIN);
    steeringWheel.setSteeringAngleDeg(potValue);
    float startTimer = (float)millis();

    while (((float)millis() - startTimer) < 2000) {
        steeringWheel.setSteeringAngleDeg(potValue);
    }
}

float time1;

/*

void loop(){
    time1 = (float)millis();
    while (((float)millis() - time1) < 2000) {
        steeringWheel.setSteeringAngleDeg((30.0f));
    }
    time1 = (float)millis();
    while (((float)millis() - time1) < 2000) {
        steeringWheel.setSteeringAngleDeg((-30.0f));
    }
}
*/


void loop() {
    if (potValue > 90.0f)
    {
        increment = -1.0f;
    }
    else if(potValue < -90.0f){
        increment = 1.0f;
    }
    Serial.println(potValue);
  time1 = (float)millis();
  while ((float)millis() - time1 < 200)
  {
    steeringWheel.setSteeringAngleDeg(potValue);    // Send the signal to the ESC
    delay(1);
  }
  potValue += increment;
}
