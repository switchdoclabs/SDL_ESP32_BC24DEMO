//
// Non volalile preferences functions
//


// Global preference variables

unsigned int currentMode;
String WiFi_SSID = "";
String WiFi_psk = "";

int ClockTimeOffsetToUTC = 0;



void resetPreferences()
{
  preferences.begin("BC24init", false);
  preferences.clear();
  preferences.end();

  ClockTimeOffsetToUTC = DEFAULTCLOCKTIMEOFFSETTOUTC;
  Serial.println("----Clearing Preferences---");


}

void writePreferences()
{
  preferences.begin("BC24init", false);

  preferences.putUInt("currentMode", currentMode);
  preferences.putString("WiFi_SSID", WiFi_SSID);
  preferences.putString("WiFi_psk", WiFi_psk);

  Serial.print("ClockTimeOffsetToUTC=");
  Serial.println(ClockTimeOffsetToUTC);
  preferences.putInt("COffsetToUTC", ClockTimeOffsetToUTC);


  preferences.end();

#ifdef BC24DEBUG
  Serial.println("----Writing Preferences---");
  Serial.printf("currentMode=%i\n", currentMode);
  Serial.printf("SSID="); Serial.println(WiFi_SSID);
  Serial.printf("psk="); Serial.println(WiFi_psk);
  Serial.printf("COffsetToUTC="); Serial.println(ClockTimeOffsetToUTC);
  Serial.println("--------------------------");
#endif


}

void readPreferences()
{
  preferences.begin("BC24init", false);

  currentMode = preferences.getUInt("currentMode", 0);
  WiFi_SSID = preferences.getString("WiFi_SSID");
  WiFi_psk = preferences.getString("WiFi_psk");
  ClockTimeOffsetToUTC = preferences.getInt("COffsetToUTC",DEFAULTCLOCKTIMEOFFSETTOUTC);
  preferences.end();

#ifdef BC24DEBUG
  Serial.println("----Reading Preferences---");
  Serial.printf("currentMode=%i\n", currentMode);
  Serial.printf("SSID="); Serial.println(WiFi_SSID);
  Serial.printf("psk="); Serial.println(WiFi_psk);
  Serial.printf("COffsetToUTC="); Serial.println(ClockTimeOffsetToUTC);
  Serial.println("--------------------------");
#endif
}



