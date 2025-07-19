#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define ENABLE_REMOTE_START_STOP 1
#define REMOTE_START_PIN 10
#define REMOTE_STOP_PIN 20

void setup() {
    #if ENABLE_REMOTE_START_STOP == 1
        pinMode(REMOTE_STOP_PIN, INPUT);
        pinMode(REMOTE_START_PIN, INPUT);
    #endif
    
    Serial.begin(9600);
}
void loop() {
    // Remote State Reading
    #if ENABLE_REMOTE_START_STOP == 1
      if (digitalRead(REMOTE_STOP_PIN) == HIGH) {
        Serial.println("STOP");
      }
      else if(digitalRead(REMOTE_START_PIN) == HIGH){
        Serial.println("START");
      }
    #endif
}
