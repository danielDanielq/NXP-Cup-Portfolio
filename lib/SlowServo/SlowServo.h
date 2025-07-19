/*
* Copyright 2023 Constantin Dumitru Petre RĂDULICEA
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*   http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#ifndef _SLOWSERVO_H_
#define _SLOWSERVO_H_

#include "Arduino.h"

#if ENABLE_ARDUINO == 1
  #include <Servo.h>
#else
  #include <PWMServo.h>
#endif

//#include <map>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#define DEFAULT_ANGLE_INCRESE 4
#define MIN_UPDATE_TIMEOUT_MS 10
#define DEFAULT_UPDATE_TIMEOUT_MS MIN_UPDATE_TIMEOUT_MS
#define CALLBACK_TIMER_CLOCKS (1000 * DEFAULT_UPDATE_TIMEOUT_MS)

#if ENABLE_ARDUINO == 1
  class SlowServo : public Servo
#else
  class SlowServo : public PWMServo
#endif
{
private:
    static unsigned int curID;
    //static std::map<unsigned int, SlowServo*> instances;
    //static IntervalTimer timer;

    unsigned int InstanceID;
    int finalAngle;
    int tempAngle;
    unsigned int LastUpdateTime_ms;
    unsigned int UpdateTimeout_ms;
    int angleIncrease;

/*
    static void SlowWriteCallback(){
        //Serial.println("Callback: " + String(millis()));
        auto instances_ = SlowServo::GetMapOfActiveObjects();

        for (auto it = instances_.begin(); it != instances_.end(); ++it) {
            it->second->UpdateTempAngle();
            //Serial.println("Callback: local angle=" + String(it->second->tempAngle));
        }
        if (instances_.size() <= 0) {
            SlowServo::GetTimer().end();
        }
    }
    
    void UpdateTempAngle(){
        if((millis() - LastUpdateTime_ms) >= UpdateTimeout_ms){
            if (finalAngle != tempAngle)
            {
                if (tempAngle > finalAngle)
                {
                    tempAngle = std::max((tempAngle - angleIncrease), finalAngle);
                }
                else{
                    tempAngle = std::min((tempAngle + angleIncrease), finalAngle);
                }
                this->write(tempAngle);
                LastUpdateTime_ms = millis();
            }
            else{
                if (SlowServo::instances.find(InstanceID) != SlowServo::instances.end()) {
                    SlowServo::instances.erase(InstanceID);
                }
            }
        }
    }
    */
public:
#if ENABLE_ARDUINO == 1
  SlowServo() : Servo() {
#else
  SlowServo() : PWMServo() {
#endif
        this->InstanceID = curID++;
        this->finalAngle = 90;
        this->tempAngle = 90;
        this->angleIncrease = DEFAULT_ANGLE_INCRESE;
        this->UpdateTimeout_ms = DEFAULT_UPDATE_TIMEOUT_MS;
        this->LastUpdateTime_ms = 0;
    }

    ~SlowServo(){
        /*
        if (SlowServo::instances.find(InstanceID) != SlowServo::instances.end()) {
            SlowServo::instances.erase(InstanceID);
        }
        */
    }
/*
    static std::map<unsigned int, SlowServo*>& GetMapOfActiveObjects() { 
        return instances;
    }

    static IntervalTimer& GetTimer(){
        return timer;
    }
*/
    void SlowWrite(int angleArg){
        this->SlowWrite(angleArg, this->UpdateTimeout_ms, this->angleIncrease);
    }

    void SlowWrite(int angleArg, unsigned int milliseconds, unsigned int angleIncrease){
        if(milliseconds < MIN_UPDATE_TIMEOUT_MS){
            this->tempAngle = angleArg;
            this->finalAngle = angleArg;
            this->write(angleArg);
            return;
        }
        else if (angleArg == this->tempAngle) {
            this->finalAngle = this->tempAngle;
            return;
        }
        else if(angleIncrease == 0){
            return;
        }
        
        [[maybe_unused]]size_t nrInstances;
        this->finalAngle = angleArg;
        this->UpdateTimeout_ms = milliseconds;
        this->angleIncrease = angleIncrease;
        //nrInstances = SlowServo::instances.size();
        //SlowServo::instances[InstanceID] = this;
        /*
        if (nrInstances <= 0) {
            SlowServo::timer.begin(SlowServo::SlowWriteCallback, CALLBACK_TIMER_CLOCKS);
        }
        */
    }
    void setUpdateTimeout_ms(unsigned int milliseconds){
        this->UpdateTimeout_ms = milliseconds;
    }

    unsigned int getUpdateTimeout_ms(){
        return this->UpdateTimeout_ms;
    }

    void setAngleIncrease(int angleIncrease){
        this->angleIncrease = angleIncrease;
    }

    int getAngleIncrease(){
        return this->angleIncrease;
    }

    int getTempAngle(){
        return this->tempAngle;
    }
    int getFinalAngle(){
        return this->finalAngle;
    }
};

unsigned int SlowServo::curID = 0;
//std::map<unsigned int, SlowServo*> SlowServo::instances;
//IntervalTimer SlowServo::timer;

#endif