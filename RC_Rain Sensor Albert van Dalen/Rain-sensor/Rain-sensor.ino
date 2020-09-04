/*
Board = Pololu A-Star 328PB

Copyright (C) 2015  Albert van Dalen
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .
Version 
16-11-2015
20-8-2017
*/

#include <Streaming.h>
#include <avdweb_AnalogReadFast.h>
#include "avdweb_HeartBeat.h"
#include "avdweb_VirtualDelay.h" 
#include "Classes.h"
#include "Definitions.h"

HeartBeat heartBeat(ledRedPin);
Led led;
Somfy somfy;
RainSensor rainSensor; 
LightSensor lightSensor;
Heather heather;
VirtualDelay heaterOffDelay, heaterOnDelay, printDelay;

bool Up;

void setup() 
{ Serial.begin(115200); 
  Serial << "Start\n";
  rainSensor.calibrate(); // calibration: power-up with dry and clean rainsensor    
  lightSensor.init();  
}

void loop() 
{ lightSensor.update();
  rainSensor.update();
  heartBeat.poll(); 
  
  if(lightSensor.night() || rainSensor.rain()) somfy.up(1);
  else somfy.up(0);

  printDelay.start(500);
  if(printDelay.elapsed()) 
    Serial << "\n" << "rain: " << rainSensor.RCtime_us << " light: " << lightSensor.light << " up: " << Up; 

  if(heaterOffDelay.elapsed()) // this sequence has a deadlock
  { heather.on(); 
    heaterOnDelay.start(300); // 300/(300+700) = 30%
  }
  if(heaterOnDelay.elapsed()) // P= 12V^2/42ohm=3,4W 1W = 30% on 
  { heather.off(); 
    heaterOffDelay.start(700);
  }
  DO_ONCE(heaterOffDelay.start(700)); // breaks the deadlock, you can start with any delay object you want e.g. delay2.start(0);

  if(somfy.upState) led.red(1);  
}
