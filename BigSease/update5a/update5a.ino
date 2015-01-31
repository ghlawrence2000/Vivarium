// Vivarium Management Controller w/ 7" touchscreen Interface
// Written and coded by David Sease, aka Bigsease30 and ghlawrence2000 on arduino.cc!
// Much code was swiped, modified, and integrated or otherwise inspired from other
// public works
// All code is public domain, feel free to use, abuse, edit, and share
// Written for Arduino Mega 2560

// VERSION:  1.0
// - initial version

// CODE ORDER:
// variables & setup
// main loop
// routines to draw every screen
// code to capture all touches
// all other routines

// EEPROM locations (saved settings)    // <---- Need EEPROM Everything library....On Dropbox.
// SPACE // DESCRIPTION  // DEFAULTS    //
// 0 //  ID    : 32                     // 32 if previously ran
// 1 //  screen: return home            // 1 min
// 2 //  screen: brightness if no dim   // 16 max = 255
// 3 //  screen: AutoDim level          // 1
// 4 //  screen: AutoDim seconds        // 10 sec
// 5 //  Heater: Float byte 0           //\
// 6 //  Heater: Float byte 1           //  _ Initial value
// 7 //  Heater: Float byte 2           //    42.9F
// 8 //  Heater: Float byte 3           ///
// 9 //  Fan   : Float byte 0           //\
// 10 // Fan   : Float byte 1           //  _ Initial value
// 11 // Fan   : Float byte 2           //    102.9F
// 12 // Fan   : Float byte 3           ///
// 13 // Lights: OnHours1
// 14 // Lights: OnMinutes1
// 15 // Lights: OffHours1
// 16 // Lights: OffMinutes1
// 17 // Lights: OnHours2
// 18 // Lights: OnMinutes2
// 19 // Lights: OffHours2
// 20 // Lights: OffMinutes2
// 21 //
// 22 //
// 23 //
// 24 //
// 25 //
// 26 //
// 27 //
// 28 //
// 29 //
// 30 //
// 31 //
// 32 //
// 33 //
// 34 //
// 35 //
////////////////////////////  BEGIN GLOBAL VARIABLES  /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h> // needed by tons of stuff
#include <EEPROM.h>  // used to store and retrieve settings from memory
#include <UTFT.h>  // used to interface with the TFT display
#include <UTouch.h>  // used to interface with the touch controller on the TFT display
#include <SdFat.h> // used to access the SD card
#include <UTFT_SdRaw.h>  // used to read .raw images from the SD card
#include <RTClib.h>
#include <Time.h> // allows conversion to UNIX time for easier date/time math
#include <TimeAlarms.h>  // used to power schedules
#include <OneWire.h> // network library to communicate with the DallasTemperature sensor, 
#include <DallasTemperature.h>  // library for the Temp sensor itself
#include <dht.h>
#define SD_CHIP_SELECT  53  // SD chip select pin
// file system object
SdFat sd;

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t Sinclair_S[];
extern uint8_t arial_bold[];
extern uint8_t Ubuntubold[];

#define fastest 1 // <======= 1 = MAX speed all screens in own page / 0 = 2 Screens share a page...
#define debug 1   // <======= 1 = Serial output enabled
#define notusestring 1 // <== 1 = No String class used / 0 = Use string class
// Declare Pins for Relay
// all analog to save on digital pins
#define LIGHTS1 A0     // Arduino Name / Pins                   
#define LIGHTS2 A1
#define TEMP1 A2
#define FOGGER1 A3
#define MISTING1 A4
#define AUX1 A5

#define ONE_WIRE_BUS 8
#if notusestring
#else
float probe01;
float probe02;
float probe03;

char sensorPrintout[10];  //[4]moded her for print out full numbers of dallas ds18b20
char sensorPrintout1[10];  //[4]moded her for print out full numbers of dallas ds18b20
char sensorPrintout2[10];  //[4]moded her for print out full numbers of dallas ds18b20
#endif
boolean BackLightTouch = true; // initial setting of true to allow the screen to stay bright after boot

// Pins for temperature sensors


// Use software or hardware RTC
#define DS1307 0 // <========= 1 for DS1307, 0 for Software

// for time
#if DS1307 ==1
RTC_DS1307 rtc;
#else
RTC_Millis rtc;
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
#endif

// used for time
tmElements_t tmpRTC, prevRTC, saveRTC;

// used for storing power states of relays
/*struct PWR
{
  byte pwrLight1;
  byte pwrLight2;
  byte pwrTemp1;
  byte pwrFogger1;
  byte pwrMisting1;
  byte pwrAUX1;
}*/


// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 8
#define TEMPERATURE_PRECISION 9
#define DHTPIN 9     // what pin we're connected to
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
dht DHT;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
//DeviceAddress tempLeft, tempMiddle, tempRight;
//DeviceAddress tempLeft = { 0x28, 0x24, 0x18, 0x5D, 0x06, 0x00, 0x00, 0xA2 };
//DeviceAddress tempMiddle = { 0x28, 0x39, 0x0B, 0x5C, 0x06, 0x00, 0x00, 0x0E };
//DeviceAddress tempRight = { 0x28, 0xED, 0x16, 0x5C, 0x06, 0x00, 0x00, 0x86 };


// initiate the screen and touch
UTFT myGLCD(CTE70CPLD, 38, 39, 40, 41); // start up an instance of the TFT screen
UTouch  myTouch( 6, 5, 4, 3, 2);  // start up an instance of for touch

UTFT_SdRaw myFiles(&myGLCD);  // start up an instance to read images from the SD card

byte dispScreen = 1;  // screens are listed below
// 1-home, 2-lights, , 3-temp, 4-fogger, 5-mister
// 6-fan, 7-clock, 8-screen

byte scheduleItem; // track which item is being scheduled

boolean updateTime = false; // keep track of when to udpate the clock

//word x, y; //touch coordinates

