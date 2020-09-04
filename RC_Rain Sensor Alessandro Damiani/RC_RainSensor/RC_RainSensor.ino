#include "Classes.h"
#include "Definitions.h"

#define RT0 1218   // ?
#define B 3977      // K
#define VCC 5    //Supply voltage
#define R 10000  //R=10K?

//Variables
float RT, VR, ln, sensorTemperature, T0, VRT;
 
RainSensor rainSensor;
Heater heater;

int rainTicks = 0;
long heaterOnTicks = 0;
long heaterOffTicks = 0;
char raining = 'I';
char heating = '-';

long startTime = 0;
long sleepTime = 0;

 
void setup()
{ Serial.begin(9600);
  // Initializing
  pinMode(heaterPin, OUTPUT);
  pinMode(rainLedPin, OUTPUT);
  pinMode(heaterLedPin, OUTPUT);
  pinMode(thermistorPin, INPUT);
  digitalWrite(heaterPin, LOW);
  digitalWrite(rainLedPin, LOW);
  digitalWrite(heaterLedPin, LOW);
  T0 = 25 + 273.15; 
  // Heating up to dry sensor...
  heater.initHeater(); 
  delay(1000);
  // Setting DRY value...
  rainSensor.calibrate();
  // Initialization DONE.
}

void monitorHeaterState() {
  // Get state from heater and perform calculations to determine behavior
  switch (heater.getState()) {
    case 'H':
      heaterOnTicks++;
      if (sensorTemperature >= heaterTargetTemp + hysteresisHalfCycle) {
        heater.coolDown();
        heaterOnTicks = 0;
      }
      break;
    case 'C':
      heaterOffTicks++;
      if (sensorTemperature <= heaterTargetTemp - hysteresisHalfCycle) {
        heater.setCooling(false);
        heater.off();
        heaterOffTicks = 0;
      }
      break;
    default:
      heaterOnTicks = 0;
      heaterOffTicks = 0;
  }
}

void outputCurrentState() {
  // Print formatted data on serial
  Serial.print("$");
  Serial.print(raining);
  Serial.print(",");
  Serial.print(heater.getState());
  Serial.print(",");
  Serial.print(rainSensor.usec());
  Serial.print(",");
  Serial.print(rainSensor.dry_us);
  Serial.print(",");
  Serial.print(sensorTemperature);
  Serial.print(",");

  int rainTickHours = rainTicks / 60 / 60;
  int rainTickMinutes = (rainTicks - (rainTickHours * 60)) / 60;
  int rainTickSeconds = rainTicks % 60;

  if (rainTickHours < 10) {
    Serial.print("0");
  }
  Serial.print(rainTickHours);
  Serial.print(":");
  
  if (rainTickMinutes < 10) {
    Serial.print("0");
  }
  Serial.print(rainTickMinutes);
  Serial.print(":");
  
  if (rainTickSeconds < 10) {
    Serial.print("0");
  }
  Serial.print(rainTickSeconds);
  Serial.println("#");
}

float getThermistorReading() {
  VRT = analogRead(thermistorPin);   //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT
  ln = log(RT / RT0);
  sensorTemperature = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
  sensorTemperature = sensorTemperature - 273.15;                 //Conversion to Celsius

  return sensorTemperature;
}
 
void loop()
{
  startTime = millis();
  monitorHeaterState();
  getThermistorReading();
  outputCurrentState();
  if (rainSensor.rain()) { 
    // Turn on rain LED
    digitalWrite(rainLedPin, HIGH);
    if (!heater.isCooling()) {
        // Turn on heater if it is not during cooling phase
        heater.on();
      }
    // Wait for 100ms, to make LED flashing visible
    delay(100);
    
    if (++rainTicks >= ticksForPositiveRain) {
      // Positive rain, this is (ticksForPositiveRain)th reading
      raining = 'R';
      
    } else {
      // Still don't know if it is raining, not enough ticks
      raining = '?';
      // Turn off rain LED
      digitalWrite(rainLedPin, LOW);   
    }
    
  } else {
    // Sensor is dry, reset all variables
    rainTicks = 0;
    raining = 'D';
    // Turn rain LED off
    digitalWrite(rainLedPin, LOW);
    // Turn heater off
    heater.minimalHeat();
  }

  // If the cycle took less than 1 seconds, sleep for the rest of the time. 
  // This is done to get exactly 1 reading per second
  sleepTime = frequencyTimerPeriod_ms - (millis() - startTime);
  if (sleepTime > 0);
    delay(sleepTime);
    
}