#include <RCSwitch.h>
#include "lamp.h"

Lamp::Lamp(uint8_t id, uint8_t pin) {
  rcSwitch.enableTransmit(pin);
  _lampId = id;
}

void Lamp::turnOn() {
  this->rcSwitch.switchOn("10101",_lampId);
  _state=1;
}

void Lamp::turnOff() {
  this->rcSwitch.switchOff("10101",_lampId);
  _state=0;
}

void Lamp::toggle() {
  if (_state==1) {
    this->turnOff();
  } else {
    this->turnOn();
  }
}

uint8_t Lamp::isOn() {
  return _state;
}
