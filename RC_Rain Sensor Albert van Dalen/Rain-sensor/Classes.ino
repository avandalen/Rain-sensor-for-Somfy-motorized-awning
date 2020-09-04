int LightSensor::getLight()
{ return 1023-analogReadFast(lightSensorPin);  
}

void LightSensor::update()
{ int alfa = 16;
  light = (light*(alfa-1) + getLight())/alfa; 
}

void LightSensor::init()
{ light = getLight(); // don't start filter with 0
}

bool LightSensor::night()
{ return light < darkLight;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

RainSensor::RainSensor()
{ pinMode(rainSensorOutPin, OUTPUT);          
  pinMode(rainSensorInPin, INPUT);    
}

void RainSensor::update()
{ int alfa = 128;
  RCtime_us = (RCtime_us*(alfa-1) + getRCtime())/alfa; 
}

void RainSensor::calibrate()
{ led.green(1);
  const int n = 2000;
  long t=0;
  for(int i=0; i<n; i++) t += getRCtime();
  dryRCtime_us = t/n;
  Serial << "\ndryRCtime_us: " << rainSensor.dryRCtime_us;
  RCtime_us =  dryRCtime_us; // don't start filter with 0
  led.green(0);
}

bool RainSensor::rain()
{ return RCtime_us > dryRCtime_us + rainSensorMarge;
}

int RainSensor::getRCtime()
{ digitalWrite(rainSensorOutPin, 0); 
  delay(1); // 1ms discharge
  unsigned long t1 = micros();
  digitalWrite(rainSensorOutPin, 1); // charge
  while(!digitalRead(rainSensorInPin) && (micros()-t1) < timeOut_us); // while < 2.5V
  int us = micros()-t1;
  
  delay(1); // 1ms charge to 5V
  unsigned long t0 = micros();
  digitalWrite(rainSensorOutPin, 0); // discharge
  while(digitalRead(rainSensorInPin) && (micros()-t0) < timeOut_us); // while > 2.5V 
  us += micros()-t0;
  return us;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

Somfy::Somfy()
{ openDrain(somfyUpPin, 1); // button off
}

void Somfy::up(bool b)
{ Up=b;
  if(b && !upState)
  { upState=1;
    openDrain(somfyUpPin, 0); // button on 
    Serial << "\nup";
    delay(somfyUpButton_ms); // delay is allowed
    openDrain(somfyUpPin, 1); // button off
  }
  if(!b && upState) upState=0; // reset upState
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

Led::Led()
{ pinMode(ledRedPin, OUTPUT); 
  pinMode(ledGreenPin, OUTPUT);
}

void Led::red(bool on)
{ digitalWrite(ledRedPin, on);
}

void Led::green(bool on)
{ digitalWrite(ledGreenPin, on);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

Heather::Heather() 
{ pinMode(heatherPin, OUTPUT); 
}

void Heather::on()
{ digitalWrite(heatherPin, 1);
  //led.red(1); 
}

void Heather::off()
{ digitalWrite(heatherPin, 0);
  //led.red(0); 
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

bool SimpleSoftPWM2::pwm(byte value) // 0 ... 255, only for LEDs
{ unsigned long now_us = micros();
  if((now_us - start_us) >= periodTime_us) start_us = now_us;
  return((now_us - start_us) < value * 78); // 19890/255=78
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

void openDrain(byte pin, bool value)
{ if(value) pinMode(pin, INPUT);
  else pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW); 
}
