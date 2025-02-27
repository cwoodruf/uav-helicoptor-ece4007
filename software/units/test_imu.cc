/*
* Copyright (c) 2012 Joey Yore
*
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
* OTHER DEALINGS IN THE SOFTWARE.
*/
#include <iostream>
#include "../imu.h"
#include "../events/timeout.h"
#include "../math/vector3.h"
#include "../util.h"

#define RATE		0.0012
#define FACTOR		0.0010

IMU imu(RATE,FACTOR);
Vector3 angles;

void imu_loop() {
    imu.update(angles);
    register_timeout(imu_loop,RATE);
}

int main() {
	T_TONDelay delay = {false,false,1.0,0};
	T_TONDelay imudelay = {false,false,RATE,0};

    imu.calibrate();
    if(!imu.isCalibrated()) {
		std::cout << "FAILED TO CALIBRATE" << std::endl;
        return -1;
    }

    while(1) {
		if(ton_delay(imudelay,true)) {
    		imu.update(angles);
			ton_delay(imudelay,false);
		}

    	if(ton_delay(delay,true)) {
			std::cout << angles << std::endl;
			ton_delay(delay,false);
		}
		
    }
    return 0;
}