// days and month character strings for displaing at the top of the screen
char *Day[] = {
  "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
char *Mon[] = {
  "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// small power icons for the home screen, off and on
char *pwrLightIconS[] = {
  "swon.raw", "swoff.raw"
};

// various millis to keep track of
unsigned long prevMillisTouch = 0; // track time between touches
unsigned long prevMillis5sec = 0; // track 5 seconds for refreshing clock and temp
unsigned long millisDim = 0; // used for brightness adjustment
unsigned long millisHome = 0; // used for returning home after configured time
unsigned long TouchRepeatDelay = 500; // millis
unsigned long tmpTouchRepeatDelay = 500; //millis
unsigned long humidTime = 0; // 2 seconds minimum reading
// screen settings corresponding to eeprom values 1-4
byte HomeDelay;  // (Return Home) Minutes 0-59
byte BackLight;  // (Bright) (not AutoDim) startup brightness to 100% 1-16
byte AutoDim;    // (AutoDim Level) maximum brightness when dimmed 1-5
byte TouchDelay; // (AutoDim Seconds) 0-59
float heater; // = 35.9;
float fan; // = 87.9;
byte Light1OnHr, Light1OnMin, Light1OffHr, Light1OffMin;
byte Light2OnHr, Light2OnMin, Light2OffHr, Light2OffMin;


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// VOID SETUP CODE /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  // start serial port
  Serial.begin(57600);

  // set default values for the first ever run, you can comment this out after the first run if you want
  firstRunSetup();

  // get screen settings from eeprom
  HomeDelay    = EEPROM.read(1);
  BackLight    = EEPROM.read(2);
  AutoDim      = EEPROM.read(3);
  TouchDelay   = EEPROM.read(4);
  heater       = EEPROM.readFloat(5);
  fan          = EEPROM.readFloat(9);
  Light1OnHr   = EEPROM.read(13);
  Light1OnMin  = EEPROM.read(14);
  Light1OffHr  = EEPROM.read(15);
  Light1OffMin = EEPROM.read(16);
  Light2OnHr   = EEPROM.read(17);
  Light2OnMin  = EEPROM.read(18);
  Light2OffHr  = EEPROM.read(19);
  Light2OffMin = EEPROM.read(20);


  // DS18b20 Temp and Humidity Sensors
  sensors.begin();
  //dht.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");


  // initiate the screen and touch
#if debug
  Serial.println("Initialising SD card...");
#endif
  bool mysd = 0;
  // see if the card is present and can be initialized:
  while (!mysd)
  {
    if (!sd.begin(SD_CHIP_SELECT, SPI_FULL_SPEED)) {
#if debug
      Serial.println(F("Card failed, or not present"));
      Serial.println(F("Retrying...."));
#endif
    }
    else
    {
      mysd = 1;
#if debug
      Serial.println(F("Card initialised."));
#endif
    }
  }

  myGLCD.InitLCD();        // <= Dont need LANDSCAPE since is default
  myGLCD.setBrightness(0); // <= Backlight Off
  myGLCD.setWritePage(1);  // <= Select writepage 1
  myGLCD.clrScr();         // <= Clear writepage 1
  myGLCD.setDisplayPage(1); //<= Display writepage 1
  myGLCD.setBrightness(BackLight); //<= Turn on BackLight Max brightness
  myTouch.InitTouch();      //<= Dont need LANDSCAPE since is default
  myTouch.setPrecision(PREC_MEDIUM);

  // DHT22 Humidity Sensor


  // DS18b20 Temp Sensors


  Wire.begin();
#if(DS1307)
  rtc.begin(); // Start the Clock
  delay(250);
  if (! rtc.isrunning())
  {
    // If no RTC is installed, alert on serial
#if debug
    Serial.println(F("DS1307 is NOT running!"));  // Store this string in PROGMEM
    Serial.println(F("Define DS1307 0!"));  //
#endif
  }
#else

  rtc.begin(DateTime(F(__DATE__), F(__TIME__))); // Start the Clock
#if debug
  Serial.println(F("Software RTC is running!"));  // Store this string in PROGMEM
  Serial.println(F("Send sync message eg T1420722534 (Can type in serial monitor)"));
#endif
#endif

  setSyncProvider(syncProvider);     // reference our syncProvider function instead of RTC_DS1307::get();

  // Turns OFF all relays until called in code (NO MISTAKE, HIGH IS OFF, LOW IS ON)
  digitalWrite(LIGHTS1, HIGH);
  digitalWrite(LIGHTS2, HIGH);
  digitalWrite(TEMP1, HIGH);
  digitalWrite(FOGGER1, HIGH);
  digitalWrite(MISTING1, HIGH);
  digitalWrite(AUX1, HIGH);

  // Initialise the Arduino data pins for OUTPUT
  pinMode(LIGHTS1, OUTPUT);
  pinMode(LIGHTS2, OUTPUT);
  pinMode(TEMP1, OUTPUT);
  pinMode(FOGGER1, OUTPUT);
  pinMode(MISTING1, OUTPUT);
  pinMode(AUX1, OUTPUT);
  delay(250); //Check that all relays are inactive at Reset
  //myGLCD.setDisplayPage(1);

  myGLCD.setWritePage(0);  //  <= Still displaying writepage 1
  myGLCD.clrScr();        //   <= Clear writepage 0

  // boot up logo
#if debug
  unsigned long performance = millis();
#endif
  myFiles.loadcpld(230, 40, 378, 268, "Logo.raw", 0, 6);  // <==== Write to writepage 0
  myFiles.loadcpld(340, 360, 149, 47, "Copy.raw", 0, 6);  // <==== Write to writepage 0
  myGLCD.setDisplayPage(0);                               // <==== Display writepage 0
  myFiles.loadcpld(0, 0, 800, 480, "1Home.raw", 1, 3);
  myFiles.loadcpld(0, 0, 800, 480, "2Lights.raw", 2, 3);
  myFiles.loadcpld(0, 0, 800, 480, "3Temp.raw", 3, 3);
  myFiles.loadcpld(0, 0, 800, 480, "456Sched.raw", 4, 3); //Fan/Fog/Mist to save video pages...
  //myFiles.loadcpld(0, 0, 800, 480, "4Fogger.raw", 4, 3);
  //myFiles.loadcpld(0, 0, 800, 480, "5Misting.raw", 5, 3);
  //myFiles.loadcpld(0, 0, 800, 480, "6Fan.raw", 6, 3);
#if fastest
  myFiles.loadcpld(0, 0, 800, 480, "7Clock.raw", 5, 3);
  myFiles.loadcpld(0, 0, 800, 480, "8Screen.raw", 6, 3);
#endif

  // display home screen
  screenHome();
  millisDim = millis();
#if debug
  Serial.print(F("Init time "));
  Serial.print(millisDim - performance);
  Serial.println(F(" ms"));
#endif
  myCopyTime(&prevRTC);
}

time_t syncProvider()
{
  //this does the same thing as RTC_DS1307::get()
#if(DS1307)
  return rtc.now().unixtime();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// VOID LOOP CODE /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
#if(!DS1307)
  if (Serial.available()) {
    processSyncMessage();
  }
#endif
  unsigned long currentMillis = millis(); // get current millis
  if (timeStatus() != timeNotSet) {
    //Serial.println("updateTimeDate");
    updateTimeDate();
  }

  if ((AutoDim) != 0) {
    if (currentMillis - millisDim > TouchDelay * 1000) {
      myGLCD.setBrightness((AutoDim - 1) * 2);
      BackLightTouch = false;
    }
    else {
      myGLCD.setBrightness(16);
    }
  }
  else
  {
    myGLCD.setBrightness(BackLight);
  }


  if ((currentMillis - millisHome > (HomeDelay * 60000UL)) && dispScreen != 1) {
    millisHome = currentMillis;
    screenHome();
  }
  // check for touch events
  if (myTouch.dataAvailable())  {
    if (currentMillis - prevMillisTouch > TouchRepeatDelay) // make sure it's been .5 sec (initially) between touches
    {
      // set BackLight touch if not already set and turn up brightness
      if (BackLightTouch == false) // BackLight touch is for adjusting brightness after touch
      {
        //analogWrite(screenBrightPin, backLight);
        myGLCD.setBrightness(BackLight);
        BackLightTouch = true;
      }

      prevMillisTouch = currentMillis; // reset the touch timer
      millisDim = currentMillis; // reset screen dim timer
      millisHome = currentMillis; // reset return home timer

      //      Serial.print("Updating millisHome to: ");
      //      Serial.print(millisHome);
      //      Serial.print("\n");

      processMyTouch();
    }
  }
  unsigned long rightNow = now();
  //    Serial.println(rightNow);
  //    Serial.println(humidTime);
  //    Serial.println(rightNow-humidTime);

  // Get the time in seconds (since 1970)
  if ((rightNow - humidTime) > 2) {
    if (dispScreen == 1 | dispScreen == 3)
    {
      Serial.println("tempread/humid");
    }
    tempRead();
    humRead();
  }

}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// PAGE SETUP ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void screenHome()  // draw main home screen showing overview info
{
  DoScreen(1, 1);
  //myGLCD.setDisplayPage(1);
  //myGLCD.setWritePage(1);
  dispScreen = 1;
#if debug
  Serial.println(F("Home Screen"));
#endif
  updateTime = false;
  if (tmpTouchRepeatDelay != TouchRepeatDelay) TouchRepeatDelay = tmpTouchRepeatDelay;
  // Draw Temp to screen
  tempRead();
  // Draw Humidity to screen
  humRead();
  // draw power status of outputs
  //myFiles.load(557, 416, 22, 22, pwrLightIconS[globalPower.pwrLight1]);
}

void screenLights()  // Lights screen
{
  DoScreen(2, 2);
  //myGLCD.setDisplayPage(2);
  //myGLCD.setWritePage(2);
  //dispScreen = 2;
#if debug
  Serial.println(F("Lights Screen"));
#endif
  tmpTouchRepeatDelay = TouchRepeatDelay;
  TouchRepeatDelay = 150;
  updateTime = false;
  //myFiles.loadcpld(0, 0, 800, 480, "2Lights.raw",2,3);
}

void screenTemp()  // Temp Screen
{
  DoScreen(3, 3);
  //myGLCD.setDisplayPage(3);
  //myGLCD.setWritePage(3);
  dispScreen = 3;
#if debug
  Serial.println(F("Temp Screen"));
#endif
  // Draw Temp to screen
  tempRead();
  // Draw Humidity to screen
  humRead();
  tmpTouchRepeatDelay = TouchRepeatDelay;
  TouchRepeatDelay = 100;
  myGLCD.printNumF(heater, 1, 510, 296, '.', 4);
  myGLCD.printNumF(fan, 1, 628, 296, '.', 5);

  updateTime = false;
  //  myFiles.load(0, 0, 800, 480, "3Temp.raw");;
}

void screenFogger()  // Fogging screen
{
  myFiles.loadcpld(21, 6, 336, 74, "FogTitle.raw", 4, 6);
  DoScreen(4, 4);
  //myGLCD.setDisplayPage(4);
  //myGLCD.setWritePage(4);
  //dispScreen = 4;
#if debug
  Serial.println(F("Fogger Screen"));
#endif
  updateTime = false;

  //myFiles.load(0, 0, 800, 480, "4Fogger.raw",3);
}

void screenMist()  // Misting Screen
{
  myFiles.loadcpld(21, 6, 336, 74, "MistTitl.raw", 4, 6);
  DoScreen(4, 5);
  //myGLCD.setDisplayPage(4);
  //myGLCD.setWritePage(4);
  //dispScreen = 5;
#if debug
  Serial.println(F("Misting Screen"));
#endif
  updateTime = false;
  //myFiles.load(0, 0, 800, 480, "5Misting.raw",3);
}

void screenFan()  // Fan screen
{
  myFiles.loadcpld(21, 6, 336, 74, "FanTitle.raw", 4, 6);
  DoScreen(4, 6);
  //myGLCD.setDisplayPage(4);
  //myGLCD.setWritePage(4);
  //dispScreen = 6;
#if debug
  Serial.println(F("Fan Screen"));
#endif
  updateTime = false;
  //myFiles.load(0, 0, 800, 480, "6Fan.raw",3);
}

//////////////////////////////////////////////////////////////////////////////
//   Modify the following 2 screens to be like the others for instant load  //
//////////////////////////////////////////////////////////////////////////////

void screenClock()  // Clock screen
{
#if fastest
  DoScreen(5, 7);
  //myGLCD.setWritePage(5);
  //myGLCD.setDisplayPage(5);
#else
  myGLCD.setWritePage(0);
  myGLCD.clrScr();
  myGLCD.setDisplayPage(0);
  myFiles.load(0, 0, 800, 480, "7Clock.raw", 3);
  dispScreen = 7;
#endif
  //dispScreen = 7;
#if debug
  Serial.println(F("Clock Screen"));
#endif
  myCopyTime(&tmpRTC);
  tmpTouchRepeatDelay = TouchRepeatDelay;
  TouchRepeatDelay = 150;
  updateTime = false;
}

void screenScreen()  // Screen Settings
{
#if fastest
  DoScreen(6, 8);
  //myGLCD.setWritePage(6);
  //myGLCD.setDisplayPage(6);
#else
  myGLCD.setWritePage(0);
  myGLCD.clrScr();
  myGLCD.setDisplayPage(0);
  myFiles.load(0, 0, 800, 480, "8Screen.raw", 3);
  dispScreen = 8;
#endif
  //dispScreen = 8;
#if debug
  Serial.println(F("Screen Screen"));
#endif
  myGLCD.setFont(Ubuntubold);
  myGLCD.printNumI(AutoDim, 713, 167);
  if (AutoDim != 0) {
    myGLCD.printNumI(TouchDelay, 689, 340, 3);
  }
  else
  {
    myGLCD.print("   ", 689, 340);
  }
  myGLCD.printNumI(HomeDelay, 338, 167, 2);
  myGLCD.printNumI(BackLight, 313, 340, 2);
  tmpTouchRepeatDelay = TouchRepeatDelay;
  TouchRepeatDelay = 150;
  updateTime = false;
}


////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  PROCESS MY TOUCH ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void processMyTouch() // this is a huge block dedicated to processing all touch screen events
{
  static volatile byte tempAD, tempTD, tempHD, tempBL;
  static volatile float tempHeater, tempFan;
  static volatile byte tempL1OnHr, tempL1OnMin;
  static volatile byte tempL1OffHr, tempL1OffMin;
  static volatile byte tempL2OnHr, tempL2OnMin;
  static volatile byte tempL2OffHr, tempL2OffMin;

  myTouch.read();
  word x = myTouch.getX();
  word y = myTouch.getY();

  // we evaluate touch based on which screen we are in
  switch (dispScreen)

  {

    case 1:  // home screen

      if ((y >= 117) && (y < 235))
      {
        if ((x >= 59) && (x <= 134)) // Light Settings Page
        {

          tempL1OnHr = Light1OnHr;
          tempL1OnMin = Light1OnMin;
          tempL1OffHr = Light1OffHr;
          tempL1OffMin = Light1OffMin;
          tempL2OnHr = Light2OnHr;
          tempL2OnMin = Light2OnMin;
          tempL2OffHr = Light2OffHr;
          tempL2OffMin = Light2OffMin;
          screenLights();
          myGLCD.setFont(Ubuntubold);
          myGLCD.printNumI(tempL1OnHr, 211, 207, 2);
          myGLCD.printNumI(tempL1OffHr, 211, 342, 2);
          myGLCD.printNumI(tempL1OnMin, 330, 207, 2, '0');
          myGLCD.printNumI(tempL1OffMin, 330, 342, 2, '0');
          myGLCD.printNumI(tempL2OnHr, 591, 207, 2);
          myGLCD.printNumI(tempL2OffHr, 591, 342, 2);
          myGLCD.printNumI(tempL2OnMin, 709, 207, 2, '0');
          myGLCD.printNumI(tempL2OffMin, 709, 342, 2, '0');
        }

        if ((x >= 175) && (x <= 240)) // Temp Settings Page
        {
          tempHeater = heater;
          tempFan = fan;
          screenTemp();
        }

        if ((x >= 270) && (x <= 362)) // Fogger Settings Page
        {
          screenFogger();
        }
      }

      if (( y >= 285) && (y < 411)) {

        if ((x >= 46) && (x <= 141)) // Misting Settings Page
        {
          screenMist();
        }

        if ((x >= 171) && (x <= 237)) // Fan Settings Page
        {
          screenFan();
        }

        if ((x >= 270) && (x <= 370)) // Clock Page
        {
          screenClock();
          SetDatePrint(56, 263);
          SetTimePrint(463, 263);
        }

      }

      if ((x >= 712) && (x <= 779) && (y >= 13) && (y <= 77)) // Screen Page
      {
        tempAD = AutoDim;
        tempTD = TouchDelay;
        tempHD = HomeDelay;
        tempBL = BackLight;
        screenScreen();
      }
      break;

    case 2:    // Light screen

      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479)) // Return Home button
      {
        TouchRepeatDelay = tmpTouchRepeatDelay;
        if (tempL1OnHr != Light1OnHr) {
          EEPROM.write(13, tempL1OnHr);
          Light1OnHr = tempL1OnHr;
#if debug
          Serial.println(F("Light1OnHr -> EEPROM"));
#endif
        }
        if (tempL1OnMin != Light1OnMin) {
          EEPROM.writeFloat(14, tempL1OnMin);
          Light1OnMin = tempL1OnMin;
#if debug
          Serial.println(F("Light1OnMin -> EEPROM"));
#endif
        }
        if (tempL1OffHr != Light1OffHr) {
          EEPROM.write(15, tempL1OffHr);
          Light1OffHr = tempL1OffHr;
#if debug
          Serial.println(F("Light1OffHr -> EEPROM"));
#endif
        }
        if (tempL1OffMin != Light1OffMin) {
          EEPROM.writeFloat(16, tempL1OffMin);
          Light1OffMin = tempL1OffMin;
#if debug
          Serial.println(F("Light1OffMin -> EEPROM"));
#endif
        }
        if (tempL2OnHr != Light2OnHr) {
          EEPROM.write(17, tempL2OnHr);
          Light2OnHr = tempL2OnHr;
#if debug
          Serial.println(F("Light2OnHr -> EEPROM"));
#endif
        }
        if (tempL2OnMin != Light2OnMin) {
          EEPROM.writeFloat(18, tempL2OnMin);
          Light2OnMin = tempL2OnMin;
#if debug
          Serial.println(F("Light2OnMin -> EEPROM"));
#endif
        }
        if (tempL2OffHr != Light2OffHr) {
          EEPROM.write(19, tempL2OffHr);
          Light2OffHr = tempL2OffHr;
#if debug
          Serial.println(F("Light2OffHr -> EEPROM"));
#endif
        }
        if (tempL2OffMin != Light2OffMin) {
          EEPROM.writeFloat(20, tempL2OffMin);
          Light2OffMin = tempL2OffMin;
#if debug
          Serial.println(F("Light2OffMin -> EEPROM"));
#endif
        }
        screenHome();
      }
      if ((x >= 150) && (x < 200)) // --> Hours
      {
        if ((y >= 173) && (y < 218)) // --> On1 Hours +
        {
          if (tempL1OnHr < 23)
            tempL1OnHr += 1;
        }
        if ((y >= 227) && (y < 272)) // --> On1 Hours -
        {
          if (tempL1OnHr > 0)
            tempL1OnHr -= 1;
        }
        if ((y >= 306) && (y < 351)) // --> Off1 Hours +
        {
          if (tempL1OffHr < 23)
            tempL1OffHr += 1;
        }
        if ((y >= 360) && (y < 405)) // --> Off1 Hours -
        {
          if (tempL1OffHr > 0)
            tempL1OffHr -= 1;
        }
        myGLCD.setFont(Ubuntubold);
        myGLCD.printNumI(tempL1OnHr, 211, 207, 2);
        myGLCD.printNumI(tempL1OffHr, 211, 342, 2);

      }
      if ((x >= 268) && (x < 318)) // ---> Minutes
      {
        if ((y >= 173) && (y < 218)) // -> On1 Minutes +
        {
          if (tempL1OnMin < 59)
            tempL1OnMin += 1;
        }
        if ((y >= 227) && (y < 272)) // -> On1 Minutes -
        {
          if (tempL1OnMin > 0)
            tempL1OnMin -= 1;
        }
        if ((y >= 306) && (y < 351)) // -> Off1 Minutes +
        {
          if (tempL1OffMin < 59)
            tempL1OffMin += 1;
        }
        if ((y >= 360) && (y < 405)) // -> Off1 Minutes -
        {
          if (tempL1OffMin > 0)
            tempL1OffMin -= 1;
        }
        myGLCD.setFont(Ubuntubold);
        myGLCD.printNumI(tempL1OnMin, 330, 207, 2, '0');
        myGLCD.printNumI(tempL1OffMin, 330, 342, 2, '0');
      }
      if ((x >= 530) && (x < 580)) // --> Hours
      {
        if ((y >= 173) && (y < 218)) // --> On1 Hours +
        {
          if (tempL2OnHr < 23)
            tempL2OnHr += 1;
        }
        if ((y >= 227) && (y < 272)) // --> On1 Hours -
        {
          if (tempL2OnHr > 0)
            tempL2OnHr -= 1;
        }
        if ((y >= 306) && (y < 351)) // --> Off1 Hours +
        {
          if (tempL2OffHr < 23)
            tempL2OffHr += 1;
        }
        if ((y >= 360) && (y < 405)) // --> Off1 Hours -
        {
          if (tempL2OffHr > 0)
            tempL2OffHr -= 1;
        }
        myGLCD.setFont(Ubuntubold);
        myGLCD.printNumI(tempL2OnHr, 591, 207, 2);
        myGLCD.printNumI(tempL2OffHr, 591, 342, 2);

      }
      if ((x >= 650) && (x < 700)) // ---> Minutes
      {
        if ((y >= 173) && (y < 218)) // -> On1 Minutes +
        {
          if (tempL2OnMin < 59)
            tempL2OnMin += 1;
        }
        if ((y >= 227) && (y < 272)) // -> On1 Minutes -
        {
          if (tempL2OnMin > 0)
            tempL2OnMin -= 1;
        }
        if ((y >= 306) && (y < 351)) // -> Off1 Minutes +
        {
          if (tempL2OffMin < 59)
            tempL2OffMin += 1;
        }
        if ((y >= 360) && (y < 405)) // -> Off1 Minutes -
        {
          if (tempL2OffMin > 0)
            tempL2OffMin -= 1;
        }
        myGLCD.setFont(Ubuntubold);
        myGLCD.printNumI(tempL2OnMin, 709, 207, 2, '0');
        myGLCD.printNumI(tempL2OffMin, 709, 342, 2, '0');
      }


      break;

    case 3:    // Temp screen

      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479)) // Return Home button
      {
        TouchRepeatDelay = tmpTouchRepeatDelay;
        if (tempFan != fan) {
          EEPROM.writeFloat(9, tempFan);
          fan = tempFan;
#if debug
          Serial.println(F("Fan -> EEPROM"));
#endif
        }
        if (tempHeater != heater) {
          EEPROM.writeFloat(5, tempHeater);
          heater = tempHeater;
#if debug
          Serial.println(F("Heater -> EEPROM"));
#endif
        }
        screenHome();
      }
      myGLCD.setFont(Ubuntubold);
      if ((x >= 520) && (x < 595))
      {
        if ((y >= 208) && (y < 282)) // Low Temp +
        {
          tempHeater += 0.1;
        }
        if ((y >= 342) && (y < 416)) // Low Temp -
        {
          tempHeater -= 0.1;
        }
        myGLCD.printNumF(tempHeater, 1, 510, 296, '.', 4);
      }
      if ((x >= 650) && (x < 725))
      {
        if ((y >= 208) && (y < 282)) // High Temp +
        {
          tempFan += 0.1;
        }
        if ((y >= 342) && (y < 416)) // High Temp -
        {
          tempFan -= 0.1;
        }
        myGLCD.printNumF(tempFan, 1, 628, 296, '.', 5);
      }
      break;

    case 4:    // Fogger screen

      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479)) // Return Home button
      {
        screenHome();
      }
      break;

    case 5:    // Misting screen

      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479)) // Return Home button
      {
        screenHome();
      }
      break;

    case 6:    // Fan screen

      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479)) // Return Home button
      {
        screenHome();
      }
      break;

    case 7:    // Clock screen

      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479)) // Return Home button
      {
        TouchRepeatDelay = tmpTouchRepeatDelay;
        screenHome();
      }

      if ((y >= 175) && (y < 250)) {  // 'Increase' row
        if ((x >= 55) && (x < 130)) { // 'Months'
          //Serial.println(F("Month+"));
          if (tmpRTC.Month < 12) tmpRTC.Month += 1;
        }
        if ((x >= 165) && (x < 240)) { // 'Days'
          //Serial.println(F("Days+"));
          if (((tmpRTC.Day < 31) && (tmpRTC.Month != 9 && tmpRTC.Month != 4 && tmpRTC.Month != 6 && tmpRTC.Month != 11 &&  tmpRTC.Month != 2))
              || ((tmpRTC.Day < 30) && (tmpRTC.Month == 9 || tmpRTC.Month == 4 || tmpRTC.Month == 6 || tmpRTC.Month == 11 || tmpRTC.Month != 2))
              || ((tmpRTC.Day < 28) && (tmpRTC.Month == 2))
              || ((tmpRTC.Day < 29) && (tmpRTC.Month == 2) && (((float)(tmpRTC.Year / 4.0) - (int)(tmpRTC.Year / 4.0) == 0.00 )
                  || ((float)(tmpRTC.Year / 400.0) - (int)(tmpRTC.Year / 400.0) == 0.00 )) && ((float)(tmpRTC.Year / 100.0) - (int)(tmpRTC.Year / 100.0) != 0.00 ))
             ) tmpRTC.Day += 1;
        }
        if ((x >= 275) && (x < 350)) { // 'Years'
          //Serial.println(F("Years+"));
          tmpRTC.Year += 1;
        }
        if ((x >= 450) && (x < 525)) { // '24Hrs'
          if (tmpRTC.Hour < 23) tmpRTC.Hour += 1;
        }
        if ((x >= 560) && (x < 635)) { // Minutes'
          if (tmpRTC.Minute < 59) tmpRTC.Minute += 1;
        }
        if ((x >= 665) && (x < 740)) { // 'Seconds'
          // Not implementing second updates....
        }
        SetDatePrint(56, 263);
        SetTimePrint(463, 263);
      }

      if ((y >= 310) && (y < 385)) {  // 'Decrease' row
        if ((x >= 55) && (x < 130)) { // 'Months'
          //Serial.println(F("Month-"));
          if (tmpRTC.Month > 1) tmpRTC.Month -= 1;
        }
        if ((x >= 165) && (x < 240)) { // 'Days'
          //Serial.println(F("Days-"));
          if (tmpRTC.Day > 1) tmpRTC.Day -= 1;
        }
        if ((x >= 275) && (x < 350)) { // 'Years'
          //Serial.println(F("Years-"));
          tmpRTC.Year -= 1;
        }
        if ((x >= 450) && (x < 525)) { // '24Hrs'
          if (tmpRTC.Hour > 0 ) tmpRTC.Hour -= 1;
        }
        if ((x >= 560) && (x < 635)) { // Minutes'
          if (tmpRTC.Minute > 0 ) tmpRTC.Minute -= 1;
        }
        if ((x >= 665) && (x < 740)) { // 'Seconds'
          // Not implementing second updates....
        }
        SetDatePrint(56, 263);
        SetTimePrint(463, 263);
      }

      if ((y >= 410) && (y < 445)) { // 'Set' 'Cancel' Row
        if ((x >= 225) && (x < 345)) { //'Set'
#if debug
          Serial.println(F("Time Set"));
#endif
          time_t xo = makeTime(tmpRTC);
          rtc.adjust(xo);
          rtc.now();
          //Serial.println(xo);
          TouchRepeatDelay = tmpTouchRepeatDelay;
          screenHome();
        }
        if ((x >= 460) && (x < 580)) { //'Cancel'
          TouchRepeatDelay = tmpTouchRepeatDelay;
          screenHome();
        }
      }

      break;

    case 8:    // Screen screen
      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479)) // Return Home button
      {
        TouchRepeatDelay = tmpTouchRepeatDelay;
        if (tempAD != AutoDim) {
          EEPROM.write(3, tempAD);
          AutoDim = tempAD;
#if debug
          Serial.println(F("AutoDim -> EEPROM"));
#endif
        }
        if (tempTD != TouchDelay) {
          EEPROM.write(4, tempTD);
          TouchDelay = tempTD;
#if debug
          Serial.println(F("TouchDelay -> EEPROM"));
#endif
        }
        if (tempHD != HomeDelay) {
          EEPROM.write(1, tempHD);
          HomeDelay = tempHD;
#if debug
          Serial.println(F("HomeDelay -> EEPROM"));
#endif
        }
        if (tempBL != BackLight) {
          EEPROM.write(2, tempBL);
          BackLight = tempBL;
#if debug
          Serial.println(F("BackLight -> EEPROM"));
#endif
        }
        screenHome();
      }

      if ((x >= 610 && x < 685)) { // 'Autodim' Column
        if ((y >= 105 && y < 175)) { // 'Level +'
          if ( tempAD < 5) {
            tempAD += 1;
          }
        }
        if ((y >= 190 && y < 260)) { // 'Level -'
          if (tempAD > 0) {
            tempAD -= 1;
          }
        }
        if ((y >= 280 && y < 350)) {  // 'Seconds +'
          if (tempTD < 255) {
            tempTD += 1;
          }
        }
        if ((y >= 365 && y < 435)) {  // 'Seconds -'
          if (tempTD > 0) {
            tempTD -= 1;
          }
        }
        myGLCD.setFont(Ubuntubold);
        myGLCD.printNumI(tempAD, 713, 167);
        if (tempAD != 0) {
          myGLCD.printNumI(tempTD, 689, 340, 3);
        }
        else
        {
          myGLCD.print("   ", 689, 340);
        }
      }

      if ((x >= 200 && x < 275)) { // 'Home' & 'Bright' Column
        if ((y >= 105 && y < 175)) { // 'Home +'
          if ( tempHD < 20) {
            tempHD += 1;
          }
        }
        if ((y >= 190 && y < 260)) { // 'Home -'
          if (tempHD > 1) {
            tempHD -= 1;
          }
        }
        if ((y >= 280 && y < 350)) {  // 'Bright +'
          if (tempBL < 16) {
            tempBL += 1;
          }
        }
        if ((y >= 365 && y < 435)) {  // 'Bright -'
          if (tempBL > 2) {
            tempBL -= 1;
          }
        }
        myGLCD.setFont(Ubuntubold);
        myGLCD.printNumI(tempHD, 338, 167, 2);
        myGLCD.printNumI(tempBL, 313, 340, 2);
      }
      break;

  }

}

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  ALL OTHER ROUTINES //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void updateTimeDate()
{
  myGLCD.setColor(255, 77, 0);
  myGLCD.setFont(arial_bold);

  time_t timeNow = rtc.now().unixtime();
  // draw date and time

  if ((hour(timeNow) != prevRTC.Hour) || (minute(timeNow) != prevRTC.Minute) || !updateTime) { //time
    prevRTC.Hour = hour(timeNow);
    prevRTC.Minute = minute(timeNow);
    byte ampm = isPM(timeNow);
    int hr_12 = hourFormat12(timeNow);
    printTime(timeNow, 250, 2);
  }
  if ((day(timeNow) != prevRTC.Day) || (month(timeNow) != prevRTC.Month) || (year(timeNow) != prevRTC.Year
      || !updateTime) ) {     //date
    prevRTC.Day = day(timeNow);
    prevRTC.Month = month(timeNow);
    printDate(timeNow, 400, 2);
  }
  updateTime = true;
}

