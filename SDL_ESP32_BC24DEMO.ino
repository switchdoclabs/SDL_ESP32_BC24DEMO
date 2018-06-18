// SDL_ESP32_BC24DEMO
// SwitchDoc Labs BC24 Demo for Pixel ESP32 Board
// April 2018
//
//

#define BC24SOFTWAREVERSION "006"
#undef BC24DEBUG
#define BC24

#define BUTTONPIN 17

#define BLINKPIN 13


// BC24 Commands and Functions and Modes

#define BC24NUMBEROFMODES 4


#define BC24VARIETYOFDISPLAY 0
#define BC24DOWJONES 1
#define BC24CIRCLEDISPLAY 2
#define BC24CLOCK 3




#define BC24DISPLAYCURRENTMODE 9999




#if defined(ARDUINO) && ARDUINO >= 100
// No extras
#elif defined(ARDUINO) // pre-1.0
// No extras
#elif defined(ESP_PLATFORM)
#include "arduinoish.hpp"
#endif


bool WiFiPresent = false;
#include "Utility.h"

#include <time.h>


#include <WiFi.h>

#include <esp_wps.h>
#include <esp_smartconfig.h>

#define ESP_WPS_MODE WPS_TYPE_PBC

esp_wps_config_t config = WPS_CONFIG_INIT_DEFAULT(ESP_WPS_MODE);


#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#include <TimeLib.h>

#include "Clock.h"

#include <Preferences.h>;

// DS3231 Library functions
/*
  #include "RtcDS3231.h"
  void printDateTime(const RtcDateTime& dt);
  String returnDateTime(const RtcDateTime& dt);

  RtcDS3231 Rtc;
*/

bool RTC_SetAndPresent = false;

#include "NTPClient.h"

WiFiUDP ntpUDP;


NTPClient timeClient(ntpUDP);

/* create an instance of Preferences library */
Preferences preferences;





#include "BC24Preferences.h";

#include "BigCircleFunctions.h"

#include "RTOSDefs.h"

#include "RTOSTasks.h"

#include "DowJones.h"

// AP Variables

#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager

//gets called when WiFiManager enters configuration mode


void configModeCallback (WiFiManager *myWiFiManager)
//void configModeCallback ()
{

  Serial.println("Entered config mode");

  Serial.println(WiFi.softAPIP());

}

#define WEB_SERVER_PORT 80
String APssid;

String Wssid;
String WPassword;

WiFiServer server(WEB_SERVER_PORT);

// include GET IP routines
#include "SDL_ESP32_BC24_GETIP.h"


