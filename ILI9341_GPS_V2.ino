#include <TinyGPS++.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//TFT I2C pins
#define TFT_DC 9
#define TFT_CS 10
#define HWSERIAL Serial1

// Create the TinyGPS++ object
TinyGPSPlus gps;
// Create the TFT Display object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup()
{
  // Serial connection for the Serial Monitor window for debugging purposes
  Serial.begin(115200);
  
  // The Serial connection to the GPS device
  HWSERIAL.begin(9600);

  // Initialise the device, clear screenm set cursor position, font size and colour
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
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
    //Serial.println(F("No GPS detected: check wiring."));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("No GPS detected: check wiring."));
    while(true);
  }
  
}

void displayInfo()
{
  // Set the cursor to the top left pixel
  tft.setCursor(0, 0);

// ##### DATE ##################################################################
   tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
   //Serial.print(F("  Date/Time: "));
  tft.print("Date: ");
  if (gps.date.isValid())
  {
    //Serial.print(gps.date.day());
    //Serial.print(F("/"));
    //Serial.print(gps.date.month());
    //Serial.print(F("/"));
    //Serial.print(gps.date.year());
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
    //Serial.print(" ???");
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("???"));
  }

// ##### TIME ##################################################################
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    //Serial.print(F(" "));
  tft.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) //Serial.print(F("0"));
    //Serial.print(gps.time.hour());
    //Serial.print(F(":"));
    if (gps.time.minute() < 10) //Serial.print(F("0"));
    //Serial.print(gps.time.minute());
    //Serial.print(F(":"));
    if (gps.time.second() < 10) //Serial.print(F("0"));
    //Serial.print(gps.time.second());
    //Serial.print(F("."));

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
    //Serial.print(" ???");
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("???"));
  }

  
  // ##### LOCATION ########################################################
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
  //Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    //Serial.print(gps.location.lat(), 6);
    //Serial.print(F(","));
    //Serial.print(gps.location.lng(), 6);
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
    //Serial.print(" ???");
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
    //Serial.print(" ???");
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("???"));
  }

    // ##### COURSE #####
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("Deg : ");
  if (gps.course.isValid())
  {
    
    tft.println(gps.course.deg());
    tft.println("   ");
  }
  else
  {
    //Serial.print(" ???");
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("???"));
  }


  // ##### SPEED ##################################################################
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
    tft.print("MPH : ");
  if (gps.course.isValid())
  {
    
    tft.println(gps.speed.mph());
    tft.println("   ");
  }
  else
  {
    //Serial.print(" ???");
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("???"));
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
    //Serial.print(" ???");
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK); 
    tft.println(F("???"));
  }

  // ##### COURSE ##################################################################
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
  tft.print("Dir : ");
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.value()) : "*** ", 6);
  //Serial.println();
}

void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    tft.print(i<slen ? str[i] : ' ');
}
