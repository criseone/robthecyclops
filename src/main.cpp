#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#ifdef _AVR_
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN 15
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

static const int servoPinRudi = 27;
static const int servoPinRolf = 26;

Servo Rudi;
Servo Rolf;

long phase1 = 10000;
long phase2 = 5000;
long phase3 = 5000;
long phase4 = 10000;

long timestamp = 0;

int state = 1;

int pos = 0;

int sensor = 0;

void switchState(int s);

boolean isTimeUp(int duration)
{
  return millis() - timestamp > duration;
}

boolean checkPhoneSensor()
{
  sensor = analogRead(0);
  return sensor > 3300;
  //return sensor >= 0;
}

void setup()
{
  Rudi.attach(servoPinRudi);
  Rolf.attach(servoPinRolf); // attaches the servo on pin 9 to the servo object

  Serial.begin(9600);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  switchState(0);
}

void loop()
{
  if (state == 0)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    Rudi.write(55);
    Rolf.write(55);
    Serial.println("Waiting");
    sensor = analogRead(0);
    Serial.println(sensor);
    delay(100);

    if (checkPhoneSensor() == true)
    {
      switchState(1);
    }
  }

  if (state == 1)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
    }
    pixels.show();
    Rudi.write(55);
    Rolf.write(55);
    Serial.println("Phase 1: Charging");
    sensor = analogRead(0);
    Serial.println(sensor);
    delay(100);

    //if (checkPhoneSensor() == true)
    if (isTimeUp(phase1))
    {
      switchState(2);
    }
  }

  if (state == 2)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(255, 127, 0));
    }
    pixels.show();
    for (pos = 55; pos <= 60; pos += 1)
    { // goes from 55 degrees to 180 degrees
      Rudi.write(pos);
      Rolf.write(pos);
      delay(50);
    }
    for (pos = 60; pos >= 55; pos -= 1)
    { // goes from 75 degrees to 55 degrees
      Rudi.write(pos);
      Rolf.write(pos);
      delay(50);
    }
    Serial.println("Phase 2: Slow");

    if (isTimeUp(phase2))
    {
      switchState(3);
    }
  }

    if (state == 3)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(255, 50, 0));
    }
    pixels.show();
    for (pos = 55; pos <= 70; pos += 1)
    { // goes from 55 degrees to 180 degrees
      Rudi.write(pos);
      Rolf.write(pos);
      delay(40);
    }
    for (pos = 70; pos >= 55; pos -= 1)
    { // goes from 75 degrees to 55 degrees
      Rudi.write(pos);
      Rolf.write(pos);
      delay(40);
    }
    Serial.println("Phase 3: Medium");

    if (isTimeUp(phase3))
    {
      switchState(4);
    }
  }

  if (state == 4)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    }
    pixels.show();
    for (pos = 55; pos <= 120; pos += 1)
    { // goes from 55 degrees to 180 degrees
      Rolf.write(pos);
      delay(15);
    }
    for (pos = 120; pos >= 55; pos -= 1)
    { // goes from 75 degrees to 55 degrees
      Rolf.write(pos);
      delay(15);
    }
        for (pos = 55; pos <= 90; pos += 1)
    { // goes from 55 degrees to 180 degrees
      Rudi.write(pos);
      delay(15);
    }
    for (pos = 90; pos >= 55; pos -= 1)
    { // goes from 75 degrees to 55 degrees
      Rudi.write(pos);
      delay(15);
    }
    Serial.println("Phase 4: Fast");

    if (isTimeUp(phase4))
    {
      switchState(0);
    }
  }
}

void switchState(int s)
{
  state = s;
  timestamp = millis();
}