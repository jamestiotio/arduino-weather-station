#include <RH_ASK.h>
//#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;

int group_number = 53;

void setup()
{
  Serial.begin(9600); // Debugging only
  if (!driver.init())
    Serial.println("init failed");
}

void loop()
{
  uint8_t buf[12] = {};
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    if (buf[0] == group_number) {
      int i;

      Serial.print("Group Number: ");
      Serial.println(buf[0]);
      
      // Message with a good checksum received, dump it.
      Serial.print("Humidity: ");
      Serial.print(buf[1]);
      Serial.print(".");
      Serial.print(buf[2]);
      Serial.println(" %");

      Serial.print("Temperature: ");
      Serial.print(buf[3]);
      Serial.print(".");
      Serial.print(buf[4]);
      Serial.println(" degC");

      Serial.print("Pressure: ");
      Serial.print(buf[5]);
      if (buf[5] > 9) {
        Serial.print(buf[6]);
      }
      else {
        Serial.print("0");
        Serial.print(buf[6]);
      }
      Serial.print(".");
      Serial.print(buf[7]);
      Serial.println(" hPa");

      Serial.print("Altitude: ");
      Serial.print(buf[8]);
      Serial.print(".");
      Serial.print(buf[9]);
      Serial.println(" m");

      Serial.print("Wind speed: ");
      Serial.print(buf[10]);
      Serial.print(".");
      Serial.print(buf[11]);
      Serial.println(" km/h");
      Serial.println();
    }
  }
}