void printTime(time_t timeNow, int posx, int posy)
{
  char tmpTime[8], charT[3];

  tmpTime[0] = '\0';

  if (hourFormat12(timeNow) >= 0 && hourFormat12(timeNow) < 10) { //add space
    strcat(tmpTime, " ");
    itoa(hourFormat12(timeNow), charT, 10);
    strcat(tmpTime, charT);
  }
  else
    itoa(hourFormat12(timeNow), tmpTime, 10);

  strcat(tmpTime, ":");

  if (minute(timeNow) >= 0 && minute(timeNow) < 10) { //add 0
    strcat(tmpTime, "0");
    itoa(minute(timeNow), charT, 10);
    strcat(tmpTime, charT);
  }
  else {
    itoa(minute(timeNow), charT, 10);
    strcat(tmpTime, charT);
  }
  if (isPM(timeNow) == 0) strcat(tmpTime, "am");
  else strcat(tmpTime, "pm");
  myGLCD.setColor(255, 77, 0);
  myGLCD.setFont(arial_bold);
  myGLCD.print(tmpTime, posx, posy);           // Display time
}

void printDate(time_t timeNow, int x, int y)
{
  char  chDate[25], tmpChar[5];
  strcat(chDate, "     ");
  chDate[0] = '\0';
  strcat(chDate, Day[weekday(timeNow)]);
  strcat(chDate, ", ");
  strcat(chDate, Mon[month(timeNow)]);
  if (day(timeNow) < 10)
  {
    strcat(chDate, "  ");
  }
  else
  {
    strcat(chDate, " ");
  }
  itoa(day(timeNow), tmpChar, 10);
  strcat(chDate, tmpChar);
  // this line is for omitting year
  strcat(chDate, "  ");
  myGLCD.setColor(255, 77, 0);
  myGLCD.setFont(arial_bold);
  myGLCD.print(chDate, x, y);            //Display date
}

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                Supporting 'stuff' added by Graham Lawrence                               //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////
void SetDatePrint(int x, int y)
{
  char  chDate[25], tmpChar[5];
  strcat(chDate, "     ");
  chDate[0] = '\0';
  strcat(chDate, Mon[tmpRTC.Month]);
  if (tmpRTC.Day < 10) {
    strcat(chDate, "   ");
  }
  else
  {
    strcat(chDate, "  ");
  }
  itoa(tmpRTC.Day, tmpChar, 10);
  strcat(chDate, tmpChar);
  strcat(chDate, " ");
  itoa(tmpRTC.Year + 1970, tmpChar, 10);
  strcat(chDate, tmpChar);
  myGLCD.setFont(Ubuntubold);
  myGLCD.print(chDate, x, y);
}
void SetTimePrint(int x, int y)
{
  char  chDate[25], tmpChar[5];
  strcat(chDate, "     ");
  chDate[0] = '\0';
  if (tmpRTC.Hour < 10) {
    strcat(chDate, " ");
  }
  itoa(tmpRTC.Hour, tmpChar, 10);
  strcat(chDate, tmpChar);
  strcat(chDate, " : ");
  if (tmpRTC.Minute < 10) {
    strcat(chDate, "0");
  }
  itoa(tmpRTC.Minute, tmpChar, 10);
  strcat(chDate, tmpChar);
  strcat(chDate, " .00");
  myGLCD.setFont(Ubuntubold);
  myGLCD.print(chDate, x, y);
}

