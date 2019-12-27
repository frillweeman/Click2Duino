#include <Arduino.h>

#define DEFAULT_SAMPLES_PER_AVERAGE 10
#define HIGH_TRIGGERED_THRESHOLD 100

class RadioModule
{
public:
  RadioModule(int analogPin = 0, int samplesPerAverage = DEFAULT_SAMPLES_PER_AVERAGE)
      : pin(analogPin),
        samplesPerAverage(samplesPerAverage)
  {
    phase = 0;
    currentAnalogValue = -1;
    values = new int[samplesPerAverage];
  }

  void update()
  {
    values[phase] = analogRead(pin);

    // update phase
    phase = (phase + 1) % samplesPerAverage;

    // update avg value
    calculateAverageValue();

    if (currentAnalogValue == 0)
      triggered = true;
    else
      triggered = false;
  }

  int getAverageValue() const
  {
    return currentAnalogValue;
  }

  bool isTriggered() const
  {
    return triggered;
  }

private:
  int pin;
  int samplesPerAverage;
  int *values;
  int phase;
  int currentAnalogValue;
  bool triggered;

  void calculateAverageValue()
  {
    int accumulator = 0;
    for (int i = 0; i < samplesPerAverage; ++i)
    {
      accumulator += values[i];
    }
    currentAnalogValue = round(double(accumulator) / samplesPerAverage);
  }

  void setSamplesPerAverage(int samplesPerAverage)
  {
    this->samplesPerAverage = samplesPerAverage;
  }
};