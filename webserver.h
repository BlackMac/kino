#include <EtherCard.h>

class WebServer
{
    private:
          static const char _page_err[];
          void * (*_responder)(BufferFiller &bfill, char command, char value);
          void _respond(char command, char value);

    public:
          WebServer(void * (*responder)(BufferFiller &bfill, char command, char value));
          void process();
          uint8_t setup();
};
