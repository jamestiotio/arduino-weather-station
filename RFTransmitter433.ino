#include <Adafruit_BMP085_U.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
//#include <DallasTemperature.h>
#include <RH_ASK.h>
#include "DHT.h"
#include <Wire.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

//#include <SPI.h> // Not actually used but needed to compile
//#define ONE_WIRE_BUS 2
#define DHTPIN 2
#define DHTTYPE DHT22
#define speed_pin A0

int group_number = 53;

DHT dht(DHTPIN, DHTTYPE);
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);
RH_ASK driver;
uint8_t data[12];

void setup()
{
  Serial.begin(9600);   // Debugging only
  if (!driver.init())
    Serial.println("init failed");
////  sensors.begin();
  dht.begin();
  /* Initialise the sensor */
  if (!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

}

void loop()
{
  delay(2000);
  data[0] = group_number;
  float h = dht.readHumidity();
  if (isnan(h)) {
    return;
  }
  data[1] = int(h);
  //Digits after decimal point
  data[2] = int(100 * (h - data[1]));

//  sensors.requestTemperatures();
  //  const char *msg = "Hello World!";
  float temp = dht.readTemperature(0);
  //Digits before decimal point
  data[3] = int(temp);
  //Digits after decimal point
  data[4] = int(100 * (temp - data[3]));

  sensors_event_t event;
  bmp.getEvent(&event);

  if (event.pressure)
  {
    data[5] = int(event.pressure / 100.0);
    data[6] = int(100*(event.pressure / 100.0-data[5]));
    data[7] = int(100 * (event.pressure - int(event.pressure)));
  }

  float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
  float alt = bmp.pressureToAltitude(seaLevelPressure, event.pressure);
  data[8] = int(alt);
  data[9] = int(100 * (alt - data[8]));

  int wind_temp;
  float windspeed;
  wind_temp = analogRead(speed_pin);
  windspeed = 0.0049*wind_temp;


  data[10] = int(windspeed*22.0);
  data[11] = int(100*(windspeed*22.0 - data[10]));
  

//  Serial.println(sensors.getTempCByIndex(0));
  driver.send(data, 12);
  driver.waitPacketSent();
  Serial.println("sent");

}
