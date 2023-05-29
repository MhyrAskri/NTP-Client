#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <time.h>

class NTPClient
{

  private:

    char IP[15];
    String dateTime = "";
    byte mac[6] =
    {
      0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
    };

    unsigned int localPort = 8888;
    static const int NTP_PACKET_SIZE = 48; 
    byte packetBuffer[NTP_PACKET_SIZE];
    EthernetUDP Udp;

  public:

    bool begin(const char * address);
    void SetOffset(time_t offsetInSecond = 0);
    String GetDateTimeWithOffset(unsigned long epoch, time_t offsetInSecond);
    String GetDateTime();

};