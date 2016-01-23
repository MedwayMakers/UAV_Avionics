#include <TinyGPS++.h>
#include "SPI.h"
#include "ILI9341_t3.h"

//TFT I2C pins
#define TFT_DC 9
#define TFT_CS 10
#define HWSERIAL Serial1

// Create the TinyGPS++ object
TinyGPSPlus gps;
// Create the TFT Display object
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

void setup()
{
    // The Serial connection to the GPS device
  HWSERIAL.begin(9600);

  // Initialise the device, clear screen, set cursor position, font size and colour
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  
  tft.setTextSize(2);
  tft.setTextWrap(true);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (HWSERIAL.available() > 0)
    // Pass the serial data read to the gps object
    if (gps.encode(HWSERIAL.read()))
      displayInfo();

  // if 5 seconds have passed and no characters have been processed = FAULT
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("No GPS detected: check wiring."));
    while(true);
  }

  if(!(millis()%30000)) {tft.fillScreen(ILI9341_BLACK);}
}

void displayInfo()
{
  // Set the cursor to the top left pixel
  tft.setCursor(0, 2);

// ##### DATE ##################################################################
   tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
   //Serial.print(F("  Date/Time: "));
  tft.print("Date: ");
  if (gps.date.isValid())
  {
    if (gps.date.day() < 10) tft.print(F("0"));
    tft.print(gps.date.day());
    tft.print("/");
    if (gps.date.month() < 10) tft.print(F("0"));
    tft.print(gps.date.month());
    tft.print("/");
    tft.println(gps.date.year());
  }
  else
  {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("?"));
  }

  // ##### TIME ##################################################################
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 

  tft.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10)
    if (gps.time.minute() < 10) //Serial.print(F("0"));

    if (gps.time.second() < 10) //Serial.print(F("0"));

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
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("?"));
  }

  // ##### LOCATION ########################################################
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
  if (gps.location.isValid())
  {
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
    tft.println(F("LOC: INVALID"));
  }

  // ##### ALTITUDE ########################################################
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("Alt : ");
  if (gps.altitude.isValid())
  {
    
    tft.print(gps.altitude.meters());
    tft.println(" m ");
  }
  else
  {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("?"));
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
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("?"));
  }

  // ##### SPEED ##################################################################
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("MPH : ");
  if (gps.course.isValid())
  {  
    tft.println(gps.speed.mph());
  }
  else
  {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("?"));
  }

  // ##### SATELLITES ####################################################
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("Sats: ");
  if (gps.satellites.isValid())
  {
    tft.print(gps.satellites.value());
    tft.println("   ");
  }
  else
  {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("?"));
  }

  Distance();
}

void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    tft.print(i<slen ? str[i] : ' ');
}

void Distance() {
    tft.setCursor(0, 142);
  const double WAYPOINT_LAT = 51.428817;
  const double WAYPOINT_LNG = 0.3998756;
  double distanceKm =
    gps.distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      WAYPOINT_LAT,
      WAYPOINT_LNG) / 1000.0;
  double courseTo =
    gps.courseTo(
      gps.location.lat(),
      gps.location.lng(),
      WAYPOINT_LAT,
      WAYPOINT_LNG);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK); 
  tft.println("Distance(km) to Waypoint:");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
  tft.println(distanceKm);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.println("Course to Waypoint: ");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.println(courseTo);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.println("Cardinal directions: ");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.println(gps.cardinal(courseTo));
}