#if DS1307 == 0
void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if ( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      rtc.adjust(pctime); // Sync Arduino clock to the time received on the serial port
      setTime(pctime);
#if debug
      Serial.println("Time Set");
#endif
    }
  }
}
#endif

void myCopyTime(tmElements_t * mytime)
{
  DateTime now = rtc.now();
  mytime->Month = now.month();
  mytime->Day = now.day();
  mytime->Year = now.year() - 1970;
  mytime->Hour = now.hour();
  mytime->Minute = now.minute();
  mytime->Second = 0;
}

void DoScreen(byte page, byte screen) {
  myGLCD.setDisplayPage(page);
  myGLCD.setWritePage(page);
  dispScreen = screen;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////    DS18b20 TEMP SENSORS   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


void tempRead()
{
  //  gettemp(); // Read the temperature.
#if notusestring
  float probe1g = sensors.getTempFByIndex(0);
  float probe2g = sensors.getTempFByIndex(1);
  float probe3g = sensors.getTempFByIndex(2);
  char sensor1[6], sensor2[6], sensor3[6];
  dtostrf(probe1g, 4, 1, sensor1);
  dtostrf(probe2g, 4, 1, sensor2);
  dtostrf(probe3g, 4, 1, sensor3);
#else
  String probe01 = String(sensors.getTempFByIndex(0));
  probe01.toCharArray(sensorPrintout, 5);
  String probe02 = String(sensors.getTempFByIndex(1));
  probe02.toCharArray(sensorPrintout1, 5);
  String probe03 = String(sensors.getTempFByIndex(2));
  probe03.toCharArray(sensorPrintout2, 5);
#endif
  if (dispScreen == 1) {
    myGLCD.setColor(255, 77, 0);
    myGLCD.setFont(arial_bold);
#if notusestring
    myGLCD.print(sensor1, 424, 316);
    myGLCD.print(sensor2, 552, 316);
    myGLCD.print(sensor3, 675, 316);
#else
    myGLCD.print(sensorPrintout, 424, 316);
    myGLCD.print(sensorPrintout1, 552, 316);
    myGLCD.print(sensorPrintout2, 675, 316);
#endif
    //delay(100);
  }

  if (dispScreen == 3) {
    myGLCD.setColor(255, 77, 0);
    myGLCD.setFont(Ubuntubold);
#if notusestring
    myGLCD.print(sensor1, 47, 325);
    myGLCD.print(sensor2, 197, 325);
    myGLCD.print(sensor3, 353, 325);
#else
    myGLCD.print(sensorPrintout, 47, 325);
    myGLCD.print(sensorPrintout1, 197, 325);
    myGLCD.print(sensorPrintout2, 353, 325);
#endif
    //delay(100);
  }
}


//void gettemp() // call sensors.requestTemperatures() to issue
//// a global temp request to all devices on the bus.
//{
//  sensors.requestTemperatures(); // Send the command to get temperatures.
//  probe01 = (sensors.getTempFByIndex(0));
//  probe02 = (sensors.getTempFByIndex(1));
//  probe03 = (sensors.getTempFByIndex(2));
//
//}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    DHT22 HUMIDITY SENSORS   ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void humRead()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity;
  int chk=DHT.read22(DHTPIN);
  if (chk==DHTLIB_OK) humidity = DHT.humidity;
  //Serial.print("Humidity: ");
  //Serial.println(humidity);

  char hum[6];
  dtostrf(humidity, 5, 1, hum);
  if (dispScreen == 1) {
    myGLCD.setColor(255, 77, 0);
    myGLCD.setFont(Ubuntubold);
    //Printing Humidity
    myGLCD.print(hum, 403, 405);
    //myGLCD.print("°",430,405);
  }
  humidTime = now();
}

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    ALARM HANDLERS/   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    FIRST RUN SETUP   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void firstRunSetup()
{
  // I'm going to save 26 into EEPROM bank 2000 as a check
  if (EEPROM.read(2000) != 26)
  {
    EEPROM.write(2000, 26); // write to EEPROM so this never runs again
    // default screen settings
    EEPROM.write(1, 1); //         <- HomeDelay
    EEPROM.write(2, 16);//         <- BackLight
    EEPROM.write(3, 1); //         <- AutoDim
    EEPROM.write(4, 10);//         <- TouchDelay
    EEPROM.writeFloat(5, 42.9);//  <- Heater
    EEPROM.writeFloat(9, 102.9);// <- Fan
    EEPROM.write(13, 8); //        <- Light1OnHr
    EEPROM.write(14, 15); //       <- Light1OnMin
    EEPROM.write(15, 10); //       <- Light1OffHr
    EEPROM.write(16, 30); //       <- Light1OffMin
    EEPROM.write(17, 16); //       <- Light2OnHr
    EEPROM.write(18, 30); //       <- Light2OnMin
    EEPROM.write(19, 19); //       <- Light2OffHr
    EEPROM.write(20, 45); //       <- Light2Offmin

  }
}

