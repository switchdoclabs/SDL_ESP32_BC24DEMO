<H1>SDL_ESP32_BC24DEMO <BR>
BC24 Demonstration Program
<BR>
SwitchDoc Labs <BR></H1>
Start: June 2018<BR>
www.switchdoc.labs<BR>
<BR>
<BR>

Version 007  June 30, 2018:   Added Kludge to fix latest ESP32 SDK problem<BR>
Version 006  June 18, 2018:   Added multiple methods for getting IP addresses from WiFi - Provisioning<BR>


<H2> Requirements for Compiling<H2>

In order to install the Arduino IDE with support for the ESP32 on the BC24, please follow the excellent Adafruit tutorial here:

https://learn.adafruit.com/adafruit-huzzah32-esp32-feather/using-with-arduino-ide

<B>Select the <i>Adafruit ESP32 Feather</i> under Tools</b>

Plug a USB Micro to USB connector into your computer (the one with the power supply will work) to establish the connection.

<H2>Description</H2>

The Demo program has four different modes:<BR>
We have four goodies for you right out of the box.

- Rainbow Display Demo (Mode 1)
- Monitor the Dow Jones Average Live! (Mode 2)
- Blue Theater Chasing Circle (Mode 3)
- Digital LED Clock (synced with NIST clocks for high accuracy) (Mode 4)

THe program remmebers which mode you pushed last.   Whne you push the mode button, it may take a while before the software recognizes the button push, but it will.  Then it will display the mode number (one green light) and you are now in the new mode.  Note:  Mode 2 and Mode 4 requires a WiFi connection.  FIVE red lights will flash if you go to 2 or 4 without a WiFi Connection.

<H2>Startup</H2>

On Startup the software looks for a previously set WiFI connection.  If it finds it then, you will see three green lights flash.  If it doesn't find the WiFi or has never been connected to a WiFi, then it starts though the WiFi detection sequence.

<H2>WiFi Connection Sequence - Provisioning</H2>

- SmartConfig<BR>
- WPS Button on AP<BR>
- Local AP on ESP32 (192.168.4.1)<BR>

Note:  This was developed for the BC24 ESP32 Based 24 RGBW Pixel LED Board <BR>

<H2>
SmartConfig<BR>
</H2>
This method uses the TI method of provisiong
"SmartConfig" was invented by TI. You can refer to it here:<BR>
https://community.particle.io/t/smart-config-the-missing-manual-now-available/442<BR>
In order to do SmartConfig, you need a smartphone or tablet (Android or iOS) that connected to WiFi network (which you want ESP32 to connect to) and installed a special application. 
On this application, you just supply the ssid and password of WiFi network so that the application can use, encode them and then broadcast (via UDP) encoded ssid and password (under packet format) over the air. 
When this software is being run, the ESP32 (with the SmartConfig software loaded) will capture these packets, decode back ssid and password and use them to connect to Wifi network. 

After connecting to WiFi, the  ESP32 will use mDNS to multicast a message to the application to notify that it has successfully connected to WiFi.

The source code of special application is supplied by Espressif. You can download at:<BR>
https://github.com/EspressifApp/EsptouchForAndroid<BR>
https://github.com/EspressifApp/EsptouchForIOS<BR>
This application is also available on App Store. You can use it to test SmartConfig feature.<BR>
- For Android, this application is available under name "IOT_Espressif" or another application "ESP8266 SmartConfig" (this is for ESP8266 but you can use it for ESP32):<BR>

https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch<BR>
https://play.google.com/store/apps/details?id=com.espressif.iot<BR>
<P>
- For iOS, this application is available under name "Espressif Esptouch":<BR>
https://itunes.apple.com/us/app/espressif-esptouch/id1071176700?mt=8  <BR>

There is also another app on the iOS Appstore, search on "SmartConfig"<BR>
<h3> Color sequence </h3>
Default Time:  15 seconds to hit Smart Config on app, 30 seconds timeout on response<BR>
- Three White Lights<BR>
- Three Red on failure<BR>
- Three Green on success<BR>
<BR>
<H2>
WPS Button on AP<BR>
</H2>
Wi-Fi Protected Setup (WPS; originally, Wi-Fi Simple Config) is a network security standard to create a secure wireless home network.

Introduced in 2006, the goal of the protocol is to allow home users who know little of wireless security and may be intimidated by the available security options to set up Wi-Fi Protected Access, as well as making it easy to add new devices to an existing network without entering long passphrases.

This library will wait 60 seconds (in the example) for the WPS packets to be recieved by the ESP32.  

A major security flaw was revealed in December 2011 that affects wireless routers with the WPS PIN feature, which most recent models have enabled by default.

This software does not use the PIN feature.

<h3> Color sequence </h3>
Default Time:  30 seconds timeout if no WPS button sent from AP <BR>
- Three White Lights<BR>
- Two White Lights during search<BR>
- Two Red Lights on failure<BR>
- Two Green on success<BR>

<H2>
Local AP (192.168.4.1)<BR>
</H2>


For the third provisioning method, the ESP32 is set up as an access point (192.168.4.1) - look at your list of WiFi APs on your computer when it is running.   A small web server is started that will allow you to select the AP that you want the ESP32 to connect to and then you can enter the password for the access point.
It runs for 60 seconds by default.
<h3> Color sequence </h3>
Default Time:  60 seconds timeout if No completed Webform returned<BR>
- One Blink of single White Light <BR>
- One Blinking White Light during AP page active <BR>
- One Red Light on failure<BR>
- One Green on success<BR>

<B>Remember that all of these features may fail and have to be repeated.  Nature of the beast.</B>

<H2>
Software Notes<BR>
</H2>

<H3>Reset to Default state</H3>
Hold the Mode button down while you press and release Reset - erases stored information

<H3>Software Defines</H3>
The two general defines in the example are: <BR>
- BC24:<BR>
Define BC24 if you are using a BC24 ESP32 based device.<BR>

- BC24DEBUG:<BR>
Define BC24DEBUG (by default in the example it is defined) to see lots of debugging information which can help resolve problems and issues.

