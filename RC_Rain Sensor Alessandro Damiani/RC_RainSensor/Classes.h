class RainSensor
{
public:
  RainSensor();
  int usec();
  bool rain(); 
  void calibrate();
  int dry_us;
private:
  int RCtime_us();
  const int rainSamples = 128;
  const int timeOut_us = 1000;
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------
 
class Heater
{
public:
  Heater();
  void initHeater();
  void heatForMS(int ms);
  void coolDown();
  void on();
  void off();
  char getState();
  bool isCooling();
  void setCooling(bool toSet);
  void heatToTemp(float targetTemp);
  void minimalHeat();
private:
  char state;
  bool cooling;
};