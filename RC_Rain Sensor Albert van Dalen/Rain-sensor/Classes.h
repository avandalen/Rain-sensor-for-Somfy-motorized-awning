// Classes.h

class Somfy
{ 
public:
  Somfy();
  void up(bool b); 
  bool upState; // give just one somfyUpButton pulse
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------

class Led
{ 
public:
  Led();
  void red(bool);
  void green(bool);
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------

class RainSensor
{
public:
  RainSensor();
  void update();
  int getRCtime();
  void calibrate();
  bool rain();

  float RCtime_us;
  int dryRCtime_us;
private:
  const int timeOut_us = 1000;
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------

class LightSensor
{
public:
  void update();
  bool night();
  void init();

  float light;
private:
  int getLight(); 
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------

class Heather
{
public:
  Heather();
  void on();
  void off();
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------

class SimpleSoftPWM2 // only for LEDs and heating
{ public:
    bool pwm(byte value); // 0 ... 255
 
  private:
    unsigned long start_us, periodTime_us=19890; // minimum frequency to prevent LED flickering = 50Hz
};
