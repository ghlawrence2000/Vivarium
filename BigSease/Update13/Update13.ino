//////////////////////////////////////////////////////////////////////////////////////////////////////
// Vivarium Management Controller w/ 7" touchscreen Interface                                       //
// Written and coded by David Sease, aka Bigsease30 and ghlawrence2000 on arduino.cc!               //
// Much code was swiped, modified, and integrated or otherwise inspired from other public works     //
//                                                                                                  //
// All code is public domain, feel free to use, abuse, edit, and share                              //
// Written for Arduino Mega 2560                                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     VERSION:  02/02/15 20:15GMT                                  //
//                                        Development Version 13                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         CODE ORDER:                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//    Declare EEPROM useage                                                                         //
//    Declare Libraries needed                                                                      //
//    Compile time options                                                                          //
//    Initiate Screen, Touch, Imaging and Fonts                                                     //
//    Define Pins                                                                                   //
//    Global variables & setup                                                                      //
//    Main loop                                                                                     //
//    Routines to draw every screen                                                                 //
//    Routine to handle touch events                                                                //
//    Other supporting routines                                                                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 EEPROM locations (saved settings)                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////
// SPACE //         DESCRIPTION              // DEFAULTS                                            //
//     0 //  ID     : 32                     // 32 if previously ran                                //
//     1 //  screen : return home            // 1 min                                               //
//     2 //  screen : brightness if no dim   // 16 max                                              //
//     3 //  screen : AutoDim level          // 1                                                   //
//     4 //  screen : AutoDim seconds        // 10 sec                                              //
//     5 //  Heater : Float byte 0           //\                                                    //
//     6 //  Heater : Float byte 1           //  _ Initial value                                    //
//     7 //  Heater : Float byte 2           //    42.9F                                            //
//     8 //  Heater : Float byte 3           ///                                                    //
//     9 //  Fan    : Float byte 0           //\                                                    //
//    10 //  Fan    : Float byte 1           //  _ Initial value                                    //
//    11 //  Fan    : Float byte 2           //    102.9F                                           //
//    12 //  Fan    : Float byte 3           ///                                                    //
//    13 //  Lights1: On1Hr                  // 8 am  //Light 1 ON (Day Light)                      //
//    14 //  Lights1: On1Min                 // 15 min                                              //
//    15 //  Lights1: Off1Hr                 // 20 pm //Light 1 OFF                                 //
//    16 //  Lights1: Off1Min                // 30 min                                              //
//    17 //  Lights2: On2Hr                  // 20 pm //Light 2 ON (Night Light)                    //
//    18 //  Lights2: On2Min                 // 30 min                                              //
//    19 //  Lights2: Off2Hr                 // 8 am //Light 2 OFF                                  //
//    20 //  Lights2: Off2Min                // 15 min                                              //
//    21 //  Fog    : Fog.Enable             // ( F0 = 1 enable, 2 disable, FF = 1 & 2 enable )     //
//    22 //  Fog    : Fog.On1Hr              //                                                     //
//    23 //  Fog    : Fog.On1Min             //                                                     //
//    24 //  Fog    : Fog.Dur1Min            //                                                     //
//    25 //  Fog    : Fog.Dur1Sec            //                                                     //
//    26 //  Fog    : Fog.On2Hr              //                                                     //
//    27 //  Fog    : Fog.On2Min             //                                                     //
//    28 //  Fog    : Fog.Dur2Min            //                                                     //
//    29 //  Fog    : Fog.Dur2Sec            //                                                     //
//    30 //  Fog    : Fog.OnDay              //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x3c=Tue-Fri       //
//    31 //  Mist1&2: Mist.Enable            // ( F0 = 1 enable, 2 disable, FF = 1 & 2 enable )     //
//    32 //  Mist1  : Mist.On1Hr             //                                                     //
//    33 //  Mist1  : Mist.On1Min            //                                                     //
//    34 //  Mist1  : Mist.Dur1Min           //                                                     //
//    35 //  Mist1  : Mist.Dur1Sec           //                                                     //
//    36 //  Mist2  : Mist.On2Hr             //                                                     //
//    37 //  Mist2  : Mist.On2Min            //                                                     //
//    38 //  Mist2  : Mist.Dur2Min           //                                                     //
//    39 //  Mist2  : Mist.Dur2Sec           //                                                     //
//    40 //  Mist2  : Mist.OnDay             //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x70 =Thu-Sat      //
//    41 //  Mist3&4: Mist2.Enable           // ( F0 = 3 enable, 4 disable, FF = 3 & 4 enable )     //
//    42 //  Mist3  : Mist2.On3Hr            //                                                     //
//    43 //  Mist3  : Mist2.On3Min           //                                                     //
//    44 //  Mist3  : Mist2.Dur3Min          //                                                     //
//    45 //  Mist3  : Mist2.Dur3Sec          //                                                     //
//    46 //  Mist4  : Mist2.On3Hr            //                                                     //
//    47 //  Mist4  : Mist2.On3Min           //                                                     //
//    48 //  Mist4  : Mist2.Dur3Min          //                                                     //
//    49 //  Mist4  : Mist2.Dur3Sec          //                                                     //
//    50 //  Mist4  : Mist2.OnDay            //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x0F =Sun-Wed      //
//    51 //  Fan    : Fan.Enable             // ( F0 = 1 enable, 2 disable, FF = 1 & 2 enable )     //
//    52 //  Fan    : Fan.On1Hr              //                                                     //
//    53 //  Fan    : Fan.On1Min             //                                                     //
//    54 //  Fan    : Fan.Dur1Min            //                                                     //
//    55 //  Fan    : Fan.Dur1Sec            //                                                     //
//    56 //  Fan    : Fan.On2Hr              //                                                     //
//    57 //  Fan    : Fan.On2Min             //                                                     //
//    58 //  Fan    : Fan.Dur2Min            //                                                     //
//    59 //  Fan    : Fan.Dur2Sec            //                                                     //
//    60 //  Fan    : Fan.OnDay              //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x0F =Sun-Wed      //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Decare Libraries                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Wire.h>                  // needed by tons of stuff  <== Davids excellent description!!   //
#include <EEPROM.h>                // used to store and retrieve settings from memory               //
#include <UTFT.h>                  // used to interface with the TFT display                        //
#include <UTouch.h>                // used to interface with the touch controller on the TFT display//
#include <SdFat.h>                 // used to access the SD card                                    //
#include <UTFT_SdRaw.h>            // used to read .raw images from the SD card                     //
#include <RTClib.h>                // RTC library (software or hardware capable)                    //
#include <Time.h>                  // allows conversion to UNIX time for easier date/time math      //
#include <TimeAlarms.h>            // used to power schedules                                       //
#include <OneWire.h>               // library to communicate with the DallasTemperature sensor      // 
#include <DallasTemperature.h>     // library for the Temp sensor itself                            //
#include <dht.h>                   // Rob Tillaart's improved humidity library                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Compile time options                                           //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define DS1307   0 // <====== 1 for DS1307, 0 for Software                                          //
#define fastest  1 // <====== 1 = MAX speed all screens in own page / 0 = 2 Screens share a page... //
#define debug    1 // <====== 1 = Serial output enabled                                             //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                           Initiate Screen, Touch, Imaging and Fonts                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////
UTFT myGLCD(CTE70CPLD, 38, 39, 40, 41); // start up an instance of the TFT screen                   //
UTouch  myTouch( 6, 5, 4, 3, 2);        // start up an instance of for touch                        //
UTFT_SdRaw myFiles(&myGLCD);            // start up an instance to read images from SD              //
SdFat sd;                               // start up as instance of SD                               //
extern uint8_t SmallFont[];                                                                         //
extern uint8_t Sinclair_S[];                                                                        //
extern uint8_t arial_bold[];                                                                        //
extern uint8_t Ubuntubold[];                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         Define pins                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//           Declare Pins and binary bits for Relays (analog to save digital pins)                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define FAN1        A0                                                                              //
#define MISTING1    A1                                                                              //
#define FOGGER1     A2                                                                              //
#define TEMP1       A3                                                                              //
#define LIGHTS2     A4                                                                              //    
#define LIGHTS1     A5                                                                              //                     
#define pwrFan1      0   // binary  1                                                               //  
#define pwrMisting1  1   // binary  2                                                               //
#define pwrFogger1   2   // binary  4                                                               //
#define pwrTemp1     3   // binary  8                                                               //
#define pwrLight2    4   // binary 16                                                               //
#define pwrLight1    5   // binary 32                                                               //
byte Relay = 0x00, prevRelay = 0x40;                                                                //
/*struct PWR                                                                                        //
{                                                                                                   //
  byte pwrLight1;                                                                                   //
  byte pwrLight2;                                                                                   //
  byte pwrTemp1;                                                                                    //
  byte pwrFogger1;                                                                                  //
  byte pwrMisting1;                                                                                 //
  byte pwrAUX1;                                                                                     //
}*/                                                                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         Pins for sensors                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define ONE_WIRE_BUS     8                                                                          //
//#define TEMPERATURE_PRECISION 9           // Unused ????                                          //
#define DHTPIN           9                                                                          //
#define SD_CHIP_SELECT  53                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                    Humidity and Temperature                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup our humidity sensor                                                                        //
dht DHT;                                                                                            //
// Setup oneWire instance to communicate with any OneWire devices                                   //
OneWire oneWire(ONE_WIRE_BUS);                                                                      //
// Pass our oneWire reference to Dallas Temperature.                                                //
DallasTemperature sensors(&oneWire);                                                                //
//DeviceAddress tempLeft, tempMiddle, tempRight;                                                    //
//DeviceAddress tempLeft = { 0x28, 0x24, 0x18, 0x5D, 0x06, 0x00, 0x00, 0xA2 };                      //
//DeviceAddress tempMiddle = { 0x28, 0x39, 0x0B, 0x5C, 0x06, 0x00, 0x00, 0x0E };                    //
//DeviceAddress tempRight = { 0x28, 0xED, 0x16, 0x5C, 0x06, 0x00, 0x00, 0x86 };                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Time                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#if DS1307 ==1                                                                                      //
RTC_DS1307 rtc;                                                                                     //
#else                                                                                               //
RTC_Millis rtc;                                                                                     //
#define TIME_HEADER  "T"   // Header tag for serial time sync message                               //
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message                     //
#endif                                                                                              //
boolean updateTime = false; // keep track of when to udpate the clock                               //
// days and month character strings for displaing at the top of the screen                          //
const char *Day[] = {
  "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};                                                                                                  //
const char *Mon[] = {
  "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};                                                                                                  //
tmElements_t tmpRTC, prevRTC, lastLoopRTC;                                                          //
unsigned long tempTime;                                                                             //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Various timings to keep track of                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long prevMillisTouch = 0; // track time between touches                                    //
unsigned long prevMillis5sec = 0; // track 5 seconds for refreshing clock and temp                  //
unsigned long millisDim = 0; // used for brightness adjustment                                      //
unsigned long millisHome = 0; // used for returning home after configured time                      //
unsigned long TouchRepeatDelay = 500; // millis                                                     //
unsigned long tmpTouchRepeatDelay = 500; //millis                                                   //
unsigned long humidTime = 0; // 2 seconds minimum reading                                           //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Various screen settings                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////
byte HomeDelay;  // (Return Home) Minutes 0-59                                                      //
byte BackLight;  // (Bright) (not AutoDim) startup brightness to 100% 1-16                          //
byte AutoDim;    // (AutoDim Level) maximum brightness when dimmed 1-5                              //
byte TouchDelay; // (AutoDim Seconds) 0-59                                                          //
byte dispScreen = 1;  // Currently displayed screen, screens shown below                            //
// 1-home, 2-lights, , 3-temp, 4-fogger, 5-mister, 6-fan, 7-clock, 8-screen                         //
byte mistScreen = 1;                                                                                //
boolean BackLightTouch = true; // initial setting to allow the screen to stay on after boot         //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                               Scheduling and Alarm related variables                             //
//////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
  byte Enable;   // If Lights, not used                                                             //
  byte On1Hr;                                                                                       //
  byte On1Min;                                                                                      //
  byte Dur1Min;  // If Lights, then Lights1 Off1Hr                                                  //
  byte Dur1Sec;  // If Lights, then Lights1 Off1Min                                                 //
  byte On2Hr;                                                                                       //
  byte On2Min;                                                                                      //
  byte Dur2Min;  // If Lights, then Lights2 Off2Hr                                                  //
  byte Dur2Sec;  // If Lights, then Lights2 Off2Min                                                 //
  byte OnDay;    // If Lights, not used                                                             //
} SCHEDULE ;                                                                                        //
SCHEDULE Light, Fog, Mist, Mist2, Fan;                                                              //
float heater;    // = 35.9;                                                                         //
float fan;       // = 87.9;                                                                         //
float temp1 = 0.0, temp2 = 0.0, temp3 = 0.0, hum1 = 0.0;                                            //
//byte FogOn1Hr, FogOn1Min, FogDur1Min, FogDur1Sec;                                                 //
//byte FogOn2Hr, FogOn2Min, FogDur2Min, FogDur2Sec;                                                 //
//byte FogDay;                                                                                      //
//byte MistOn1Hr, MistOn1Min, MistDur1Min, MistDur1Sec;                                             //
//byte MistOn2Hr, MistOn2Min, MistDur2Min, MistDur2Sec;                                             //
//byte MistDay;                                                                                     //
//byte FanOn1Hr, FanOn1Min, FanDur1Min, FanDur1Sec;                                                 //
//byte FanOn2Hr, FanOn2Min, FanDur2Min, FanDur2Sec;                                                 //
//byte FanDay;                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      SETUP CODE                                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
#if debug
  // start serial port
  Serial.begin(57600);
#endif
  // Set default values for the first ever run
  firstRunSetup();
  // get screen settings from eeprom
  HomeDelay     = EEPROM.read(1);
  BackLight     = EEPROM.read(2);
  AutoDim       = EEPROM.read(3);
  TouchDelay    = EEPROM.read(4);
  heater        = EEPROM.readFloat(5);
  fan           = EEPROM.readFloat(9);
  Light.On1Hr   = EEPROM.read(13);
  Light.On1Min  = EEPROM.read(14);
  Light.Dur1Min = EEPROM.read(15);
  Light.Dur1Sec = EEPROM.read(16);
  Light.On2Hr   = EEPROM.read(17);
  Light.On2Min  = EEPROM.read(18);
  Light.Dur2Min = EEPROM.read(19);
  Light.Dur2Sec = EEPROM.read(20);
  Fog.Enable    = EEPROM.read(21);
  Fog.On1Hr     = EEPROM.read(22);
  Fog.On1Min    = EEPROM.read(23);
  Fog.Dur1Min   = EEPROM.read(24);
  Fog.Dur1Sec   = EEPROM.read(25);
  Fog.On2Hr     = EEPROM.read(26);
  Fog.On2Min    = EEPROM.read(27);
  Fog.Dur2Min   = EEPROM.read(28);
  Fog.Dur2Sec   = EEPROM.read(29);
  Fog.OnDay     = EEPROM.read(30);
  Mist.Enable   = EEPROM.read(31);
  Mist.On1Hr    = EEPROM.read(32);
  Mist.On1Min   = EEPROM.read(33);
  Mist.Dur1Min  = EEPROM.read(34);
  Mist.Dur1Sec  = EEPROM.read(35);
  Mist.On2Hr    = EEPROM.read(36);
  Mist.On2Min   = EEPROM.read(37);
  Mist.Dur2Min  = EEPROM.read(38);
  Mist.Dur2Sec  = EEPROM.read(39);
  Mist.OnDay    = EEPROM.read(40);
  Mist2.Enable  = EEPROM.read(41);
  Mist2.On1Hr   = EEPROM.read(42);
  Mist2.On1Min  = EEPROM.read(43);
  Mist2.Dur1Min = EEPROM.read(44);
  Mist2.Dur1Sec = EEPROM.read(45);
  Mist2.On2Hr   = EEPROM.read(46);
  Mist2.On2Min  = EEPROM.read(47);
  Mist2.Dur2Min = EEPROM.read(48);
  Mist2.Dur2Sec = EEPROM.read(49);
  Mist2.OnDay   = EEPROM.read(50);
  Fan.Enable    = EEPROM.read(51);
  Fan.On1Hr     = EEPROM.read(52);
  Fan.On1Min    = EEPROM.read(53);
  Fan.Dur1Min   = EEPROM.read(54);
  Fan.Dur1Sec   = EEPROM.read(55);
  Fan.On2Hr     = EEPROM.read(56);
  Fan.On2Min    = EEPROM.read(57);
  Fan.Dur2Min   = EEPROM.read(58);
  Fan.Dur2Sec   = EEPROM.read(59);
  Fan.OnDay     = EEPROM.read(60);
  ///////////////////////////////////////////////////////////////////////////////////////////
  //                           Start up DS18B20 Temp Sensors                               //
  sensors.begin();                                                                         //
  ///////////////////////////////////////////////////////////////////////////////////////////
#if debug
  // locate DS18B20 temperature devices on the bus
  Serial.print(F("Locating Dallas 1820 devices... "));
  Serial.print(F("Found "));
  Serial.print(sensors.getDeviceCount());
  Serial.println(F("."));
#endif
  ///////////////////////////////////////////////////////////////////////////////////////////
  //                                 Initiate the SD Card                                  //
  ///////////////////////////////////////////////////////////////////////////////////////////
#if debug
  Serial.println(F("Initialising SD card..."));
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
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //                            Start screen functions                                                //
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  myGLCD.InitLCD();        // <= Dont need LANDSCAPE since is default
  myGLCD.setBrightness(0); // <= Backlight Off
  myGLCD.setWritePage(1);  // <= Select writepage 1
  myGLCD.clrScr();         // <= Clear writepage 1
  myGLCD.setDisplayPage(1); //<= Display writepage 1
  myGLCD.setBrightness(BackLight); //<= Turn on BackLight Max brightness
  myTouch.InitTouch();      //<= Dont need LANDSCAPE since is default
  myTouch.setPrecision(PREC_MEDIUM);
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //                              Start RTC                                                           //
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  Wire.begin();
#if(DS1307)
  rtc.begin(); // Start the Clock
  delay(250);
  if (! rtc.isrunning())
  {
    // If no RTC is installed, alert on serial
#if debug
    Serial.println(F("DS1307 is NOT running!"));
    Serial.println(F("Define DS1307 0!"));
#endif
  }
#else
  rtc.begin(DateTime(F(__DATE__), F(__TIME__))); // Start the Clock
#if debug
  Serial.println(F("Software RTC is running!"));
  Serial.println(F("Send sync message eg T1420722534 (Can type in serial monitor)"));
#endif
#endif
  setSyncProvider(syncProvider);// reference our syncProvider function instead of RTC_DS1307::get();
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Turns OFF all relays until called in code (NO MISTAKE, HIGH IS OFF, LOW IS ON)
  digitalWrite(LIGHTS1, HIGH);
  digitalWrite(LIGHTS2, HIGH);
  digitalWrite(TEMP1, HIGH);
  digitalWrite(FOGGER1, HIGH);
  digitalWrite(MISTING1, HIGH);
  digitalWrite(FAN1, HIGH);
  // Initialise the Arduino data pins for OUTPUT
  pinMode(LIGHTS1, OUTPUT);
  pinMode(LIGHTS2, OUTPUT);
  pinMode(TEMP1, OUTPUT);
  pinMode(FOGGER1, OUTPUT);
  pinMode(MISTING1, OUTPUT);
  pinMode(FAN1, OUTPUT);
  delay(250); //Check that all relays are inactive at Reset


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
  myFiles.loadcpld(0, 0, 800, 480, "456Sched.raw", 4, 3); //Fan/Fog to save video pages...
  myFiles.loadcpld(0, 0, 800, 480, "5MistG.raw", 7, 3);   //New Mist...

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
  sensors.setWaitForConversion(true);  // makes it wait for first reading
  tempRead(temp1, temp2, temp3);
  sensors.setWaitForConversion(false);  // makes it async (no more waiting)
  humRead(hum1);
  //relayOff(byte flag)
  //relayOn(byte flag)
  //#define pwrFan1      0   // binary  1                                                               //
  //#define pwrMisting1  1   // binary  2                                                               //
  //#define pwrFogger1   2   // binary  4                                                               //
  //#define pwrTemp1     3   // binary  8                                                               //
  //#define pwrLight2    4   // binary 16                                                               //
  //#define pwrLight1    5   // binary 32                                                               //

  Serial.print("Relays ");
  Serial.println(~Relay, HEX);
  relayOn(pwrFogger1);

  Serial.print("Relays ");
  Serial.println(~Relay, HEX );
  relayOn(pwrLight2);

  Serial.print("Relays ");
  Serial.println(~Relay, HEX);
  relayOn(pwrFan1);

  Serial.print("Relays ");
  Serial.println(~Relay, HEX);
  relayOff(pwrLight2);

  Serial.print("Relays ");
  Serial.println(~Relay, HEX);
  relayOn(pwrLight1);

  Serial.print("Relays ");
  Serial.println(~Relay, HEX);
  relayOff(pwrFogger1);

  Serial.print("Relays ");
  Serial.println(~Relay, HEX);
  relayOn(pwrLight2);

  Serial.print("Relays ");
  Serial.println(~Relay, HEX);
  relayOff(pwrFan1);

  Serial.print("Relays ");
  Serial.println(~Relay, HEX);
  //relayOff(pwrLight2)
  //
  //  Serial.print("Relays ");
  //  Serial.println(Relay);
  //relayOn(pwrLight1)
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
      processMyTouch();
    }
  }
  myCopyTime(&lastLoopRTC);
  unsigned long rightNow = now();  // Get the time in seconds (since 1970)
  if ((rightNow - humidTime) >= 2) {
    float tmphum;
    humRead(tmphum);
    if (hum1 != tmphum) {
      hum1 = tmphum;
#if debug
      Serial.print(F("Internal humidity update: "));
      Serial.println(hum1);
#endif
      if (dispScreen == 1)
      {
        char hum[6];
        dtostrf(hum1, 4, 1, hum);
        myGLCD.setColor(255, 77, 0);
        myGLCD.setFont(Ubuntubold);
        myGLCD.print(hum, 425, 405);
#if debug
        Serial.print(F("Screen (Home) humidity update: "));
        Serial.println(hum1);
#endif
      }
    }
  }
  if ((rightNow - tempTime) >= 1) {
    myGLCD.setColor(255, 77, 0);

    if (dispScreen == 1)
    {
      myGLCD.setFont(Ubuntubold);
    }
    if (dispScreen == 3)
    {
      myGLCD.setFont(Ubuntubold);
    }
    char sensor[6] = "     ";
    float tmptemp1, tmptemp2, tmptemp3;
    tempRead(tmptemp1, tmptemp2, tmptemp3);
    if (temp1 != tmptemp1)
    {
      temp1 = tmptemp1;
#if debug
      Serial.print(F("Internal temp1 update "));
      Serial.println(temp1);
#endif
      if (dispScreen == 1)
      {
        dtostrf(temp1, 4, 1, sensor);
        myGLCD.print(sensor, 411, 316);
#if debug
        Serial.print(F("Screen (Home) temp1 update "));
        Serial.println(temp1);
#endif
      }
      if (dispScreen == 3)
      {
        dtostrf(temp1, 4, 1, sensor);
        myGLCD.print(sensor, 47, 325);
#if debug
        Serial.print(F("Screen (Temp) temp1 update "));
        Serial.println(temp1);
#endif
      }
    }
    if (temp2 != tmptemp2)
    {
      temp2 = tmptemp2;
#if debug
      Serial.print(F("Internal temp2 update "));
      Serial.println(temp2);
#endif
      if (dispScreen == 1)
      {
        dtostrf(temp2, 4, 1, sensor);
        myGLCD.print(sensor, 537, 316);
#if debug
        Serial.print(F("Screen (Home) temp2 update "));
        Serial.println(temp2);
#endif
      }
      if (dispScreen == 3)
      {
        dtostrf(temp2, 4, 1, sensor);
        myGLCD.print(sensor, 197, 325);
#if debug
        Serial.print(F("Screen (Temp) temp2 update "));
        Serial.println(temp2);
#endif
      }
    }
    if (temp3 != tmptemp3) {
      temp3 = tmptemp3;
#if debug
      Serial.print(F("Internal temp3 update"));
      Serial.println(temp3);
#endif
      if (dispScreen == 1)
      {
        dtostrf(temp3, 4, 1, sensor);
        myGLCD.print(sensor, 668, 316);
#if debug
        Serial.print(F("Screen (Home) temp3 update "));
        Serial.println(temp3);
#endif
      }
      if (dispScreen == 3)
      {
        dtostrf(temp3, 4, 1, sensor);
        myGLCD.print(sensor, 353, 325);
#if debug
        Serial.print(F("Screen (Temp) temp3 update "));
        Serial.println(temp3);
#endif
      }
    }
  }
  procRelays(Relay);
}
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// PAGE SETUP ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void screenHome()  // draw main home screen showing overview info
{
  DoScreen(1, 1);
#if debug
  Serial.println(F("Home Screen"));
#endif
  updateTime = false;
  if (tmpTouchRepeatDelay != TouchRepeatDelay) TouchRepeatDelay = tmpTouchRepeatDelay;
  // Draw Temp to screen
  float tmptemp1, tmptemp2, tmptemp3, tmphum;
  if (temp1 + temp2 + temp3 == 0.0) {
    tempRead(temp1, temp2, temp3);
  }
  unsigned long rightNow = now();
  if ((rightNow - tempTime) > 1) {
    tempRead(tmptemp1, tmptemp2, tmptemp3);
    if (temp1 != tmptemp1) {
      temp1 = tmptemp1;
    }
    if (temp2 != tmptemp2) {
      temp2 = tmptemp2;
    }
    if (temp3 != tmptemp3) {
      temp3 = tmptemp3;
    }
  }
  myGLCD.setColor(255, 77, 0);
  myGLCD.setFont(Ubuntubold);
  char sensor[6] = "     ";
  dtostrf(temp1, 4, 1, sensor);
  myGLCD.print(sensor, 411, 316);
  if (temp2 != 0.0)
  {
    dtostrf(temp2, 4, 1, sensor);
    myGLCD.print(sensor, 537, 316);
  }
  if (temp3 != 0.0)
  {
    dtostrf(temp3, 4, 1, sensor);
    myGLCD.print(sensor, 668, 316);
  }
  // Draw Humidity to screen
  if (hum1 == 0.0) {
    humRead(hum1);
  }
  if ((rightNow - humidTime) >= 2)
  {
    humRead(tmphum);
    hum1 = tmphum;
  }
  char hum[6];
  dtostrf(hum1, 4, 1, hum);
  myGLCD.setColor(255, 77, 0);
  myGLCD.setFont(Ubuntubold);
  myGLCD.print(hum, 425, 405);
}
void screenLights()  // Lights screen
{
  DoScreen(2, 2);
#if debug
  Serial.println(F("Lights Screen"));
#endif
  tmpTouchRepeatDelay = TouchRepeatDelay;
  TouchRepeatDelay = 150;
  updateTime = false;
}
void screenTemp()  // Temp Screen
{
  DoScreen(3, 3);
#if debug
  Serial.println(F("Temp Screen"));
#endif
  // Draw Temp to screen
  float tmptemp1, tmptemp2, tmptemp3, tmphum;
  unsigned long rightNow = now();
  if ((rightNow - tempTime) > 1) {
    tempRead(tmptemp1, tmptemp2, tmptemp3);
    if (temp1 != tmptemp1) {
      temp1 = tmptemp1;
    }
    if (temp2 != tmptemp2) {
      temp2 = tmptemp2;
    }
    if (temp3 != tmptemp3) {
      temp3 = tmptemp3;
    }
  }
  myGLCD.setColor(255, 77, 0);
  myGLCD.setFont(Ubuntubold);
  char sensor[6] = "     ";
  dtostrf(temp1, 4, 1, sensor);
  myGLCD.print(sensor, 47, 325);
  if (temp2 != 0.0)
  {
    dtostrf(temp2, 4, 1, sensor);
    myGLCD.print(sensor, 197, 325);
  }
  if (temp3 != 0.0)
  {
    dtostrf(temp3, 4, 1, sensor);
    myGLCD.print(sensor, 353, 325);
  }
  tmpTouchRepeatDelay = TouchRepeatDelay;
  TouchRepeatDelay = 100;
  myGLCD.printNumF(heater, 1, 510, 296, '.', 4);
  myGLCD.printNumF(fan, 1, 628, 296, '.', 5);
  updateTime = false;
}
void screenFogger()  // Fogging screen
{
  myFiles.loadcpld(21, 6, 336, 74, "FogTitle.raw", 4, 6);
  DoScreen(4, 4);
#if debug
  Serial.println(F("Fogger Screen"));
#endif
  updateTime = false;
}
void screenMist()  // Misting Screen
{
  DoScreen(7, 5);
#if debug
  Serial.println(F("Misting Screen"));
#endif
  updateTime = false;
}
void screenFan()  // Fan screen
{
  myFiles.loadcpld(21, 6, 336, 74, "FanTitle.raw", 4, 6);
  DoScreen(4, 6);
#if debug
  Serial.println(F("Fan Screen"));
#endif
  updateTime = false;
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
#else
  myGLCD.setWritePage(0);
  myGLCD.clrScr();
  myGLCD.setDisplayPage(0);
  myFiles.load(0, 0, 800, 480, "8Screen.raw", 3);
  dispScreen = 8;
#endif
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
  static volatile byte tempM1e, tempM2e;


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
          tempL1OnHr = Light.On1Hr;
          tempL1OnMin = Light.On1Min;
          tempL1OffHr = Light.Dur1Min;
          tempL1OffMin = Light.Dur1Sec;
          tempL2OnHr = Light.On2Hr;
          tempL2OnMin = Light.On2Min;
          tempL2OffHr = Light.Dur2Min;
          tempL2OffMin = Light.Dur2Sec;
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
          tempM1e = Mist.Enable;
          tempM2e = Mist2.Enable;
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
        if (tempL1OnHr != Light.On1Hr) {
          EEPROM.write(13, tempL1OnHr);
          Light.On1Hr = tempL1OnHr;
#if debug
          Serial.println(F("Light.On1Hr -> EEPROM"));
#endif
        }
        if (tempL1OnMin != Light.On1Min) {
          EEPROM.write(14, tempL1OnMin);
          Light.On1Min = tempL1OnMin;
#if debug
          Serial.println(F("Light.On1Min -> EEPROM"));
#endif
        }
        if (tempL1OffHr != Light.Dur1Min) {
          EEPROM.write(15, tempL1OffHr);
          Light.Dur1Min = tempL1OffHr;
#if debug
          Serial.println(F("Light.Dur1Min -> EEPROM"));
#endif
        }
        if (tempL1OffMin != Light.Dur1Sec) {
          EEPROM.write(16, tempL1OffMin);
          Light.Dur1Sec = tempL1OffMin;
#if debug
          Serial.println(F("Light.Dur1Sec -> EEPROM"));
#endif
        }
        if (tempL2OnHr != Light.On2Hr) {
          EEPROM.write(17, tempL2OnHr);
          Light.On2Hr = tempL2OnHr;
#if debug
          Serial.println(F("Light.On2Hr -> EEPROM"));
#endif
        }
        if (tempL2OnMin != Light.On2Min) {
          EEPROM.write(18, tempL2OnMin);
          Light.On2Min = tempL2OnMin;
#if debug
          Serial.println(F("Light.On2Min -> EEPROM"));
#endif
        }
        if (tempL2OffHr != Light.Dur2Min) {
          EEPROM.write(19, tempL2OffHr);
          Light.Dur2Min = tempL2OffHr;
#if debug
          Serial.println(F("Light.Dur2Min -> EEPROM"));
#endif
        }
        if (tempL2OffMin != Light.Dur2Sec) {
          EEPROM.write(20, tempL2OffMin);
          Light.Dur2Sec = tempL2OffMin;
#if debug
          Serial.println(F("Light.Dur2Sec -> EEPROM"));
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
      if ((x >= 183) && (x < 236))
      {
        if ((y >= 209) && (y < 259))
        {
          switch (mistScreen) {
            case 1:
              if ((tempM1e & 0xF0) == 0x00) // Set Enable Mist 1
              {
                myFiles.load(187, 216, 45, 36, "Frog.raw", 16, 0);
                tempM1e = ((tempM1e & 0x0F) + 0xF0);
              }
              else
              {
                myGLCD.setColor(0, 0, 0);
                myGLCD.fillRect(187, 216, 231, 251);
                tempM1e = (tempM1e & 0x0F);
              }
              break;
            case 2:
              if ((tempM1e & 0x0F) == 0x00) // Set Enable Mist 2
              {
                myFiles.load(187, 216, 45, 36, "Frog.raw", 16, 0);
                tempM1e = ((tempM1e & 0xF0) + 0x0F);
              }
              else
              {
                myGLCD.setColor(0, 0, 0);
                myGLCD.fillRect(187, 216, 231, 251);
                tempM1e = (tempM1e & 0xF0);
              }
              break;
            case 3:
              if ((tempM2e & 0xF0) == 0x00) // Set Enable Mist 3
              {
                myFiles.load(187, 216, 45, 36, "Frog.raw", 16, 0);
                tempM2e = ((tempM2e & 0x0F) + 0xF0);
              }
              else
              {
                myGLCD.setColor(0, 0, 0);
                myGLCD.fillRect(187, 216, 231, 251);
                tempM2e = (tempM2e & 0x0F);
              }
              break;
            case 4:
              if ((tempM2e & 0x0F) == 0x00)  // Set Enable Mist 4
              {
                myFiles.load(187, 216, 45, 36, "Frog.raw", 16, 0);
                tempM2e = ((tempM2e & 0xF0) + 0x0F);
              }
              else
              {
                myGLCD.setColor(0, 0, 0);
                myGLCD.fillRect(187, 216, 231, 251);
                tempM2e = (tempM2e & 0xF0);
              }
              break;
          }
        }
      }


      if ((x >= 51) && (x < 129))
      {
        if ((y >= 106) && (y < 184))
        {
          myFiles.load(51, 106, 78, 78, "blues.raw", 16, 0);
          if ((tempM1e & 0xF0) != 0xF0) // is blue set?
          {
            myGLCD.setColor(0, 0, 0);
            myGLCD.fillRect(187, 216, 231, 251);
          }
          else
          {
            myFiles.load(187, 216, 45, 36, "Frog.raw", 16, 0);
          }

          switch (mistScreen)
          {
            case 2:
              myFiles.load(51, 193, 78, 78, "yellow.raw", 16, 0);
              break;
            case 3:
              myFiles.load(51, 281, 78, 78, "green.raw", 16, 0);
              break;
            case 4:
              myFiles.load(51, 281, 78, 78, "grey.raw", 16, 0);
              break;
          }
          mistScreen = 1;
        }
        if ((y >= 193) && (y < 271))
        {
          myFiles.load(51, 193, 78, 78, "yellows.raw", 16, 0);
          if ((tempM1e & 0x0F) != 0x0F) // is yellow set?
          {
            myGLCD.setColor(0, 0, 0);
            myGLCD.fillRect(187, 216, 231, 251);
          }
          else
          {
            myFiles.load(187, 216, 45, 36, "Frog.raw", 16, 0);
          }

          switch (mistScreen)
          {
            case 1:
              myFiles.load(51, 106, 78, 78, "blue.raw", 16, 0);
              break;
            case 3:
              myFiles.load(51, 281, 78, 78, "green.raw", 16, 0);
              break;
            case 4:
              myFiles.load(51, 368, 78, 78, "grey.raw", 16, 0);
              break;
          }
          mistScreen = 2;
        }
        if ((y >= 281) && (y < 359))
        {
          myFiles.load(51, 281, 78, 78, "greens.raw", 16, 0);
          if ((tempM2e & 0xF0) != 0xF0) // is green set?
          {
            myGLCD.setColor(0, 0, 0);
            myGLCD.fillRect(187, 216, 231, 251);
          }
          else
          {
            myFiles.load(187, 216, 45, 36, "Frog.raw", 16, 0);
          }

          switch (mistScreen)
          {
            case 1:
              myFiles.load(51, 106, 78, 78, "blue.raw", 16, 0);
              break;
            case 2:
              myFiles.load(51, 193, 78, 78, "yellow.raw", 16, 0);
              break;
            case 4:
              myFiles.load(51, 368, 78, 78, "grey.raw", 16, 0);
              break;
          }
          mistScreen = 3;
        }
        if ((y >= 368) && (y < 446))
        {
          myFiles.load(51, 368, 78, 78, "greys.raw", 16, 0);
          if ((tempM2e & 0x0F) != 0x0F) // is grey set?
          {
            myGLCD.setColor(0, 0, 0);
            myGLCD.fillRect(187, 216, 231, 251);
          }
          else
          {
            myFiles.load(187, 216, 45, 36, "Frog.raw", 16, 0);
          }

          switch (mistScreen)
          {
            case 1:
              myFiles.load(51, 106, 78, 78, "blue.raw", 16, 0);
              break;
            case 2:
              myFiles.load(51, 193, 78, 78, "yellow.raw", 16, 0);
              break;
            case 3:
              myFiles.load(51, 281, 78, 78, "green.raw", 16, 0);
              break;
          }
          mistScreen = 4;
        }
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
time_t syncProvider()
{
  //this does the same thing as RTC_DS1307::get()
#if(DS1307)
  return rtc.now().unixtime();
#endif
}
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
  if ((day(timeNow) != prevRTC.Day) || (month(timeNow) != prevRTC.Month) ||
      (year(timeNow) != prevRTC.Year || !updateTime))
  {
    prevRTC.Day = day(timeNow);
    prevRTC.Month = month(timeNow);
    printDate(timeNow, 400, 2);
  }
  updateTime = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
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
  if (minute(timeNow) >= 0 && minute(timeNow) < 10) {
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
  myGLCD.print(tmpTime, posx, posy);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////
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
      Serial.println(F("Time Set"));
#endif
    }
  }
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////
void DoScreen(byte page, byte screen) {
  myGLCD.setDisplayPage(page);
  myGLCD.setWritePage(page);
  dispScreen = screen;
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////    DS18b20 TEMP SENSORS   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void tempRead(float &temp1a, float &temp2a, float &temp3a)
{
  byte numSensors = sensors.getDeviceCount();
  temp1a = 0.0, temp2a = 0.0, temp3a = 0.0;
  if (numSensors >= 1)
  {
    temp1a = sensors.getTempFByIndex(0);
  }
  if (numSensors >= 2)
  {
    temp2a = sensors.getTempFByIndex(1);
  }
  if (numSensors >= 3)
  {
    temp3a = sensors.getTempFByIndex(2);
  }
  sensors.requestTemperatures();
  tempTime = now();
}
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////    DHT22 HUMIDITY SENSORS   ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void humRead(float &hum1a)
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int chk = DHT.read22(DHTPIN);
  if (chk == DHTLIB_OK) {
    hum1a = DHT.humidity;
    humidTime = now();
  }
}
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    ALARM HANDLERS/   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    FIRST RUN SETUP   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void firstRunSetup()
{
  // I'm going to save 47 into EEPROM bank 2000 as a check
  if (EEPROM.read(2000) != 47)
  {
    EEPROM.write(2000, 47); // write to EEPROM so this never runs again
    // default screen settings
    EEPROM.write(1, 1); //         <- HomeDelay
    EEPROM.write(2, 16);//         <- BackLight
    EEPROM.write(3, 1); //         <- AutoDim
    EEPROM.write(4, 10);//         <- TouchDelay
    EEPROM.writeFloat(5, 42.9);//  <- Heater
    EEPROM.writeFloat(9, 102.9);// <- Fan
    EEPROM.write(13, 8); //        <- Light.On1Hr
    EEPROM.write(14, 15); //       <- Light.On1Min
    EEPROM.write(15, 10); //       <- Light.Dur1Min
    EEPROM.write(16, 30); //       <- Light.Dur1Sec
    EEPROM.write(17, 16); //       <- Light.On2Hr
    EEPROM.write(18, 30); //       <- Light.On2Min
    EEPROM.write(19, 19); //       <- Light.Dur2Min
    EEPROM.write(20, 45); //       <- Light.Dur2Sec
    EEPROM.write(21, 0x00);//      <- Fog.Enable ( F0 = 1 enable, 2 disable, FF = 1 & 2 enable )
    EEPROM.write(22, 8); //        <- Fog.On1Hr
    EEPROM.write(23, 15); //       <- Fog.On1Min
    EEPROM.write(24, 10); //       <- Fog.Dur1Min
    EEPROM.write(25, 30); //       <- Fog.Dur1Sec
    EEPROM.write(26, 16); //       <- Fog.On2Hr
    EEPROM.write(27, 30); //       <- Fog.On2Min
    EEPROM.write(28, 19); //       <- Fog.Dur2Min
    EEPROM.write(29, 45); //       <- Fog.Dur2Sec
    EEPROM.write(30, 127); //      <- Fog.OnDay
    EEPROM.write(31, 0x00);//      <- Mist.Enable ( F0 = 1 enable, 2 disable, FF = 1 & 2 enable )
    EEPROM.write(32, 8); //        <- Mist.On1Hr
    EEPROM.write(33, 15); //       <- Mist.On1Min
    EEPROM.write(34, 10); //       <- Mist.Dur1Min
    EEPROM.write(35, 30); //       <- Mist.Dur1Sec
    EEPROM.write(36, 16); //       <- Mist.On2Hr
    EEPROM.write(37, 30); //       <- Mist.On2Min
    EEPROM.write(38, 19); //       <- Mist.Dur2Min
    EEPROM.write(39, 45); //       <- Mist.Dur2Sec
    EEPROM.write(40, 127); //      <- Mist.OnDay
    EEPROM.write(41, 0x00);//      <- Mist2.Enable ( F0 = 3 enable, 4 disable, FF = 3 & 4 enable )
    EEPROM.write(42, 8); //        <- Mist2.On1Hr
    EEPROM.write(43, 15); //       <- Mist2.On1Min
    EEPROM.write(44, 10); //       <- Mist2.Dur1Min
    EEPROM.write(45, 30); //       <- Mist2.Dur1Sec
    EEPROM.write(46, 16); //       <- Mist2.On2Hr
    EEPROM.write(47, 30); //       <- Mist2.On2Min
    EEPROM.write(48, 19); //       <- Mist2.Dur2Min
    EEPROM.write(49, 45); //       <- Mist2.Dur2Sec
    EEPROM.write(50, 127); //      <- Mist2.OnDay  ( 0x7F = All week; 0/Sat/Fri/Thu/Wed/Tue/Mon/Sun )
    EEPROM.write(51, 0x00);//      <- Fan.Enable ( 0xF0 = 1 enable, 2 disable, 0xFF = 1 & 2 enable )
    EEPROM.write(52, 8); //        <- Fan.On1Hr
    EEPROM.write(53, 15); //       <- Fan.On1Min
    EEPROM.write(54, 10); //       <- Fan.Dur1Min
    EEPROM.write(55, 30); //       <- Fan.Dur1Sec
    EEPROM.write(56, 16); //       <- Fan.On2Hr
    EEPROM.write(57, 30); //       <- Fan.On2Min
    EEPROM.write(58, 19); //       <- Fan.Dur2Min
    EEPROM.write(59, 45); //       <- Fan.Dur2Sec
    EEPROM.write(60, 127); //      <- Fan.OnDay
  }
}
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    Relay handlers    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void relayOff(byte flag)
{
  Relay |= (1 << flag);
}
void relayOn(byte flag)
{
  Relay &= ~(1 << flag);
}

void procRelays(byte relay)
{
  char relays[][10] = {"swon.raw" , "swoff.raw" };
  if (relay != prevRelay)
  {
    if ((relay & (1 << pwrLight1)) == (1 << pwrLight1)) // Lights 1
    {
      myFiles.load(555, 418, 22, 22, relays[1], 16, 0);
    }
    else
    {
      myFiles.load(555, 418, 22, 22, relays[0], 16, 0);
    }
    if ((relay & (1 << pwrLight2)) == (1 << pwrLight2)) // Lights 2
    {
      myFiles.load(580, 418, 22, 22, relays[1], 16, 0);
    }
    else
    {
      myFiles.load(580, 418, 22, 22, relays[0], 16, 0);
    }
    if ((relay & (1 << pwrTemp1)) == (1 << pwrTemp1)) // Temp 1
    {
      myFiles.load(614, 418, 22, 22, relays[1], 16, 0);
    }
    else
    {
      myFiles.load(614, 418, 22, 22, relays[0], 16, 0);
    }
    if ((relay & (1 << pwrFogger1)) == (1 << pwrFogger1)) // Fog 1
    {
      myFiles.load(655, 418, 22, 22, relays[1], 16, 0);
    }
    else
    {
      myFiles.load(655, 418, 22, 22, relays[0], 16, 0);
    }
    if ((relay & (1 << pwrMisting1)) == (1 << pwrMisting1)) // Mist 1
    {
      myFiles.load(694, 418, 22, 22, relays[1], 16, 0);
    }
    else
    {
      myFiles.load(694, 418, 22, 22, relays[0], 16, 0);
    }
    if ((relay & (1 << pwrFan1)) == (1 << pwrFan1)) // Fan 1
    {
      myFiles.load(732, 418, 22, 22, relays[1], 16, 0);
    }
    else
    {
      myFiles.load(732, 418, 22, 22, relays[0], 16, 0);
    }
    prevRelay = relay;
  }
}

