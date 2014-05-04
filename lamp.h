#include <RCSwitch.h>

class Lamp
{
    private:
          RCSwitch rcSwitch;
          uint8_t _lampId;
          uint8_t _state;

    public:
          Lamp(uint8_t id, uint8_t pin);
          void turnOn();
          void turnOff();
          void toggle();
          uint8_t isOn();
};
