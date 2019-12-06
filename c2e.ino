#include "Color.cpp"

// define standard colors
Color red(255, 0, 0);
Color green(0, 255, 0);
Color orange(255, 165, 0);
Color off(0, 0, 0);

struct Pins
{
  int BUTTON = 2;
  int RELAY = 7;
  int GREEN = 9;
  int BLUE = 10;
  int RED = 11;
} pins;

// C2E Activation Sequence
const int HOLD_TIME = 1500;
const int RELEASE_TIME = 1000;

void setup()
{
  // set pin modes
  pinMode(pins.BLUE, OUTPUT);
  pinMode(pins.RED, OUTPUT);
  pinMode(pins.GREEN, OUTPUT);
  pinMode(pins.RELAY, OUTPUT);
  pinMode(pins.BUTTON, INPUT_PULLUP);

  // output initializations
  digitalWrite(pins.RELAY, LOW);
  setLED(green);
}

void loop()
{
  // when radio signal is detected
  if (isSignal())
  {
    // set LED to red
    setLED(red);

    // check for continuous press for HOLD_TIME ms
    for (int i = 0; i < HOLD_TIME / 10; ++i)
    {
      if (!isSignal())
      {
        // reset if released
        setLED(green);
        return;
      }
      delay(10);
    }

    // held for correct length of time

    // turn LED off
    setLED(off);

    // wait for user to release radio PTT within RELEASE_TIME ms
    unsigned long releaseTime = millis();
    while (isSignal() && millis() - releaseTime <= RELEASE_TIME)
      delay(1);

    // if PTT held down too long
    if (isSignal())
    {
      // error LED and reset
      setLED(orange);
      unsigned long startError = millis();
      while (isSignal())
        ;
      while (millis() - startError <= 2000)
        ;
      setLED(green);
      return;
    }

    // TRIGGERED

    // activate relay (gate)
    digitalWrite(pins.RELAY, HIGH);

    // flash red LED to indicate triggered
    for (int i = 0; i < 6; ++i)
    {
      setLED(red);
      delay(150);
      setLED(off);
      delay(150);
    }

    // deactivate relay (gate)
    digitalWrite(pins.RELAY, LOW);

    // reset
    setLED(green);
  }
}

bool isSignal()
{
  return !digitalRead(pins.BUTTON);
}

void setLED(Color color)
{
  analogWrite(pins.RED, color.red);
  analogWrite(pins.GREEN, color.green);
  analogWrite(pins.BLUE, color.blue);
}