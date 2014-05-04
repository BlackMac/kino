/*
  POWER: 20C
  16:9 : A0C
*/

#include "config.h"
#include <EtherCard.h>
#include <IRremote.h>
#include <RCSwitch.h>
#include "lamp.h"
#include "webserver.h"

Lamp lampScreen (1, RF_OUT_PIN);
Lamp lampBar (2, RF_OUT_PIN);

void *responder(BufferFiller &bfill, char command, char value) {
  /**
    http://xxx.xx/[command][value]
    where command:
      l = light
      d = darkness
    value:
      s = screen Lamps
      b = bar Lamps
  **/
  if (value == 's') {
    if (command == 'l') {
      lampScreen.turnOn();
    }
    if (command == 'd') {
      lampScreen.turnOff();
    }
  }
  if (value == 'b') {
    if (command == 'l') {
      lampBar.turnOn();
    }
    if (command == 'd') {
      lampBar.turnOff();
    }
  }


  bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Retry-After: 600\r\n"
    "\r\n"
    "{"
    "\"status\":\"ok\","
    "\"lamps_on\":{"
    "\"screen\":$F,"
    "\"bar\":$F"
    "}"
    "}"),lampScreen.isOn()?PSTR("true"):PSTR("false"),lampBar.isOn()?PSTR("true"):PSTR("false"),command,value);
}

IRrecv irReceiver(IR_IN_PIN);
WebServer webServer(&responder);
decode_results results;



void setup()
{
  irReceiver.enableIRIn(); // Start the receiver
  webServer.setup();
  lampScreen.turnOff();
  lampBar.turnOff();
}

void loop() {
  if (irReceiver.decode(&results)) {
    if (results.value == 0x20C) {
      lampScreen.toggle();
    } else if (results.value == 0xA0C) {
      lampBar.toggle();
    }
    irReceiver.resume(); // Receive the next value
  }

  webServer.process();
}
