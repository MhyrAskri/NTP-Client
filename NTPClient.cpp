#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <time.h>
#include "Timelib.h"
#include "NTPClient.h"

bool NTPClient::begin(const char * address, unsigned int port)
{
  bool active = false;
  strncpy(IP, address, sizeof(IP));
  localPort = port;

  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  if (!Serial)
  {
    Serial.println("Can not start Serial for ethernet shield in NTP-Client!");
  }
  
  // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } 
    else if (Ethernet.linkStatus() == LinkOFF)
    {
      Serial.println("Ethernet cable is not connected.");
    }
  }
  else
  {
    Udp.begin(localPort);
    active = true;
  }
  
  return active;
}

void NTPClient::SetOffset(int offsetInSecond)
{
  offset = offsetInSecond;
}
  
void NTPClient::GetDatetimeFromNTP()
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision

  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(IP, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
  delay(500);

  unsigned long epoch = 0;

  if (Udp.parsePacket())
  {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    epoch = secsSince1900 - seventyYears;
  }
  dateTime = GetDateTimeWithOffset(epoch);
  // wait ten seconds before asking for the time again
  delay(10000);

  Ethernet.maintain();

}

String NTPClient::GetDateTimeWithOffset(unsigned long epoch)
{
  char buff[25];
  epoch += offset;
  sprintf(buff, "%04d/%02d/%02d %02d:%02d:%02d", year(epoch), month(epoch), day(epoch), hour(epoch), minute(epoch), second(epoch));
  return buff;
}

String NTPClient::GetDateTime()
{
  GetDatetimeFromNTP();
  return dateTime;
}
