RainSensor::RainSensor()
{ pinMode(rainSensorOutPin, OUTPUT);         
  pinMode(rainSensorInPin, INPUT);   
}
 
bool RainSensor::rain()
{ 
  int currentUsec = usec();
  if (currentUsec < dry_us) {
    dry_us = currentUsec;
  }
  return (currentUsec > (dry_us + rainSensorMinValue));
}
 
void RainSensor::calibrate()
{ rainSensor.dry_us = rainSensor.usec();
}
 
int RainSensor::usec()
{ long us=0;
  for (int i=0; i < rainSamples; i++) us += RCtime_us();
  us /= rainSamples;
  return us;
}
 
int RainSensor::RCtime_us()
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
 
Heater::Heater()
{ 
  pinMode(heaterPin, OUTPUT);
  state = false;
  cooling= false;
}


char Heater::getState() {
  return state;
}

bool Heater::isCooling() {
  return cooling;
}

void Heater::coolDown() {
  cooling = true;
  off();
}

void Heater::setCooling(bool toSet) {
  cooling = toSet;
}

void Heater::initHeater() {
  heatToTemp(heaterTargetTemp); 
}

void Heater::heatToTemp(float targetTemp) {
  
  on();  
  do {
    startTime = millis();
    outputCurrentState();
    sleepTime = frequencyTimerPeriod_ms - (millis() - startTime);
    if (sleepTime > 0);
      delay(sleepTime);
    } while (getThermistorReading() < targetTemp);
  off();
}

void Heater::on()
{ 
  digitalWrite(heaterPin, HIGH);
  digitalWrite(heaterLedPin, HIGH);
  
  state = 'H';
  
  cooling = false;
}
 
void Heater::off()
{ 
  digitalWrite(heaterPin, LOW);
  digitalWrite(heaterLedPin, LOW);
  if (cooling) {
    state = 'C';
  } else {
    state = '-';
  }
}

void Heater::minimalHeat() {
  if (getThermistorReading() >= heaterMinTemp + hysteresisHalfCycle && state == 'H') {
    off();
  }
  if (getThermistorReading() <= heaterMinTemp - hysteresisHalfCycle && state != 'H') {
    on();
  }
}
