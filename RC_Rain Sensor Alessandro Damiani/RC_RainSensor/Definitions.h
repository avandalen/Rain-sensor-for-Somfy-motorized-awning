const byte rainSensorInPin = 2;
const byte rainSensorOutPin = 11;
const byte rainLedPin = 4;
const byte heaterLedPin = 5;
const byte heaterPin = 10;
const byte thermistorPin = A2;

const int rainSensorMinValue = 10; // Measure above "dry level" to consider it a positive reading
const int ticksForPositiveRain = 30; // Number of consecutive readings above "dry level" before considering it rain

const float heaterTargetTemp = 62.0;
const float hysteresisHalfCycle = 1.5;

const float heaterMinTemp = 40;

const int frequencyTimerPeriod_ms = 1000; // Overall duration of reading cycle
