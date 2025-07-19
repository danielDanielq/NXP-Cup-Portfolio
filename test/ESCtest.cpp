#include <Arduino.h>
#include <PWMServo.h>

#define DRIVER_MOTOR_PIN 23  // PWM output for the ESC

PWMServo ESC;
int throttleValue = 0;  // Start at minimum throttle for proper arming
bool runCalibration = false;

void setup() {
  Serial.begin(115200);
  delay(100); // Allow time for Teensy initialization

  // Attach the ESC (maps 0 to 180 into pulse widths between 1000µs and 2000µs)
  ESC.attach(DRIVER_MOTOR_PIN, 1000, 2000);
  delay(100);

  // (Assuming calibration was already performed)
  Serial.println("Normal Operation: Holding at MIN throttle for arming...");
  ESC.write(0);  // Minimum throttle (1000µs pulse)
  delay(3000);   // Wait 3 seconds for the ESC to arm

  Serial.println("ESC Ready for normal operation!");
  // Set initial throttle to a low value, then ramp up.
  throttleValue = 10;  // Adjust this start value if needed
}

void loop() {
  // Gradually ramp throttle from a low value up to near full throttle
  if (throttleValue > 170) {
    throttleValue = 10;
  }
  ESC.write(throttleValue);
  Serial.printf("Throttle value: %d\n", throttleValue);
  throttleValue++;  // Increase throttle step-by-step
  delay(1000);      // Delay between steps to observe motor response
}