void setup()
{

  ClockTimeOffsetToUTC = -25200;

#ifdef BC24DEBUG
  esp_log_level_set("*", ESP_LOG_VERBOSE);
#endif

  Serial.begin(115200);

  // set up button Pin
  pinMode (BUTTONPIN, INPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);  // Pull up to 3.3V on input - some buttons already have this done

  Serial.print("Button=");
  Serial.println(digitalRead(BUTTONPIN));

  // Now check for clearing Preferences on hold down of Mode pin on reboot
  if (digitalRead(BUTTONPIN) == 0)
  {

    resetPreferences();
  }


  // setup preferences
  readPreferences();



  // initalize our friend the BC24!
  //BC24inititialzeCircle();

  dumpSysInfo();
  getMaxMalloc(1 * 1024, 16 * 1024 * 1024);

  if (digitalLeds_initStrands(STRANDS, STRANDCNT)) {
    Serial.println("Init FAILURE: halting");
    while (true) {};
  }
  for (int i = 0; i < STRANDCNT; i++) {
    strand_t * pStrand = &STRANDS[i];
    Serial.print("Strand ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print((uint32_t)(pStrand->pixels), HEX);
    Serial.println();
#if DEBUG_ESP32_DIGITAL_LED_LIB
    dumpDebugBuffer(-2, digitalLeds_debugBuffer);
#endif
    digitalLeds_resetPixels(pStrand);
#if DEBUG_ESP32_DIGITAL_LED_LIB
    dumpDebugBuffer(-1, digitalLeds_debugBuffer);
#endif
  }
  Serial.println("Init complete");


  Serial.println();
  Serial.println();
  Serial.println("--------------------");
  Serial.println("BC24 Demo Software");
  Serial.println("--------------------");
  Serial.print("Version: ");
  Serial.println(BC24SOFTWAREVERSION);

  Serial.print("Compiled at:");
  Serial.print (__TIME__);
  Serial.print(" ");
  Serial.println(__DATE__);


  // Free heap on ESP32
  Serial.print("Free Heap Space on BC24:");
  Serial.println(ESP.getFreeHeap());

#ifdef BC24DEBUG
  Serial.print("CPU0 reset reason: ");
  print_reset_reason(rtc_get_reset_reason(0));

  Serial.print("CPU1 reset reason: ");
  print_reset_reason(rtc_get_reset_reason(1));
#endif

  Serial.print("RAM left: ");
  Serial.println(system_get_free_heap_size());

  // RTOS

  xSemaphoreSingleBlink = xSemaphoreCreateBinary();

  xSemaphoreGive( xSemaphoreSingleBlink);   // initialize

  xSemaphoreTake( xSemaphoreSingleBlink, 10);   // start with this off

  xTaskCreatePinnedToCore(
    taskButtonRead,          /* Task function. */
    "TaskButtonRead",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

  xTaskCreatePinnedToCore(
    taskSingleBlink,          /* Task function. */
    "TaskButtonRead",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

  // sendCurrentMode();





  //---------------------
  // Setup WiFi Interface
  //---------------------
  // 5 cases:
  // 1) Use stored SSID/psk
  // 2) SmartConfig received
  // 3) WPS Button
  // 4) Local AP
  // 5) Wifi Fails


  WiFiPresent = false;

  WiFi.begin();

  // check for SSID

  if (WiFi_SSID.length() != 0)
  {
    // use existing SSID
    Serial.println("Using existing SSID/psk");

    Serial.printf("SSID="); Serial.println(WiFi_SSID);
    Serial.printf("psk="); Serial.println(WiFi_psk);
    WiFi.begin(WiFi_SSID.c_str(), WiFi_psk.c_str());
    //Wait for WiFi to connect to AP
    Serial.println("Waiting for Saved WiFi");
#define WAITFORCONNECT 10
    for (int i = 0; i < WAITFORCONNECT * 2; i++)
    {
      if (WiFi.status() == WL_CONNECTED)
      {

        Serial.println("");
        Serial.println("WiFI Connected.");
        WiFiPresent = true;
#ifdef BC24
        BC24ThreeBlink(Green, 1000);
#endif

        break;
      }

      Serial.print(".");
      WiFiPresent = false;
      BC24ThreeBlink(White, 1000);
    }
    Serial.println();

  }


  if (WiFiPresent == false)
  {
    // do SmartConfig
#define WAITFORSTART 15
#define WAITFORCONNECT 20

    WiFiPresent = SmartConfigGetIP(WAITFORSTART, WAITFORCONNECT);

  } // if not already programmed before





  if (WiFiPresent != true)
  {
#define WPSTIMEOUTSECONDS 30
    // now try WPS Button

    WiFiPresent = WPSGetIP(WPSTIMEOUTSECONDS);

  }

  if (WiFiPresent != true)
  {
#define APTIMEOUTSECONDS 60
    WiFiPresent = localAPGetIP(APTIMEOUTSECONDS);
  }


  if (WiFiPresent == true)
  {

    WiFi_SSID = WiFi.SSID();
    WiFi_psk = WiFi.psk();
  }
  // write out preferences

  writePreferences();
  if (WiFiPresent == true)
  {
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.print("WiFi Channel= ");
    Serial.println(WiFi.channel());
  }

  //---------------------
  // End Setup WiFi Interface
  //---------------------

  blinkLED(2, 300);  // blink twice - OK!

  SetRandomSeed();

  // Now do NTP (set's time to 00:00:00 if not present)  January 1, 1970


  // changed later with setTimeOffset() ). Additionaly you can specify the
  // update interval (in milliseconds, can be changed using setUpdateInterval() ).

  timeClient.setTimeOffset(ClockTimeOffsetToUTC);
  if (WiFiPresent == true)
  {
    timeClient.setUpdateInterval(3600000);
    timeClient.update();
  }
  time_t t;
  if (WiFiPresent == true)
  {
    t = timeClient.getEpochTime();
  }
  else
  {
    t = 1;
  }


  setTime(t);


  digitalClockDisplay();





}



void loop()
{

  Serial.print("currentMode =");
  Serial.print(currentMode);
  Serial.print("/");
  Serial.println(currentModeToString(currentMode));

  switch (currentMode)
  {
    case BC24VARIETYOFDISPLAY:

      BC24writeCurrentMode();
      BC24CircleRainbow();
      break;

    case BC24DOWJONES:
      BC24writeCurrentMode();
      if (WiFiPresent)
      {

        doDowJones();  // down jones exits on change of currentMode
      }
      else
      {
        // no Wifi
        while (1)
        {
          BC24BottomFiveBlink(Red, 1000);
          if (currentMode != BC24DOWJONES)
          {
            return;
          }
        }
      }


      break;
    case BC24CIRCLEDISPLAY:
      BC24writeCurrentMode();
      {
        int currentPixel = 0;

        while (1)
        {

          displayCircleLED(currentPixel, Blue);
          currentPixel = (currentPixel + 1) % 24;

          vTaskDelay(10 / portTICK_PERIOD_MS);
          if (currentMode != BC24CIRCLEDISPLAY)
          {
            return;
          }
        }
      }
      break;

    case BC24CLOCK:
      BC24writeCurrentMode();
      if (WiFiPresent)
      {
        int currentPixel = 0;

        pixelColor_t myStatusColor;

        for ( ;; )
        {

#ifdef BC24DEBUG
          Serial.println("Clock task running");
#endif
          time_t currentTime;
          currentTime = timeClient.getEpochTime();
          Serial.println("After epoch time");



          tmElements_t tm_currentTime;
          breakTime(currentTime, tm_currentTime);
          Serial.println("After breaktime");
          digitalClockDisplay();

          displayClock(tm_currentTime.Hour, tm_currentTime.Minute, tm_currentTime.Second);
          Serial.println("After displayClock");
          if (currentMode != BC24CLOCK)
          {
            return;
          }
          vTaskDelay(1000 / portTICK_PERIOD_MS);
          Serial.println("after vTaskDealy");
        }
      }
      else
      {
        // no Wifi
        while (1)
        {
          BC24BottomFiveBlink(Red, 1000);
          if (currentMode != BC24CLOCK)
          {
            return;
          }
        }
      }

      break;

    default:
      break;
  }
  delay(100);
}





