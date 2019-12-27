#include "Color.cpp"
#include "RadioModule.cpp"

#define RADIO_PIN A7 // analog
#define SAMPLES_PER_AVG 10

// define standard colors
Color red(255, 0, 0);
Color green(0, 255, 0);
Color orange(255, 165, 0);
Color off(0, 0, 0);

RadioModule radio(RADIO_PIN, SAMPLES_PER_AVG);

enum Pins
{
  RELAY = 7,
  GREEN = 9,
  BLUE = 10,
  RED = 11
};

enum State
{
  IDLE,
  ACTIVATING,
  RELEASE_CHECK,
  ACTIVATED,
  ERROR
} state,
    lastState;

// C2E Activation Sequence
const int HOLD_TIME = 1500;
const int RELEASE_TIME = 1000;

unsigned long startHoldTime;
unsigned long startReleaseTime;

void setup()
{
  // set pin modes
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RELAY, OUTPUT);

  // output initializations
  digitalWrite(RELAY, LOW);
  setLED(green);

  state = IDLE;
  lastState = IDLE;

  // debug
  Serial.begin(9600);
}

void loop()
{
  // poll the radio module for signal
  radio.update();

  State beforeChange = state;

  Serial.println(radio.getAverageValue());

  switch (state)
  {
  case IDLE:
    idle();
    break;
  case ACTIVATING:
    activating();
    break;
  case RELEASE_CHECK:
    release_check();
    break;
  case ACTIVATED:
    activated();
    break;
  case ERROR:
    error();
    break;
  }

  lastState = beforeChange;
  delay(10);
}

void setState(State newState)
{
  state = newState;
}

void idle()
{
  setLED(green);
  if (radio.isTriggered())
  {
    state = ACTIVATING;
    return;
  }
}

void activating()
{
  if (lastState != ACTIVATING)
  {
    Serial.println("Activating rising edge");
    // set LED to red
    setLED(red);

    // check for continuous press for HOLD_TIME ms
    startHoldTime = millis();
  }

  // if radio key released, return to idle
  if (!radio.isTriggered())
  {
    state = IDLE;
    return;
  }

  // if not released and time elapsed, advance state
  if (millis() - startHoldTime > HOLD_TIME)
  {
    Serial.println("entering release check");
    state = RELEASE_CHECK;
    return;
  }
}

void release_check()
{
  if (lastState != RELEASE_CHECK)
  {
    Serial.println("turning LED off");
    setLED(off);
    startReleaseTime = millis();
  }

  const unsigned long howLong = millis() - startReleaseTime;

  // check if time elapsed
  if (millis() - startReleaseTime > RELEASE_TIME)
  {
    if (radio.getAverageValue() != 1)
    {
      Serial.println("held down too long");
      state = ERROR;
      return;
    }
  }
  else if (radio.getAverageValue() == 1)
  {
    Serial.println("released within required time");
    // time has not elapsed and released
    state = ACTIVATED;
    return;
  }
}

void activated()
{
  // activate relay (gate)
  digitalWrite(RELAY, HIGH);

  // flash red LED to indicate triggered
  for (int i = 0; i < 6; ++i)
  {
    setLED(red);
    delay(150);
    setLED(off);
    delay(150);
  }

  // deactivate relay (gate)
  digitalWrite(RELAY, LOW);

  // reset
  state = IDLE;
}

void error()
{
  setLED(orange);
  delay(3000);
  state = IDLE;
}

void setLED(Color color)
{
  analogWrite(RED, color.red);
  analogWrite(GREEN, color.green);
  analogWrite(BLUE, color.blue);
}