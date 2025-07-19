#include "Config.h"

Pixy2SPI_SS pixy_1;
Pixy2SPI_SS pixy_2;

int8_t pixyResult;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(100);
  }

  pinMode(SPI_SS_PIXY_1_PIN, OUTPUT);
  pinMode(SPI_SS_PIXY_2_PIN, OUTPUT);
  
  pixyResult = pixy_2.init(SPI_SS_PIXY_2_PIN);
  Serial.print("pixy_2.init=");
  Serial.println(pixyResult);

  pixyResult = pixy_1.init(SPI_SS_PIXY_1_PIN);
  Serial.print("pixy_1.init=");
  Serial.println(pixyResult);



  pixyResult = pixy_1.changeProg("line");
  Serial.print("pixy_1.changeProg=");
  Serial.println(pixyResult);

  pixyResult = pixy_2.changeProg("line");
  Serial.print("pixy_2.changeProg=");
  Serial.println(pixyResult);
}


void loop() {
    pixyResult = pixy_1.line.getAllFeatures(LINE_VECTOR | LINE_INTERSECTION);
    if(pixyResult >= (int8_t)0) {
        for (size_t i = 0; i < pixy_1.line.numVectors; i++) {
            Serial.print("pixy_1: ");
            pixy_1.line.vectors[i].print();
        }
    }
    else{
        Serial.print("ERROR: pixy_1.line.getAllFeatures=");
        Serial.println(pixyResult);
    }

    pixyResult = pixy_2.line.getAllFeatures(LINE_VECTOR | LINE_INTERSECTION);
    if(pixyResult >= (int8_t)0) {
        for (size_t i = 0; i < pixy_2.line.numVectors; i++) {
            Serial.print("pixy_2: ");
            pixy_2.line.vectors[i].print();
        }
    }
    else{
        Serial.print("ERROR: pixy_2.line.getAllFeatures=");
        Serial.println(pixyResult);
    }
}
