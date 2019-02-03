#include <MillisTimer.h>

#include <Adafruit_NeoPixel.h>
const int pwPin1 = 4;
long sensor, sensorIN, sensorCM, sensorFT;
int oldVal;
int MaxCM = 365;
int MinCM = 45;//1.5 feet
int NumToLight;
//Neopixel  Setup
#define PIN      2
#define N_LEDS 144
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t invisible = strip.Color(0, 0, 0);
// Create a timer that fires every 1000 milliseconds.
MillisTimer timer1 = MillisTimer(1000);
int timeout=60;
bool timerstarted;
void setup() {
  pinMode(pwPin1, INPUT);
  strip.begin();
  ClearLEDStrip();
  strip.setBrightness(200);
  strip.show();
  Serial.begin(9600);
  //timer1.setRepeats(5);

}
void loop() {
  timer1.run();
  delay(50);
  sensor = pulseIn(pwPin1, HIGH);
  sensorCM = sensorToCM(sensor);
  sensorCM = constrain(sensorCM, MinCM, MaxCM); //Constrain 5 inches to 15 feet
  NumToLight = map(sensorCM, MinCM, MaxCM, N_LEDS, 0); //map the amount of LEDs to light depending on the sensor Value.
  NumToLight = constrain(NumToLight, 0, N_LEDS); //Constrain 5 inches to 15 feet
  if (NumToLight != oldVal) //only update leds if val has changed.
  {
    if(NumToLight>143)
    {
       LedCreep(N_LEDS, 144, red, red);
       delay(200);
       ClearLEDStrip();
       RestartTimer();
    }
    LedCreep(N_LEDS, NumToLight, red, green);
   RestartTimer();
  }
  else//if numtolight hasnt changed..
  {
    if (timerstarted == false)//start the timer
    {
      timer1.start();//start the timer
      timerstarted = true;//set timerstarted to true
    }
    if (!timer1.isRunning())//if timer timeout and isnt running anymore
    {
      ClearLEDStrip();
    }
  }
  oldVal = NumToLight;//set old value to new value

}

//this fuction tatkes in the amount of LEDs on teh strip and allows the user to input the amount of LEd's he wants to light up on each END of the Strip.
static void LedCreep(int TotalLeds, int AmountToLight, uint32_t c, uint32_t cbackground ) {
  int _TotalLeds = (TotalLeds / 2);
  int _AmountToLight = AmountToLight / 2;
  for (uint16_t i = 0; i <= _AmountToLight; i++) {//turns on the leds starting from beginning
    strip.setPixelColor(i, c);
  }
  for (uint16_t i = _AmountToLight; i <= _TotalLeds; i++) {//turns off the rest of the leds that dont need to be on , starting at end of previous 4.
    int gradient = map(i, 0, _TotalLeds, 0, 255);
    gradient = constrain(gradient, 0, 255); //Constrain 5 inches to 15 feet
    strip.setPixelColor(i, gradient, 255, 0);
  }
  for (uint16_t i = TotalLeds; i >= (TotalLeds - _AmountToLight); i--) { //turns on the leds starting from beginning
    strip.setPixelColor(i, c);
  }
  for (uint16_t i = ((TotalLeds - _AmountToLight) - 1); i >= (_TotalLeds); i--) { //turns on the leds starting from beginning
    int gradient = map(i, TotalLeds, _TotalLeds, 0, 255);
    gradient = constrain(gradient, 0, 255); //Constrain 5 inches to 15 feet
    strip.setPixelColor(i, gradient, 255, 0);
  }
  strip.show();
}
int sensorToCM(int sensorReading)
{
  int _sensorCM = 0;
  int _sensorIN = 0;
  _sensorIN = sensorReading / 147; //convert to Inches by dividing PW by 147
  _sensorCM = _sensorIN * 2.54; //convert to CM
  return _sensorCM;
}
static void ClearLEDStrip() {
  strip.clear();
  strip.show();
}
static void RestartTimer() {
 timer1.setRepeats(timeout);//reset timer timout to 5 seconds
    timer1.start();//start the timer
    timerstarted = false;//set timerstarted to false
}
