float fetchValue(String myDowJones, int count)
{

  int startIndex = 0;
  int startOfValue;
  int endOfValue;
  float returnValue = 0.0;


  startIndex = myDowJones.indexOf(":", startIndex) + 1;

  for (int i = 0; i < count; i++)
  {
    startIndex = myDowJones.indexOf(":", startIndex) + 1;
    startOfValue = myDowJones.indexOf("\"", startIndex);
    endOfValue = myDowJones.indexOf("\"", startOfValue + 1);

    String returnValueString;
    returnValueString = myDowJones.substring(startOfValue + 1, endOfValue - 1);
    returnValue = returnValueString.toFloat();



  }

  Serial.println(returnValue);
  return returnValue;
}

// Dow Jones Task

const char* test_root_ca = \
                           "-----BEGIN CERTIFICATE-----\n" \
                           "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
                           "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
                           "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
                           "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
                           "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
                           "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
                           "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
                           "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
                           "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
                           "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
                           "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
                           "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
                           "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
                           "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
                           "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
                           "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
                           "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
                           "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
                           "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
                           "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
                           "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
                           "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
                           "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
                           "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
                           "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
                           "-----END CERTIFICATE-----\n";


void doDowJones( )
{

  WiFiClient client;

  float DowJonesDailyStart = 0.0;
  float DJOpenValue;
  float DJHighValue;
  float DJLowValue;
  float DJCloseValue;

  pixelColor_t myStatusColor;

  for ( ;; )
  {
    /* Block to wait for Dow Jones to notify this task. */


    if (currentMode == BC24DOWJONES)
    {

#ifdef BC24DEBUG
      Serial.println("Dow Jones task running");
#endif

#define MYALPHAKEY String("FC8ERZZ7GP7IKW0E")

      HTTPClient http;

      //String myURL = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=DJI&interval=1min&apikey=" + MYALPHAKEY;
      String myURL = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY_ADJUSTED&symbol=DJI&interval=1min&apikey=" + MYALPHAKEY;
      Serial.println(myURL);
      http.begin(myURL.c_str(), test_root_ca); //Specify the URL and certificate

      Serial.print("[HTTP] begin...\n");

      // create buffer for read
#define BUFFSIZE 1000

      uint8_t buff[BUFFSIZE] = { 0 };

#ifdef BC24DEBUG
      Serial.print("[HTTP] GET...\n");
#endif

      // start connection and send HTTP header
      int httpCode = http.GET();
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
#ifdef BC24DEBUG
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
#endif
        // file found at server
        if (httpCode == HTTP_CODE_OK) {

          // get lenght of document (is -1 when Server sends no Content-Length header)
          int len = http.getSize();
#ifdef BC24DEBUG
          Serial.print("len=");
          Serial.println(len);
#endif


          // get tcp stream
          WiFiClient * stream = http.getStreamPtr();

          // read all data from server
          while (http.connected() && (len > 0 || len == -1)) {

            // get available data size
            size_t size = stream->available();

            //  if (size) {

            // read up to 1000 bytes
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            // write it to Serial
            //Serial.write(buff, c);

            if (len > 0) {
              len -= c;
            }

            // check for read greater than 150 and if then quit - only read the first 150-200 characters
            if (size > 150)
              break;
          }


          // delay(1);
          //}

#ifdef BC24DEBUG
          Serial.println();
          Serial.print("[HTTP] connection closed or file end.\n");
#endif
        }
      } else
      {
#ifdef BC24DEBUG
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
#endif
      }

      http.end();

      // OK now we check the string out and parse it.
      // BTW, all of this cutting is done because the alpha vantage API won't let us get at just the last value

      // make buffer into a CString

      buff[BUFFSIZE - 1] = '\0';

      // Stringify the buffer

      String DowJones = String((char *)buff);

      //Serial.println(DowJones);

      if (DowJones.length() > 50)
      {
        unsigned int myIndex = DowJones.indexOf("Time Series (Daily)");
        unsigned int startOfCurrentDow = 32;

        DowJones.remove(0, myIndex + startOfCurrentDow);

#ifdef BC24DEBUG
        Serial.print("DowJonesStripped=");
        Serial.println(DowJones);
#endif
        // Get the last block time
        int secondQuote = 0;
        secondQuote = DowJones.indexOf("\"", 1);

        String time_details = DowJones.substring(1, secondQuote);
        time_details = time_details + " 00:00:00";



        struct tm tm;

        strptime(time_details.c_str(), "%Y-%m-%d %H:%M:%S", &tm);





        time_t currentTime;
        currentTime = timeClient.getEpochTime();



        // adjust currentTime to EST

        currentTime = (currentTime - ClockTimeOffsetToUTC) - 14400;



        tmElements_t tm_currentTime;
        breakTime(currentTime, tm_currentTime);
        Serial.print("CT - Day=");
        Serial.println(tm_currentTime.Day);

        Serial.print("ST - Day=");
        Serial.println(tm.tm_mday);


        // Check to see if current time is same calendar day as the last sample

        if (tm.tm_mday == tm_currentTime.Day)
        {


          // Same Calendar Day, means we need to check current Time to see if it is between 6:30 and 4:00 EST

          // check to see if is after 4pm EST
          // convert back to time_t

          Serial.print("Hour=");
          Serial.print(tm_currentTime.Hour);
          Serial.print("Minute");
          Serial.println(tm_currentTime.Minute);

          if (tm_currentTime.Hour > 15)
          {
            // Outside of trading hours
            myStatusColor = White;
          }
          else
          {
            // now check for before 9:30am


            if (tm_currentTime.Hour < 6)
            {
              myStatusColor = White;
            }
            else
            {

              if (tm_currentTime.Hour == 6)
              {
                // check for less than 30

                if (tm_currentTime.Minute < 30)
                {
                  myStatusColor = White;
                }
                else
                {
                  // OK, must be trading then!
                  myStatusColor = Blue;
                }
              }
              else
              {
                // OK, must be trading then!
                myStatusColor = Blue;
              }

            }
          }



        }
        else
        {

          if (DowJonesDailyStart > 0.0)
            myStatusColor = White;
          else
          {

            displaySinglePixel(6, DarkYellow);
          }
        }

        if (tm.tm_mday == tm_currentTime.Day)   // if not current day, do not update

        {

          // scan all the values of the first block





          DJOpenValue = fetchValue(DowJones, 1);
          DJHighValue = fetchValue(DowJones, 2);
          DJLowValue = fetchValue(DowJones, 3);
          DJCloseValue = fetchValue(DowJones, 4);


#ifdef BC24DEBUG
          digitalClockDisplay();
#endif
          /*      //  Need Time Based Logic for Start/End, plus day of the week
                if (DowJonesDailyStart > 0)
                {
                  Serial.print("Daily Start Already Set:");
                  Serial.println(DowJonesDailyStart);
                }
                else
                {
                  DowJonesDailyStart = DJOpenValue;
                  Serial.print("Daily Start Set:");
                  Serial.println(DowJonesDailyStart);
                }

          */

          DowJonesDailyStart = DJOpenValue;
          Serial.print("Daily Start Set:");
          Serial.println(DowJonesDailyStart);

          Serial.print("DJCloseValue=");
          Serial.println(DJCloseValue);

          float currentDelta;
          currentDelta = DJCloseValue - DowJonesDailyStart;

          Serial.print("currentDelta=");
          Serial.println(currentDelta);


          //displayDowJones(currentDJ, openDJ, lastUpdateSeconds);
          displayDowJones(DJCloseValue, DowJonesDailyStart , myStatusColor);


        }
        vTaskDelay(15000 / portTICK_PERIOD_MS);




      }

    }
    else
    {
      return;  // we are done
    }
  }
}



