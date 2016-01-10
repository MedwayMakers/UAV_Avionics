// This code will take the serial NMEA data from a GPS device and display it on a colour ILI9341 LCD display.

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//TFT I2C pins
#define TFT_DC 9
#define TFT_CS 10

static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  
  tft.setTextSize(3);
  tft.setTextWrap(true);
}

void loop()
{
  
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("No GPS detected: check wiring."));
    while(true);
  }

  
}

void displayInfo()
{
  tft.setCursor(0, 0);

  // ##### LOCATION ########################################################
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK); 
    tft.print("Lat : ");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.println(gps.location.lat(), 6);
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK); 
    tft.print("Long: ");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.println(gps.location.lng(), 6);
  }
  else
  {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    Serial.print(F("INVALID"));
    tft.println(F("LOC: INVALID"));
  }

  // ##### ALTITUDE ########################################################
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("Alt : ");
  if (gps.altitude.isValid())
  {
    
    tft.print(gps.altitude.meters());
    tft.println(" m  ");
  }
  else
  {
    Serial.print(F("INVALID"));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("INVALID"));
  }

    // ##### COURSE #####
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("Deg : ");
  if (gps.course.isValid())
  {
    
    tft.println(gps.course.deg());
  }
  else
  {
    Serial.print(F("INVALID"));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("INVALID"));
  }

  // ##### SPEED #####
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("MPH : ");
  if (gps.course.isValid())
  {
    
    tft.println(gps.speed.mph());
  }
  else
  {
    Serial.print(F("INVALID"));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("INVALID"));
  }

   tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
   Serial.print(F("  Date/Time: "));
  tft.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
    tft.print(gps.date.day());
    tft.print("/");
    tft.print(gps.date.month());
    tft.print("/");
    tft.println(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("INVALID"));
  }

    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    Serial.print(F(" "));
  tft.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));

    if (gps.time.hour() < 10) tft.print(F("0"));
    tft.print(gps.time.hour());
    tft.print(F(":"));
    if (gps.time.minute() < 10) tft.print(F("0"));
    tft.print(gps.time.minute());
    tft.print(F(":"));
    if (gps.time.second() < 10) tft.print(F("0"));
    tft.println(gps.time.second());
  }
  else
  {
    Serial.print(F("INVALID"));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("INVALID"));
  }


  // ##### SATELITTES #####
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("Sats: ");
  if (gps.satellites.isValid())
  {
    
    tft.println(gps.satellites.value());
  }
  else
  {
    Serial.print(F("INVALID"));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("INVALID"));
  }



  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.value()) : "*** ", 6);

  Serial.println();
}

void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    tft.print(i<slen ? str[i] : ' ');
}
