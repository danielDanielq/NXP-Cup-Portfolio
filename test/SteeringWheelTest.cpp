#include <Arduino.h>
#include <SPI.h>
#include "SteeringWheel.h"
#include "ParseNum.h"
#include "pixy2_libs/host/arduino/libraries/Pixy2/Pixy2.h"
#include "PixelGreyscaleRow.h"
// #include <stdio.h>
// #include <stdint.h>

#define STEERING_SERVO_ANGLE_MIDDLE     85    // 90 middle
#define STEERING_SERVO_ANGLE_MAX_RIGHT  40    // 38 max right
#define STEERING_SERVO_ANGLE_MAX_LEFT   130   // 135 max left

#define STEERING_SERVO_PIN  3
#define DRIVER_MOTOR_PIN  9


float servoAngle = 50;
int servoTimeout = 7;
int servoAngleIncrease = 4;
SteeringWheel steeringWheel(STEERING_SERVO_ANGLE_MAX_LEFT, STEERING_SERVO_ANGLE_MIDDLE, STEERING_SERVO_ANGLE_MAX_RIGHT);
PWMServo driverMotor;
Pixy2 pixy;
int8_t res;

PixelGreyscaleRow pixelRow;


// 90;10;2
//servoAngle;servoTimeout;servoAngleIncrease
int ParseSerialInput() {
  int i = 0;
  int values_parsed = 0;
  int char_parsed;
  int milliseconds, increase;
  float angle;
  const char* _str = input_buffer.c_str();
  Serial.print("%");
  Serial.println(_str);

  char_parsed = ReadNextFloat(&(_str[i]), &angle);
  if (char_parsed == 0) {
    return values_parsed;
  }
  values_parsed++;
  i += char_parsed + 1;

  char_parsed = ReadNextInt(&(_str[i]), (int*)&milliseconds);
  if (char_parsed == 0) {
    return values_parsed;
  }
  values_parsed++;
  i += char_parsed + 1;

    char_parsed = ReadNextInt(&(_str[i]), (int*)&increase);
  if (char_parsed == 0) {
    return values_parsed;
  }
  values_parsed++;
  i += char_parsed + 1;

  servoTimeout = milliseconds;
  servoAngle = angle;
  servoAngleIncrease = increase;
  
  return values_parsed;
}


void setup() {
    Serial.begin(115200);
    while (!Serial);
    //Serial.println("Starting...");
    steeringWheel.attach(STEERING_SERVO_PIN);
    driverMotor.attach(DRIVER_MOTOR_PIN, 1000, 2000);
    // we must initialize the pixy object
    res = pixy.init();
    //Serial.println("pixy.init() = " + String(res));
    // Getting the RGB pixel values requires the 'video' program
    res = pixy.changeProg("video");
    //Serial.println("pixy.changeProg(video) = " + String(res));
    
}

uint8_t r, g, b;
uint8_t greyscale;
PixelRowBlackLine leftLine, rightLine;
int i;
void loop() {
    if (ReadSerial())
    {
      ParseSerialInput();
      input_buffer = "";
    }
    //res = pixy.line.getAllFeatures();
    //steeringWheel.SlowWrite(servoAngle, servoTimeout, servoAngleIncrease);
    //steeringWheel.write(servoAngle);
    //steeringWheel.setSteeringAngle(servoAngle);
    //Serial.println("servoTempAngle: " + String(steeringWheel.getTempAngle()) + "\tservoFinalAngle: " + String(steeringWheel.getFinalAngle()) + "\tsteerANgle: " + String(steeringWheel.getSteeringAngle()));
    

    for (i = 0; i < pixy.frameWidth; i++)
    {
      if (pixy.video.getRGB(i, pixy.frameHeight/2, &r, &g, &b)==0)
      {
        greyscale = PixelGreyscaleRow::RGBtoGreyscale(r, g, b);
        Serial.print(String((int)greyscale) + ";");
        // Serial.print(String(r) + ",");
        // Serial.print(String(g) + ",");
        // Serial.print(String(b) + ";");
      }
      else{
        greyscale = 255;
        // Serial.print("-1,");
        // Serial.print("-1,");
        Serial.print("-1;");
      }
      pixelRow.addPixelGreyscale(greyscale);
    }
    
    leftLine = pixelRow.getFirstBlackLine(servoAngle, servoTimeout);
    rightLine = pixelRow.getLastBlackLine(servoAngle, servoTimeout);
    pixelRow.clear();

    Serial.print(",");
    if (leftLine.beginIndex == rightLine.beginIndex)
    {
      if (leftLine.beginIndex != leftLine.endIndex)
      {
        Serial.print(String(leftLine.beginIndex) + ";" + String(leftLine.endIndex));
      }
      else{
        Serial.print(";");
      }
       Serial.print(";;");
    }
    else{
      if (leftLine.beginIndex != leftLine.endIndex)
      {
        Serial.print(String(leftLine.beginIndex) + ";" + String(leftLine.endIndex));
      }
      else{
        Serial.print(";");
      }
      Serial.print(";");
      if (rightLine.beginIndex != rightLine.endIndex)
      {
        Serial.print(String(rightLine.beginIndex) + ";" + String(rightLine.endIndex));
      }
      else{
        Serial.print(";");
      }
    }
    Serial.print(";");
    Serial.println();
    delay(50);
}

