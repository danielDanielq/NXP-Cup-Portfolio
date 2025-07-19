#include <Arduino.h>
#include <SPI.h>
#include "pixy2_libs/host/arduino/libraries/Pixy2/Pixy2.h"


Pixy2 pixy;
int8_t res;

void setup() {
    // serial Initialization
    Serial.begin(115200);
    //Serial.begin(250000);
    delay(100);
    
    // we must initialize the pixy object
    res = pixy.init();
    //Serial.println("% pixy.init() = " + String(res));
    // Getting the RGB pixel values requires the 'video' program
    res = pixy.changeProg("video");
    //Serial.println("% pixy.changeProg(video) = " + String(res));
    pixy.setCameraBrightness((uint8_t)50);
}

inline uint16_t serial_read_uint16_t(){
    uint16_t res;
    while (!Serial.available());
    
    Serial.readBytes((char*)(&res), sizeof(uint16_t));
    return res;
}

inline void serial_write_uint16_t(uint16_t &val){
    Serial.write((const uint8_t*)&val, sizeof(val));
}
inline void serial_write_uint8_t(uint8_t &val){
    Serial.write((const uint8_t*)&val, sizeof(val));
}

uint16_t x, y;
uint8_t r, g, b;
int count = 0;


void loop(){
    /*
    x = serial_read_uint16_t();
    y = serial_read_uint16_t();
    pixy.video.getRGB(x, y, &r, &g, &b, false);
    serial_write_uint8_t(r);
    serial_write_uint8_t(g);
    serial_write_uint8_t(b);
*/
Serial.println(String(count) + " " + String(millis()));
    for ( y = 0; y < pixy.frameHeight; y++)
    {
        for ( x = 0; x < pixy.frameWidth; x++)
        {
            count++;
            pixy.video.getRGB(x, y, &r, &g, &b, false);
            
        }
    }
    Serial.println(String(count) + " " + String(millis()));
}



