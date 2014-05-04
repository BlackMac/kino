#include <EtherCard.h>
#include "webserver.h"

const char WebServer::_page_err[] PROGMEM =
"HTTP/1.0 500 Internal Server Error\r\n"
"Content-Type: application/json\r\n"
"Retry-After: 600\r\n"
"\r\n"
"{\"status\":\"error\"}"
;

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[800]; // tcp/ip send and receive buffer

WebServer::WebServer(void * (*responder)(BufferFiller &bfill, char command, char value)) {
  _responder = responder;
}

void WebServer::_respond(char command, char value) {
  BufferFiller bufferfill;
  bufferfill = ether.tcpOffset();
  _responder(bufferfill, command, value);
  ether.httpServerReply(bufferfill.position());
}

uint8_t WebServer::setup() {
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    return 0;

  if (!ether.dhcpSetup())
    return 0;

  return 1;
}

void WebServer::process() {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  if (pos) {

    char *data = (char *) Ethernet::buffer + pos;
    if (strncmp("GET /", data, 5) == 0) {
      char command = data[5];
      char value = data[6];
      _respond(command, value);
    } else {
      memcpy_P(ether.tcpOffset(), _page_err, sizeof _page_err);
      ether.httpServerReply(sizeof _page_err - 1);

      // data += 5;
      // if (data[0] == '1') {
      //   lampScreen.turnOn();
      // } else if (data[0] == '2') {
      //   lampBar.turnOn();
      // } else if (data[0] == '3') {
      //   lampScreen.turnOff();
      // } else if (data[0] == '4') {
      //   lampBar.turnOff();
      // }
      // delay(1);
      // returnStatus();
    }
  }
}
