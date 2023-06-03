#include "NTPClient.h"

NTPClient ntpClient;
const char IP[] = "time.nist.gov"; //time.nist.gov
unsigned int port = 8888;
String dateTime = "";

void setup() {
  Serial.begin(9600);
  if(!ntpClient.begin(IP, port))
  {
    Serial.println("Can not begin NTP-Client!");
  }
  ntpClient.SetOffset(12600);
}

void loop() {
  dateTime = ntpClient.GetDateTime();
  Serial.println(dateTime);
  delay(10000);
}
