//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                             Vivarium Management Controller w/ 7" touchscreen Interface                       //
//                    written and coded by Graham Lawrence aka ghlawrence2000 on Arduino.cc                     //
//                          everything else was sourced by David Sease, aka Bigsease30                          //
//                                       Written for Arduino Mega 2560                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               VERSION:  27/02/15 06.10GMT                                    //
//                                              Development Version Release6_Beta1                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               CODE ORDER:                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    Declare EEPROM useage                                                                                     //
//    Declare Libraries needed                                                                                  //
//    Compile time options                                                                                      //
//    Initiate Screen, Touch, Imaging and Fonts                                                                 //
//    Define Pins                                                                                               //
//    Global variables & setup                                                                                  //
//    Main loop                                                                                                 //
//    Routines to draw every screen                                                                             //
//    Routine to handle touch events                                                                            //
//    Other supporting routines                                                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       EEPROM locations (saved settings)                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SPACE //         DESCRIPTION              // DEFAULTS                                                        //
//     0 //  ID     : 57                     // 57 if previously ran                                            //
//     1 //  screen : return home            // 1 min                                                           //
//     2 //  screen : brightness if no dim   // 16 max                                                          //
//     3 //  screen : AutoDim level          // 1                                                               //
//     4 //  screen : AutoDim seconds        // 10 sec                                                          //
//     5 //  Heater : Float byte 0           //\                                                                //
//     6 //  Heater : Float byte 1           //  _ Initial value                                                //
//     7 //  Heater : Float byte 2           //    42.9F                                                        //
//     8 //  Heater : Float byte 3           ///                                                                //
//     9 //  Fan    : Float byte 0           //\                                                                //
//    10 //  Fan    : Float byte 1           //  _ Initial value                                                //
//    11 //  Fan    : Float byte 2           //    102.9F                                                       //
//    12 //  Fan    : Float byte 3           ///                                                                //
//    13 //  Lights1: On1Hr                  // 8 am  //Light 1 ON (Day Light)                                  //
//    14 //  Lights1: On1Min                 // 15 min                                                          //
//    15 //  Lights1: Off1Hr                 // 20 pm //Light 1 OFF                                             //
//    16 //  Lights1: Off1Min                // 30 min                                                          //
//    17 //  Lights2: On2Hr                  // 20 pm //Light 2 ON (Night Light)                                //
//    18 //  Lights2: On2Min                 // 30 min                                                          //
//    19 //  Lights2: Off2Hr                 // 8 am //Light 2 OFF                                              //
//    20 //  Lights2: Off2Min                // 15 min                                                          //
//    21 //  Fog    : Fog.Enable             // ( F0 = 1 enable, 2 disable, FF = 1 & 2 enable )                 //
//    22 //  Fog    : Fog.On1Hr              //                                                                 //
//    23 //  Fog    : Fog.On1Min             //                                                                 //
//    24 //  Fog    : Fog.Dur1Min            //                                                                 //
//    25 //  Fog    : Fog.Dur1Sec            //                                                                 //
//    26 //  Fog    : Fog.On2Hr              //                                                                 //
//    27 //  Fog    : Fog.On2Min             //                                                                 //
//    28 //  Fog    : Fog.Dur2Min            //                                                                 //
//    29 //  Fog    : Fog.Dur2Sec            //                                                                 //
//    30 //  Fog    : Fog.OnDay              //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x3c=Tue-Fri                   //
//    31 //  Fog    : Fog.OnDay2             //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x3c=Tue-Fri                   //
//    32 //  Mist1&2: Mist.Enable            // ( F0 = 1 enable, 2 disable, FF = 1 & 2 enable )                 //
//    33 //  Mist1  : Mist.On1Hr             //                                                                 //
//    34 //  Mist1  : Mist.On1Min            //                                                                 //
//    35 //  Mist1  : Mist.Dur1Min           //                                                                 //
//    36 //  Mist1  : Mist.Dur1Sec           //                                                                 //
//    37 //  Mist2  : Mist.On2Hr             //                                                                 //
//    38 //  Mist2  : Mist.On2Min            //                                                                 //
//    39 //  Mist2  : Mist.Dur2Min           //                                                                 //
//    40 //  Mist2  : Mist.Dur2Sec           //                                                                 //
//    41 //  Mist2  : Mist.OnDay             //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x70 =Thu-Sat                  //
//    42 //  Mist2  : Mist.OnDay             //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x70 =Thu-Sat                  //
//    43 //  Mist3&4: Mist2.Enable           // ( F0 = 3 enable, 4 disable, FF = 3 & 4 enable )                 //
//    44 //  Mist3  : Mist2.On3Hr            //                                                                 //
//    45 //  Mist3  : Mist2.On3Min           //                                                                 //
//    46 //  Mist3  : Mist2.Dur3Min          //                                                                 //
//    47 //  Mist3  : Mist2.Dur3Sec          //                                                                 //
//    48 //  Mist4  : Mist2.On3Hr            //                                                                 //
//    49 //  Mist4  : Mist2.On3Min           //                                                                 //
//    50 //  Mist4  : Mist2.Dur3Min          //                                                                 //
//    51 //  Mist4  : Mist2.Dur3Sec          //                                                                 //
//    52 //  Mist4  : Mist2.OnDay            //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x0F =Sun-Wed                  //
//    53 //  Mist4  : Mist2.OnDay2           //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x0F =Sun-Wed                  //
//    54 //  Fan    : Fan.Enable             // ( F0 = 1 enable, 2 disable, FF = 1 & 2 enable )                 //
//    55 //  Fan    : Fan.On1Hr              //                                                                 //
//    56 //  Fan    : Fan.On1Min             //                                                                 //
//    57 //  Fan    : Fan.Dur1Min            //                                                                 //
//    58 //  Fan    : Fan.Dur1Sec            //                                                                 //
//    59 //  Fan    : Fan.On2Hr              //                                                                 //
//    60 //  Fan    : Fan.On2Min             //                                                                 //
//    61 //  Fan    : Fan.Dur2Min            //                                                                 //
//    62 //  Fan    : Fan.Dur2Sec            //                                                                 //
//    63 //  Fan    : Fan.OnDay              //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x0F =Sun-Wed                  //
//    64 //  Fan    : Fan.OnDay2             //  0/Sat/Fri/Thu/Wed/Tue/Mon/Sun   0x0F =Sun-Wed                  //
// 65+66 //  Hi/Lo  : temp1Lo                //                                                                 //
// 67+68 //  Hi/Lo  : temp1Hi                //                                                                 //
// 69+70 //  Hi/Lo  : temp2Lo                //                                                                 //
// 71+72 //  Hi/Lo  : temp2Hi                //                                                                 //
// 73+74 //  Hi/Lo  : temp3Lo                //                                                                 //
// 75+76 //  Hi/Lo  : temp3Hi                //                                                                 //
// 77+78 //  Hi/Lo  : humi1Lo                //                                                                 //
// 79+80 //  Hi/Lo  : humi1Hi                //                                                                 //
//    81 //  C / F  : tempUnits              //           0xF0 = DegF : 0x0F = DegC                             //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               Compile time options                                           //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DS1307   1 // <====== 1 for DS1307, 0 for Software                                                      //
#define fastest  1 // <====== 1 = MAX speed all screens in own page / 0 = 2 Screens share a page...             //
#define debug    1 // <====== 1 = Serial output enabled                                                         //
#define Auxdisp  1 // <====== 1 = I2C Columns*Rows LCD display (57 bytes of Dynamic used, 1126 bytes of flash)  //
#if Auxdisp //                                                                                                  //
#define lcdR     2 // <====== 2 = Aux LCD Rows                                                                  //
#define lcdC    16 // <===== 16 = Aux LCD Columns                                                               //
#endif //                                                                                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              Decare Libraries                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Wire.h>                  // needed by tons of stuff  <== Davids excellent description!!               //
#if Auxdisp //                                                                                                  //
#include <LiquidCrystal_I2C.h>     // Auxiliary Display                                                         //
#endif //                                                                                                       //
#include <EEPROM.h>                // used to store and retrieve settings from memory                           //
#include <UTFT.h>                  // used to interface with the TFT display                                    //
#include <UTouch.h>                // used to interface with the touch controller on the TFT display            //
#include <SdFat.h>                 // used to access the SD card                                                //
#include <UTFT_SdRaw.h>            // used to read .raw images from the SD card                                 //
#include <RTClib.h>                // RTC library (software or hardware capable)                                //
#include <Time.h>                  // allows conversion to UNIX time for easier date/time math                  //
#include <TimeAlarms.h>            // used to power schedules                                                   //
#include <OneWire.h>               // library to communicate with the DallasTemperature sensor                  // 
#include <DallasTemperature.h>     // library for the Temp sensor itself                                        //
#include <dht.h>                   // Rob Tillaart's improved humidity library                                  //
#include "includes.h"              // Workaround for quirk in Arduino IDE                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Initiate Screen, Touch, Imaging and Fonts                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UTFT myGLCD(CPLD, 38, 39, 40, 41); // start up an instance of the TFT screen                                    //
UTouch  myTouch( 6, 5, 4, 3, 2);        // start up an instance of for touch                                    //
UTFT_SdRaw myFiles(&myGLCD);            // start up an instance to read images from SD                          //
#if Auxdisp                                                                                                     //
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //  Auxiliary lcdC*lcdR  (16*2)                 //
#endif                                                                                                          //
SdFat sd;                               // start up as instance of SD                                           //
extern uint8_t SmallFont[];                                                                                     //
extern uint8_t Sinclair_S[];                                                                                    //
extern uint8_t arial_bold[];                                                                                    //
extern uint8_t Ubuntubold[];                                                                                    //
extern unsigned int frog[];                                                                                     //
extern unsigned int smfrog[];                                                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 Define pins                                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Declare Pins and binary bits for Relays (analog to save digital pins)                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FAN1        A0                                                                                          //
#define MISTING1    A1                                                                                          //
#define FOGGER1     A2                                                                                          //
#define TEMP1       A3                                                                                          //
#define LIGHTS1     A4                                                                                          //    
#define LIGHTS2     A5                                                                                          //                     
#define pwrFan1      0   // binary  1                                                                           //  
#define pwrMisting1  1   // binary  2                                                                           //
#define pwrFogger1   2   // binary  4                                                                           //
#define pwrTemp1     3   // binary  8                                                                           //
#define pwrLight1    4   // binary 16                                                                           //
#define pwrLight2    5   // binary 32                                                                           //
byte Relay = 0x3F, prevRelay = 0x40;                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               Pins for sensors                                               //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ONE_WIRE_BUS     8                                                                                      //
#define DHTPIN           9                                                                                      //
#define SD_CHIP_SELECT  53                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Humidity and Temperature                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup our humidity sensor                                                                                    //
dht DHT;                                                                                                        //
// Setup oneWire instance to communicate with any OneWire devices                                               //
OneWire oneWire(ONE_WIRE_BUS);                                                                                  //
// Pass our oneWire reference to Dallas Temperature.                                                            //
DallasTemperature sensors(&oneWire);                                                                            //
//DeviceAddress tempLeft, tempMiddle, tempRight;                                                                //
//DeviceAddress tempLeft = { 0x28, 0x24, 0x18, 0x5D, 0x06, 0x00, 0x00, 0xA2 };                                  //
//DeviceAddress tempMiddle = { 0x28, 0x39, 0x0B, 0x5C, 0x06, 0x00, 0x00, 0x0E };                                //
//DeviceAddress tempRight = { 0x28, 0xED, 0x16, 0x5C, 0x06, 0x00, 0x00, 0x86 };                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   Time                                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if DS1307 ==1                                                                                                  //
RTC_DS1307 rtc;                                                                                                 //
#else                                                                                                           //
RTC_Millis rtc;                                                                                                 //
#define TIME_HEADER  "T"   // Header tag for serial time sync message                                           //
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message                                 //
#endif                                                                                                          //
boolean updateTime = false; // keep track of when to udpate the clock                                           //
// days and month character strings for displaing at the top of the screen                                      //
const char *Day[] = { "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };                                    //                                                                                                  //
const char *Month[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}; //                                                                                                  //
tmElements_t tmpRTC, prevRTC;                                                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Various timings to keep track of                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned long daysec = 86400UL;                                                                           //
unsigned long prevMillisTouch = 0; // track time between touches                                                //
unsigned long prevMillis5sec = 0; // track 5 seconds for refreshing clock and temp                              //
unsigned long millisDim = 0; // used for brightness adjustment                                                  //
unsigned long millisHome = 0; // used for returning home after configured time                                  //
unsigned long TouchRepeatDelay = 500; // millis                                                                 //
unsigned long tmpTouchRepeatDelay = 500; //millis                                                               //
unsigned long humidTime = 0; // 2 seconds minimum reading                                                       //
unsigned long tempTime;      // 1 second  minimum reading                                                       //                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             Various screen settings                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte HomeDelay;  // (Return Home) Minutes 0-59                                                                  //
byte BackLight;  // (Bright) (not AutoDim) startup brightness to 100% 1-16                                      //
byte AutoDim;    // (AutoDim Level) maximum brightness when dimmed 1-5                                          //
byte TouchDelay; // (AutoDim Seconds) 0-59                                                                      //
byte dispScreen = 1;  // Currently displayed screen, screens shown below                                        //
// 1-home, 2-lights, , 3-temp, 4-fogger, 5-mister, 6-fan, 7-clock, 8-screen                                     //
byte mistScreen = 1;                                                                                            //
byte fanScreen = 1;                                                                                             //
byte fogScreen = 1;                                                                                             //
boolean BackLightTouch = true; // initial setting to allow the screen to stay on after boot                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     Scheduling and Alarm related variables                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SCHEDULE Light, Fog, Mist, Mist2, Fan;                                                                          //
SCHEDULE tempM1, tempM2, tempFan1, tempFog1;                                                                    //
float heater;    // = 35.9;                                                                                     //
float fan;       // = 87.9;                                                                                     //
float temp1 = 0.0, temp2 = 0.0, temp3 = 0.0, hum1 = 0.0;                                                        //
AlarmID_t alarms[116];                                                                                          //
byte heatOn = 1, fanOn = 0, tempUnits, temptempUnits;                                                           //
float prevlowTemp = 0.00;                                                                                       //
float prevhiTemp = 0.00;                                                                                        //
byte alarmset = 255, hourcount = 0, zz = 0;                                                                     //
word temp1Lo, temp1Hi, temp2Lo, temp2Hi, temp3Lo, temp3Hi, hum1Lo, hum1Hi ;                                     //                                                                                //
#define Sun          0   // binary  1                                                                           //  
#define Mon          1   // binary  2                                                                           //
#define Tue          2   // binary  4                                                                           //
#define Wed          3   // binary  8                                                                           //
#define Thu          4   // binary 16                                                                           //
#define Fri          5   // binary 32                                                                           //
#define Sat          6   // binary 64                                                                           //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  SETUP CODE                                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()                                                                                                    //
{ //                                                                                                            //
#if debug                                                                                                       //
  // start serial port                                                                                          //
  Serial.begin(19200);                                                                                          //
#endif                                                                                                          //
  // Set default values for the first ever run                                                                  //
  firstRunSetup();                                                                                              //
  // get screen settings from eeprom                                                                            //
  HomeDelay     = EEPROM.read(1);                                                                               //
  BackLight     = EEPROM.read(2);                                                                               //
  AutoDim       = EEPROM.read(3);                                                                               //
  TouchDelay    = EEPROM.read(4);                                                                               //
  heater        = EEPROM.readFloat(5);                                                                          //
  fan           = EEPROM.readFloat(9);                                                                          //
  Light.On1Hr   = EEPROM.read(13);                                                                              //
  Light.On1Min  = EEPROM.read(14);                                                                              //
  Light.Dur1Min = EEPROM.read(15);                                                                              //
  Light.Dur1Sec = EEPROM.read(16);                                                                              //
  Light.On2Hr   = EEPROM.read(17);                                                                              //
  Light.On2Min  = EEPROM.read(18);                                                                              //
  Light.Dur2Min = EEPROM.read(19);                                                                              //
  Light.Dur2Sec = EEPROM.read(20);                                                                              //
  Fog.Enable    = EEPROM.read(21);                                                                              //
  Fog.On1Hr     = EEPROM.read(22);                                                                              //
  Fog.On1Min    = EEPROM.read(23);                                                                              //
  Fog.Dur1Min   = EEPROM.read(24);                                                                              //
  Fog.Dur1Sec   = EEPROM.read(25);                                                                              //
  Fog.On2Hr     = EEPROM.read(26);                                                                              //
  Fog.On2Min    = EEPROM.read(27);                                                                              //
  Fog.Dur2Min   = EEPROM.read(28);                                                                              //
  Fog.Dur2Sec   = EEPROM.read(29);                                                                              //
  Fog.OnDay     = EEPROM.read(30);                                                                              //
  Fog.OnDay2    = EEPROM.read(31);                                                                              //
  Mist.Enable   = EEPROM.read(32);                                                                              //
  Mist.On1Hr    = EEPROM.read(33);                                                                              //
  Mist.On1Min   = EEPROM.read(34);                                                                              //
  Mist.Dur1Min  = EEPROM.read(35);                                                                              //
  Mist.Dur1Sec  = EEPROM.read(36);                                                                              //
  Mist.On2Hr    = EEPROM.read(37);                                                                              //
  Mist.On2Min   = EEPROM.read(38);                                                                              //
  Mist.Dur2Min  = EEPROM.read(39);                                                                              //
  Mist.Dur2Sec  = EEPROM.read(40);                                                                              //
  Mist.OnDay    = EEPROM.read(41);                                                                              //
  Mist.OnDay2   = EEPROM.read(42);                                                                              //
  Mist2.Enable  = EEPROM.read(43);                                                                              //
  Mist2.On1Hr   = EEPROM.read(44);                                                                              //
  Mist2.On1Min  = EEPROM.read(45);                                                                              //
  Mist2.Dur1Min = EEPROM.read(46);                                                                              //
  Mist2.Dur1Sec = EEPROM.read(47);                                                                              //
  Mist2.On2Hr   = EEPROM.read(48);                                                                              //
  Mist2.On2Min  = EEPROM.read(49);                                                                              //
  Mist2.Dur2Min = EEPROM.read(50);                                                                              //
  Mist2.Dur2Sec = EEPROM.read(51);                                                                              //
  Mist2.OnDay   = EEPROM.read(52);                                                                              //
  Mist2.OnDay2  = EEPROM.read(53);                                                                              //
  Fan.Enable    = EEPROM.read(54);                                                                              //
  Fan.On1Hr     = EEPROM.read(55);                                                                              //
  Fan.On1Min    = EEPROM.read(56);                                                                              //
  Fan.Dur1Min   = EEPROM.read(57);                                                                              //
  Fan.Dur1Sec   = EEPROM.read(58);                                                                              //
  Fan.On2Hr     = EEPROM.read(59);                                                                              //
  Fan.On2Min    = EEPROM.read(60);                                                                              //
  Fan.Dur2Min   = EEPROM.read(61);                                                                              //
  Fan.Dur2Sec   = EEPROM.read(62);                                                                              //
  Fan.OnDay     = EEPROM.read(63);                                                                              //
  Fan.OnDay2    = EEPROM.read(64);                                                                              //
  temp1Lo       = EEPROM.readWord(65);                                                                          //
  temp1Hi       = EEPROM.readWord(67);                                                                          //
  temp2Lo       = EEPROM.readWord(69);                                                                          //
  temp2Hi       = EEPROM.readWord(71);                                                                          //
  temp3Lo       = EEPROM.readWord(73);                                                                          //
  temp3Hi       = EEPROM.readWord(75);                                                                          //
  hum1Lo        = EEPROM.readWord(77);                                                                          //
  hum1Hi        = EEPROM.readWord(79);                                                                          //
  tempUnits     = EEPROM.read(81);                                                                              //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                              Start up DS18B20 Temp Sensors                                 //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  sensors.begin();                                                                                              //
#if debug                                                                                                       //
  // locate DS18B20 temperature devices on the bus                                                              //
  Serial.print(F("Locating Dallas 1820 devices... "));                                                          //
  Serial.print(F("Found "));                                                                                    //
  Serial.print(sensors.getDeviceCount());                                                                       //
  Serial.println(F("."));                                                                                       //
#endif                                                                                                          //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                              Initiate the SD Card                                          //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if debug                                                                                                       //
  Serial.println(F("Initialising SD card..."));                                                                 //
#endif                                                                                                          //
  // see if the card is present and can be initialized:                                                         //
  while (heatOn)                                                                                                //
  { //                                                                                                          //
    if (!sd.begin(SD_CHIP_SELECT, SPI_FULL_SPEED)) {                                                            //
#if debug                                                                                                       //
      Serial.println(F("Card failed, or not present"));                                                         //
      Serial.println(F("Retrying...."));                                                                        //
#endif                                                                                                          //
    }                                                                                                           //
    else                                                                                                        //
    { //                                                                                                        //
      heatOn = 0;                                                                                               //
#if debug                                                                                                       //
      Serial.println(F("Card initialised."));                                                                   //
#endif                                                                                                          //
    }                                                                                                           //
  }                                                                                                             //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                    Start screen functions                                                  //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  myGLCD.InitLCD();                                                                                             //
  myGLCD.setBrightness(0);          // <= Backlight Off                                                         //
  myGLCD.setWritePage(1);           // <= Select writepage 1                                                    //
  myGLCD.clrScr();                  // <= Clear writepage 1                                                     //
  myGLCD.setDisplayPage(1);         // <= Display writepage 1                                                   //
  myGLCD.setBrightness(BackLight);  // <= Turn on BackLight Max brightness                                      //
  myTouch.InitTouch();                                                                                          //
  myTouch.setPrecision(PREC_MEDIUM);                                                                            //
#if Auxdisp                                                                                                     //
  char buf[6];                                                                                                  //
  lcd.begin ( lcdC, lcdR );                                                                                     //
  lcd.clear ( );                                                                                                //
  if (temp1Lo != 65535)                                                                                         //
  { //                                                                                                          //
    lcd.setCursor(0, 0);                                                                                        //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf((temp1Lo / 100.00) , 4, 1, buf);                                                                  //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = ((((temp1Lo / 100.00) - 32.00) / 9.00) * 5.00);                                           //
      dtostrf(tmptemp , 4, 1, buf);                                                                             //
    } //                                                                                                        //
    lcd.print(buf);                                                                                             //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    lcd.setCursor(4, 0);                                                                                        //
  } //                                                                                                          //
  lcd.print(F("/"));                                                                                            //
  if (temp1Hi != 0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf((temp1Hi / 100.00) , 4, 1, buf);                                                                  //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = ((((temp1Hi / 100.00) - 32.00) / 9.00) * 5.00);                                           //
      dtostrf(tmptemp , 4, 1, buf);                                                                             //
    } //                                                                                                        //
    lcd.print(buf);                                                                                             //
  } //                                                                                                          //
  lcd.setCursor(15, 0);                                                                                         //
  if (tempUnits == 0xF0)                                                                                        //
  { //                                                                                                          //
    lcd.print(F("F"));                                                                                          //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    lcd.print(F("C"));                                                                                          //
  } //                                                                                                          //
  if (hum1Lo != 65535)                                                                                          //
  { //                                                                                                          //
    lcd.setCursor(0, 1);                                                                                        //
    dtostrf((hum1Lo / 100.00), 4, 1, buf);                                                                      //
    lcd.print(buf);                                                                                             //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    lcd.setCursor(4, 1);                                                                                        //
  } //                                                                                                          //
  lcd.print(F("/"));                                                                                            //
  if (hum1Hi != 0)                                                                                              //
  { //                                                                                                          //
    dtostrf((hum1Hi / 100.00), 4, 1, buf);                                                                      //
    lcd.print(buf);                                                                                             //
  } //                                                                                                          //
  lcd.setCursor(15, 1);                                                                                         //
  lcd.print(F("%"));                                                                                            //
#endif  //                                                                                                      //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                  Start RTC                                                 //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Wire.begin();                                                                                                 //
#if(DS1307)                                                                                                     //
  rtc.begin(); // Start the Clock                                                                               //
  delay(250);                                                                                                   //
  if (! rtc.isrunning())                                                                                        //
  { //                                                                                                          //
    // If no RTC is installed, alert on serial                                                                  //
#if debug                                                                                                       //
    Serial.println(F("DS1307 is NOT running!"));                                                                //
    Serial.println(F("Define DS1307 0!"));                                                                      //
#endif                                                                                                          //
  }                                                                                                             //
#else                                                                                                           //
  rtc.begin(DateTime(F(__DATE__), F(__TIME__))); // Start the Clock                                             //
#if debug                                                                                                       //
  Serial.println(F("Software RTC is running!"));                                                                //
  Serial.println(F("Send sync message eg T1420722534 (Can type in serial monitor)"));                           //
#endif                                                                                                          //
#endif                                                                                                          //
  setSyncInterval(60);                                                                                          //
  setSyncProvider(syncProvider);// reference our syncProvider function instead of RTC_DS1307::get();            //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                   Set internal clock with RTC at zero seconds                              //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if DS1307 == 1                                                                                                 //
  time_t timeNow1 = rtc.now().unixtime();                                                                       //
  while (second(timeNow1) != 57)                                                                                //
  { //                                                                                                          //
    timeNow1 = rtc.now().unixtime();                                                                            //
  } //                                                                                                          //
  setTime(timeNow1); // set system time to rtc time                                                             //
  setAlarms();                                                                                                  //
#endif                                                                                                          //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                Turn OFF all outputs and define relays pin.  Active Low, High is off.                       //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  digitalWrite(LIGHTS1, HIGH);                                                                                  //
  digitalWrite(LIGHTS2, HIGH);                                                                                  //
  digitalWrite(TEMP1, HIGH);                                                                                    //
  digitalWrite(FOGGER1, HIGH);                                                                                  //
  digitalWrite(MISTING1, HIGH);                                                                                 //
  digitalWrite(FAN1, HIGH);                                                                                     //
  //                       Initialise the Arduino data pins for OUTPUT                                          //
  pinMode(LIGHTS1, OUTPUT);                                                                                     //
  pinMode(LIGHTS2, OUTPUT);                                                                                     //
  pinMode(TEMP1, OUTPUT);                                                                                       //
  pinMode(FOGGER1, OUTPUT);                                                                                     //
  pinMode(MISTING1, OUTPUT);                                                                                    //
  pinMode(FAN1, OUTPUT);                                                                                        //
  delay(250); //              Check that all relays are inactive at Reset                                       //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                  Setup display and stored video pages                                      //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  myGLCD.setWritePage(0);  //  <= Still displaying writepage 1                                                  //
  myGLCD.clrScr();        //   <= Clear writepage 0                                                             //
#if debug                                                                                                       //
  unsigned long performance = millis();                                                                         //
#endif                                                                                                          //
  // boot up logo                                                                                               //
  myFiles.loadcpld(230, 40, 378, 268, "Logo.raw", 0, 4);  // <==== Write to writepage 0                         //
  myFiles.loadcpld(340, 360, 149, 47, "Copy.raw", 0, 4);  // <==== Write to writepage 0                         //
  myGLCD.setDisplayPage(0);                               // <==== Display writepage 0                          //
  myFiles.loadcpld(0, 0, 800, 480, "1Home.raw", 1, 2);                                                          //
  myFiles.loadcpld(0, 0, 800, 480, "2Lights.raw", 2, 2);                                                        //
  myFiles.loadcpld(0, 0, 800, 480, "3TempGHL.raw", 3, 2);                                                       //
  if (tempUnits == 0xF0)                                                                                        //
  { //                                                                                                          //
    myFiles.loadcpld(41, 365, 73, 75, "DegF.raw", 3, 6);                                                        //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    myFiles.loadcpld(124, 365, 73, 75, "DegC.raw", 3, 6);                                                       //
  } //                                                                                                          //
  myFiles.loadcpld(0, 0, 800, 480, "46SchedG.raw", 4, 2); //Fan/Fog to save video pages...                      //
  myFiles.loadcpld(0, 0, 800, 480, "5MistG.raw", 7, 2);   //New Mist...                                         //
#if fastest                                                                                                     //
  myFiles.loadcpld(0, 0, 800, 480, "7Clock.raw", 5, 2);                                                         //
  myFiles.loadcpld(0, 0, 800, 480, "8Screen.raw", 6, 2);                                                        //
#endif                                                                                                          //
  // display home screen                                                                                        //
  screenHome();                                                                                                 //
  millisDim = millis();                                                                                         //
#if debug                                                                                                       //
  Serial.print(F("Init time "));                                                                                //
  Serial.print(millisDim - performance);                                                                        //
  Serial.println(F(" ms"));                                                                                     //
#endif                                                                                                          //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                        Take initial readings of sensors                                    //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  myCopyTime(&prevRTC);                                                                                         //
  sensors.setWaitForConversion(true);   // makes it wait for first reading                                      //
  tempRead(temp1, temp2, temp3);                                                                                //
  sensors.setWaitForConversion(false);  // makes it async (no more waiting)                                     //
  humRead(hum1);                                                                                                //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        End of Setup                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                              //
//                                                                                                              //
//                                                                                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Main Loop function                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()                                                                                                     //
{ //                                                                                                            //
#if(!DS1307)                                                                                                    //
  if (Serial.available()) {  //                                                                                 //
    processSyncMessage();                                                                                       //
  } //                                                                                                          //
#endif                                                                                                          //
  unsigned long currentMillis = millis(); // get current millis                                                 //
  if (timeStatus() != timeNotSet) {  //                                                                         //
    updateTimeDate();                                                                                           //
  }  //                                                                                                         //
  if ((AutoDim) != 0) { //                                                                                      //
    if (currentMillis - millisDim > TouchDelay * 1000UL) {  //                                                  //
      myGLCD.setBrightness((AutoDim - 1) * 2);                                                                  //
#if Auxdisp                                                                                                     //
      lcd.setBacklight(LOW);                                                                                    //
#endif                                                                                                          //
      BackLightTouch = false;                                                                                   //
    }  //                                                                                                       //
    else {  //                                                                                                  //
#if Auxdisp                                                                                                     //
      lcd.setBacklight(HIGH);                                                                                   //
#endif                                                                                                          //
      myGLCD.setBrightness(16);                                                                                 //
    }  //                                                                                                       //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    myGLCD.setBrightness(BackLight);                                                                            //
  } //                                                                                                          //
  if ((currentMillis - millisHome > (HomeDelay * 60000UL)) && dispScreen != 1) { //                             //
    millisHome = currentMillis;                                                                                 //
    screenHome();                                                                                               //
  } //                                                                                                          //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                           Check for touch events                                           //
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (myTouch.dataAvailable())  { //                                                                            //
    //                     make sure it's been .5 sec (initially) between touches                               //
    if (currentMillis - prevMillisTouch > TouchRepeatDelay)                                                     //
    { //                                                                                                        //
      //            set BackLight touch if not already set and turn up brightness                               //
      if (BackLightTouch == false) // BackLight touch is for adjusting brightness after touch                   //
      { //                                                                                                      //
        myGLCD.setBrightness(BackLight);                                                                        //
        BackLightTouch = true;                                                                                  //
      } //                                                                                                      //
      prevMillisTouch = currentMillis; // reset the touch timer                                                 //
      millisDim = currentMillis;       // reset screen dim timer                                                //
      millisHome = currentMillis;      // reset return home timer                                               //
      processMyTouch();                                                                                         //
    } //                                                                                                        //
  } //                                                                                                          //
  unsigned long rightNow = now();  // Get the time in seconds (since 1970)                                      //
  if ((rightNow - humidTime) >= 2) { //                                                                         //
    float tmphum;                                                                                               //
    humRead(tmphum);                                                                                            //
    if (hum1 != tmphum) { //                                                                                    //
      hum1 = tmphum;                                                                                            //
      if (hum1 > 28.0 && hum1 < 100.0)                                                                          //
      { //                                                                                                      //
        char hum[6];                                                                                            //
        dtostrf(hum1, 4, 1, hum);                                                                               //
        if ((hum1 * 100.0) < hum1Lo)                                                                            //
        { //                                                                                                    //
          hum1Lo = (hum1 * 100.0);                                                                              //
          EEPROM.writeWord(77, hum1Lo); //                  <- Hi/Lo hum1Lo                                     //
#if Auxdisp                                                                                                     //
          lcd.setCursor(0, 1);                                                                                  //
          lcd.print(hum);                                                                                       //
#endif                                                                                                          //
        } //                                                                                                    //
        if ((hum1 * 100.0) > hum1Hi)                                                                            //
        { //                                                                                                    //
          hum1Hi = (hum1 * 100.0);                                                                              //
          EEPROM.writeWord(79, hum1Hi); //                  <- Hi/Lo hum1Hi                                     //
#if Auxdisp                                                                                                     //
          lcd.setCursor(5, 1);                                                                                  //
          lcd.print(hum);                                                                                       //
#endif                                                                                                          //
        } //                                                                                                    //
#if debug                                                                                                       //
        Serial.print(F("Internal humidity update: "));                                                          //
        Serial.println(hum1);                                                                                   //
#endif                                                                                                          //
        if (dispScreen == 1)                                                                                    //
        { //                                                                                                    //
          char hum[6];                                                                                          //
          dtostrf(hum1, 4, 1, hum);                                                                             //
#if Auxdisp                                                                                                     //
          lcd.setCursor(11, 1);                                                                                 //
          lcd.print(hum);                                                                                       //
#endif                                                                                                          //
          ubuntuRed();                                                                                          //
          myGLCD.print(hum, 425, 405);                                                                          //
#if debug                                                                                                       //
          Serial.print(F("Screen (Home) humidity update: "));                                                   //
          Serial.println(hum1);                                                                                 //
#endif                                                                                                          //
        } //                                                                                                    //
      } //                                                                                                      //
    } //                                                                                                        //
  } //                                                                                                          //
  if ((rightNow - tempTime) >= 1) { //                                                                          //
    if (dispScreen == 1 || dispScreen == 3)                                                                     //
    { //                                                                                                        //
      ubuntuRed();                                                                                              //
    } //                                                                                                        //
    char sensor[6] = "     ";                                                                                   //
    float tmptemp1, tmptemp2, tmptemp3;                                                                         //
    tempRead(tmptemp1, tmptemp2, tmptemp3);                                                                     //
    if (temp1 != tmptemp1)                                                                                      //
    { //                                                                                                        //
      temp1 = tmptemp1;                                                                                         //
      if (temp1 > 28.0 && temp1 < 185.0)                                                                        //
      { //                                                                                                      //
        dtostrf(temp1, 4 , 1, sensor);                                                                          //
        if ((temp1 * 100.00) < temp1Lo)                                                                         //
        { //                                                                                                    //
          temp1Lo = (temp1 * 100.00);                                                                           //
          EEPROM.writeWord(65, temp1Lo); //                 <- Hi/Lo temp1Lo                                    //
#if Auxdisp                                                                                                     //
          lcd.setCursor(0, 0);                                                                                  //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
            dtostrf((temp1Lo / 100.00) , 4, 1, sensor);                                                         //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            float tmptemp = ((((temp1Lo / 100.00) - 32.00) / 9.00) * 5.00);                                     //
            dtostrf(tmptemp , 4, 1, sensor);                                                                    //
          } //                                                                                                  //
          lcd.print(sensor);                                                                                    //
#endif                                                                                                          //
        } //                                                                                                    //
        if ((temp1 * 100.00) > temp1Hi)                                                                         //
        { //                                                                                                    //
          temp1Hi = (temp1 * 100.00);                                                                           //
          EEPROM.writeWord(67, temp1Hi); //                 <- Hi/Lo temp1Lo                                    //
#if Auxdisp                                                                                                     //
          lcd.setCursor(5, 0);                                                                                  //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
            dtostrf((temp1Hi / 100.00) , 4, 1, sensor);                                                         //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            float tmptemp = ((((temp1Hi / 100.00) - 32.00) / 9.00) * 5.00);                                     //
            dtostrf(tmptemp , 4, 1, sensor);                                                                    //
          } //                                                                                                  //
          lcd.print(sensor);                                                                                    //
#endif                                                                                                          //
        } //                                                                                                    //
#if Auxdisp                                                                                                     //
        lcd.setCursor(11, 0);                                                                                   //
        if (tempUnits == 0xF0)                                                                                  //
        { //                                                                                                    //
          dtostrf(temp1 , 4, 1, sensor);                                                                        //
        } //                                                                                                    //
        else                                                                                                    //
        { //                                                                                                    //
          float tmptemp = (((temp1 - 32.00) / 9.00) * 5.00);                                                    //
          dtostrf(tmptemp , 4, 1, sensor);                                                                      //
        } //                                                                                                    //
        lcd.print(sensor);                                                                                      //
#endif                                                                                                          //
        if (tempUnits == 0xF0)                                                                                  //
        { //                                                                                                    //
#if debug//                                                                                                     // 
          Serial.print(F("Internal temp1 update "));                                                            //
          Serial.println(temp1);                                                                                //
#endif                                                                                                          //
        } //                                                                                                    //
        else                                                                                                    //
        { //                                                                                                    //
          float tmptemp = (((temp1 - 32.00) / 9.00) * 5.00);                                                    //
          dtostrf(tmptemp , 4, 1, sensor);                                                                      //
#if debug//                                                                                                     // 
          Serial.print(F("Internal temp1 update "));                                                            //
          Serial.println(tmptemp);                                                                              //
#endif                                                                                                          //
        } //                                                                                                    //
        if (dispScreen == 1)                                                                                    //
        { //                                                                                                    //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
            dtostrf(temp1, 4, 1, sensor);                                                                       //
#if debug                                                                                                       //
            Serial.print(F("Screen (Home) temp1 update "));                                                     //
            Serial.println(temp1);                                                                              //
#endif                                                                                                          //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            float tmptemp = (((temp1 - 32.00) / 9.00) * 5.00);                                                  //
            dtostrf(tmptemp , 4, 1, sensor);                                                                    //
#if debug                                                                                                       //
            Serial.print(F("Screen (Home) temp1 update "));                                                     //
            Serial.println(tmptemp);                                                                            //
#endif                                                                                                          //
          } //                                                                                                  //
          myGLCD.print(sensor, 411, 316);                                                                       //
        } //                                                                                                    //
        if (dispScreen == 3)                                                                                    //
        { //                                                                                                    //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
            dtostrf(temp1, 4, 1, sensor);                                                                       //
#if debug                                                                                                       //
            Serial.print(F("Screen (Temp) temp1 update "));                                                     //
            Serial.println(temp1);                                                                              //
#endif                                                                                                          //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            float tmptemp = (((temp1 - 32.00) / 9.00) * 5.00);                                                  //
            dtostrf(tmptemp , 4, 1, sensor);                                                                    //
#if debug                                                                                                       //
            Serial.print(F("Screen (Temp) temp1 update "));                                                     //
            Serial.println(tmptemp);                                                                            //
#endif                                                                                                          //
          } //                                                                                                  //
          myGLCD.print(sensor, 55, 215);                                                                        //
        } //                                                                                                    //
      } //                                                                                                      //
    } //                                                                                                        //
    if (temp2 != tmptemp2)                                                                                      //
    { //                                                                                                        //
      temp2 = tmptemp2;                                                                                         //
      if (temp2 > 28.0 && temp2 < 185.0)                                                                        //
      { //                                                                                                      //
        dtostrf(temp2, 4, 1, sensor);                                                                           //
        if ((temp2 * 100.0) < temp2Lo)                                                                          //
        { //                                                                                                    //
          temp2Lo = (temp2 * 100.0);                                                                            //
          EEPROM.writeWord(69, temp2Lo); //                 <- Hi/Lo temp2Lo                                    //
        } //                                                                                                    //
        if ((temp2 * 100.0) > temp2Hi)                                                                          //
        { //                                                                                                    //
          temp2Hi = (temp2 * 100.0);                                                                            //
          EEPROM.writeWord(71, temp2Hi); //                 <- Hi/Lo temp2Hi                                    //
        } //                                                                                                    //
        if (tempUnits == 0xF0)                                                                                  //
        { //                                                                                                    //
#if debug                                                                                                       //
          Serial.print(F("Internal temp2 update "));                                                            //
          Serial.println(temp2);                                                                                //
#endif                                                                                                          //
        } //                                                                                                    //
        else                                                                                                    //
        { //                                                                                                    //
          float tmptemp = (((temp2 - 32.00) / 9.00) * 5.00);                                                    //
          dtostrf(tmptemp , 4, 1, sensor);                                                                      //
#if debug                                                                                                       //
          Serial.print(F("Internal temp2 update "));                                                            //
          Serial.println(tmptemp);                                                                              //
#endif                                                                                                          //
        } //                                                                                                    //
        if (dispScreen == 1)                                                                                    //
        { //                                                                                                    //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
#if debug                                                                                                       //
            Serial.print(F("Screen (Home) temp2 update "));                                                     //
            Serial.println(temp2);                                                                              //
#endif                                                                                                          //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            float tmptemp = (((temp2 - 32.00) / 9.00) * 5.00);                                                  //
            dtostrf(tmptemp , 4, 1, sensor);                                                                    //
#if debug                                                                                                       //
            Serial.print(F("Screen (Home) temp2 update "));                                                     //
            Serial.println(tmptemp);                                                                            //
#endif                                                                                                          //
          } //                                                                                                  //
          myGLCD.print(sensor, 537, 316);                                                                       //
        } //                                                                                                    //
        if (dispScreen == 3)                                                                                    //
        { //                                                                                                    //
          //dtostrf(temp2, 4, 1, sensor);                                                                       //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
#if debug                                                                                                       //
            Serial.print(F("Screen (Temp) temp2 update "));                                                     //
            Serial.println(temp2);                                                                              //
#endif                                                                                                          //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            float tmptemp = (((temp2 - 32.00) / 9.00) * 5.00);                                                  //
            dtostrf(tmptemp , 4, 1, sensor);                                                                    //
#if debug                                                                                                       //
            Serial.print(F("Screen (Home) temp2 update "));                                                     //
            Serial.println(tmptemp);                                                                            //
#endif                                                                                                          // 
          } //                                                                                                  //
          myGLCD.print(sensor, 207, 215);                                                                       //
        } //                                                                                                    //
      } //                                                                                                      //
    } //                                                                                                        //
    if (temp3 != tmptemp3) { //                                                                                 //
      temp3 = tmptemp3;                                                                                         //
      if (temp3 > 28.0 && temp3 < 185.0)                                                                        //
      { //                                                                                                      //
        dtostrf(temp3, 4, 1, sensor);                                                                           //
        if ((temp3 * 100.0) < temp3Lo)                                                                          //
        { //                                                                                                    //
          temp3Lo = (temp3 * 100.0);                                                                            //
          EEPROM.writeWord(73, temp3Lo); //                 <- Hi/Lo temp3Lo                                    //
        } //                                                                                                    //
        if ((temp3 * 100.0) > temp3Hi)                                                                          //
        { //                                                                                                    //
          temp3Hi = (temp3 * 100.0);                                                                            //
          EEPROM.writeWord(75, temp3Hi); //                 <- Hi/Lo temp3Lo                                    //
        } //                                                                                                    //
        if (tempUnits == 0xF0)                                                                                  //
        { //                                                                                                    //
#if debug                                                                                                       // 
          Serial.print(F("Internal temp3 update"));                                                             //
          Serial.println(temp3);                                                                                //
#endif                                                                                                          //
        } //                                                                                                    //
        else                                                                                                    //
        { //                                                                                                    //
          float tmptemp = (((temp3 - 32.00) / 9.00) * 5.00);                                                    //
          dtostrf(tmptemp , 4, 1, sensor);                                                                      //
#if debug                                                                                                       // 
          Serial.print(F("Internal temp3 update"));                                                             //
          Serial.println(tmptemp);                                                                              //
#endif                                                                                                          //
        } //                                                                                                    //
        if (dispScreen == 1) //                                                                                 //
        { //                                                                                                    //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
#if debug                                                                                                       // 
            Serial.print(F("Screen (Home) temp3 update "));                                                     //
            Serial.println(temp3);                                                                              //
#endif                                                                                                          //
          }                                                                                                     //
          else                                                                                                  //
          { //                                                                                                  //
            float tmptemp = (((temp3 - 32.00) / 9.00) * 5.00);                                                  //
            dtostrf(tmptemp , 4, 1, sensor);                                                                    //
#if debug                                                                                                       // 
            Serial.print(F("Screen (Home) temp3 update "));                                                     //
            Serial.println(tmptemp);                                                                            //
#endif                                                                                                          //
          } //                                                                                                  //
          myGLCD.print(sensor, 668, 316);                                                                       //
        }                                                                                                       //
        if (dispScreen == 3)                                                                                    //
        { //                                                                                                    //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
#if debug                                                                                                       // 
            Serial.print(F("Screen (Temp) temp3 update "));                                                     //
            Serial.println(temp3);                                                                              //
#endif                                                                                                          //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            float tmptemp = (((temp3 - 32.00) / 9.00) * 5.00);                                                  //
            dtostrf(tmptemp , 4, 1, sensor);                                                                    //
#if debug                                                                                                       // 
            Serial.print(F("Screen (Temp) temp3 update "));                                                     //
            Serial.println(tmptemp);                                                                            //
#endif                                                                                                          //
          } //                                                                                                  //
          myGLCD.print(sensor, 359, 215);                                                                       //
        } //                                                                                                    //
      } //                                                                                                      //
    } //                                                                                                        //
  } //                                                                                                          //
  Alarm.delay(0);                                                                                               //
  lowTemp();                                                                                                    //
  hiTemp();                                                                                                     //
  procRelays(Relay);  // Display active relay icons on 'Home' Page                                              //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              End of Main Loop                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                              //
//                                                                                                              //
//                                                                                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   PAGE SETUP                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Home Screen                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenHome()  // draw main home screen showing overview info                                               //
{ //                                                                                                            //
  DoScreen(1, 1);                                                                                               //
#if debug                                                                                                       //
  Serial.println(F("Home Screen"));                                                                             //
#endif                                                                                                          //
  updateTime = false;                                                                                           //
  if (tmpTouchRepeatDelay != TouchRepeatDelay) TouchRepeatDelay = tmpTouchRepeatDelay;                          //
  // Draw Temp to screen                                                                                        //
  float tmptemp1, tmptemp2, tmptemp3, tmphum;                                                                   //
  if (temp1 + temp2 + temp3 == 0.0) {                                                                           //
    tempRead(temp1, temp2, temp3);                                                                              //
  } //                                                                                                          //
  unsigned long rightNow = now();                                                                               //
  ubuntuRed();                                                                                                  //
  char sensor[6] = "     ";                                                                                     //
  if (tempUnits == 0xF0)                                                                                        //
  { //                                                                                                          //
    dtostrf(temp1, 4, 1, sensor  );                                                                             //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    float tmptemp = (((temp1 - 32.00) / 9.00) * 5.00);                                                          //
    dtostrf(tmptemp , 4, 1, sensor);                                                                            //
  } //                                                                                                          //
  myGLCD.print(sensor, 411, 316);                                                                               //
  if (temp2 != 0.0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf(temp2, 4, 1, sensor);                                                                             //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = (((temp2 - 32.00) / 9.00) * 5.00);                                                        //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 537, 316);                                                                             //
  } //                                                                                                          //
  if (temp3 != 0.0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf(temp3, 4, 1, sensor);                                                                             //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = (((temp3 - 32.00) / 9.00) * 5.00);                                                        //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 668, 316);                                                                             //
  } //                                                                                                          //
  // Draw Humidity to screen                                                                                    //
  if (hum1 == 0.0) { //                                                                                         //
    humRead(hum1);                                                                                              //
  } //                                                                                                          //
  if ((rightNow - humidTime) >= 2)                                                                              //
  { //                                                                                                          //
    humRead(tmphum);                                                                                            //
    hum1 = tmphum;                                                                                              //
  } //                                                                                                          //
  dtostrf(hum1, 4, 1, sensor);                                                                                  //
  ubuntuRed();                                                                                                  //
  myGLCD.print(sensor, 425, 405);                                                                               //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Lights Screen                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenLights() //                                                                                          //
{ //                                                                                                            //
  DoScreen(2, 2);                                                                                               //
#if debug                                                                                                       //
  Serial.println(F("Lights Screen"));                                                                           //
#endif                                                                                                          //
  tmpTouchRepeatDelay = TouchRepeatDelay;                                                                       //
  TouchRepeatDelay = 150;                                                                                       //
  updateTime = false;                                                                                           //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Temp Screen                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenTemp() //                                                                                            //
{ //                                                                                                            //
  DoScreen(3, 3);                                                                                               //
#if debug                                                                                                       //
  Serial.println(F("Temp Screen"));                                                                             //
#endif                                                                                                          //
  // Draw Temp to screen                                                                                        //
  float tmptemp1, tmptemp2, tmptemp3, tmphum;                                                                   //
  unsigned long rightNow = now();                                                                               //
  if ((rightNow - tempTime) > 1) { //                                                                           //
    tempRead(tmptemp1, tmptemp2, tmptemp3);                                                                     //
    if (temp1 != tmptemp1) { //                                                                                 //
      temp1 = tmptemp1;                                                                                         //
    } //                                                                                                        //
    if (temp2 != tmptemp2) { //                                                                                 //
      temp2 = tmptemp2;                                                                                         //
    } //                                                                                                        //
    if (temp3 != tmptemp3) { //                                                                                 //
      temp3 = tmptemp3;                                                                                         //
    } //                                                                                                        //
  } //                                                                                                          //
  ubuntuRed();                                                                                                  //
  char sensor[6] = "     ";                                                                                     //
  if (temp1 != 0.0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf(temp1, 4, 1, sensor);                                                                             //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = (((temp1 - 32.00) / 9.00) * 5.00);                                                        //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 55, 215);                                                                              //
  } //                                                                                                          //
  if (temp2 != 0.0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf(temp2, 4, 1, sensor);                                                                             //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = (((temp2 - 32.00) / 9.00) * 5.00);                                                        //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 207, 215);                                                                             //
  } //                                                                                                          //
  if (temp3 != 0.0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf(temp3, 4, 1, sensor);                                                                             //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = (((temp3 - 32.00) / 9.00) * 5.00);                                                        //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 359, 215);                                                                             //
  } //                                                                                                          //
  tmpTouchRepeatDelay = TouchRepeatDelay;                                                                       //
  TouchRepeatDelay = 100;                                                                                       //
  if (tempUnits == 0xF0)                                                                                        //
  { //                                                                                                          //
    dtostrf(heater, 4, 1 , sensor);                                                                             //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    float tmptemp = (((heater - 32.00) / 9.00) * 5.00);                                                         //
    dtostrf(tmptemp, 4, 1 , sensor);                                                                            //
  } //                                                                                                          //
  myGLCD.print(sensor, 510, 296);                                                                               //
  if (tempUnits == 0xF0)                                                                                        //
  { //                                                                                                          //
    dtostrf(fan, 4, 1 , sensor);                                                                                //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    float tmptemp = (((fan - 32.00) / 9.00) * 5.00);                                                            //
    dtostrf(tmptemp, 4, 1 , sensor);                                                                            //
  } //                                                                                                          //
  myGLCD.print(sensor, 640, 296);                                                                               //
  updateTime = false;                                                                                           //
  myGLCD.setColor(255, 0, 36);                                                                                  //
  myGLCD.setFont(arial_bold);                                                                                   //
  if (temp1Hi != 0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf((temp1Hi / 100.00), 4, 1, sensor);                                                                //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = ((((temp1Hi / 100.00) - 32.00) / 9.00) * 5.00);                                           //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 106, 304);                                                                             //
  } //                                                                                                          //
  if (temp2Hi != 0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf((temp2Hi / 100.00), 4, 1, sensor);                                                                //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = ((((temp2Hi / 100.00) - 32.00) / 9.00) * 5.00);                                           //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 258, 304);                                                                             //
  } //                                                                                                          //
  if (temp3Hi != 0)                                                                                             //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf((temp3Hi / 100.00), 4, 1, sensor);                                                                //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = ((((temp3Hi / 100.00) - 32.00) / 9.00) * 5.00);                                           //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 410, 304);                                                                             //
  } //                                                                                                          //
  myGLCD.setColor(48, 169, 198);                                                                                //
  if (temp1Lo != 65535)                                                                                         //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf((temp1Lo / 100.00), 4, 1, sensor);                                                                //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = ((((temp1Lo / 100.00) - 32.00) / 9.00) * 5.00);                                           //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 35, 304);                                                                              //
  } //                                                                                                          //
  if (temp2Lo != 65535)                                                                                         //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf((temp2Lo / 100.00), 4, 1, sensor);                                                                //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = ((((temp2Lo / 100.00) - 32.00) / 9.00) * 5.00);                                           //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 187, 304);                                                                             //
  } //                                                                                                          //
  if (temp3Lo != 65535)                                                                                         //
  { //                                                                                                          //
    if (tempUnits == 0xF0)                                                                                      //
    { //                                                                                                        //
      dtostrf((temp3Lo / 100.00), 4, 1, sensor);                                                                //
    } //                                                                                                        //
    else                                                                                                        //
    { //                                                                                                        //
      float tmptemp = ((((temp3Lo / 100.00) - 32.00) / 9.00) * 5.00);                                           //
      dtostrf(tmptemp , 4, 1, sensor);                                                                          //
    } //                                                                                                        //
    myGLCD.print(sensor, 339, 304);                                                                             //
  } //                                                                                                          //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Fogger Screen                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenFogger() //                                                                                          //
{ //                                                                                                            //
  myFiles.loadcpld(21, 6, 336, 74, "FogTitle.raw", 4, 6);                                                       //
  DoScreen(4, 4);                                                                                               //
#if debug                                                                                                       //
  Serial.println(F("Fogger Screen"));                                                                           //
#endif                                                                                                          //
  tmpTouchRepeatDelay = TouchRepeatDelay;                                                                       //
  TouchRepeatDelay = 100;                                                                                       //
  updateTime = false;                                                                                           //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               Misting Screen                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenMist() //                                                                                            //
{ //                                                                                                            //
  DoScreen(7, 5);                                                                                               //
#if debug                                                                                                       //
  Serial.println(F("Misting Screen"));                                                                          //
#endif                                                                                                          //
  tmpTouchRepeatDelay = TouchRepeatDelay;                                                                       //
  TouchRepeatDelay = 150;                                                                                       //
  updateTime = false;                                                                                           //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 Fan Screen                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenFan() //                                                                                             //
{ //                                                                                                            //
  myFiles.loadcpld(21, 6, 336, 74, "FanTitle.raw", 4, 6);                                                       //
  DoScreen(4, 6);                                                                                               //
#if debug                                                                                                       //
  Serial.println(F("Fan Screen"));                                                                              //
#endif                                                                                                          //
  tmpTouchRepeatDelay = TouchRepeatDelay;                                                                       //
  TouchRepeatDelay = 150;                                                                                       //
  updateTime = false;                                                                                           //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                   Modify the following 2 screens to be like the others for instant load                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 Clock Screen                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenClock() //                                                                                           //
{ //                                                                                                            //
#if fastest                                                                                                     //
  DoScreen(5, 7);                                                                                               //
#else                                                                                                           //
  myGLCD.setWritePage(0);                                                                                       //
  myGLCD.clrScr();                                                                                              //
  myGLCD.setDisplayPage(0);                                                                                     //
  myFiles.load(0, 0, 800, 480, "7Clock.raw", 3);                                                                //
  dispScreen = 7;                                                                                               //
#endif                                                                                                          //
#if debug                                                                                                       //
  Serial.println(F("Clock Screen"));                                                                            //
#endif                                                                                                          //
  myCopyTime(&tmpRTC);                                                                                          //
  tmpTouchRepeatDelay = TouchRepeatDelay;                                                                       //
  TouchRepeatDelay = 150;                                                                                       //
  updateTime = false;                                                                                           //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               Settings Screen                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenScreen() //                                                                                          //
{ //                                                                                                            //
#if fastest                                                                                                     //
  DoScreen(6, 8);                                                                                               //
#else                                                                                                           //
  myGLCD.setWritePage(0);                                                                                       //
  myGLCD.clrScr();                                                                                              //
  myGLCD.setDisplayPage(0);                                                                                     //
  myFiles.load(0, 0, 800, 480, "8Screen.raw", 3);                                                               //
  dispScreen = 8;                                                                                               //
#endif                                                                                                          //
#if debug                                                                                                       //
  Serial.println(F("Screen Screen"));                                                                           //
#endif                                                                                                          //
  myGLCD.setFont(Ubuntubold);                                                                                   //
  myGLCD.printNumI(AutoDim, 713, 167);                                                                          //
  if (AutoDim != 0) { //                                                                                        //
    myGLCD.printNumI(TouchDelay, 689, 340, 3);                                                                  //
  } //                                                                                                          //
  else                                                                                                          //
  { //                                                                                                          //
    myGLCD.print("   ", 689, 340);                                                                              //
  } //                                                                                                          //
  myGLCD.printNumI(HomeDelay, 338, 167, 2);                                                                     //
  myGLCD.printNumI(BackLight, 313, 340, 2);                                                                     //
  tmpTouchRepeatDelay = TouchRepeatDelay;                                                                       //
  TouchRepeatDelay = 150;                                                                                       //
  updateTime = false;                                                                                           //
} //                                                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  End Of Screens                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              Process Touch Functions                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void processMyTouch() //                                                                                        //
{ //                                                                                                            //
  static volatile byte tempAD, tempTD, tempHD, tempBL;                                                          //
  static volatile float tempHeater, tempFan;                                                                    //
  static volatile byte tempL1OnHr, tempL1OnMin;                                                                 //
  static volatile byte tempL1OffHr, tempL1OffMin;                                                               //
  static volatile byte tempL2OnHr, tempL2OnMin;                                                                 //
  static volatile byte tempL2OffHr, tempL2OffMin;                                                               //
  myTouch.read();                                                                                               //
  word x = myTouch.getX();                                                                                      //
  word y = myTouch.getY();                                                                                      //
  switch (dispScreen) //                                                                                        //
  { //                                                                                                          //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                Home Screen                                               //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 1:                                                                                                     //
#if debug                                                                                                       //
      if ((y >= 64) && (y < 114))                                                                               //
      { //                                                                                                      //
        if ((x >= 371) && ( x < 421)) // 'Secret button'                                                        //
        { //                                                                                                    //
          listActiveAlarms();                                                                                   //
        } //                                                                                                    //
      } //                                                                                                      //
#endif                                                                                                          //
      if ((y >= 117) && (y < 235))                                                                              //
      { //                                                                                                      //
        if ((x >= 59) && (x <= 134)) // Light Settings Page                                                     //
        { //                                                                                                    //
          tempL1OnHr = Light.On1Hr;                                                                             //
          tempL1OnMin = Light.On1Min;                                                                           //
          tempL1OffHr = Light.Dur1Min;                                                                          //
          tempL1OffMin = Light.Dur1Sec;                                                                         //
          tempL2OnHr = Light.On2Hr;                                                                             //
          tempL2OnMin = Light.On2Min;                                                                           //
          tempL2OffHr = Light.Dur2Min;                                                                          //
          tempL2OffMin = Light.Dur2Sec;                                                                         //
          screenLights();                                                                                       //
          myGLCD.setFont(Ubuntubold);                                                                           //
          myGLCD.printNumI(tempL1OnHr, 211, 207, 2);                                                            //
          myGLCD.printNumI(tempL1OffHr, 211, 342, 2);                                                           //
          myGLCD.printNumI(tempL1OnMin, 330, 207, 2, '0');                                                      //
          myGLCD.printNumI(tempL1OffMin, 330, 342, 2, '0');                                                     //
          myGLCD.printNumI(tempL2OnHr, 591, 207, 2);                                                            //
          myGLCD.printNumI(tempL2OffHr, 591, 342, 2);                                                           //
          myGLCD.printNumI(tempL2OnMin, 709, 207, 2, '0');                                                      //
          myGLCD.printNumI(tempL2OffMin, 709, 342, 2, '0');                                                     //
        } //                                                                                                    //
        if ((x >= 175) && (x <= 240)) // Temp Settings Page                                                     //
        { //                                                                                                    //
          temptempUnits = tempUnits;                                                                            //
          tempHeater = heater;                                                                                  //
          tempFan = fan;                                                                                        //
          screenTemp();                                                                                         //
        } //                                                                                                    //
        if ((x >= 270) && (x <= 362)) // Fogger Settings Page                                                   //
        { //                                                                                                    //
          tempFog1 = Fog;                                                                                       //
          screenFogger();                                                                                       //
          if (!(Fog.Enable & 0xF0))                                                                             //
          { //                                                                                                  //
            blankFogfanTime(1);                                                                                 //
          } //                                                                                                  //
          if (!(Fog.Enable & 0x0F))                                                                             //
          { //                                                                                                  //
            blankFogfanTime(2);                                                                                 //
          } //                                                                                                  //
          ffTimePrint(tempFog1, fogScreen);                                                                     //
        } //                                                                                                    //
      } //                                                                                                      //
      if (( y >= 285) && (y < 411)) { //                                                                        //
        if ((x >= 46) && (x <= 141)) // Misting Settings Page                                                   //
        { //                                                                                                    //
          tempM1 = Mist;                                                                                        //
          tempM2 = Mist2;                                                                                       //
          screenMist();                                                                                         //
          if ( mistScreen < 3)                                                                                  //
          { //                                                                                                  //
            MistTimePrint(tempM1, mistScreen);                                                                  //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            MistTimePrint(tempM2, mistScreen);                                                                  //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((x >= 171) && (x <= 237)) // Fan Settings Page                                                      //
        { //                                                                                                    //
          tempFan1 = Fan;                                                                                       //
          screenFan();                                                                                          //
          if (!(Fan.Enable & 0xF0))                                                                             //
          { //                                                                                                  //
            blankFogfanTime(1);                                                                                 //
          } //                                                                                                  //
          if (!(Fan.Enable & 0x0F))                                                                             //
          { //                                                                                                  //
            blankFogfanTime(2);                                                                                 //
          } //                                                                                                  //
          ffTimePrint(tempFan1, fanScreen);                                                                     //
        } //                                                                                                    //
        if ((x >= 270) && (x <= 370)) // Clock Page                                                             //
        { //                                                                                                    //
          screenClock();                                                                                        //
          SetDatePrint(56, 263);                                                                                //
          SetTimePrint(463, 263);                                                                               //
        } //                                                                                                    //
      } //                                                                                                      //
      if ((x >= 712) && (x <= 779) && (y >= 13) && (y <= 77)) // Screen Page                                    //
      { //                                                                                                      //
        tempAD = AutoDim;                                                                                       //
        tempTD = TouchDelay;                                                                                    //
        tempHD = HomeDelay;                                                                                     //
        tempBL = BackLight;                                                                                     //
        screenScreen();                                                                                         //
      } //                                                                                                      //
      break;                                                                                                    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                Lights Screen                                             //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 2:                                                                                                     //
      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479)) // Return Home button                           //
      { //                                                                                                      //
        byte change = 0xFF;                                                                                     //
        TouchRepeatDelay = tmpTouchRepeatDelay;                                                                 //
        if (tempL1OnHr != Light.On1Hr) { //                                                                     //
          EEPROM.write(13, tempL1OnHr);                                                                         //
          Light.On1Hr = tempL1OnHr;                                                                             //
#if debug                                                                                                       //
          Serial.println(F("Light.On1Hr -> EEPROM"));                                                           //
#endif                                                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (tempL1OnMin != Light.On1Min) { //                                                                   //
          EEPROM.write(14, tempL1OnMin);                                                                        //
          Light.On1Min = tempL1OnMin;                                                                           //
#if debug                                                                                                       //
          Serial.println(F("Light.On1Min -> EEPROM"));                                                          //
#endif                                                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (tempL1OffHr != Light.Dur1Min) { //                                                                  //
          EEPROM.write(15, tempL1OffHr);                                                                        //
          Light.Dur1Min = tempL1OffHr;                                                                          //
#if debug                                                                                                       //
          Serial.println(F("Light.Dur1Min -> EEPROM"));                                                         //
#endif                                                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (tempL1OffMin != Light.Dur1Sec) { //                                                                 //
          EEPROM.write(16, tempL1OffMin);                                                                       //
          Light.Dur1Sec = tempL1OffMin;                                                                         //
#if debug                                                                                                       //
          Serial.println(F("Light.Dur1Sec -> EEPROM"));                                                         //
#endif                                                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (tempL2OnHr != Light.On2Hr) { //                                                                     //
          EEPROM.write(17, tempL2OnHr);                                                                         //
          Light.On2Hr = tempL2OnHr;                                                                             //
#if debug                                                                                                       //
          Serial.println(F("Light.On2Hr -> EEPROM"));                                                           //
#endif                                                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (tempL2OnMin != Light.On2Min) { //                                                                   //
          EEPROM.write(18, tempL2OnMin);                                                                        //
          Light.On2Min = tempL2OnMin;                                                                           //
#if debug                                                                                                       //
          Serial.println(F("Light.On2Min -> EEPROM"));                                                          //
#endif                                                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (tempL2OffHr != Light.Dur2Min) { //                                                                  //
          EEPROM.write(19, tempL2OffHr);                                                                        //
          Light.Dur2Min = tempL2OffHr;                                                                          //
#if debug                                                                                                       //
          Serial.println(F("Light.Dur2Min -> EEPROM"));                                                         //
#endif                                                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (tempL2OffMin != Light.Dur2Sec) { //                                                                 //
          EEPROM.write(20, tempL2OffMin);                                                                       //
          Light.Dur2Sec = tempL2OffMin;                                                                         //
#if debug                                                                                                       //
          Serial.println(F("Light.Dur2Sec -> EEPROM"));                                                         //
#endif                                                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (!change)                                                                                            //
        { //                                                                                                    //
          setAlarms();                                                                                          //
        } //                                                                                                    //
        screenHome();                                                                                           //
      } //                                                                                                      //
      if ((x >= 150) && (x < 200)) // --> Hours                                                                 //
      { //                                                                                                      //
        if ((y >= 173) && (y < 218)) // --> On1 Hours +                                                         //
        { //                                                                                                    //
          if (tempL1OnHr < 23)                                                                                  //
          { //                                                                                                  //
            tempL1OnHr ++;                                                                                      //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL1OnHr = 0;                                                                                     //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 227) && (y < 272)) // --> On1 Hours -                                                         //
        { //                                                                                                    //
          if (tempL1OnHr > 0)                                                                                   //
          { //                                                                                                  //
            tempL1OnHr --;                                                                                      //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL1OnHr = 23;                                                                                    //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 306) && (y < 351)) // --> Off1 Hours +                                                        //
        { //                                                                                                    //
          if (tempL1OffHr < 23)                                                                                 //
          { //                                                                                                  //
            tempL1OffHr ++;                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL1OffHr = 0;                                                                                    //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 360) && (y < 405)) // --> Off1 Hours -                                                        //
        { //                                                                                                    //
          if (tempL1OffHr > 0)                                                                                  //
          { //                                                                                                  //
            tempL1OffHr --;                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL1OffHr = 23;                                                                                   //
          } //                                                                                                  //
        } //                                                                                                    //
        myGLCD.setFont(Ubuntubold);                                                                             //
        myGLCD.printNumI(tempL1OnHr, 211, 207, 2);                                                              //
        myGLCD.printNumI(tempL1OffHr, 211, 342, 2);                                                             //
      } //                                                                                                      //
      if ((x >= 268) && (x < 318)) // ---> Minutes                                                              //
      { //                                                                                                      //
        if ((y >= 173) && (y < 218)) // -> On1 Minutes +                                                        //
        { //                                                                                                    //
          if (tempL1OnMin < 59)                                                                                 //
          { //                                                                                                  //
            tempL1OnMin ++;                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL1OnMin = 0;                                                                                    //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 227) && (y < 272)) // -> On1 Minutes -                                                        //
        { //                                                                                                    //
          if (tempL1OnMin > 0)                                                                                  //
          { //                                                                                                  //
            tempL1OnMin --;                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL1OnMin = 59;                                                                                   //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 306) && (y < 351)) // -> Off1 Minutes +                                                       //
        { //                                                                                                    //
          if (tempL1OffMin < 59)                                                                                //
          { //                                                                                                  //
            tempL1OffMin ++;                                                                                    //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL1OffMin = 0;                                                                                   //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 360) && (y < 405)) // -> Off1 Minutes -                                                       //
        { //                                                                                                    //
          if (tempL1OffMin > 0)                                                                                 //
          { //                                                                                                  //
            tempL1OffMin --;                                                                                    //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL1OffMin = 59;                                                                                  //
          } //                                                                                                  //
        } //                                                                                                    //
        myGLCD.setFont(Ubuntubold);                                                                             //
        myGLCD.printNumI(tempL1OnMin, 330, 207, 2, '0');                                                        //
        myGLCD.printNumI(tempL1OffMin, 330, 342, 2, '0');                                                       //
      } //                                                                                                      //
      if ((x >= 530) && (x < 580)) // --> Hours                                                                 //
      { //                                                                                                      //
        if ((y >= 173) && (y < 218)) // --> On2 Hours +                                                         //
        { //                                                                                                    //
          if (tempL2OnHr < 23) //                                                                               //
          { //                                                                                                  //
            tempL2OnHr ++;                                                                                      //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL2OnHr = 0;                                                                                     //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 227) && (y < 272)) // --> On2 Hours -                                                         //
        { //                                                                                                    //
          if (tempL2OnHr > 0)                                                                                   //
          { //                                                                                                  //
            tempL2OnHr --;                                                                                      //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL2OnHr = 23;                                                                                    //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 306) && (y < 351)) // --> Off2 Hours +                                                        //
        { //                                                                                                    //
          if (tempL2OffHr < 23)                                                                                 //
          { //                                                                                                  //
            tempL2OffHr ++;                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL2OffHr = 0;                                                                                    //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 360) && (y < 405)) // --> Off2 Hours -                                                        //
        { //                                                                                                    //
          if (tempL2OffHr > 0)                                                                                  //
          { //                                                                                                  //
            tempL2OffHr --;                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL2OffHr = 23;                                                                                   //
          } //                                                                                                  //
        } //                                                                                                    //
        myGLCD.setFont(Ubuntubold);                                                                             //
        myGLCD.printNumI(tempL2OnHr, 591, 207, 2);                                                              //
        myGLCD.printNumI(tempL2OffHr, 591, 342, 2);                                                             //
      } //                                                                                                      //
      if ((x >= 650) && (x < 700)) // ---> Minutes                                                              //
      { //                                                                                                      //
        if ((y >= 173) && (y < 218)) // -> On2 Minutes +                                                        //
        { //                                                                                                    //
          if (tempL2OnMin < 59)                                                                                 //
          { //                                                                                                  //
            tempL2OnMin ++;                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL2OnMin = 0;                                                                                    //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 227) && (y < 272)) // -> On2 Minutes -                                                        //
        { //                                                                                                    //
          if (tempL2OnMin > 0)                                                                                  //
          { //                                                                                                  //
            tempL2OnMin --;                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL2OnMin = 59;                                                                                   //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 306) && (y < 351)) // -> Off2 Minutes +                                                       //
        { //                                                                                                    //
          if (tempL2OffMin < 59)                                                                                //
          { //                                                                                                  //
            tempL2OffMin ++;                                                                                    //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL2OffMin = 0;                                                                                   //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((y >= 360) && (y < 405)) // -> Off2 Minutes -                                                       //
        { //                                                                                                    //
          if (tempL2OffMin > 0)                                                                                 //
          { //                                                                                                  //
            tempL2OffMin --;                                                                                    //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempL2OffMin = 59;                                                                                  //
          } //                                                                                                  //
        } //                                                                                                    //
        myGLCD.setFont(Ubuntubold);                                                                             //
        myGLCD.printNumI(tempL2OnMin, 709, 207, 2, '0');                                                        //
        myGLCD.printNumI(tempL2OffMin, 709, 342, 2, '0');                                                       //
      } //                                                                                                      //
      break;                                                                                                    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                Temp Screen                                               //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 3:                                                                                                     //
      if ((x >= 363) && (x <= 444) && (y >= 422) && (y <= 479) || zz >= 20) // Return Home button               //
      { //                                                                                                      //
        if (zz >= 20)                                                                                           //
        { //                                                                                                    //
          EEPROM.writeWord(65, 65535); //       <- Hi/Lo temp1Lo                                                //
          EEPROM.writeWord(67, 0);     //       <- Hi/Lo temp1Hi                                                //
          EEPROM.writeWord(69, 65535); //       <- Hi/Lo temp2Lo                                                //
          EEPROM.writeWord(71, 0);     //       <- Hi/Lo temp2Hi                                                //
          EEPROM.writeWord(73, 65535); //       <- Hi/Lo temp3Lo                                                //
          EEPROM.writeWord(75, 0);     //       <- Hi/Lo temp3Hi                                                //
          EEPROM.writeWord(77, 65535); //       <- Hi/Lo hum1Lo                                                 //
          EEPROM.writeWord(79, 0);     //       <- Hi/Lo hum1Hi                                                 //
          temp1Lo       = EEPROM.readWord(65);                                                                  //
          temp1Hi       = EEPROM.readWord(67);                                                                  //
          temp2Lo       = EEPROM.readWord(69);                                                                  //
          temp2Hi       = EEPROM.readWord(71);                                                                  //
          temp3Lo       = EEPROM.readWord(73);                                                                  //
          temp3Hi       = EEPROM.readWord(75);                                                                  //
          hum1Lo        = EEPROM.readWord(77);                                                                  //
          hum1Hi        = EEPROM.readWord(79);                                                                  //
          zz = 0;                                                                                               //
          Serial.println(F("Hi/Lo Reset"));                                                                     //
        } //                                                                                                    //
        TouchRepeatDelay = tmpTouchRepeatDelay;                                                                 //
        if (tempFan != fan) { //                                                                                //
          EEPROM.writeFloat(9, tempFan);                                                                        //
          prevhiTemp = 0;                                                                                       //
          fan = tempFan;                                                                                        //
#if debug                                                                                                       //
          Serial.println(F("Fan -> EEPROM"));                                                                   //
#endif                                                                                                          //
        } //                                                                                                    //
        if (tempHeater != heater) { //                                                                          //
          EEPROM.writeFloat(5, tempHeater);                                                                     //
          prevlowTemp = 0;                                                                                      //
          heater = tempHeater;                                                                                  //
#if debug                                                                                                       //
          Serial.println(F("Heater -> EEPROM"));                                                                //
#endif                                                                                                          //
        } //                                                                                                    //
        if (temptempUnits != tempUnits) {                                                                       //
          EEPROM.write(81, temptempUnits);                                                                      //
          tempUnits = temptempUnits;                                                                            //
#if Auxdisp                                                                                                     //
          char buf[6];                                                                                          //
          if (temp1Lo != 65535)                                                                                 //
          { //                                                                                                  //
            lcd.setCursor(0, 0);                                                                                //
            if (tempUnits == 0xF0)                                                                              //
            { //                                                                                                //
              dtostrf((temp1Lo / 100.00) , 4, 1, buf);                                                          //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              float tmptemp = ((((temp1Lo / 100.00) - 32.00) / 9.00) * 5.00);                                   //
              dtostrf(tmptemp , 4, 1, buf);                                                                     //
            } //                                                                                                //
            lcd.print(buf);                                                                                     //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            lcd.setCursor(4, 0);                                                                                //
          } //                                                                                                  //
          lcd.print(F("/"));                                                                                    //
          if (temp1Hi != 0)                                                                                     //
          { //                                                                                                  //
            if (tempUnits == 0xF0)                                                                              //
            { //                                                                                                //
              dtostrf((temp1Hi / 100.00) , 4, 1, buf);                                                          //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              float tmptemp = ((((temp1Hi / 100.00) - 32.00) / 9.00) * 5.00);                                   //
              dtostrf(tmptemp , 4, 1, buf);                                                                     //
            } //                                                                                                //
            lcd.print(buf);                                                                                     //
          } //                                                                                                  //
          lcd.setCursor(15, 0);                                                                                 //
          if (tempUnits == 0xF0)                                                                                //
          { //                                                                                                  //
            lcd.print(F("F"));                                                                                  //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            lcd.print(F("C"));                                                                                  //
          } //                                                                                                  //
          if ((temp1 > 28.0) && (temp1 < 185.0))                                                                //
          { //                                                                                                  //
            if (tempUnits == 0xF0)                                                                              //
            { //                                                                                                //
              dtostrf(temp1 , 4, 1, buf);                                                                       //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              float tmptemp = (((temp1 - 32.00) / 9.00) * 5.00);                                                //
              dtostrf(tmptemp , 4, 1, buf);                                                                     //
            } //                                                                                                //
            lcd.setCursor(11, 0);                                                                               //
            lcd.print(buf);                                                                                     //
          } //                                                                                                  //
#endif                                                                                                          //
#if debug                                                                                                       //
          Serial.println(F("TempUnits -> EEPROM"));                                                             //
#endif                                                                                                          //
        } //                                                                                                    //
        screenHome();                                                                                           //
      } //                                                                                                      //
      myGLCD.setFont(Ubuntubold);                                                                               //
      if ((y >= 365) && (y < 440))                                                                              //
      { //                                                                                                      //
        if ((x >= 41) && (x < 114))                                                                             //
        { //                                                                                                    //
          myFiles.loadcpld(41, 365, 73, 75, "DegF.raw", 3, 6);                                                  //
          myFiles.loadcpld(124, 365, 73, 75, "DegCoff.raw", 3, 6);                                              //
          // Deg f button                                                                                       //
          temptempUnits = 0xF0;                                                                                 //
        } //                                                                                                    //
        if ((x >= 124) && (x < 197))                                                                            //
        { //                                                                                                    //
          myFiles.loadcpld(41, 365, 73, 75, "DegFoff.raw", 3, 6);                                               //
          myFiles.loadcpld(124, 365, 73, 75, "DegC.raw", 3, 6);                                                 //
          temptempUnits = 0x0F;                                                                                 //
          // Deg c button                                                                                       //
        } //                                                                                                    //
      } //                                                                                                      //
      if ((y >= 400) && (y < 436))                                                                              //
      { //                                                                                                      //
        if ((x >= 224) && (x < 342))                                                                            //
        { //                                                                                                    //
          // Hi/Lo reset                                                                                        //
          zz++;                                                                                                 //
        } //                                                                                                    //
      } //                                                                                                      //
      if ((x >= 520) && (x < 595))                                                                              //
      { //                                                                                                      //
        if ((y >= 208) && (y < 282)) // Low Temp +                                                              //
        { //                                                                                                    //
          tempHeater += 0.1;                                                                                    //
        } //                                                                                                    //
        if ((y >= 342) && (y < 416)) // Low Temp -                                                              //
        { //                                                                                                    //
          tempHeater -= 0.1;                                                                                    //
        } //                                                                                                    //
        char buf[6];                                                                                            //
        if (tempUnits == 0xF0)                                                                                  //
        { //                                                                                                    //
          dtostrf(tempHeater, 4, 1, buf);                                                                       //
        } //                                                                                                    //
        else                                                                                                    //
        { //                                                                                                    //
          float tmptemp = (((tempHeater - 32.00) / 9.00) * 5.00);                                               //
          dtostrf (tmptemp, 4, 1, buf);                                                                         //
        } //                                                                                                    //
        myGLCD.print(buf, 510, 296);                                                                            //
      } //                                                                                                      //
      if ((x >= 650) && (x < 725))                                                                              //
      { //                                                                                                      //
        if ((y >= 208) && (y < 282)) // High Temp +                                                             //
        { //                                                                                                    //
          tempFan += 0.1;                                                                                       //
        } //                                                                                                    //
        if ((y >= 342) && (y < 416)) // High Temp -                                                             //
        { //                                                                                                    //
          tempFan -= 0.1;                                                                                       //
        } //                                                                                                    //
        char buf[6];                                                                                            //
        if (tempUnits == 0xF0)                                                                                  //
        { //                                                                                                    //
          dtostrf(tempFan, 4, 1, buf);                                                                          //
        } //                                                                                                    //
        else                                                                                                    //
        { //                                                                                                    //
          float tmptemp = (((tempFan - 32.00) / 9.00) * 5.00);                                                  //
          dtostrf (tmptemp, 4, 1, buf);                                                                         //
        } //                                                                                                    //
        myGLCD.print(buf, 640, 296);                                                                            //
      } //                                                                                                      //
      break;                                                                                                    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                Fogger Screen                                             //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 4:                                                                                                     //
      if ((x >= 363) && (x < 445) && (y >= 422) && (y < 480)) // Return Home button                             //
      { //                                                                                                      //
        TouchRepeatDelay = tmpTouchRepeatDelay;                                                                 //
        byte change = 0xFF;                                                                                     //
        if (Fog.Enable != tempFog1.Enable)                                                                      //
        { //                                                                                                    //
          Fog.Enable = tempFog1.Enable;                                                                         //
          EEPROM.write(21, Fog.Enable);                                                                         //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.On1Hr != tempFog1.On1Hr)                                                                        //
        { //                                                                                                    //
          Fog.On1Hr = tempFog1.On1Hr;                                                                           //
          EEPROM.write(22, Fog.On1Hr);                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.On1Min != tempFog1.On1Min)                                                                      //
        { //                                                                                                    //
          Fog.On1Min = tempFog1.On1Min;                                                                         //
          EEPROM.write(23, Fog.On1Min);                                                                         //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.Dur1Min != tempFog1.Dur1Min)                                                                    //
        { //                                                                                                    //
          Fog.Dur1Min = tempFog1.Dur1Min;                                                                       //
          EEPROM.write(24, Fog.Dur1Min);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.Dur1Sec != tempFog1.Dur1Sec)                                                                    //
        { //                                                                                                    //
          Fog.Dur1Sec = tempFog1.Dur1Sec;                                                                       //
          EEPROM.write(25, Fog.Dur1Sec);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.On2Hr != tempFog1.On2Hr)                                                                        //
        { //                                                                                                    //
          Fog.On2Hr = tempFog1.On2Hr;                                                                           //
          EEPROM.write(26, Fog.On2Hr);                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.On2Min != tempFog1.On2Min)                                                                      //
        { //                                                                                                    //
          Fog.On2Min = tempFog1.On2Min;                                                                         //
          EEPROM.write(27, Fog.On2Min);                                                                         //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.Dur2Min != tempFog1.Dur2Min)                                                                    //
        { //                                                                                                    //
          Fog.Dur2Min = tempFog1.Dur2Min;                                                                       //
          EEPROM.write(28, Fog.Dur2Min);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.Dur2Sec != tempFog1.Dur2Sec)                                                                    //
        { //                                                                                                    //
          Fog.Dur2Sec = tempFog1.Dur2Sec;                                                                       //
          EEPROM.write(29, Fog.Dur2Sec);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.OnDay != tempFog1.OnDay)                                                                        //
        { //                                                                                                    //
          Fog.OnDay = tempFog1.OnDay;                                                                           //
          EEPROM.write(30, Fog.OnDay);                                                                          //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Fog.OnDay2 != tempFog1.OnDay2)                                                                      //
        { //                                                                                                    //
          Fog.OnDay2 = tempFog1.OnDay2;                                                                         //
          EEPROM.write(31, Fog.OnDay2);                                                                         //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (!change)                                                                                            //
        { //                                                                                                    //
          setAlarms();                                                                                          //
        } //                                                                                                    //
        screenHome();                                                                                           //
      } //                                                                                                      //
      if ((x >= 78) && (x < 115))                                                                               //
      { //                                                                                                      //
        if ((y >= 229) && (y < 263))                                                                            //
        { //                                                                                                    //
          if (!(tempFog1.Enable & 0xF0)) // Set Enable Fog 1                                                    //
          { //                                                                                                  //
            tempFog1.Enable = ((tempFog1.Enable & 0x0F) + 0xF0);                                                //
            fogScreen = 1;                                                                                      //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempFog1.Enable = (tempFog1.Enable & 0x0F);                                                         //
            blankFogfanTime(1);                                                                                 //
            fogScreen = 2;                                                                                      //
          } //                                                                                                  //
          ffTimePrint(tempFog1, fogScreen);                                                                     //
        } //                                                                                                    //
        if ((y >= 359) && (y < 393))                                                                            //
        { //                                                                                                    //
          if (!(tempFog1.Enable & 0x0F)) // Set Enable Fog 2                                                    //
          { //                                                                                                  //
            tempFog1.Enable = ((tempFog1.Enable & 0xF0) + 0x0F);                                                //
            fogScreen = 2;                                                                                      //
          } //                                                                                                  //
          else                                                                                                  //
          { //                                                                                                  //
            tempFog1.Enable = (tempFog1.Enable & 0xF0);                                                         //
            blankFogfanTime(2);                                                                                 //
            fogScreen = 1;                                                                                      //
          } //                                                                                                  //
          ffTimePrint(tempFog1, fogScreen);                                                                     //
        } //                                                                                                    //
      } //                                                                                                      //
      if ((tempFog1.Enable & 0xF0)) // If Enable Fog 1                                                          //
      { //                                                                                                      //
        if ((y >= 174) && (y < 218))                                                                            //
        { //                                                                                                    //
          if ((x >= 151) && (x < 200))                                                                          //
          { //                                                                                                  //
            fogScreen = 1;                                                                                      //
            if (tempFog1.On1Hr < 23)                                                                            //
            { //                                                                                                //
              tempFog1.On1Hr++;                                                                                 //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.On1Hr = 0;                                                                               //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 270) && (x < 318))                                                                          //
          { //                                                                                                  //
            fogScreen = 1;                                                                                      //
            if (tempFog1.On1Min < 59)                                                                           //
            { //                                                                                                //
              tempFog1.On1Min++;                                                                                //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.On1Min = 0;                                                                              //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 421) && (x < 470))                                                                          //
          { //                                                                                                  //
            fogScreen = 1;                                                                                      //
            if (tempFog1.Dur1Min < 59)                                                                          //
            { //                                                                                                //
              tempFog1.Dur1Min++;                                                                               //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.Dur1Min = 0;                                                                             //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 540) && (x < 588))                                                                          //
          { //                                                                                                  //
            fogScreen = 1;                                                                                      //
            if (tempFog1.Dur1Sec < 59)                                                                          //
            { //                                                                                                //
              tempFog1.Dur1Sec++;                                                                               //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.Dur1Sec = 0;                                                                             //
            } //                                                                                                //
          } //                                                                                                  //
          ffTimePrint(tempFog1, fogScreen);                                                                     //
        } //                                                                                                    //
        if ((y >= 227) && (y < 271))                                                                            //
        { //                                                                                                    //
          if ((x >= 151) && (x < 200))                                                                          //
          { //                                                                                                  //
            fogScreen = 1;                                                                                      //
            if (tempFog1.On1Hr > 0)                                                                             //
            { //                                                                                                //
              tempFog1.On1Hr--;                                                                                 //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.On1Hr = 23;                                                                              //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 270) && (x < 318))                                                                          //
          { //                                                                                                  //
            fogScreen = 1;                                                                                      //
            if (tempFog1.On1Min > 0)                                                                            //
            { //                                                                                                //
              tempFog1.On1Min--;                                                                                //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.On1Min = 59;                                                                             //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 421) && (x < 470))                                                                          //
          { //                                                                                                  //
            fogScreen = 1;                                                                                      //
            if (tempFog1.Dur1Min > 0)                                                                           //
            { //                                                                                                //
              tempFog1.Dur1Min--;                                                                               //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.Dur1Min = 59;                                                                            //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 540) && (x < 588))                                                                          //
          { //                                                                                                  //
            fogScreen = 1;                                                                                      //
            if (tempFog1.Dur1Sec > 0)                                                                           //
            { //                                                                                                //
              tempFog1.Dur1Sec--;                                                                               //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.Dur1Sec = 59;                                                                            //
            } //                                                                                                //
          } //                                                                                                  //
          ffTimePrint(tempFog1, fogScreen);                                                                     //
        } //                                                                                                    //
        if (fogScreen == 1)                                                                                     //
        { //                                                                                                    //
          if ((x >= 674) && (x < 717))                                                                          //
          { //                                                                                                  //
            if ((y >= 125) && (y < 166)) // Sun                                                                 //
            { //                                                                                                //
              tempFog1.OnDay ^= 1 << Sun;                                                                       //
            } //                                                                                                //
            if ((y >= 217) && (y < 258)) // Tue                                                                 //
            { //                                                                                                //
              tempFog1.OnDay ^= 1 << Tue;                                                                       //
            } //                                                                                                //
            if ((y >= 309) && (y < 350)) // Thu                                                                 //
            { //                                                                                                //
              tempFog1.OnDay ^= 1 << Thu;                                                                       //
            } //                                                                                                //
            if ((y >= 400) && (y < 441)) // Sat                                                                 //
            { //                                                                                                //
              tempFog1.OnDay ^= 1 << Sat;                                                                       //
            } //                                                                                                //
            ffTimePrint(tempFog1, fogScreen);                                                                   //
          } //                                                                                                  //
          if ((x >= 718) && (x < 781))                                                                          //
          { //                                                                                                  //
            if ((y >= 171) && (y < 212)) // Mon                                                                 //
            { //                                                                                                //
              tempFog1.OnDay ^= 1 << Mon;                                                                       //
            } //                                                                                                //
            if ((y >= 263) && (y < 304)) // Wed                                                                 //
            { //                                                                                                //
              tempFog1.OnDay ^= 1 << Wed;                                                                       //
            } //                                                                                                //
            if ((y >= 355) && (y < 396)) // Fri                                                                 //
            { //                                                                                                //
              tempFog1.OnDay ^= 1 << Fri;                                                                       //
            } //                                                                                                //
            ffTimePrint(tempFog1, fogScreen);                                                                   //
          } //                                                                                                  //
        } //                                                                                                    //
      } //                                                                                                      //
      if ((tempFog1.Enable & 0x0F)) // If Enable Fog 2                                                          //
      { //                                                                                                      //
        if ((y >= 308) && (y < 353))                                                                            //
        { //                                                                                                    //
          if ((x >= 151) && (x < 200))                                                                          //
          { //                                                                                                  //
            fogScreen = 2;                                                                                      //
            if (tempFog1.On2Hr < 23)                                                                            //
            { //                                                                                                //
              tempFog1.On2Hr++;                                                                                 //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.On2Hr = 0;                                                                               //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 270) && (x < 318))                                                                          //
          { //                                                                                                  //
            fogScreen = 2;                                                                                      //
            if (tempFog1.On2Min < 59)                                                                           //
            { //                                                                                                //
              tempFog1.On2Min++;                                                                                //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.On2Min = 0;                                                                              //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 421) && (x < 470))                                                                          //
          { //                                                                                                  //
            fogScreen = 2;                                                                                      //
            if (tempFog1.Dur2Min < 59)                                                                          //
            { //                                                                                                //
              tempFog1.Dur2Min++;                                                                               //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.Dur2Min = 0;                                                                             //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 540) && (x < 588))                                                                          //
          { //                                                                                                  //
            fogScreen = 2;                                                                                      //
            if (tempFog1.Dur2Sec < 59)                                                                          //
            { //                                                                                                //
              tempFog1.Dur2Sec++;                                                                               //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.Dur2Sec = 0;                                                                             //
            } //                                                                                                //
          } //                                                                                                  //
          ffTimePrint(tempFog1, fogScreen);                                                                     //
        } //                                                                                                    //
        if ((y >= 361) && (y < 405))                                                                            //
        { //                                                                                                    //
          if ((x >= 151) && (x < 200))                                                                          //
          { //                                                                                                  //
            fogScreen = 2;                                                                                      //
            if (tempFog1.On2Hr > 0)                                                                             //
            { //                                                                                                //
              tempFog1.On2Hr--;                                                                                 //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.On2Hr = 23;                                                                              //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 270) && (x < 318))                                                                          //
          { //                                                                                                  //
            fogScreen = 2;                                                                                      //
            if (tempFog1.On2Min > 0)                                                                            //
            { //                                                                                                //
              tempFog1.On2Min--;                                                                                //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.On2Min = 59;                                                                             //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 421) && (x < 470))                                                                          //
          { //                                                                                                  //
            fogScreen = 2;                                                                                      //
            if (tempFog1.Dur2Min > 0)                                                                           //
            { //                                                                                                //
              tempFog1.Dur2Min--;                                                                               //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.Dur2Min = 59;                                                                            //
            } //                                                                                                //
          } //                                                                                                  //
          if ((x >= 540) && (x < 588))                                                                          //
          { //                                                                                                  //
            fogScreen = 2;                                                                                      //
            if (tempFog1.Dur2Sec > 0)                                                                           //
            { //                                                                                                //
              tempFog1.Dur2Sec--;                                                                               //
            } //                                                                                                //
            else                                                                                                //
            { //                                                                                                //
              tempFog1.Dur2Sec = 59;                                                                            //
            } //                                                                                                //
          } //                                                                                                  //
          ffTimePrint(tempFog1, fogScreen);                                                                     //
        } //                                                                                                    //
        if (fogScreen == 2)                                                                                     //
        { //                                                                                                    //
          if ((x >= 674) && (x < 717))                                                                          //
          { //                                                                                                  //
            if ((y >= 125) && (y < 166)) // Sun                                                                 //
            { //                                                                                                //
              tempFog1.OnDay2 ^= 1 << Sun;                                                                      //
            } //                                                                                                //
            if ((y >= 217) && (y < 258)) // Tue                                                                 //
            { //                                                                                                //
              tempFog1.OnDay2 ^= 1 << Tue;                                                                      //
            } //                                                                                                //
            if ((y >= 309) && (y < 350)) // Thu                                                                 //
            { //                                                                                                //
              tempFog1.OnDay2 ^= 1 << Thu;                                                                      //
            } //                                                                                                //
            if ((y >= 400) && (y < 441)) // Sat                                                                 //
            { //                                                                                                //
              tempFog1.OnDay2 ^= 1 << Sat;                                                                      //
            } //                                                                                                //
            ffTimePrint(tempFog1, fogScreen);                                                                   //
          } //                                                                                                  //
          if ((x >= 718) && (x < 781))                                                                          //
          { //                                                                                                  //
            if ((y >= 171) && (y < 212)) // Mon                                                                 //
            { //                                                                                                //
              tempFog1.OnDay2 ^= 1 << Mon;                                                                      //
            } //                                                                                                //
            if ((y >= 263) && (y < 304)) // Wed                                                                 //
            { //                                                                                                //
              tempFog1.OnDay2 ^= 1 << Wed;                                                                      //
            } //                                                                                                //
            if ((y >= 355) && (y < 396)) // Fri                                                                 //
            { //                                                                                                //
              tempFog1.OnDay2 ^= 1 << Fri;                                                                      //
            } //                                                                                                //
            ffTimePrint(tempFog1, fogScreen);                                                                   //
          } //                                                                                                  //
        } //                                                                                                    //
      } //                                                                                                      //
      break;                                                                                                    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                               Misting Screen                                             //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 5:    // Misting screen                                                                                //
      if ((x >= 363) && (x < 445) && (y >= 422) && (y < 480)) // Return Home button                             //
      { //                                                                                                      //
        TouchRepeatDelay = tmpTouchRepeatDelay;                                                                 //
        byte change = 255;                                                                                      //
        if (Mist.Enable != tempM1.Enable)                                                                       //
        { //                                                                                                    //
          Mist.Enable = tempM1.Enable;                                                                          //
          EEPROM.write(32, Mist.Enable);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.On1Hr != tempM1.On1Hr)                                                                         //
        { //                                                                                                    //
          Mist.On1Hr = tempM1.On1Hr;                                                                            //
          EEPROM.write(33, Mist.On1Hr);                                                                         //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.On1Min != tempM1.On1Min)                                                                       //
        { //                                                                                                    //
          Mist.On1Min = tempM1.On1Min;                                                                          //
          EEPROM.write(34, Mist.On1Min);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.Dur1Min != tempM1.Dur1Min)                                                                     //
        { //                                                                                                    //
          Mist.Dur1Min = tempM1.Dur1Min;                                                                        //
          EEPROM.write(35, Mist.Dur1Min);                                                                       //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.Dur1Sec != tempM1.Dur1Sec)                                                                     //
        { //                                                                                                    //
          Mist.Dur1Sec = tempM1.Dur1Sec;                                                                        //
          EEPROM.write(36, Mist.Dur1Sec);                                                                       //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.On2Hr != tempM1.On2Hr)                                                                         //
        { //                                                                                                    //
          Mist.On2Hr = tempM1.On2Hr;                                                                            //
          EEPROM.write(37, Mist.On2Hr);                                                                         //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.On2Min != tempM1.On2Min)                                                                       //
        { //                                                                                                    //
          Mist.On2Min = tempM1.On2Min;                                                                          //
          EEPROM.write(38, Mist.On2Min);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.Dur2Min != tempM1.Dur2Min)                                                                     //
        { //                                                                                                    //
          Mist.Dur2Min = tempM1.Dur2Min;                                                                        //
          EEPROM.write(39, Mist.Dur2Min);                                                                       //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.Dur2Sec != tempM1.Dur2Sec)                                                                     //
        { //                                                                                                    //
          Mist.Dur2Sec = tempM1.Dur2Sec;                                                                        //
          EEPROM.write(40, Mist.Dur2Sec);                                                                       //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.OnDay != tempM1.OnDay)                                                                         //
        { //                                                                                                    //
          Mist.OnDay = tempM1.OnDay;                                                                            //
          EEPROM.write(41, Mist.OnDay);                                                                         //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist.OnDay2 != tempM1.OnDay2)                                                                       //
        { //                                                                                                    //
          Mist.OnDay2 = tempM1.OnDay2;                                                                          //
          EEPROM.write(42, Mist.OnDay2);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.Enable != tempM2.Enable)                                                                      //
        { //                                                                                                    //
          Mist2.Enable = tempM2.Enable;                                                                         //
          EEPROM.write(43, Mist2.Enable);                                                                       //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.On1Hr != tempM2.On1Hr)                                                                        //
        { //                                                                                                    //
          Mist2.On1Hr = tempM2.On1Hr;                                                                           //
          EEPROM.write(44, Mist2.On1Hr);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.On1Min != tempM2.On1Min)                                                                      //
        { //                                                                                                    //
          Mist2.On1Min = tempM2.On1Min;                                                                         //
          EEPROM.write(45, Mist2.On1Min);                                                                       //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.Dur1Min != tempM2.Dur1Min)                                                                    //
        { //                                                                                                    //
          Mist2.Dur1Min = tempM2.Dur1Min;                                                                       //
          EEPROM.write(46, Mist2.Dur1Min);                                                                      //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.Dur1Sec != tempM2.Dur1Sec)                                                                    //
        { //                                                                                                    //
          Mist2.Dur1Sec = tempM2.Dur1Sec;                                                                       //
          EEPROM.write(47, Mist2.Dur1Sec);                                                                      //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.On2Hr != tempM2.On2Hr)                                                                        //
        { //                                                                                                    //
          Mist2.On2Hr = tempM2.On2Hr;                                                                           //
          EEPROM.write(48, Mist2.On2Hr);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.On2Min != tempM2.On2Min)                                                                      //
        { //                                                                                                    //
          Mist2.On2Min = tempM2.On2Min;                                                                         //
          EEPROM.write(49, Mist2.On2Min);                                                                       //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.Dur2Min != tempM2.Dur2Min)                                                                    //
        { //                                                                                                    //
          Mist2.Dur2Min = tempM2.Dur2Min;                                                                       //
          EEPROM.write(50, Mist2.Dur2Min);                                                                      //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.Dur2Sec != tempM2.Dur2Sec)                                                                    //
        { //                                                                                                    //
          Mist2.Dur2Sec = tempM2.Dur2Sec;                                                                       //
          EEPROM.write(51, Mist2.Dur2Sec);                                                                      //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.OnDay != tempM2.OnDay)                                                                        //
        { //                                                                                                    //
          Mist2.OnDay = tempM2.OnDay;                                                                           //
          EEPROM.write(52, Mist2.OnDay);                                                                        //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if (Mist2.OnDay2 != tempM2.OnDay2)                                                                      //
        { //                                                                                                    //
          Mist2.OnDay2 = tempM2.OnDay2;                                                                         //
          EEPROM.write(53, Mist2.OnDay2);                                                                       //
          change = 0;                                                                                           //
        } //                                                                                                    //
        if ( !change )                                                                                          //
        { //                                                                                                    //
          setAlarms();                                                                                          //
        } //                                                                                                    //
        screenHome();                                                                                           //
      } //                                                                                                      //
      if ((x >= 183) && (x < 236))                                                                              //
      { //                                                                                                      //
        if ((y >= 209) && (y < 259))                                                                            //
        { //                                                                                                    //
          switch (mistScreen) { //                                                                              //
            case 1:                                                                                             //
              if (!(tempM1.Enable & 0xF0)) // Set Enable Mist 1                                                 //
              { //                                                                                              //
                tempM1.Enable = ((tempM1.Enable & 0x0F) + 0xF0);                                                //
              } //                                                                                              //
              else                                                                                              //
              { //                                                                                              //
                tempM1.Enable = (tempM1.Enable & 0x0F);                                                         //
              } //                                                                                              //
              MistTimePrint(tempM1, 1);                                                                         //
              break;                                                                                            //
            case 2:                                                                                             //
              if (!(tempM1.Enable & 0x0F)) // Set Enable Mist 2                                                 //
              { //                                                                                              //
                tempM1.Enable = ((tempM1.Enable & 0xF0) + 0x0F);                                                //
              } //                                                                                              //
              else                                                                                              //
              { //                                                                                              //
                tempM1.Enable = (tempM1.Enable & 0xF0);                                                         //
              } //                                                                                              //
              MistTimePrint(tempM1, 2);                                                                         //
              break;                                                                                            //
            case 3:                                                                                             //
              if (!(tempM2.Enable & 0xF0)) // Set Enable Mist 3                                                 //
              { //                                                                                              //
                tempM2.Enable = ((tempM2.Enable & 0x0F) + 0xF0);                                                //
              } //                                                                                              //
              else                                                                                              //
              { //                                                                                              //
                tempM2.Enable = (tempM2.Enable & 0x0F);                                                         //
              } //                                                                                              //
              MistTimePrint(tempM2, 3);                                                                         //
              break;                                                                                            //
            case 4:                                                                                             //
              if (!(tempM2.Enable & 0x0F))  // Set Enable Mist 4                                                //
              { //                                                                                              //
                tempM2.Enable = ((tempM2.Enable & 0xF0) + 0x0F);                                                //
              } //                                                                                              //
              else                                                                                              //
              { //                                                                                              //
                tempM2.Enable = (tempM2.Enable & 0xF0);                                                         //
              } //                                                                                              //
              MistTimePrint(tempM2, 4);                                                                         //
              break;                                                                                            //
          } //                                                                                                  //
        } //                                                                                                    //
      } //                                                                                                      //
      if ((y >= 174) && (y < 222)) // Up arrows row                                                             //
      { //                                                                                                      //
        if ((x >= 254) && (x < 308))                                                                            //
        { // Hr +1                                                                                              //
          switch (mistScreen)                                                                                   //
          { //                                                                                                  //
            case 1:                                                                                             //
              if ((tempM1.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 1 Hour +1"));                                                            //
#endif                                                                                                          //
                if (tempM1.On1Hr < 23)                                                                          //
                { //                                                                                            //
                  tempM1.On1Hr++;                                                                               //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM1.On1Hr = 0;                                                                             //
                } //                                                                                            //
                MistTimePrint(tempM1, 1);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 2:                                                                                             //
              if ((tempM1.Enable & 0x0F) == 0x0F) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 2 Hour +1"));                                                            //
#endif                                                                                                          //
                if (tempM1.On2Hr < 23)                                                                          //
                { //                                                                                            //
                  tempM1.On2Hr++;                                                                               //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM1.On2Hr = 0;                                                                             //
                } //                                                                                            //
                MistTimePrint(tempM1, 2);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 3:                                                                                             //
              if ((tempM2.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 3 Hour +1"));                                                            //
#endif                                                                                                          //
                if (tempM2.On1Hr < 23)                                                                          //
                { //                                                                                            //
                  tempM2.On1Hr++;                                                                               //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM2.On1Hr = 0;                                                                             //
                } //                                                                                            //
                MistTimePrint(tempM2, 1);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 4:                                                                                             //
              if ((tempM2.Enable & 0x0F) == 0x0F) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 4 Hour +1"));                                                            //
#endif                                                                                                          //
                if (tempM2.On2Hr < 23)                                                                          //
                { //                                                                                            //
                  tempM2.On2Hr++;                                                                               //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM2.On2Hr = 0;                                                                             //
                } //                                                                                            //
                MistTimePrint(tempM2, 2);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((x >= 368) && (x < 422))                                                                            //
        { // Min +1                                                                                             //
          switch (mistScreen)                                                                                   //
          { //                                                                                                  //
            case 1:                                                                                             //
              if ((tempM1.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 1 Min +1"));                                                             //
#endif                                                                                                          //
                if (tempM1.On1Min < 59)                                                                         //
                { //                                                                                            //
                  tempM1.On1Min++;                                                                              //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM1.On1Min = 0;                                                                            //
                } //                                                                                            //
                MistTimePrint(tempM1, 1);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 2:                                                                                             //
              if ((tempM1.Enable & 0x0F) == 0x0F) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 2 Min +1"));                                                             //
#endif                                                                                                          //
                if (tempM1.On2Min < 59)                                                                         //
                { //                                                                                            //
                  tempM1.On2Min++;                                                                              //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM1.On2Min = 0;                                                                            //
                } //                                                                                            //
                MistTimePrint(tempM1, 2);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 3:                                                                                             //
              if ((tempM2.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 3 Min +1"));                                                             //
#endif                                                                                                          //
                if (tempM2.On1Min < 59)                                                                         //
                { //                                                                                            //
                  tempM2.On1Min++;                                                                              //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM2.On1Min = 0;                                                                            //
                } //                                                                                            //
                MistTimePrint(tempM2, 1);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 4:                                                                                             //
              if ((tempM2.Enable & 0x0F) == 0x0F) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 4 Min +1"));                                                             //
#endif                                                                                                          //
                if (tempM2.On2Min < 59)                                                                         //
                { //                                                                                            //
                  tempM2.On2Min++;                                                                              //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM2.On2Min = 0;                                                                            //
                } //                                                                                            //
                MistTimePrint(tempM2, 2);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((x >= 516) && (x < 570))                                                                            //
        { // Min +1                                                                                             //
          switch (mistScreen)                                                                                   //
          { //                                                                                                  //
            case 1:                                                                                             //
              if ((tempM1.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 1 Min +1"));                                                             //
#endif                                                                                                          //
                if (tempM1.Dur1Min < 59)                                                                        //
                { //                                                                                            //
                  tempM1.Dur1Min++;                                                                             //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM1.Dur1Min = 0;                                                                           //
                } //                                                                                            //
                MistTimePrint(tempM1, 1);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 2:                                                                                             //
              if ((tempM1.Enable & 0x0F) == 0x0F) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 2 Min +1"));                                                             //
#endif                                                                                                          //
                if (tempM1.Dur2Min < 59)                                                                        //
                { //                                                                                            //
                  tempM1.Dur2Min++;                                                                             //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM1.Dur2Min = 0;                                                                           //
                } //                                                                                            //
                MistTimePrint(tempM1, 2);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 3:                                                                                             //
              if ((tempM2.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 3 Min +1"));                                                             //
#endif                                                                                                          //
                if (tempM2.Dur1Min < 59)                                                                        //
                { //                                                                                            //
                  tempM2.Dur1Min++;                                                                             //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM2.Dur1Min = 0;                                                                           //
                } //                                                                                            //
                MistTimePrint(tempM2, 1);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 4:                                                                                             //
              if ((tempM2.Enable & 0x0F) == 0x0F) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 4 Min +1"));                                                             //
#endif                                                                                                          //
                if (tempM2.Dur2Min < 59)                                                                        //
                { //                                                                                            //
                  tempM2.Dur2Min++;                                                                             //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM2.Dur2Min = 0;                                                                           //
                } //                                                                                            //
                MistTimePrint(tempM2, 2);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
          } //                                                                                                  //
        } //                                                                                                    //
        if ((x >= 647) && (x < 701))                                                                            //
        { // Sec +1                                                                                             //
          switch (mistScreen)                                                                                   //
          { //                                                                                                  //
            case 1:                                                                                             //
              if ((tempM1.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 1 Sec +1"));                                                             //
#endif                                                                                                          //
                if (tempM1.Dur1Sec < 59)                                                                        //
                { //                                                                                            //
                  tempM1.Dur1Sec++;                                                                             //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM1.Dur1Sec = 0;                                                                           //
                } //                                                                                            //
                MistTimePrint(tempM1, 1);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 2:                                                                                             //
              if ((tempM1.Enable & 0x0F) == 0x0F) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 2 Sec +1"));                                                             //
#endif                                                                                                          //
                if (tempM1.Dur2Sec < 59)                                                                        //
                { //                                                                                            //
                  tempM1.Dur2Sec++;                                                                             //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM1.Dur2Sec = 0;                                                                           //
                } //                                                                                            //
                MistTimePrint(tempM1, 2);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 3:                                                                                             //
              if ((tempM2.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 3 Sec +1"));                                                             //
#endif                                                                                                          //
                if (tempM2.Dur1Sec < 59)                                                                        //
                { //                                                                                            //
                  tempM2.Dur1Sec++;                                                                             //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM2.Dur1Sec = 0;                                                                           //
                } //                                                                                            //
                MistTimePrint(tempM2, 1);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
            case 4:                                                                                             //
              if ((tempM2.Enable & 0x0F) == 0x0F) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 4 Sec +1"));                                                             //
#endif                                                                                                          //
                if (tempM2.Dur2Sec < 59)                                                                        //
                { //                                                                                            //
                  tempM2.Dur2Sec++;                                                                             //
                } //                                                                                            //
                else                                                                                            //
                { //                                                                                            //
                  tempM2.Dur2Sec = 0;                                                                           //
                } //                                                                                            //
                MistTimePrint(tempM2, 2);                                                                       //
              } //                                                                                              //
              break;                                                                                            //
          } //                                                                                                  //
        } //                                                                                                    //
      } //                                                                                                      //
      if ((y >= 242) && (y < 290))  // Down arrows row                                                          //
      { //                                                                                                      //
        if ((x >= 254) && (x < 308))                                                                            //
        { // Hr -1                                                                                              //
          switch (mistScreen)                                                                                   //
          { //                                                                                                  //
            case 1:                                                                                             //
              if ((tempM1.Enable & 0xF0) == 0xF0) {                                                             //
#if debug                                                                                                       //
                Serial.println(F("Mist 1 Hour -1"));                                                            //
#endif                                                                                                          //
                if (tempM1.On1Hr > 0)                                                                           //
                {
                  tempM1.On1Hr--;
                }
                else
                {
                  tempM1.On1Hr = 23;
                }
                MistTimePrint(tempM1, 1);
              }
              break;
            case 2:
              if ((tempM1.Enable & 0x0F) == 0x0F) {
#if debug
                Serial.println(F("Mist 2 Hour -1"));
#endif
                if (tempM1.On2Hr > 0)
                {
                  tempM1.On2Hr--;
                }
                else
                {
                  tempM1.On2Hr = 23;
                }
                MistTimePrint(tempM1, 2);
              }
              break;
            case 3:
              if ((tempM2.Enable & 0xF0) == 0xF0) {
#if debug
                Serial.println(F("Mist 3 Hour -1"));
#endif
                if (tempM2.On1Hr > 0)
                {
                  tempM2.On1Hr--;
                }
                else
                {
                  tempM2.On1Hr = 23;
                }
                MistTimePrint(tempM2, 1);
              }
              break;
            case 4:
              if ((tempM2.Enable & 0x0F) == 0x0F) {
#if debug
                Serial.println(F("Mist 4 Hour -1"));
#endif
                if (tempM2.On2Hr > 0)
                {
                  tempM2.On2Hr--;
                }
                else
                {
                  tempM2.On2Hr = 23;
                }
                MistTimePrint(tempM2, 2);
              }
              break;
          }
        }
        if ((x >= 368) && (x < 422))
        { // Min -1
          switch (mistScreen)
          {
            case 1:
              if ((tempM1.Enable & 0xF0) == 0xF0) {
#if debug
                Serial.println(F("Mist 1 Min -1"));
#endif
                if (tempM1.On1Min > 0)
                {
                  tempM1.On1Min--;
                }
                else
                {
                  tempM1.On1Min = 59;
                }
                MistTimePrint(tempM1, 1);
              }
              break;
            case 2:
              if ((tempM1.Enable & 0x0F) == 0x0F) {
#if debug
                Serial.println(F("Mist 2 Min -1"));
#endif
                if (tempM1.On2Min > 0)
                {
                  tempM1.On2Min--;
                }
                else
                {
                  tempM1.On2Min = 59;
                }
                MistTimePrint(tempM1, 2);
              }
              break;
            case 3:
              if ((tempM2.Enable & 0xF0) == 0xF0) {
#if debug
                Serial.println(F("Mist 3 Min -1"));
#endif
                if (tempM2.On1Min > 0)
                {
                  tempM2.On1Min--;
                }
                else
                {
                  tempM2.On1Min = 59;
                }
                MistTimePrint(tempM2, 1);
              }
              break;
            case 4:
              if ((tempM2.Enable & 0x0F) == 0x0F) {
#if debug
                Serial.println(F("Mist 4 Min -1"));
#endif
                if (tempM2.On2Min > 0)
                {
                  tempM2.On2Min--;
                }
                else
                {
                  tempM2.On2Min = 59;
                }
                MistTimePrint(tempM2, 2);
              }
              break;
          }
        }
        if ((x >= 516) && (x < 570))
        { // Min -1
          switch (mistScreen)
          {
            case 1:
              if ((tempM1.Enable & 0xF0) == 0xF0) {
#if debug
                Serial.println(F("Mist 1 Min -1"));
#endif
                if (tempM1.Dur1Min > 0)
                {
                  tempM1.Dur1Min--;
                }
                else
                {
                  tempM1.Dur1Min = 59;
                }
                MistTimePrint(tempM1, 1);
              }
              break;
            case 2:
              if ((tempM1.Enable & 0x0F) == 0x0F) {
#if debug
                Serial.println(F("Mist 2 Min -1"));
#endif
                if (tempM1.Dur2Min > 0)
                {
                  tempM1.Dur2Min--;
                }
                else
                {
                  tempM1.Dur2Min = 59;
                }
                MistTimePrint(tempM1, 2);
              }
              break;
            case 3:
              if ((tempM2.Enable & 0xF0) == 0xF0) {
#if debug
                Serial.println(F("Mist 3 Min -1"));
#endif
                if (tempM2.Dur1Min > 0)
                {
                  tempM2.Dur1Min--;
                }
                else
                {
                  tempM2.Dur1Min = 59;
                }
                MistTimePrint(tempM2, 1);
              }
              break;
            case 4:
              if ((tempM2.Enable & 0x0F) == 0x0F) {
#if debug
                Serial.println(F("Mist 4 Min -1"));
#endif
                if (tempM2.Dur2Min > 0)
                {
                  tempM2.Dur2Min--;
                }
                else
                {
                  tempM2.Dur2Min = 59;
                }
                MistTimePrint(tempM2, 2);
              }
              break;
          }
        }
        if ((x >= 647) && (x < 701))
        { // Sec -1
          switch (mistScreen)
          {
            case 1:
              if ((tempM1.Enable & 0xF0) == 0xF0) {
#if debug
                Serial.println(F("Mist 1 Sec -1"));
#endif
                if (tempM1.Dur1Sec > 0)
                {
                  tempM1.Dur1Sec--;
                }
                else
                {
                  tempM1.Dur1Sec = 59;
                }
                MistTimePrint(tempM1, 1);
              }
              break;
            case 2:
              if ((tempM1.Enable & 0x0F) == 0x0F) {
#if debug
                Serial.println(F("Mist 2 Sec -1"));
#endif
                if (tempM1.Dur2Sec > 0)
                {
                  tempM1.Dur2Sec--;
                }
                else
                {
                  tempM1.Dur2Sec = 59;
                }
                MistTimePrint(tempM1, 2);
              }
              break;
            case 3:
              if ((tempM2.Enable & 0xF0) == 0xF0) {
#if debug
                Serial.println(F("Mist 3 Sec -1"));
#endif
                if (tempM2.Dur1Sec > 0)
                {
                  tempM2.Dur1Sec--;
                }
                else
                {
                  tempM2.Dur1Sec = 59;
                }
                MistTimePrint(tempM2, 1);
              }
              break;
            case 4:
              if ((tempM2.Enable & 0x0F) == 0x0F) {
#if debug
                Serial.println(F("Mist 4 Sec -1"));
#endif
                if (tempM2.Dur2Sec > 0)
                {
                  tempM2.Dur2Sec--;
                }
                else
                {
                  tempM2.Dur2Sec = 59;
                }
                MistTimePrint(tempM2, 2);
              }
              break;
          }
        }
      }
      if ((y >= 345) && (y < 403)) // Day of week row
      {
        if ((x >= 201) and (x < 261))
        {
          switch (mistScreen)
          {
            case 1:
              if ((tempM1.Enable & 0xF0) == 0xF0) {
                if ((tempM1.OnDay & (1 << Sun)) == (1 << Sun)) {
#if debug
                  Serial.println(F("Mist 1 Sun On"));
#endif
                  tempM1.OnDay = (tempM1.OnDay & ~ (1 << Sun));
                }
                else
                {
#if debug
                  Serial.println(F("Mist 1 Sun Off"));
#endif
                  tempM1.OnDay = (tempM1.OnDay | (1 << Sun));
                }
              }
              break;
            case 2:
              if ((tempM1.Enable & 0x0F) == 0x0F) {
                if ((tempM1.OnDay2 & (1 << Sun)) == (1 << Sun)) {
#if debug
                  Serial.println(F("Mist 2 Sun On"));
#endif
                  tempM1.OnDay2 = (tempM1.OnDay2 & ~ (1 << Sun));
                }
                else
                {
#if debug
                  Serial.println(F("Mist 2 Sun Off"));
#endif
                  tempM1.OnDay2 = (tempM1.OnDay2 | (1 << Sun));
                }
              }
              break;
            case 3:
              if ((tempM2.Enable & 0xF0) == 0xF0) {
                if ((tempM2.OnDay & (1 << Sun)) == (1 << Sun)) {
#if debug
                  Serial.println(F("Mist 3 Sun On"));
#endif
                  tempM2.OnDay = (tempM2.OnDay & ~ (1 << Sun));
                }
                else
                {
#if debug
                  Serial.println(F("Mist 3 Sun Off"));
#endif
                  tempM2.OnDay = (tempM2.OnDay | (1 << Sun));
                }
              }
              break;
            case 4:
              if ((tempM2.Enable & 0x0F) == 0x0F) {
                if ((tempM2.OnDay2 & (1 << Sun)) == (1 << Sun)) {
#if debug
                  Serial.println(F("Mist 4 Sun On"));
#endif
                  tempM2.OnDay2 = (tempM2.OnDay2 & ~ (1 << Sun));
                }
                else
                {
#if debug
                  Serial.println(F("Mist 4 Sun Off"));
#endif
                  tempM2.OnDay2 = (tempM2.OnDay2 | (1 << Sun));
                }
              }
              break;
          }
        }
        if ((x >= 277) and (x < 336))
        {
          if (mistScreen == 1)
          {
            if ((tempM1.Enable & 0xF0) == 0xF0) {
              if ((tempM1.OnDay & (1 << Mon)) == (1 << Mon)) {
#if debug
                Serial.println(F("Mist 1 Mon On"));
#endif
                tempM1.OnDay = (tempM1.OnDay & ~ (1 << Mon));
              }
              else
              {
#if debug
                Serial.println(F("Mist 1 Mon Off"));
#endif
                tempM1.OnDay = (tempM1.OnDay | (1 << Mon));
              }
            }
          }
          if (mistScreen == 2)
          {
            if ((tempM1.Enable & 0x0F) == 0x0F) {
              if ((tempM1.OnDay2 & (1 << Mon)) == (1 << Mon)) {
#if debug
                Serial.println(F("Mist 2 Mon On"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 & ~ (1 << Mon));
              }
              else
              {
#if debug
                Serial.println(F("Mist 2 Mon Off"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 | (1 << Mon));
              }
            }
          }
          if (mistScreen == 3)
          {
            if ((tempM2.Enable & 0xF0) == 0xF0) {
              if ((tempM2.OnDay & (1 << Mon)) == (1 << Mon)) {
#if debug
                Serial.println(F("Mist 3 Mon On"));
#endif
                tempM2.OnDay = (tempM2.OnDay & ~ (1 << Mon));
              }
              else
              {
#if debug
                Serial.println(F("Mist 3 Mon Off"));
#endif
                tempM2.OnDay = (tempM2.OnDay | (1 << Mon));
              }
            }
          }
          if (mistScreen == 4)
          {
            if ((tempM2.Enable & 0x0F) == 0x0F) {
              if ((tempM2.OnDay2 & (1 << Mon)) == (1 << Mon)) {
#if debug
                Serial.println(F("Mist 4 Mon On"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 & ~ (1 << Mon));
              }
              else
              {
#if debug
                Serial.println(F("Mist 4 Mon Off"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 | (1 << Mon));
              }
            }
          }
        }
        if ((x >= 350) and (x < 411))
        {
          if (mistScreen == 1)
          {
            if ((tempM1.Enable & 0xF0) == 0xF0) {
              if ((tempM1.OnDay & (1 << Tue)) == (1 << Tue)) {
#if debug
                Serial.println(F("Mist 1 Tue On"));
#endif
                tempM1.OnDay = (tempM1.OnDay & ~ (1 << Tue));
              }
              else
              {
#if debug
                Serial.println(F("Mist 1 Tue Off"));
#endif
                tempM1.OnDay = (tempM1.OnDay | (1 << Tue));
              }
            }
          }
          if (mistScreen == 2)
          {
            if ((tempM1.Enable & 0x0F) == 0x0F) {
              if ((tempM1.OnDay2 & (1 << Tue)) == (1 << Tue)) {
#if debug
                Serial.println(F("Mist 1 Tue On"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 & ~ (1 << Tue));
              }
              else
              {
#if debug
                Serial.println(F("Mist 1 Tue Off"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 | (1 << Tue));
              }
            }
          }
          if (mistScreen == 3)
          {
            if ((tempM2.Enable & 0xF0) == 0xF0) {
              if ((tempM2.OnDay & (1 << Tue)) == (1 << Tue)) {
#if debug
                Serial.println(F("Mist 3 Tue On"));
#endif
                tempM2.OnDay = (tempM2.OnDay & ~ (1 << Tue));
              }
              else
              {
#if debug
                Serial.println(F("Mist 3 Tue Off"));
#endif
                tempM2.OnDay = (tempM2.OnDay | (1 << Tue));
              }
            }
          }
          if (mistScreen == 4)
          {
            if ((tempM2.Enable & 0x0F) == 0x0F) {
              if ((tempM2.OnDay2 & (1 << Tue)) == (1 << Tue)) {
#if debug
                Serial.println(F("Mist 4 Tue On"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 & ~ (1 << Tue));
              }
              else
              {
#if debug
                Serial.println(F("Mist 4 Tue Off"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 | (1 << Tue));
              }
            }
          }
        }
        if ((x >= 427) and (x < 487))
        {
          if (mistScreen == 1)
          {
            if ((tempM1.Enable & 0xF0) == 0xF0) {
              if ((tempM1.OnDay & (1 << Wed)) == (1 << Wed)) {
#if debug
                Serial.println(F("Mist 1 Wed On"));
#endif
                tempM1.OnDay = (tempM1.OnDay & ~ (1 << Wed));
              }
              else
              {
#if debug
                Serial.println(F("Mist 1 Wed Off"));
#endif
                tempM1.OnDay = (tempM1.OnDay | (1 << Wed));
              }
            }
          }
          if (mistScreen == 2)
          {
            if ((tempM1.Enable & 0x0F) == 0x0F) {
              if ((tempM1.OnDay2 & (1 << Wed)) == (1 << Wed)) {
#if debug
                Serial.println(F("Mist 2 Wed On"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 & ~ (1 << Wed));
              }
              else
              {
#if debug
                Serial.println(F("Mist 2 Wed Off"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 | (1 << Wed));
              }
            }
          }
          if (mistScreen == 3)
          {
            if ((tempM2.Enable & 0xF0) == 0xF0) {
              if ((tempM2.OnDay & (1 << Wed)) == (1 << Wed)) {
#if debug
                Serial.println(F("Mist 3 Wed On"));
#endif
                tempM2.OnDay = (tempM2.OnDay & ~ (1 << Wed));
              }
              else
              {
#if debug
                Serial.println(F("Mist 3 Wed Off"));
#endif
                tempM2.OnDay = (tempM2.OnDay | (1 << Wed));
              }
            }
          }
          if (mistScreen == 4)
          {
            if ((tempM2.Enable & 0x0F) == 0x0F) {
              if ((tempM2.OnDay2 & (1 << Wed)) == (1 << Wed)) {
#if debug
                Serial.println(F("Mist 4 Wed On"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 & ~ (1 << Wed));
              }
              else
              {
#if debug
                Serial.println(F("Mist 4 Wed Off"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 | (1 << Wed));
              }
            }
          }
        }
        if ((x >= 503) and (x < 564))
        {
          if (mistScreen == 1)
          {
            if ((tempM1.Enable & 0xF0) == 0xF0) {
              if ((tempM1.OnDay & (1 << Thu)) == (1 << Thu)) {
#if debug
                Serial.println(F("Mist 1 Thu On"));
#endif
                tempM1.OnDay = (tempM1.OnDay & ~ (1 << Thu));
              }
              else
              {
#if debug
                Serial.println(F("Mist 1 Thu Off"));
#endif
                tempM1.OnDay = (tempM1.OnDay | (1 << Thu));
              }
            }
          }
          if (mistScreen == 2)
          {
            if ((tempM1.Enable & 0x0F) == 0x0F) {
              if ((tempM1.OnDay2 & (1 << Thu)) == (1 << Thu)) {
#if debug
                Serial.println(F("Mist 2 Thu On"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 & ~ (1 << Thu));
              }
              else
              {
#if debug
                Serial.println(F("Mist 2 Thu Off"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 | (1 << Thu));
              }
            }
          }
          if (mistScreen == 3)
          {
            if ((tempM2.Enable & 0xF0) == 0xF0) {
              if ((tempM2.OnDay & (1 << Thu)) == (1 << Thu)) {
#if debug
                Serial.println(F("Mist 3 Thu On"));
#endif
                tempM2.OnDay = (tempM2.OnDay & ~ (1 << Thu));
              }
              else
              {
#if debug
                Serial.println(F("Mist 3 Thu Off"));
#endif
                tempM2.OnDay = (tempM2.OnDay | (1 << Thu));
              }
            }
          }
          if (mistScreen == 4)
          {
            if ((tempM2.Enable & 0x0F) == 0x0F) {
              if ((tempM2.OnDay2 & (1 << Thu)) == (1 << Thu)) {
#if debug
                Serial.println(F("Mist 4 Thu On"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 & ~ (1 << Thu));
              }
              else
              {
#if debug
                Serial.println(F("Mist 4 Thu Off"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 | (1 << Thu));
              }
            }
          }
        }
        if ((x >= 581) and (x < 640))
        {
          if (mistScreen == 1)
          {
            if ((tempM1.Enable & 0xF0) == 0xF0) {
              if ((tempM1.OnDay & (1 << Fri)) == (1 << Fri)) {
#if debug
                Serial.println(F("Mist 1 Fri On"));
#endif
                tempM1.OnDay = (tempM1.OnDay & ~ (1 << Fri));
              }
              else
              {
#if debug
                Serial.println(F("Mist 1 Fri Off"));
#endif
                tempM1.OnDay = (tempM1.OnDay | (1 << Fri));
              }
            }
          }
          if (mistScreen == 2)
          {
            if ((tempM1.Enable & 0x0F) == 0x0F) {
              if ((tempM1.OnDay2 & (1 << Fri)) == (1 << Fri)) {
#if debug
                Serial.println(F("Mist 2 Fri On"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 & ~ (1 << Fri));
              }
              else
              {
#if debug
                Serial.println(F("Mist 2 Fri Off"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 | (1 << Fri));
              }
            }
          }
          if (mistScreen == 3)
          {
            if ((tempM2.Enable & 0xF0) == 0xF0) {
              if ((tempM2.OnDay & (1 << Fri)) == (1 << Fri)) {
#if debug
                Serial.println(F("Mist 3 Fri On"));
#endif
                tempM2.OnDay = (tempM2.OnDay & ~ (1 << Fri));
              }
              else
              {
#if debug
                Serial.println(F("Mist 3 Fri Off"));
#endif
                tempM2.OnDay = (tempM2.OnDay | (1 << Fri));
              }
            }
          }
          if (mistScreen == 4)
          {
            if ((tempM2.Enable & 0x0F) == 0x0F) {
              if ((tempM2.OnDay2 & (1 << Fri)) == (1 << Fri)) {
#if debug
                Serial.println(F("Mist 4 Fri On"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 & ~ (1 << Fri));
              }
              else
              {
#if debug
                Serial.println(F("Mist 4 Fri Off"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 | (1 << Fri));
              }
            }
          }
        }
        if ((x >= 655) and (x < 717))
        {
          if (mistScreen == 1)
          {
            if ((tempM1.Enable & 0xF0) == 0xF0) {
              if ((tempM1.OnDay & (1 << Sat)) == (1 << Sat)) {
#if debug
                Serial.println(F("Mist 1 Sat On"));
#endif
                tempM1.OnDay = (tempM1.OnDay & ~ (1 << Sat));
              }
              else
              {
#if debug
                Serial.println(F("Mist 1 Sat Off"));
#endif
                tempM1.OnDay = (tempM1.OnDay | (1 << Sat));
              }
            }
          }
          if (mistScreen == 2)
          {
            if ((tempM1.Enable & 0x0F) == 0x0F) {
              if ((tempM1.OnDay2 & (1 << Sat)) == (1 << Sat)) {
#if debug
                Serial.println(F("Mist 2 Sat On"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 & ~ (1 << Sat));
              }
              else
              {
#if debug
                Serial.println(F("Mist 2 Sat Off"));
#endif
                tempM1.OnDay2 = (tempM1.OnDay2 | (1 << Sat));
              }
            }
          }
          if (mistScreen == 3)
          {
            if ((tempM2.Enable & 0xF0) == 0xF0) {
              if ((tempM2.OnDay & (1 << Sat)) == (1 << Sat)) {
#if debug
                Serial.println(F("Mist 3 Sat On"));
#endif
                tempM2.OnDay = (tempM2.OnDay & ~ (1 << Sat));
              }
              else
              {
#if debug
                Serial.println(F("Mist 3 Sat Off"));
#endif
                tempM2.OnDay = (tempM2.OnDay | (1 << Sat));
              }
            }
          }
          if (mistScreen == 4)
          {
            if ((tempM2.Enable & 0x0F) == 0x0F) {
              if ((tempM2.OnDay2 & (1 << Sat)) == (1 << Sat)) {
#if debug
                Serial.println(F("Mist 4 Sat On"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 & ~ (1 << Sat));
              }
              else
              {
#if debug
                Serial.println(F("Mist 4 Sat Off"));
#endif
                tempM2.OnDay2 = (tempM2.OnDay2 | (1 << Sat));
              }
            }
          }
        }
        // end of day of week row, so update frogs ONCE
        switch (mistScreen)
        {
          case 1:
            dowBlank(tempM1.OnDay);
            if ((tempM1.Enable & 0xf0))
              dowFrog(tempM1.OnDay);
            break;
          case 2:
            dowBlank(tempM1.OnDay2);
            if ((tempM1.Enable & 0x0f))
              dowFrog(tempM1.OnDay2);
            break;
          case 3:
            dowBlank(tempM2.OnDay);
            if ((tempM2.Enable & 0xf0))
              dowFrog(tempM2.OnDay);
            break;
          case 4:
            dowBlank(tempM2.OnDay2);
            if ((tempM2.Enable & 0x0f))
              dowFrog(tempM2.OnDay2);
            break;
        }
      }
      if ((x >= 51) && (x < 129))  // Misting screen select column
      {
        if ((y >= 106) && (y < 184))
        {
          myFiles.load(51, 106, 78, 78, "blues.raw", 16, 0);
          MistTimePrint(tempM1, 1);
          clrMist(mistScreen);
          mistScreen = 1;
        }
        if ((y >= 193) && (y < 271))
        {
          myFiles.load(51, 193, 78, 78, "yellows.raw", 16, 0);
          MistTimePrint(tempM1, 2);
          clrMist(mistScreen);
          mistScreen = 2;
        }
        if ((y >= 281) && (y < 359))
        {
          myFiles.load(51, 281, 78, 78, "greens.raw", 16, 0);
          MistTimePrint(tempM2, 3);
          clrMist(mistScreen);
          mistScreen = 3;
        }
        if ((y >= 368) && (y < 446))
        {
          myFiles.load(51, 368, 78, 78, "greys.raw", 16, 0);
          MistTimePrint(tempM2, 4);
          clrMist(mistScreen);
          mistScreen = 4;
        }
      }
      break;

    case 6:    // Fan screen
      if ((x >= 363) && (x < 445) && (y >= 422) && (y < 480)) // Return Home button
      {
        TouchRepeatDelay = tmpTouchRepeatDelay;
        byte change = 255;
        if (Fan.Enable != tempFan1.Enable)
        {
          Fan.Enable = tempFan1.Enable;
          EEPROM.write(54, Fan.Enable);
          change = 0;
        }
        if (Fan.On1Hr != tempFan1.On1Hr)
        {
          Fan.On1Hr = tempFan1.On1Hr;
          EEPROM.write(55, Fan.On1Hr);
          change = 0;
        }
        if (Fan.On1Min != tempFan1.On1Min)
        {
          Fan.On1Min = tempFan1.On1Min;
          EEPROM.write(56, Fan.On1Min);
          change = 0;
        }
        if (Fan.Dur1Min != tempFan1.Dur1Min)
        {
          Fan.Dur1Min = tempFan1.Dur1Min;
          EEPROM.write(57, Fan.Dur1Min);
          change = 0;
        }
        if (Fan.Dur1Sec != tempFan1.Dur1Sec)
        {
          Fan.Dur1Sec = tempFan1.Dur1Sec;
          EEPROM.write(58, Fan.Dur1Sec);
          change = 0;
        }
        if (Fan.On2Hr != tempFan1.On2Hr)
        {
          Fan.On2Hr = tempFan1.On2Hr;
          EEPROM.write(59, Fan.On2Hr);
          change = 0;
        }
        if (Fan.On2Min != tempFan1.On2Min)
        {
          Fan.On2Min = tempFan1.On2Min;
          EEPROM.write(60, Fan.On2Min);
          change = 0;
        }
        if (Fan.Dur2Min != tempFan1.Dur2Min)
        {
          Fan.Dur2Min = tempFan1.Dur2Min;
          EEPROM.write(61, Fan.Dur2Min);
          change = 0;
        }
        if (Fan.Dur2Sec != tempFan1.Dur2Sec)
        {
          Fan.Dur2Sec = tempFan1.Dur2Sec;
          EEPROM.write(62, Fan.Dur2Sec);
          change = 0;
        }
        if (Fan.OnDay != tempFan1.OnDay)
        {
          Fan.OnDay = tempFan1.OnDay;
          EEPROM.write(63, Fan.OnDay);
          change = 0;
        }
        if (Fan.OnDay2 != tempFan1.OnDay2)
        {
          Fan.OnDay2 = tempFan1.OnDay2;
          EEPROM.write(64, Fan.OnDay2);
          change = 0;
        }
        if ( !change ) setAlarms();
        screenHome();
      }
      if ((x >= 78) && (x < 115))
      {
        if ((y >= 229) && (y < 263))
        {
          if (!(tempFan1.Enable & 0xF0)) // Set Enable Fog 1
          {
            tempFan1.Enable = ((tempFan1.Enable & 0x0F) + 0xF0);
            fanScreen = 1;
          }
          else
          {
            tempFan1.Enable = (tempFan1.Enable & 0x0F);
            blankFogfanTime(1);
            fanScreen = 2;
          }
          ffTimePrint(tempFan1, fanScreen);
        }
        if ((y >= 359) && (y < 393))
        {
          if (!(tempFan1.Enable & 0x0F)) // Set Enable Fan 2
          {
            tempFan1.Enable = ((tempFan1.Enable & 0xF0) + 0x0F);
            fanScreen = 2;
          }
          else
          {
            tempFan1.Enable = (tempFan1.Enable & 0xF0);
            blankFogfanTime(2);
            fanScreen = 1;
          }
          ffTimePrint(tempFan1, fanScreen);
        }
      }
      if ((tempFan1.Enable & 0xF0)) // If Enable Fan 1
      {
        if ((y >= 174) && (y < 218))
        {
          if ((x >= 151) && (x < 200))
          {
            fanScreen = 1;
            if (tempFan1.On1Hr < 23)
            {
              tempFan1.On1Hr++;
            }
            else
            {
              tempFan1.On1Hr = 0;
            }
          }
          if ((x >= 270) && (x < 318))
          {
            fanScreen = 1;
            if (tempFan1.On1Min < 59)
            {
              tempFan1.On1Min++;
            }
            else
            {
              tempFan1.On1Min = 0;
            }
          }
          if ((x >= 421) && (x < 470))
          {
            fanScreen = 1;
            if (tempFan1.Dur1Min < 59)
            {
              tempFan1.Dur1Min++;
            }
            else
            {
              tempFan1.Dur1Min = 0;
            }
          }
          if ((x >= 540) && (x < 588))
          {
            fanScreen = 1;
            if (tempFan1.Dur1Sec < 59)
            {
              tempFan1.Dur1Sec++;
            }
            else
            {
              tempFan1.Dur1Sec = 0;
            }
          }
          ffTimePrint(tempFan1, fanScreen);
        }
        if ((y >= 227) && (y < 271))
        {
          if ((x >= 151) && (x < 200))
          {
            fanScreen = 1;
            if (tempFan1.On1Hr > 0)
            {
              tempFan1.On1Hr--;
            }
            else
            {
              tempFan1.On1Hr = 23;
            }
          }
          if ((x >= 270) && (x < 318))
          {
            fanScreen = 1;
            if (tempFan1.On1Min > 0)
            {
              tempFan1.On1Min--;
            }
            else
            {
              tempFan1.On1Min = 59;
            }
          }
          if ((x >= 421) && (x < 470))
          {
            fanScreen = 1;
            if (tempFan1.Dur1Min > 0)
            {
              tempFan1.Dur1Min--;
            }
            else
            {
              tempFan1.Dur1Min = 59;
            }
          }
          if ((x >= 540) && (x < 588))
          {
            fanScreen = 1;
            if (tempFan1.Dur1Sec > 0)
            {
              tempFan1.Dur1Sec--;
            }
            else
            {
              tempFan1.Dur1Sec = 59;
            }
          }
          ffTimePrint(tempFan1, fanScreen);
        }
        if (fanScreen == 1)
        {
          if ((x >= 674) && (x < 717))
          {
            if ((y >= 125) && (y < 166)) // Sun
            {
              tempFan1.OnDay ^= 1 << Sun;
            }
            if ((y >= 217) && (y < 258)) // Tue
            {
              tempFan1.OnDay ^= 1 << Tue;
            }
            if ((y >= 309) && (y < 350)) // Thu
            {
              tempFan1.OnDay ^= 1 << Thu;
            }
            if ((y >= 400) && (y < 441)) // Sat
            {
              tempFan1.OnDay ^= 1 << Sat;
            }
            ffTimePrint(tempFan1, fanScreen);
          }
          if ((x >= 718) && (x < 781))
          {
            if ((y >= 171) && (y < 212)) // Mon
            {
              tempFan1.OnDay ^= 1 << Mon;
            }
            if ((y >= 263) && (y < 304)) // Wed
            {
              tempFan1.OnDay ^= 1 << Wed;
            }
            if ((y >= 355) && (y < 396)) // Fri
            {
              tempFan1.OnDay ^= 1 << Fri;
            }
            ffTimePrint(tempFan1, fanScreen);
          }
        }
      }
      if ((tempFan1.Enable & 0x0F)) // If Enable fan 2
      {
        if ((y >= 308) && (y < 353))
        {
          if ((x >= 151) && (x < 200))
          {
            fanScreen = 2;
            if (tempFan1.On2Hr < 23)
            {
              tempFan1.On2Hr++;
            }
            else
            {
              tempFan1.On2Hr = 0;
            }
          }
          if ((x >= 270) && (x < 318))
          {
            fanScreen = 2;
            if (tempFan1.On2Min < 59)
            {
              tempFan1.On2Min++;
            }
            else
            {
              tempFan1.On2Min = 0;
            }
          }
          if ((x >= 421) && (x < 470))
          {
            fanScreen = 2;
            if (tempFan1.Dur2Min < 59)
            {
              tempFan1.Dur2Min++;
            }
            else
            {
              tempFan1.Dur2Min = 0;
            }
          }
          if ((x >= 540) && (x < 588))
          {
            fanScreen = 2;
            if (tempFan1.Dur2Sec < 59)
            {
              tempFan1.Dur2Sec++;
            }
            else
            {
              tempFan1.Dur2Sec = 0;
            }
          }
          ffTimePrint(tempFan1, fanScreen);
        }
        if ((y >= 361) && (y < 405))
        {
          if ((x >= 151) && (x < 200))
          {
            fanScreen = 2;
            if (tempFan1.On2Hr > 0)
            {
              tempFan1.On2Hr--;
            }
            else
            {
              tempFan1.On2Hr = 23;
            }
          }
          if ((x >= 270) && (x < 318))
          {
            fanScreen = 2;
            if (tempFan1.On2Min > 0)
            {
              tempFan1.On2Min--;
            }
            else
            {
              tempFan1.On2Min = 59;
            }
          }
          if ((x >= 421) && (x < 470))
          {
            fanScreen = 2;
            if (tempFan1.Dur2Min > 0)
            {
              tempFan1.Dur2Min--;
            }
            else
            {
              tempFan1.Dur2Min = 59;
            }
          }
          if ((x >= 540) && (x < 588))
          {
            fanScreen = 2;
            if (tempFan1.Dur2Sec > 0)
            {
              tempFan1.Dur2Sec--;
            }
            else
            {
              tempFan1.Dur2Sec = 59;
            }
          }
          ffTimePrint(tempFan1, fanScreen);
        }
        if (fanScreen == 2)
        {
          if ((x >= 674) && (x < 717))
          {
            if ((y >= 125) && (y < 166)) // Sun
            {
              tempFan1.OnDay2 ^= 1 << Sun;
            }
            if ((y >= 217) && (y < 258)) // Tue
            {
              tempFan1.OnDay2 ^= 1 << Tue;
            }
            if ((y >= 309) && (y < 350)) // Thu
            {
              tempFan1.OnDay2 ^= 1 << Thu;
            }
            if ((y >= 400) && (y < 441)) // Sat
            {
              tempFan1.OnDay2 ^= 1 << Sat;
            }
            ffTimePrint(tempFan1, fanScreen);
          }
          if ((x >= 718) && (x < 781))
          {
            if ((y >= 171) && (y < 212)) // Mon
            {
              tempFan1.OnDay2 ^= 1 << Mon;
            }
            if ((y >= 263) && (y < 304)) // Wed
            {
              tempFan1.OnDay2 ^= 1 << Wed;
            }
            if ((y >= 355) && (y < 396)) // Fri
            {
              tempFan1.OnDay2 ^= 1 << Fri;
            }
            ffTimePrint(tempFan1, fanScreen);
          }
        }
      }
      break;

    case 7:    // Clock screen
      if ((x >= 363) && (x <= 444) && (y >= 422) && (y < 480)) // Return Home button
      {
        TouchRepeatDelay = tmpTouchRepeatDelay;
        screenHome();
      }
      if ((y >= 175) && (y < 250)) {  // 'Increase' row
        if ((x >= 55) && (x < 130)) { // 'Months'
          //Serial.println(F("Month+"));
          if (tmpRTC.Month < 12)
          {
            tmpRTC.Month ++;
            if ((tmpRTC.Month == 9 || tmpRTC.Month == 4 || tmpRTC.Month == 6 ||
                 tmpRTC.Month == 11) && tmpRTC.Day > 30)
            {
              tmpRTC.Day = 30;
            }
            if (tmpRTC.Month == 2 && tmpRTC.Day > 28)
            {
              tmpRTC.Day = 28;
            }
          }
          else
          {
            tmpRTC.Month = 1;
          }
        }
        if ((x >= 165) && (x < 240)) { // 'Days'
          //Serial.println(F("Days+"));
          if (((tmpRTC.Day < 31) && (tmpRTC.Month != 9 && tmpRTC.Month != 4 &&
                                     tmpRTC.Month != 6 && tmpRTC.Month != 11 &&  tmpRTC.Month != 2)) ||
              ((tmpRTC.Day < 30) && (tmpRTC.Month == 9 || tmpRTC.Month == 4 ||
                                     tmpRTC.Month == 6 || tmpRTC.Month == 11 || tmpRTC.Month != 2)) ||
              ((tmpRTC.Day < 28) && (tmpRTC.Month == 2)) || ((tmpRTC.Day < 29) &&
                  (tmpRTC.Month == 2) && (((float)((tmpRTC.Year + 1970.0) / 4.0)
                                           - (int)((tmpRTC.Year + 1970.0) / 4.0) == 0.0 )
                                          || ((float)((tmpRTC.Year + 1970.0) / 400.0) -
                                              (int)((tmpRTC.Year + 1970.0) / 400.0) == 0.0 )) &&
                  ((float)((tmpRTC.Year + 1970.0) / 100.0) - (int)((tmpRTC.Year + 1970.0) / 100.0) != 0.0 )))
          {
            tmpRTC.Day ++;
          }
          else
          {
            tmpRTC.Day = 1;
          }
        }
        if ((x >= 275) && (x < 350)) { // 'Years'
          //Serial.println(F("Years+"));
          tmpRTC.Year ++;
        }
        if ((x >= 450) && (x < 525)) { // '24Hrs'
          if (tmpRTC.Hour < 23)
          {
            tmpRTC.Hour ++;
          }
          else
          {
            tmpRTC.Hour = 0;
          }
        }
        if ((x >= 560) && (x < 635)) { // Minutes'
          if (tmpRTC.Minute < 59)
          {
            tmpRTC.Minute ++;
          }
          else
          {
            tmpRTC.Minute = 0;
          }
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
          if (tmpRTC.Month > 1)
          {
            tmpRTC.Month --;
            if ((tmpRTC.Month == 9 || tmpRTC.Month == 4 || tmpRTC.Month == 6 || tmpRTC.Month == 11) && tmpRTC.Day > 30) {
              tmpRTC.Day = 30;
            }
            if (tmpRTC.Month == 2 && tmpRTC.Day > 28) {
              tmpRTC.Day = 28;
            }
          }
          else
          {
            tmpRTC.Month = 12;
          }
        }
        if ((x >= 165) && (x < 240)) { // 'Days'
          //Serial.println(F("Days-"));
          if (tmpRTC.Day > 1)
          {
            tmpRTC.Day --;
          }
          else
          {
            if (tmpRTC.Month == 9 || tmpRTC.Month == 4 || tmpRTC.Month == 6 || tmpRTC.Month == 11) {
              tmpRTC.Day = 30;
            }
            if (tmpRTC.Month == 1 || tmpRTC.Month == 3 || tmpRTC.Month == 5 || tmpRTC.Month == 7 || tmpRTC.Month == 8 || tmpRTC.Month == 10 || tmpRTC.Month == 12 ) {
              tmpRTC.Day = 31;
            }
            if (tmpRTC.Month == 2) {
              tmpRTC.Day = 28;
            }
          }
        }
        if ((x >= 275) && (x < 350)) { // 'Years'
          //Serial.println(F("Years-"));
          tmpRTC.Year --;
        }
        if ((x >= 450) && (x < 525)) { // '24Hrs'
          if (tmpRTC.Hour > 0 )
          {
            tmpRTC.Hour --;
          }
          else
          {
            tmpRTC.Hour = 23;
          }
        }
        if ((x >= 560) && (x < 635)) { // Minutes'
          if (tmpRTC.Minute > 0 )
          {
            tmpRTC.Minute --;
          }
          else
          {
            tmpRTC.Minute = 59;
          }
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
          setTime(xo);
          setAlarms();
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
      if ((x >= 363) && (x <= 444) && (y >= 422) && (y < 480)) // Return Home button
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
          EEPROM.write(2, BackLight);
          //BackLight = tempBL;
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
          if (BackLight < 16) {
            BackLight += 1;
          }
        }
        if ((y >= 365 && y < 435)) {  // 'Bright -'
          if (BackLight > 2) {
            BackLight -= 1;
          }
        }
        myGLCD.setFont(Ubuntubold);
        myGLCD.printNumI(tempHD, 338, 167, 2);
        myGLCD.printNumI(BackLight, 313, 340, 2);
      }
      break;
    case 9:    // Alarm screen
      if (((x >= 365) && (x <= 440)) && ((y >= 420) && (y < 480))) // Return Home button
      {
        screenHome();
      }
      break;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  ALL OTHER ROUTINES //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
time_t syncProvider()
{
#if(DS1307 == 0)
  hourcount++;
  if (hourcount > 60)
  {
    hourcount = 0;
    time_t mynow = now();
    mynow++;
    setTime(mynow);
    setAlarms();
#if debug
    Serial.println(F("Add 1 second per hour"));
#endif
  }
#endif
  //setAlarms();
  //this does the same thing as RTC_DS1307::get()
  //#if(DS1307)
  return rtc.now().unixtime();
  //#endif
}
////////////////////////////////////////////////////////////////////////////////////////////
void updateTimeDate()
{
  ubuntuRed();
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
    //setTime(timeNow);
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
  strcat(chDate, Month[month(timeNow)]);
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
  strcat(chDate, Month[tmpRTC.Month]);
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
//                         Misting Screen supporting functions                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////
void clrMist(byte mnow)
{
  switch (mnow)
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
    case 4:
      myFiles.load(51, 368, 78, 78, "grey.raw", 16, 0);
      break;
  }
}

void MistTimePrint(SCHEDULE mist, byte mscreen)
{
  byte stH, stM, duM, duS;
  switch (mscreen) {
    case 1:
      if (!(mist.Enable & 0xf0))
      {
#if debug
        Serial.println(F("Mistscreen 1 AND disbled"));
#endif
        blankMistTime();
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(187, 216, 231, 251);
        dowBlank(0);
      }
      else
      {
#if debug
        Serial.println(F("Mistscreen 1 AND enabled"));
#endif
        stH = mist.On1Hr;
        stM = mist.On1Min;
        duM = mist.Dur1Min;
        duS = mist.Dur1Sec;
        formatMistTime(stH, stM, duM, duS);
        myGLCD.drawBitmap (187, 216, 45, 36, frog);
        dowBlank(mist.OnDay);
        dowFrog(mist.OnDay);
      }
      break;
    case 2:
      if (!(mist.Enable & 0x0f))
      {
#if debug
        Serial.println(F("Mistscreen 2 AND disabled"));
#endif
        blankMistTime();
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(187, 216, 231, 251);
        dowBlank(0);
      }
      else
      {
#if debug
        Serial.println(F("Mistscreen 2 AND enabled"));
#endif
        stH = mist.On2Hr;
        stM = mist.On2Min;
        duM = mist.Dur2Min;
        duS = mist.Dur2Sec;
        formatMistTime( stH, stM, duM, duS);
        myGLCD.drawBitmap (187, 216, 45, 36, frog);
        dowBlank(mist.OnDay2);
        dowFrog(mist.OnDay2);
      }
      break;
    case 3:
      if (!(mist.Enable & 0xf0))
      {
#if debug
        Serial.println(F("Mistscreen 3 AND disabled"));
#endif
        blankMistTime();
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(187, 216, 231, 251);
        dowBlank(0);
      }
      else
      {
#if debug
        Serial.println(F("Mistscreen 3 AND enabled"));
#endif
        stH = mist.On1Hr;
        stM = mist.On1Min;
        duM = mist.Dur1Min;
        duS = mist.Dur1Sec;
        formatMistTime( stH, stM, duM, duS);
        myGLCD.drawBitmap (187, 216, 45, 36, frog);
        dowBlank(mist.OnDay);
        dowFrog(mist.OnDay);
      }
      break;
    case 4:
      if (!(mist.Enable & 0x0f))
      {
#if debug
        Serial.println(F("Mistscreen 4 AND disabled"));
#endif
        blankMistTime();
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(187, 216, 231, 251);
        dowBlank(0);
      }
      else
      {
#if debug
        Serial.println(F("Mistscreen 4 AND enabled"));
#endif
        stH = mist.On2Hr;
        stM = mist.On2Min;
        duM = mist.Dur2Min;
        duS = mist.Dur2Sec;
        formatMistTime( stH, stM, duM, duS);
        myGLCD.drawBitmap (187, 216, 45, 36, frog);
        dowBlank(mist.OnDay2);
        dowFrog(mist.OnDay2);
      }
      break;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//                  Misting Screen time printing and blanking support routines                   //
///////////////////////////////////////////////////////////////////////////////////////////////////
void formatMistTime(byte Hr, byte Min, byte dMin, byte dSec)
{
  ubuntuRed();
  myGLCD.printNumI(Hr, 314, 216, 2, ' ');
  myGLCD.printNumI(Min, 428, 216, 2, '0');
  myGLCD.printNumI(dMin, 576, 216, 2 , ' ');
  myGLCD.printNumI(dSec, 707, 216, 2, '0');
}
void blankMistTime()
{
  myGLCD.setFont(Ubuntubold);
  myGLCD.print("  ", 314, 216);
  myGLCD.print("  ", 428, 216);
  myGLCD.print("  ", 576, 216);
  myGLCD.print("  ", 707, 216);
}
void dowFrog(byte dow)
{
  if (dow & 0x01) myGLCD.drawBitmap (209, 357, 45, 36, frog);
  if (dow & 0x02) myGLCD.drawBitmap (285, 357, 45, 36, frog);
  if (dow & 0x04) myGLCD.drawBitmap (358, 357, 45, 36, frog);
  if (dow & 0x08) myGLCD.drawBitmap (434, 357, 45, 36, frog);
  if (dow & 0x10) myGLCD.drawBitmap (511, 357, 45, 36, frog);
  if (dow & 0x20) myGLCD.drawBitmap (589, 357, 45, 36, frog);
  if (dow & 0x40) myGLCD.drawBitmap (663, 357, 45, 36, frog);
}
void dowBlank(byte dow)
{
  myGLCD.setColor(0, 0, 0);
  if (!(dow & 0x01)) myGLCD.fillRect(209, 357, 253, 392);
  if (!(dow & 0x02)) myGLCD.fillRect(285, 357, 328, 392);
  if (!(dow & 0x04)) myGLCD.fillRect(358, 357, 402, 392);
  if (!(dow & 0x08)) myGLCD.fillRect(434, 357, 479, 392);
  if (!(dow & 0x10)) myGLCD.fillRect(511, 357, 555, 392);
  if (!(dow & 0x20)) myGLCD.fillRect(589, 357, 632, 392);
  if (!(dow & 0x40)) myGLCD.fillRect(663, 357, 708, 392);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//                         Fan/Fog Screen time printing and blanking routine                     //
///////////////////////////////////////////////////////////////////////////////////////////////////
void ffTimePrint(SCHEDULE fanfog, byte ffscreen)
{
#if debug
  Serial.print(F("Enable: "));
  Serial.println(fanfog.Enable, HEX);
  Serial.print(F("OnDay1 : "));
  Serial.println(fanfog.OnDay, HEX);
  Serial.print(F("OnDay2 : "));
  Serial.println(fanfog.OnDay2, HEX);
#endif
  byte stH, stM, duM, duS, stH2, stM2, duM2, duS2;
  stH = fanfog.On1Hr;
  stM = fanfog.On1Min;
  duM = fanfog.Dur1Min;
  duS = fanfog.Dur1Sec;
  stH2 = fanfog.On2Hr;
  stM2 = fanfog.On2Min;
  duM2 = fanfog.Dur2Min;
  duS2 = fanfog.Dur2Sec;
  myGLCD.setColor(0, 0, 0);
  switch (ffscreen) {
    case 1:
      if ((fanfog.Enable & 0x0F) == 0x00)
      {
        myGLCD.fillRect(79, 363, 113, 389); // 2 Clear
      }
      else
      {
        myGLCD.drawBitmap (79, 363, 35, 27, smfrog);
      }
      if (!(fanfog.Enable & 0xF0))
      {
#if debug
        Serial.println(F("FanFog screen 1 AND disbled"));
#endif
        blankFogfanTime(ffscreen);
        myGLCD.fillRect(79, 233, 113, 259); // 1 Clear
        ffBlank(0);
      }
      else
      {
#if debug
        Serial.println(F("FanFog screen 1 AND enabled"));
#endif
        if ((fanfog.Enable & 0x0F))
        {
          formatFogfanTime(stH2, stM2, duM2, duS2, 2);
        }
        else
        {
          blankFogfanTime(2);
        }
        formatFogfanTime(stH, stM, duM, duS, 1);
        myGLCD.drawBitmap (79, 233, 35, 27, smfrog);
        ffBlank(fanfog.OnDay);
        ffFrog(fanfog.OnDay);
      }
      dowBoxes(33, 207, 30);
      break;
    case 2:
      if ((fanfog.Enable & 0xF0) == 0x00)
      {
        myGLCD.fillRect(79, 233, 113, 259); // 1 clear
      }
      else
      {
        myGLCD.drawBitmap (79, 233, 35, 27, smfrog);
      }
      if (!(fanfog.Enable & 0x0f))
      {
#if debug
        Serial.println(F("FanFog screen 2 AND disabled"));
#endif
        blankFogfanTime(ffscreen);
        myGLCD.fillRect(79, 363, 113, 389); // 2 Clear
        ffBlank(0);
      }
      else
      {
#if debug
        Serial.println(F("FanFog screen 2 AND enabled"));
#endif
        if ((fanfog.Enable & 0xF0))
        {
          formatFogfanTime(stH, stM, duM, duS, 1);
        }
        else
        {
          blankFogfanTime(1);
        }
        formatFogfanTime(stH2, stM2, duM2, duS2, 2);
        myGLCD.drawBitmap (79, 363, 35, 27, smfrog);
        ffBlank(fanfog.OnDay2);
        ffFrog(fanfog.OnDay2);
      }
      dowBoxes(247, 5, 4);
      break;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//                  Fan/Fog Screen time printing and blanking support routines                   //
///////////////////////////////////////////////////////////////////////////////////////////////////
void formatFogfanTime(byte Hr, byte Min, byte dMin, byte dSec, byte ffscreen)
{
  ubuntuRed();
  switch (ffscreen)
  {
    case 1:
      myGLCD.printNumI(Hr, 210, 210, 2, ' ');
      myGLCD.printNumI(Min, 328, 210, 2, '0');
      myGLCD.printNumI(dMin, 480, 210, 2 , ' ');
      myGLCD.printNumI(dSec, 598, 210, 2, '0');
      break;
    case 2:
      myGLCD.printNumI(Hr, 210, 344, 2, ' ');
      myGLCD.printNumI(Min, 328, 344, 2, '0');
      myGLCD.printNumI(dMin, 480, 344, 2 , ' ');
      myGLCD.printNumI(dSec, 598, 344, 2, '0');
      break;
  }
}
void blankFogfanTime(byte ffscreen)
{
  myGLCD.setFont(Ubuntubold);
  switch (ffscreen)
  {
    case 1:
      myGLCD.print("  ", 210, 210);
      myGLCD.print("  ", 328, 210);
      myGLCD.print("  ", 480, 210);
      myGLCD.print("  ", 598, 210);
      break;
    case 2:
      myGLCD.print("  ", 210, 344);
      myGLCD.print("  ", 328, 344);
      myGLCD.print("  ", 480, 344);
      myGLCD.print("  ", 598, 344);
      break;
  }
}
void ffFrog(byte dow)
{
  if (dow & 0x01) myGLCD.drawBitmap (678, 133, 35, 27, smfrog);
  if (dow & 0x02) myGLCD.drawBitmap (722, 179, 35, 27, smfrog);
  if (dow & 0x04) myGLCD.drawBitmap (678, 225, 35, 27, smfrog);
  if (dow & 0x08) myGLCD.drawBitmap (722, 271, 35, 27, smfrog);
  if (dow & 0x10) myGLCD.drawBitmap (678, 317, 35, 27, smfrog);
  if (dow & 0x20) myGLCD.drawBitmap (722, 363, 35, 27, smfrog);
  if (dow & 0x40) myGLCD.drawBitmap (678, 408, 35, 27, smfrog);
}
void ffBlank(byte dow)
{
  myGLCD.setColor(0, 0, 0);
  if (!(dow & 0x01)) myGLCD.fillRect(678, 133, 712, 159);
  if (!(dow & 0x02)) myGLCD.fillRect(722, 179, 756, 205);
  if (!(dow & 0x04)) myGLCD.fillRect(678, 225, 712, 251);
  if (!(dow & 0x08)) myGLCD.fillRect(722, 271, 756, 297);
  if (!(dow & 0x10)) myGLCD.fillRect(678, 317, 712, 343);
  if (!(dow & 0x20)) myGLCD.fillRect(722, 363, 756, 389);
  if (!(dow & 0x40)) myGLCD.fillRect(678, 408, 712, 434);
}
void dowBoxes(byte red, byte green, byte blue)
{
  myGLCD.setColor(red, green, blue);  // 33,207,30 = green // 247,5,4 = red
  subBox(674, 125);
  subBox(718, 171);
  subBox(674, 217);
  subBox(718, 263);
  subBox(674, 309);
  subBox(718, 355);
  subBox(674, 400);
}
void subBox(short x, short y)
{
  myGLCD.drawLine(x, y, x + 42, y);
  for (byte yp = 1; yp < 4; yp++)
  {
    myGLCD.drawLine(x, y + yp, x + 42, y + yp);
    myGLCD.drawLine(x, y + 37 + yp, x + 42, y + 37 + yp);
    myGLCD.drawLine(x + 39 + yp, y + 4, x + 39 + yp, y + 37);
    myGLCD.drawLine(x + yp - 1, y + 4, x + yp - 1, y + 37);
  }
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
      setAlarms();
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
/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      DS18b20 TEMP SENSORS                                       //
/////////////////////////////////////////////////////////////////////////////////////////////////////
void tempRead(float & temp1a, float & temp2a, float & temp3a)
{
  byte numSensors = sensors.getDeviceCount();
  temp1a = 0.0, temp2a = 0.0, temp3a = 0.0;
  if (numSensors >= 1)
  {
    temp1a = sensors.getTempFByIndex(0);
    if (temp1a == 185.0) temp1a = 0.0;
  }
  if (numSensors >= 2)
  {
    temp2a = sensors.getTempFByIndex(1);
    if (temp2a == 185.0) temp2a = 0.0;
  }
  if (numSensors >= 3)
  {
    temp3a = sensors.getTempFByIndex(2);
    if (temp3a == 185.0) temp3a = 0.0;
  }
  sensors.requestTemperatures();
  tempTime = now();
}
////////////////////////////////////////////////////////////////////////////////////////////
//                             DHT22 Humidity Reading                                     //
////////////////////////////////////////////////////////////////////////////////////////////
void humRead(float & hum1a)
{
  int chk = DHT.read22(DHTPIN);
  if (chk == DHTLIB_OK) {
    hum1a = DHT.humidity;
    humidTime = now();
  }
}
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    Relay handlers    /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void procRelays(byte relay)
{
  char relays[][10] = {"swon.raw" , "swoff.raw" };
  if (relay != prevRelay)
  {
    if ((relay & (1 << pwrLight2)) == (1 << pwrLight2)) // Lights 2
    {
      myFiles.loadcpld(555, 418, 22, 22, relays[1], 1, 16);
    }
    else
    {
      myFiles.loadcpld(555, 418, 22, 22, relays[0], 1, 16);
    }
    if ((relay & (1 << pwrLight1)) == (1 << pwrLight1)) // Lights 1
    {
      myFiles.loadcpld(580, 418, 22, 22, relays[1], 1, 16);
    }
    else
    {
      myFiles.loadcpld(580, 418, 22, 22, relays[0], 1, 16);
    }
    if ((relay & (1 << pwrTemp1)) == (1 << pwrTemp1)) // Temp 1
    {
      myFiles.loadcpld(614, 418, 22, 22, relays[1], 1, 16);
    }
    else
    {
      myFiles.loadcpld(614, 418, 22, 22, relays[0], 1, 16);
    }
    if ((relay & (1 << pwrFogger1)) == (1 << pwrFogger1)) // Fog 1
    {
      myFiles.loadcpld(655, 418, 22, 22, relays[1], 1, 16);
    }
    else
    {
      myFiles.loadcpld(655, 418, 22, 22, relays[0], 1, 16);
    }
    if ((relay & (1 << pwrMisting1)) == (1 << pwrMisting1)) // Mist 1
    {
      myFiles.loadcpld(694, 418, 22, 22, relays[1], 1, 16);
    }
    else
    {
      myFiles.loadcpld(694, 418, 22, 22, relays[0], 1, 16);
    }
    if ((relay & (1 << pwrFan1)) == (1 << pwrFan1)) // Fan 1
    {
      myFiles.loadcpld(732, 418, 22, 22, relays[1], 1, 16);
    }
    else
    {
      myFiles.loadcpld(732, 418, 22, 22, relays[0], 1, 16);
    }
    prevRelay = relay;
    testRelays();
  }
}
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    ALARM HANDLERS/   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void setAlarms()
{
  if (alarmset == 255)
  {
#if debug
    Serial.println(F("Alarms set"));
    //    digitalClockDisplay();
#endif
    alarmset = 0;
  }
  else
  {
    for ( byte q = 0; q < 116; q++)
    {
      Alarm.free(alarms[q]);
      Alarm.disable(alarms[q]);
      alarms[q] = 255;
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  alarms[0] = Alarm.alarmRepeat(Light.On1Hr, Light.On1Min, 0, Lights1On);                                //
  alarms[1] = Alarm.alarmRepeat(Light.Dur1Min, Light.Dur1Sec, 0, Lights1Off);                            //
  alarms[2] = Alarm.alarmRepeat(Light.On2Hr, Light.On2Min, 0, Lights2On);                                //
  alarms[3] = Alarm.alarmRepeat(Light.Dur2Min, Light.Dur2Sec, 0, Lights2Off);                            //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Fog.Enable & 0xF0) {
    unsigned long tmpOff = mySecs(0, Fog.On1Hr, Fog.On1Min, 0) + mySecs(0 , 0, Fog.Dur1Min, Fog.Dur1Sec);
    byte hr, mn;
    unsigned long tmpOff2;
    char buf[60];
    if (tmpOff > daysec)
    {
      Serial.println(F("Fog 1 off next day"));
      tmpOff2 = tmpOff - daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      Serial.println(F("Fog 1 off same day"));
      tmpOff2 = tmpOff;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }

    if (Fog.OnDay & 1 << Sun)
    {
      if (tmpOff < daysec)
      {
        alarms[4] = Alarm.alarmRepeat(dowSunday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[5] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[4] = Alarm.alarmRepeat(dowSunday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[5] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay & 1 << Mon)
    {
      if (tmpOff < daysec)
      {
        alarms[6] = Alarm.alarmRepeat(dowMonday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[7] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, FogOff);
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[6] = Alarm.alarmRepeat(dowMonday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[7] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay & 1 << Tue)
    {
      if (tmpOff < daysec)
      {
        alarms[8] = Alarm.alarmRepeat(dowTuesday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[9] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, FogOff);
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[8] = Alarm.alarmRepeat(dowTuesday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[9] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay & 1 << Wed)
    {
      if (tmpOff < daysec)
      {
        alarms[10] = Alarm.alarmRepeat(dowWednesday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[11] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, FogOff);
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[10] = Alarm.alarmRepeat(dowWednesday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[11] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay & 1 << Thu)
    {
      if (tmpOff < daysec)
      {
        alarms[12] = Alarm.alarmRepeat(dowThursday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[13] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, FogOff);
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[12] = Alarm.alarmRepeat(dowThursday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[13] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay & 1 << Fri)
    {
      if (tmpOff < daysec)
      {
        alarms[14] = Alarm.alarmRepeat(dowFriday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[15] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, FogOff);
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[14] = Alarm.alarmRepeat(dowFriday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[15] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay & 1 << Sat)
    {
      if (tmpOff < daysec)
      {
        alarms[16] = Alarm.alarmRepeat(dowSaturday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[17] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, FogOff);
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[16] = Alarm.alarmRepeat(dowSaturday, Fog.On1Hr, Fog.On1Min, 0, FogOn); //
        alarms[17] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Fog.On1Hr, Fog.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Fog.Enable & 0x0F) {
    unsigned long tmpOff = mySecs(0, Fog.On2Hr, Fog.On2Min, 0) + mySecs(0 , 0, Fog.Dur2Min, Fog.Dur2Sec);
    byte hr, mn;
    unsigned long tmpOff2;
    char buf[60];
    if (tmpOff > daysec)
    {
      Serial.println(F("Fog 2 off next day"));
      tmpOff2 = tmpOff - daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      Serial.println(F("Fog 2 off same day"));
      tmpOff2 = tmpOff;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    if (Fog.OnDay2 & 1 << Sun)
    {
      if (tmpOff < daysec)
      {
        alarms[18] = Alarm.alarmRepeat(dowSunday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[19] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[18] = Alarm.alarmRepeat(dowSunday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[19] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay2 & 1 << Mon)
    {
      if (tmpOff < daysec)
      {
        alarms[20] = Alarm.alarmRepeat(dowMonday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[21] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[20] = Alarm.alarmRepeat(dowMonday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[21] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay2 & 1 << Tue)
    {
      if (tmpOff < daysec)
      {
        alarms[22] = Alarm.alarmRepeat(dowTuesday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[23] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[22] = Alarm.alarmRepeat(dowTuesday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[23] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay2 & 1 << Wed)
    {
      if (tmpOff < daysec)
      {
        alarms[24] = Alarm.alarmRepeat(dowWednesday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[25] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[24] = Alarm.alarmRepeat(dowWednesday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[25] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay2 & 1 << Thu)
    {
      if (tmpOff < daysec)
      {
        alarms[26] = Alarm.alarmRepeat(dowThursday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[27] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[26] = Alarm.alarmRepeat(dowThursday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[27] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay2 & 1 << Fri)
    {
      if (tmpOff < daysec)
      {
        alarms[28] = Alarm.alarmRepeat(dowFriday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[29] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[28] = Alarm.alarmRepeat(dowFriday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[29] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fog.OnDay2 & 1 << Sat)
    {
      if (tmpOff < daysec)
      {
        alarms[30] = Alarm.alarmRepeat(dowSaturday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[31] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[30] = Alarm.alarmRepeat(dowSaturday, Fog.On2Hr, Fog.On2Min, 0, FogOn); //
        alarms[31] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, FogOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Fog.On2Hr, Fog.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Fan.Enable & 0xF0) {
    unsigned long tmpOff = mySecs(0, Fan.On1Hr, Fan.On1Min, 0) + mySecs(0 , 0, Fan.Dur1Min, Fan.Dur1Sec);
    byte hr, mn;
    unsigned long tmpOff2;
    char buf[60];
    if (tmpOff > daysec)
    {
      Serial.println(F("Fan 1 off next day"));
      tmpOff2 = tmpOff - daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      Serial.println(F("Fan 1 off same day"));
      tmpOff2 = tmpOff;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    if (Fan.OnDay & 1 << Sun)
    {
      if (tmpOff < daysec)
      {
        alarms[32] = Alarm.alarmRepeat(dowSunday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[33] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[32] = Alarm.alarmRepeat(dowSunday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[33] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay & 1 << Mon)
    {
      if (tmpOff < daysec)
      {
        alarms[34] = Alarm.alarmRepeat(dowMonday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[35] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[34] = Alarm.alarmRepeat(dowMonday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[35] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay & 1 << Tue)
    {
      if (tmpOff < daysec)
      {
        alarms[36] = Alarm.alarmRepeat(dowTuesday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[37] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[36] = Alarm.alarmRepeat(dowTuesday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[37] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay & 1 << Wed)
    {
      if (tmpOff < daysec)
      {
        alarms[38] = Alarm.alarmRepeat(dowWednesday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[39] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[38] = Alarm.alarmRepeat(dowWednesday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[39] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay & 1 << Thu)
    {
      if (tmpOff < daysec)
      {
        alarms[40] = Alarm.alarmRepeat(dowThursday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[41] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[40] = Alarm.alarmRepeat(dowThursday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[41] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay & 1 << Fri)
    {
      if (tmpOff < daysec)
      {
        alarms[42] = Alarm.alarmRepeat(dowFriday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[43] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[42] = Alarm.alarmRepeat(dowFriday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[43] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay & 1 << Sat)
    {
      if (tmpOff < daysec)
      {
        alarms[44] = Alarm.alarmRepeat(dowSaturday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[45] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[44] = Alarm.alarmRepeat(dowSaturday, Fan.On1Hr, Fan.On1Min, 0, FanOn); //
        alarms[45] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Fan.On1Hr, Fan.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Fan.Enable & 0x0F) {
    unsigned long tmpOff = mySecs(0, Fan.On2Hr, Fan.On2Min, 0) + mySecs(0 , 0, Fan.Dur2Min, Fan.Dur2Sec);
    byte hr, mn;
    unsigned long tmpOff2;
    char buf[60];
    if (tmpOff > daysec)
    {
      Serial.println(F("Fan 2 off next day"));
      tmpOff2 = tmpOff - daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      Serial.println(F("Fan 2 off same day"));
      tmpOff2 = tmpOff;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    if (Fan.OnDay2 & 1 << Sun)
    {
      if (tmpOff < daysec)
      {
        alarms[46] = Alarm.alarmRepeat(dowSunday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[47] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[46] = Alarm.alarmRepeat(dowSunday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[47] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay2 & 1 << Mon)
    {
      if (tmpOff < daysec)
      {
        alarms[48] = Alarm.alarmRepeat(dowMonday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[49] = Alarm.alarmRepeat(dowMonday, Fan.Dur2Min, Fan.Dur2Sec, 0, FanOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[48] = Alarm.alarmRepeat(dowMonday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[49] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay2 & 1 << Tue)
    {
      if (tmpOff < daysec)
      {
        alarms[50] = Alarm.alarmRepeat(dowTuesday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[51] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[50] = Alarm.alarmRepeat(dowTuesday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[51] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay2 & 1 << Wed)
    {
      if (tmpOff < daysec)
      {
        alarms[52] = Alarm.alarmRepeat(dowWednesday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[53] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[52] = Alarm.alarmRepeat(dowWednesday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[53] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay2 & 1 << Thu)
    {
      if (tmpOff < daysec)
      {
        alarms[54] = Alarm.alarmRepeat(dowThursday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[55] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[54] = Alarm.alarmRepeat(dowThursday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[55] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay2 & 1 << Fri)
    {
      if (tmpOff < daysec)
      {
        alarms[56] = Alarm.alarmRepeat(dowFriday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[57] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[56] = Alarm.alarmRepeat(dowFriday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[57] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Fan.OnDay2 & 1 << Sat)
    {
      if (tmpOff < daysec)
      {
        alarms[58] = Alarm.alarmRepeat(dowSaturday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[59] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[58] = Alarm.alarmRepeat(dowSaturday, Fan.On2Hr, Fan.On2Min, 0, FanOn); //
        alarms[59] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, FanOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Fan.On2Hr, Fan.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Mist.Enable & 0xF0) {
    unsigned long tmpOff = mySecs(0, Mist.On1Hr, Mist.On1Min, 0) + mySecs(0 , 0, Mist.Dur1Min, Mist.Dur1Sec);
    byte hr, mn;
    unsigned long tmpOff2;
    char buf[60];
    if (tmpOff > daysec)
    {
      Serial.println(F("Mist 1 off next day"));
      tmpOff2 = tmpOff - daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      Serial.println(F("Mist 1 off same day"));
      tmpOff2 = tmpOff;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    if (Mist.OnDay & 1 << Sun)
    {
      if (tmpOff < daysec)
      {
        alarms[60] = Alarm.alarmRepeat(dowSunday, Mist.On1Hr, Mist.On1Min, 0, MistOn); //
        alarms[61] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[60] = Alarm.alarmRepeat(dowSunday, Mist.On1Hr, Mist.On1Min, 0, MistOn); //
        alarms[61] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay & 1 << Mon)
    {
      if (tmpOff < daysec)
      {
        alarms[62] = Alarm.alarmRepeat(dowMonday, Mist.On1Hr, Mist.On1Min, 0, MistOn); //
        alarms[63] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[62] = Alarm.alarmRepeat(dowMonday, Mist.On1Hr, Mist.On1Min, 0, MistOn); //
        alarms[63] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay & 1 << Tue)
    {
      if (tmpOff < daysec)
      {
        alarms[64] = Alarm.alarmRepeat(dowTuesday, Mist.On1Hr, Mist.On1Min, 0, MistOn); //
        alarms[65] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[64] = Alarm.alarmRepeat(dowTuesday, Mist.On1Hr, Mist.On1Min, 0, MistOn); //
        alarms[65] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay & 1 << Wed)
    {
      if (tmpOff < daysec)
      {
        alarms[66] = Alarm.alarmRepeat(dowWednesday, Mist.On1Hr, Mist.On1Min, 0, MistOn);             //
        alarms[67] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[66] = Alarm.alarmRepeat(dowWednesday, Mist.On1Hr, Mist.On1Min, 0, MistOn);             //
        alarms[67] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    } //                                                                                            //
    if (Mist.OnDay & 1 << Thu)                                                                      //
    { //                                                                                            //
      if (tmpOff < daysec)
      {
        alarms[68] = Alarm.alarmRepeat(dowThursday, Mist.On1Hr, Mist.On1Min, 0, MistOn);              //
        alarms[69] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[68] = Alarm.alarmRepeat(dowThursday, Mist.On1Hr, Mist.On1Min, 0, MistOn);              //
        alarms[69] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    } //                                                                                            //
    if (Mist.OnDay & 1 << Fri)                                                                      //
    { //                                                                                            //
      if (tmpOff < daysec)
      {
        alarms[70] = Alarm.alarmRepeat(dowFriday, Mist.On1Hr, Mist.On1Min, 0, MistOn);                //
        alarms[71] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[70] = Alarm.alarmRepeat(dowFriday, Mist.On1Hr, Mist.On1Min, 0, MistOn);                //
        alarms[71] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    } //                                                                                            //
    if (Mist.OnDay & 1 << Sat)                                                                      //
    { //                                                                                            //
      if (tmpOff < daysec)
      {
        alarms[72] = Alarm.alarmRepeat(dowSaturday, Mist.On1Hr, Mist.On1Min, 0, MistOn);              //
        alarms[73] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[72] = Alarm.alarmRepeat(dowSaturday, Mist.On1Hr, Mist.On1Min, 0, MistOn);              //
        alarms[73] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Mist.On1Hr, Mist.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    } //                                                                                            //
  } //                                                                                              //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Mist.Enable & 0x0F) {
    unsigned long tmpOff = mySecs(0, Mist.On2Hr, Mist.On2Min, 0) + mySecs(0 , 0, Mist.Dur2Min, Mist.Dur2Sec);
    byte hr, mn;
    unsigned long tmpOff2;
    char buf[60];
    if (tmpOff > daysec)
    {
      Serial.println(F("Mist 2 off next day"));
      tmpOff2 = tmpOff - daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      Serial.println(F("Mist 2 off same day"));
      tmpOff2 = tmpOff;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    if (Mist.OnDay2 & 1 << Sun)
    {
      if (tmpOff < daysec)
      {
        alarms[74] = Alarm.alarmRepeat(dowSunday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[75] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[74] = Alarm.alarmRepeat(dowSunday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[75] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay2 & 1 << Mon)
    {
      if (tmpOff < daysec)
      {
        alarms[76] = Alarm.alarmRepeat(dowMonday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[77] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[76] = Alarm.alarmRepeat(dowMonday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[77] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay2 & 1 << Tue)
    {
      if (tmpOff < daysec)
      {
        alarms[78] = Alarm.alarmRepeat(dowTuesday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[79] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[78] = Alarm.alarmRepeat(dowTuesday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[79] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay2 & 1 << Wed)
    {
      if (tmpOff < daysec)
      {
        alarms[80] = Alarm.alarmRepeat(dowWednesday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[81] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[80] = Alarm.alarmRepeat(dowWednesday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[81] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay2 & 1 << Thu)
    {
      if (tmpOff < daysec)
      {
        alarms[82] = Alarm.alarmRepeat(dowThursday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[83] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[82] = Alarm.alarmRepeat(dowThursday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[83] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay2 & 1 << Fri)
    {
      if (tmpOff < daysec)
      {
        alarms[84] = Alarm.alarmRepeat(dowFriday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[85] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[84] = Alarm.alarmRepeat(dowFriday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[85] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist.OnDay2 & 1 << Sat)
    {
      if (tmpOff < daysec)
      {
        alarms[86] = Alarm.alarmRepeat(dowSaturday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[87] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[86] = Alarm.alarmRepeat(dowSaturday, Mist.On2Hr, Mist.On2Min, 0, MistOn); //
        alarms[87] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Mist.On2Hr, Mist.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Mist2.Enable & 0xF0) {
    unsigned long tmpOff = mySecs(0, Mist2.On1Hr, Mist2.On1Min, 0) + mySecs(0 , 0, Mist2.Dur1Min, Mist2.Dur1Sec);
    byte hr, mn;
    unsigned long tmpOff2;
    char buf[60];
    if (tmpOff > daysec)
    {
      Serial.println(F("Mist 3 off next day"));
      tmpOff2 = tmpOff - daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      Serial.println(F("Mist 3 off same day"));
      tmpOff2 = tmpOff;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    if (Mist2.OnDay & 1 << Sun)
    {
      if (tmpOff < daysec)
      {
        alarms[88] = Alarm.alarmRepeat(dowSunday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[89] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[88] = Alarm.alarmRepeat(dowSunday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[89] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay & 1 << Mon)
    {
      if (tmpOff < daysec)
      {
        alarms[90] = Alarm.alarmRepeat(dowMonday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[91] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[90] = Alarm.alarmRepeat(dowMonday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[91] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay & 1 << Tue)
    {
      if (tmpOff < daysec)
      {
        alarms[92] = Alarm.alarmRepeat(dowTuesday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[93] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[92] = Alarm.alarmRepeat(dowTuesday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[93] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay & 1 << Wed)
    {
      if (tmpOff < daysec)
      {
        alarms[94] = Alarm.alarmRepeat(dowWednesday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[95] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[94] = Alarm.alarmRepeat(dowWednesday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[95] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay & 1 << Thu)
    {
      if (tmpOff < daysec)
      {
        alarms[96] = Alarm.alarmRepeat(dowThursday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[97] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[96] = Alarm.alarmRepeat(dowThursday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[97] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay & 1 << Fri)
    {
      if (tmpOff < daysec)
      {
        alarms[98] = Alarm.alarmRepeat(dowFriday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[99] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[98] = Alarm.alarmRepeat(dowFriday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[99] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay & 1 << Sat)
    {
      if (tmpOff < daysec)
      {
        alarms[100] = Alarm.alarmRepeat(dowSaturday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[101] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[100] = Alarm.alarmRepeat(dowSaturday, Mist2.On1Hr, Mist2.On1Min, 0, MistOn); //
        alarms[101] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Mist2.On1Hr, Mist2.On1Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Mist2.Enable & 0x0F) {
    unsigned long tmpOff = mySecs(0, Mist2.On2Hr, Mist2.On2Min, 0) + mySecs(0 , 0, Mist2.Dur2Min, Mist2.Dur2Sec);
    byte hr, mn;
    unsigned long tmpOff2;
    char buf[60];
    if (tmpOff > daysec)
    {
      Serial.println(F("Mist 4 off next day"));
      tmpOff2 = tmpOff - daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      Serial.println(F("Mist 4 off same day"));
      tmpOff2 = tmpOff;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    if (Mist2.OnDay2 & 1 << Sun)
    {
      if (tmpOff < daysec)
      {
        alarms[102] = Alarm.alarmRepeat(dowSunday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[103] = Alarm.alarmRepeat(dowSunday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[102] = Alarm.alarmRepeat(dowSunday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[103] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sun On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay2 & 1 << Mon)
    {
      if (tmpOff < daysec)
      {
        alarms[104] = Alarm.alarmRepeat(dowMonday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[105] = Alarm.alarmRepeat(dowMonday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Mon Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[104] = Alarm.alarmRepeat(dowMonday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[105] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Mon On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay2 & 1 << Tue)
    {
      if (tmpOff < daysec)
      {
        alarms[106] = Alarm.alarmRepeat(dowTuesday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[107] = Alarm.alarmRepeat(dowTuesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Tue Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[106] = Alarm.alarmRepeat(dowTuesday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[107] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Tue On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay2 & 1 << Wed)
    {
      if (tmpOff < daysec)
      {
        alarms[108] = Alarm.alarmRepeat(dowWednesday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[109] = Alarm.alarmRepeat(dowWednesday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Wed Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[108] = Alarm.alarmRepeat(dowWednesday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[109] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Wed On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay2 & 1 << Thu)
    {
      if (tmpOff < daysec)
      {
        alarms[110] = Alarm.alarmRepeat(dowThursday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[111] = Alarm.alarmRepeat(dowThursday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Thu Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[110] = Alarm.alarmRepeat(dowThursday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[111] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Thu On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay2 & 1 << Fri)
    {
      if (tmpOff < daysec)
      {
        alarms[112] = Alarm.alarmRepeat(dowFriday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[113] = Alarm.alarmRepeat(dowFriday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Fri Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[112] = Alarm.alarmRepeat(dowFriday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[113] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Fri On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
    if (Mist2.OnDay2 & 1 << Sat)
    {
      if (tmpOff < daysec)
      {
        alarms[114] = Alarm.alarmRepeat(dowSaturday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[115] = Alarm.alarmRepeat(dowSaturday, hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sat Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
      else
      {
        alarms[114] = Alarm.alarmRepeat(dowSaturday, Mist2.On2Hr, Mist2.On2Min, 0, MistOn); //
        alarms[115] = Alarm.alarmRepeat(dowSunday,  hr, mn, tmpOff2, MistOff); //
        sprintf_P(buf, PSTR("Sat On :- %02d:%02d.%02d Sun Off :- %02d:%02d.%02d\0"), Mist2.On2Hr, Mist2.On2Min, 0, hr, mn, tmpOff2);
        Serial.println(buf);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
#if debug
  for (byte q = 0; q < 116; q++)
  {
    Serial.print(F("Alarm element "));
    Serial.print(q);
    Serial.print(' ');
    Serial.println(alarms[q]);
  }
#endif
  testRelays();
}
void testRelays()
{
  byte light1ret = checkRelays(Light, 1, 0);
  if (light1ret == 0x1F)
  {
    Lights1On();
  }
  else
  {
    Lights1Off();
  }
  byte light2ret = checkRelays(Light, 2, 0);
  if (light2ret == 0x2F)
  {
    Lights2On();
  }
  else
  {
    Lights2Off();
  }
  byte fog1ret = checkRelays(Fog, 1, 1);
  byte fog2ret = checkRelays(Fog, 2, 1);
  if ((fog1ret == 0x1F) || (fog2ret == 0x2F)) FogOn();
  if ((fog1ret == 0x10) && (fog2ret == 0x20)) FogOff();
  byte fan1ret = checkRelays(Fan, 1, 1);
  byte fan2ret = checkRelays(Fan, 2, 1);
  if ((fan1ret == 0x1F) || (fan2ret == 0x2F)) FanOn();
  if ((fan1ret == 0x10) && (fan2ret == 0x20)) FanOff();
  byte mist1ret = checkRelays(Mist, 1, 1);
  byte mist2ret = checkRelays(Mist, 2, 1);
  byte mist3ret = checkRelays(Mist2, 1, 1);
  byte mist4ret = checkRelays(Mist2, 2, 1);
  if ((mist1ret == 0x1F) || (mist2ret == 0x2F) || (mist3ret == 0x1F) || (mist4ret == 0x2F)) MistOn();
  if ((mist1ret == 0x10) && (mist2ret == 0x20) && (mist3ret == 0x10) && (mist4ret == 0x20)) MistOff();
}

byte checkRelays(SCHEDULE parameter, byte item, byte enable)
{
  unsigned long mynowRtc, mynowOn, mynowOff;
  DateTime mynow = rtc.now().unixtime();
  mynowRtc = mySecs(0, mynow.hour(), mynow.minute(), mynow.second());
  byte d;
  if (!enable)
  {
    switch (item)
    {
      case 1:
        mynowOn  = mySecs(0, parameter.On1Hr  , parameter.On1Min , 0);
        mynowOff = mySecs(0, parameter.Dur1Min, parameter.Dur1Sec, 0);
        if ((mynowOff) < (mynowOn))
        {
          if ((mynowRtc < mynowOff) && (mynowOff + daysec > mynowRtc))
          {
            mynowRtc += daysec;
            mynowOff += daysec;
          }
          else
          {
            mynowOff += daysec;
          }
        }
        if ((mynowRtc < mynowOff) && (mynowRtc > mynowOn))
        {
          return 0x1F;
        }
        else
        {
          return 0x10;
        }
        break;
      case 2:
        mynowOn  = mySecs(0, parameter.On2Hr, parameter.On2Min, 0);
        mynowOff = mySecs(0, parameter.Dur2Min, parameter.Dur2Sec, 0);
        if (mynowOff < mynowOn)
        {
          if ((mynowRtc < mynowOff) && ((mynowOff + daysec) > mynowRtc))
          {
            mynowRtc += daysec;
            mynowOff += daysec;
          }
          else
          {
            mynowOff += daysec;
          }
        }
        if ( (mynowRtc < mynowOff) && (mynowRtc > mynowOn) )
        {
          return 0x2F;
        }
        else
        {
          return 0x20;
        }
        break;
    }
  }
  else
  {
    byte mask;
    if (item == 1) mask = 0xF0;
    if (item == 2) mask = 0x0F;
    if (parameter.Enable &= mask)
    {
      switch (item)
      {
        case 1:
          mynowOn  = mySecs(0, parameter.On1Hr, parameter.On1Min, 0);
          mynowOff = mynowOn + mySecs(0, 0, parameter.Dur1Min, parameter.Dur1Sec);
          if (mynowOff < mynowOn)
          {
            if ((mynowRtc < mynowOff) && ((mynowOff + daysec) > mynowRtc))
            {
              mynowOff += daysec;
              mynowRtc += daysec;
            }
            else
            {
              mynowOff += daysec;
            }
          }
          if ( (mynowRtc < mynowOff) && (mynowRtc > mynowOn) )
          {
#if debug
            Serial.println(F("Function 1 Should be on-enabled"));
#endif
            return 0x1F;
          }
          else
          {
#if debug
            Serial.println(F("Function 1 Should be off-enabled"));
#endif
            return 0x10;
          }
          break;
        case 2:
          mynowOn  = mySecs(0, parameter.On2Hr, parameter.On2Min, 0);
          mynowOff = mynowOn + mySecs(0, 0, parameter.Dur2Min, parameter.Dur2Sec);
          if (mynowOff < mynowOn)
          {
            if ((mynowRtc < mynowOff) && ((mynowOff + daysec) > mynowRtc))
            {
              mynowOff += daysec;
              mynowRtc += daysec;
            }
            else
            {
              mynowOff += daysec;
            }
          }
          if ( (mynowRtc < mynowOff) && (mynowRtc > mynowOn) )
          {
#if debug
            Serial.println(F("Function 2 Should be on-enabled"));
#endif
            return 0x2F;
          }
          else
          {
#if debug
            Serial.println(F("Function 2 Should be off-enabled"));
#endif
            return 0x20;
          }
          break;
      }
    }
    else
    {
      switch (item)
      {
        case 1:
#if debug
          Serial.println(F("Function 1 Disabled"));
#endif
          return 0x10;
          break;
        case 2:
#if debug
          Serial.println(F("Function 2 Disabled"));
#endif
          return 0x20;
          break;
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void lowTemp()
{
  if (prevlowTemp != temp1)
  {
    if ((temp1 < heater) && (heatOn == 0))
    {
      heatOn = 1;
      //heater += 2;
      HeaterOn();
    }
    if ((heatOn == 1) && (temp1 > (heater + 0.5)))
    {
      heatOn = 0;
      //heater -= 2;
      HeaterOff();
    }
    prevlowTemp = temp1;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hiTemp()
{
  if (prevhiTemp != temp1)
  {
    if ((temp1 > fan) && (fanOn == 0))
    {
      fanOn = 1;
      //fan -= 2;
      FanOn();
    }
    if ((fanOn == 1) && (temp1 < (fan - 0.5)))
    {
      fanOn = 0;
      //fan  += 2;
      FanOff();
    }
    prevhiTemp = temp1;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void listActiveAlarms()
{
  byte hr, mn;
  unsigned long tmpOff2;
  char buf[60];
  myGLCD.setWritePage(0);
  myGLCD.clrScr();
  myGLCD.setColor(19 , 183, 8);
  myGLCD.setFont(arial_bold);
  const char a[] PROGMEM = "Alarm Dump";
  Serial.println(a);
  Serial.println();
  myGLCD.print(a, 0, 20);
  const char b[] PROGMEM = "Lights";
  Serial.println(b);
  Serial.println();
  myGLCD.print(b, 10, 40);
  myGLCD.setFont(Sinclair_S);
  myGLCD.setColor(134, 250, 124);
  sprintf_P(buf, PSTR("Lights 1_On %02d:%02d.00 Off %02d:%02d.00"), Light.On1Hr, Light.On1Min, Light.Dur1Min, Light.Dur1Sec );
  myGLCD.print(buf, 20, 80);
  Serial.print(buf);
  Serial.println();
  sprintf_P(buf, PSTR("Lights 2_On %02d:%02d.00 Off %02d:%02d.00"), Light.On2Hr, Light.On2Min, Light.Dur2Min, Light.Dur2Sec );
  myGLCD.print(buf, 20, 90);
  Serial.println(buf);
  Serial.println();
  myGLCD.setColor(19 , 183, 8);
  myGLCD.setFont(arial_bold);
  const char c[] PROGMEM = "Fogger";
  myGLCD.print(c, 10, 110);
  Serial.println(c);
  Serial.println();
  myGLCD.setFont(Sinclair_S);
  myGLCD.setColor(134, 250, 124);
  if (Fog.Enable & 0xF0) {
    tmpOff2 = mySecs(0, Fog.On1Hr, Fog.On1Min, 0) + mySecs(0, 0, Fog.Dur1Min, Fog.Dur1Sec);
    if (tmpOff2 > daysec)
    {
      tmpOff2 -= daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    sprintf_P(buf, PSTR("Fog    1_On %02d:%02d.00 Off %02d:%02d.%02d "), Fog.On1Hr, Fog.On1Min, hr, mn, tmpOff2);
    if (Fog.OnDay & 1 << Sun)
    {
      strcat(buf, "Sun ");
    } else strcat(buf, "    ");
    if (Fog.OnDay & 1 << Mon)
    {
      strcat(buf, "Mon ");
    } else strcat(buf, "    ");
    if (Fog.OnDay & 1 << Tue)
    {
      strcat(buf, "Tue ");
    } else strcat(buf, "    ");
    if (Fog.OnDay & 1 << Wed)
    {
      strcat(buf, "Wed ");
    } else strcat(buf, "    ");
    if (Fog.OnDay & 1 << Thu)
    {
      strcat(buf, "Thu ");
    } else strcat(buf, "    ");
    if (Fog.OnDay & 1 << Fri)
    {
      strcat(buf, "Fri ");
    } else strcat(buf, "    ");
    if (Fog.OnDay & 1 << Sat)
    {
      strcat(buf, "Sat ");
    } else strcat(buf, "    ");
    Serial.print(buf);
    Serial.println();
    myGLCD.print(buf, 20, 150);
  }
  else
  {
    const char d[] PROGMEM = "Fog    1_Off";
    myGLCD.print(d, 20, 150);
    Serial.print(d);
    Serial.println();
  }
  if (Fog.Enable & 0x0F) {
    tmpOff2 = mySecs(0, Fog.On2Hr, Fog.On2Min, 0) + mySecs(0, 0, Fog.Dur2Min, Fog.Dur2Sec);
    if (tmpOff2 > daysec)
    {
      tmpOff2 -= daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    sprintf_P(buf, PSTR("Fog    2_On %02d:%02d.00 Off %02d:%02d.%02d "), Fog.On2Hr, Fog.On2Min, hr, mn, tmpOff2);
    if (Fog.OnDay2 & 1 << Sun)
    {
      strcat(buf, "Sun ");
    } else strcat(buf, "    ");
    if (Fog.OnDay2 & 1 << Mon)
    {
      strcat(buf, "Mon ");
    } else strcat(buf, "    ");
    if (Fog.OnDay2 & 1 << Tue)
    {
      strcat(buf, "Tue ");
    } else strcat(buf, "    ");
    if (Fog.OnDay2 & 1 << Wed)
    {
      strcat(buf, "Wed ");
    } else strcat(buf, "    ");
    if (Fog.OnDay2 & 1 << Thu)
    {
      strcat(buf, "Thu ");
    } else strcat(buf, "    ");
    if (Fog.OnDay2 & 1 << Fri)
    {
      strcat(buf, "Fri ");
    } else strcat(buf, "    ");
    if (Fog.OnDay2 & 1 << Sat)
    {
      strcat(buf, "Sat ");
    } else strcat(buf, "    ");
    myGLCD.print(buf, 20, 160);
    Serial.println(buf);
    Serial.println();
  }
  else
  {
    const char e[] PROGMEM = "Fog    2_Off";
    myGLCD.print(e, 20, 160);
    Serial.println(e);
    Serial.println();
  }
  myGLCD.setColor(19 , 183, 8);
  myGLCD.setFont(arial_bold);
  const char f[] PROGMEM = "Misting";
  myGLCD.print(f, 10, 180);
  Serial.println(f);
  Serial.println();
  myGLCD.setFont(Sinclair_S);
  myGLCD.setColor(134, 250, 124);
  if (Mist.Enable & 0xF0) {
    tmpOff2 = mySecs(0, Mist.On1Hr, Mist.On1Min, 0) + mySecs(0, 0, Mist.Dur1Min, Mist.Dur1Sec);
    if (tmpOff2 > daysec)
    {
      tmpOff2 -= daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    sprintf_P(buf, PSTR("Mist   1_On %02d:%02d.00 Off %02d:%02d.%02d "), Mist.On1Hr, Mist.On1Min, hr, mn, tmpOff2);
    if (Mist.OnDay & 1 << Sun)
    {
      strcat(buf, "Sun ");
    } else strcat(buf, "    ");
    if (Mist.OnDay & 1 << Mon)
    {
      strcat(buf, "Mon ");
    } else strcat(buf, "    ");
    if (Mist.OnDay & 1 << Tue)
    {
      strcat(buf, "Tue ");
    } else strcat(buf, "    ");
    if (Mist.OnDay & 1 << Wed)
    {
      strcat(buf, "Wed ");
    } else strcat(buf, "    ");
    if (Mist.OnDay & 1 << Thu)
    {
      strcat(buf, "Thu ");
    } else strcat(buf, "    ");
    if (Mist.OnDay & 1 << Fri)
    {
      strcat(buf, "Fri ");
    } else strcat(buf, "    ");
    if (Mist.OnDay & 1 << Sat)
    {
      strcat(buf, "Sat ");
    } else strcat(buf, "    ");
    myGLCD.print(buf, 20, 220);
    Serial.print(buf);
    Serial.println();
  }
  else
  {
    const char g[] PROGMEM = "Mist   1_Off";
    myGLCD.print(g, 20, 220);
    Serial.print(g);
    Serial.println();
  }
  if (Mist.Enable & 0x0F) {
    tmpOff2 = mySecs(0, Mist.On2Hr, Mist.On2Min, 0) + mySecs(0, 0, Mist.Dur2Min, Mist.Dur2Sec);
    if (tmpOff2 > daysec)
    {
      tmpOff2 -= daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    sprintf_P(buf, PSTR("Mist   2_On %02d:%02d.00 Off %02d:%02d.%02d "), Mist.On2Hr, Mist.On2Min, hr, mn, tmpOff2);
    if (Mist.OnDay2 & 1 << Sun)
    {
      strcat(buf, "Sun ");
    } else strcat(buf, "    ");
    if (Mist.OnDay2 & 1 << Mon)
    {
      strcat(buf, "Mon ");
    } else strcat(buf, "    ");
    if (Mist.OnDay2 & 1 << Tue)
    {
      strcat(buf, "Tue ");
    } else strcat(buf, "    ");
    if (Mist.OnDay2 & 1 << Wed)
    {
      strcat(buf, "Wed ");
    } else strcat(buf, "    ");
    if (Mist.OnDay2 & 1 << Thu)
    {
      strcat(buf, "Thu ");
    } else strcat(buf, "    ");
    if (Mist.OnDay2 & 1 << Fri)
    {
      strcat(buf, "Fri ");
    } else strcat(buf, "    ");
    if (Mist.OnDay2 & 1 << Sat)
    {
      strcat(buf, "Sat ");
    } else strcat(buf, "    ");
    myGLCD.print(buf, 20, 230);
    Serial.print(buf);
    Serial.println();
  }
  else
  {
    const char h[] PROGMEM = "Mist   2_Off";
    myGLCD.print(h, 20, 230);
    Serial.print(h);
    Serial.println();
  }
  if (Mist2.Enable & 0xF0) {
    tmpOff2 = mySecs(0, Mist2.On1Hr, Mist2.On1Min, 0) + mySecs(0, 0, Mist2.Dur1Min, Mist2.Dur1Sec);
    if (tmpOff2 > daysec)
    {
      tmpOff2 -= daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    sprintf_P(buf, PSTR("Mist   3_On %02d:%02d.00 Off %02d:%02d.%02d "), Mist2.On1Hr, Mist2.On1Min, hr, mn, tmpOff2);
    if (Mist2.OnDay & 1 << Sun)
    {
      strcat(buf, "Sun ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay & 1 << Mon)
    {
      strcat(buf, "Mon ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay & 1 << Tue)
    {
      strcat(buf, "Tue ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay & 1 << Wed)
    {
      strcat(buf, "Wed ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay & 1 << Thu)
    {
      strcat(buf, "Thu ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay & 1 << Fri)
    {
      strcat(buf, "Fri ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay & 1 << Sat)
    {
      strcat(buf, "Sat ");
    } else strcat(buf, "    ");
    myGLCD.print(buf, 20, 240);
    Serial.print(buf);
    Serial.println();
  }
  else
  {
    const char i[] PROGMEM = "Mist   3_Off";
    myGLCD.print(i, 20, 240);
    Serial.print(i);
    Serial.println();
  }
  if (Mist2.Enable & 0x0F) {
    tmpOff2 = mySecs(0, Mist2.On2Hr, Mist2.On2Min, 0) + mySecs(0, 0, Mist2.Dur2Min, Mist2.Dur2Sec);
    if (tmpOff2 > daysec)
    {
      tmpOff2 -= daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    sprintf_P(buf, PSTR("Mist   4_On %02d:%02d.00 Off %02d:%02d.%02d "), Mist2.On2Hr, Mist2.On2Min, hr, mn, tmpOff2);
    if (Mist2.OnDay2 & 1 << Sun)
    {
      strcat(buf, "Sun ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay2 & 1 << Mon)
    {
      strcat(buf, "Mon ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay2 & 1 << Tue)
    {
      strcat(buf, "Tue ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay2 & 1 << Wed)
    {
      strcat(buf, "Wed ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay2 & 1 << Thu)
    {
      strcat(buf, "Thu ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay2 & 1 << Fri)
    {
      strcat(buf, "Fri ");
    } else strcat(buf, "    ");
    if (Mist2.OnDay2 & 1 << Sat)
    {
      strcat(buf, "Sat ");
    } else strcat(buf, "    ");
    myGLCD.print(buf, 20, 250);
    Serial.println(buf);
    Serial.println();
  }
  else
  {
    const char j[] PROGMEM = "Mist   4_Off";
    myGLCD.print(j, 20, 250);
    Serial.println(j);
    Serial.println();
  }
  myGLCD.setColor(19 , 183, 8);
  myGLCD.setFont(arial_bold);
  const char k[] PROGMEM = "Fan";
  myGLCD.print(k, 10, 270);
  Serial.println(k);
  Serial.println();
  myGLCD.setFont(Sinclair_S);
  myGLCD.setColor(134, 250, 124);
  if (Fan.Enable & 0xF0) {
    tmpOff2 = mySecs(0, Fan.On1Hr, Fan.On1Min, 0) + mySecs(0, 0, Fan.Dur1Min, Fan.Dur1Sec);
    if (tmpOff2 > daysec)
    {
      tmpOff2 -= daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    sprintf_P(buf, PSTR("Fan    1_On %02d:%02d.00 Off %02d:%02d.%02d "), Fan.On1Hr, Fan.On1Min, hr, mn, tmpOff2);
    if (Fan.OnDay & 1 << Sun)
    {
      strcat(buf, "Sun ");
    } else strcat(buf, "    ");
    if (Fan.OnDay & 1 << Mon)
    {
      strcat(buf, "Mon ");
    } else strcat(buf, "    ");
    if (Fan.OnDay & 1 << Tue)
    {
      strcat(buf, "Tue ");
    } else strcat(buf, "    ");
    if (Fan.OnDay & 1 << Wed)
    {
      strcat(buf, "Wed ");
    } else strcat(buf, "    ");
    if (Fan.OnDay & 1 << Thu)
    {
      strcat(buf, "Thu ");
    } else strcat(buf, "    ");
    if (Fan.OnDay & 1 << Fri)
    {
      strcat(buf, "Fri ");
    } else strcat(buf, "    ");
    if (Fan.OnDay & 1 << Sat)
    {
      strcat(buf, "Sat ");
    } else strcat(buf, "    ");
    myGLCD.print(buf, 20, 310);
    Serial.print(buf);
    Serial.println();
  }
  else
  {
    const char l[] PROGMEM = "Fan    1_Off";
    myGLCD.print(l, 20, 310);
    Serial.print(l);
    Serial.println();
  }
  if (Fan.Enable & 0x0F) {
    tmpOff2 = mySecs(0, Fan.On2Hr, Fan.On2Min, 0) + mySecs(0, 0, Fan.Dur2Min, Fan.Dur2Sec);
    if (tmpOff2 > daysec)
    {
      tmpOff2 -= daysec;
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    else
    {
      hr = (tmpOff2 / 3600UL);
      tmpOff2 -= (hr * 3600UL);
      mn = (tmpOff2 / 60UL);
      tmpOff2 -= (mn * 60UL);
    }
    sprintf_P(buf, PSTR("Fan    2_On %02d:%02d.00 Off %02d:%02d.%02d "), Fan.On2Hr, Fan.On2Min, hr, mn, tmpOff2);
    if (Fan.OnDay2 & 1 << Sun)
    {
      strcat(buf, "Sun ");
    } else strcat(buf, "    ");
    if (Fan.OnDay2 & 1 << Mon)
    {
      strcat(buf, "Mon ");
    } else strcat(buf, "    ");
    if (Fan.OnDay2 & 1 << Tue)
    {
      strcat(buf, "Tue ");
    } else strcat(buf, "    ");
    if (Fan.OnDay2 & 1 << Wed)
    {
      strcat(buf, "Wed ");
    } else strcat(buf, "    ");
    if (Fan.OnDay2 & 1 << Thu)
    {
      strcat(buf, "Thu ");
    } else strcat(buf, "    ");
    if (Fan.OnDay2 & 1 << Fri)
    {
      strcat(buf, "Fri ");
    } else strcat(buf, "    ");
    if (Fan.OnDay2 & 1 << Sat)
    {
      strcat(buf, "Sat ");
    } else strcat(buf, "    ");
    myGLCD.print(buf, 20, 320);
    Serial.println(buf);
    Serial.println();
  }
  else
  {
    const char m[] PROGMEM = "Fan    2_Off";
    myGLCD.print(m, 20, 320);
    Serial.println(m);
    Serial.println();
  }
  myGLCD.setColor(19 , 183, 8);
  myGLCD.setFont(arial_bold);
  const char n[] PROGMEM = "Hi/Lo";
  myGLCD.print(n, 10, 340);
  Serial.println(n);
  Serial.println();
  myGLCD.setFont(Sinclair_S);
  myGLCD.setColor(134, 250, 124);
  char outstr1[10];
  char outstr2[10];
  if (temp1Hi != 0 && temp1Lo != 65535)
  {
    dtostrf(temp1Hi / 100.0, 5, 2, outstr2);
    dtostrf(temp1Lo / 100.0, 5, 2, outstr1);
    sprintf_P(buf, PSTR("Temp1 Lo:%s Hi:%s"), outstr1, outstr2 );
    myGLCD.print(buf, 20, 380);
    Serial.print(buf);
    Serial.println();
  }
  if (temp2Hi != 0 && temp2Lo != 65535)
  {
    dtostrf(temp2Hi / 100.0, 5, 2, outstr2);
    dtostrf(temp2Lo / 100.0, 5, 2, outstr1);
    sprintf_P(buf, PSTR("Temp2 Lo:%s Hi:%s"), outstr1, outstr2);
    myGLCD.print(buf, 20, 390);
    Serial.print(buf);
    Serial.println();
  }
  if (temp3Hi != 0 && temp3Lo != 65535)
  {
    dtostrf(temp3Hi / 100.0, 5, 2, outstr2);
    dtostrf(temp3Lo / 100.0, 5, 2, outstr1);
    sprintf_P(buf, PSTR("Temp3 Lo:%s Hi:%s"), outstr1, outstr2);
    myGLCD.print(buf, 20, 400);
    Serial.print(buf);
    Serial.println();
  }
  if (hum1Hi != 0 && hum1Lo != 65535)
  {
    dtostrf(hum1Hi / 100.0, 5, 2, outstr2);
    dtostrf(hum1Lo / 100.0, 5, 2, outstr1);
    sprintf_P(buf, PSTR("Hum 1 Lo:%s Hi:%s"), outstr1, outstr2);
    myGLCD.print(buf, 20, 410);
    Serial.println(buf);
    Serial.println();
  }
  myFiles.loadcpld(20, 420, 760, 60, "9Alarm.raw", 0, 2);
  updateTime = false;
  dispScreen = 9;
  myGLCD.setDisplayPage(0);
}

void relayOff(byte flag)
{
  Relay |= (1 << flag);
  switch (flag)
  {
    case 0:
      digitalWrite(FAN1, HIGH);
      break;
    case 1:
      digitalWrite(MISTING1, HIGH);
      break;
    case 2:
      digitalWrite(FOGGER1, HIGH);
      break;
    case 3:
      digitalWrite(TEMP1, HIGH);
      break;
    case 4:
      digitalWrite(LIGHTS1, HIGH);
      break;
    case 5:
      digitalWrite(LIGHTS2, HIGH);
      break;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void relayOn(byte flag)
{
  Relay &= ~(1 << flag);
  switch (flag)
  {
    case 0:
      digitalWrite(FAN1, LOW);
      break;
    case 1:
      digitalWrite(MISTING1, LOW);
      break;
    case 2:
      digitalWrite(FOGGER1, LOW);
      break;
    case 3:
      digitalWrite(TEMP1, LOW);
      break;
    case 4:
      digitalWrite(LIGHTS1, LOW);
      break;
    case 5:
      digitalWrite(LIGHTS2, LOW);
      break;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void Lights1On() {
#if debug
  Serial.println(F("Lights1 on"));
  digitalClockDisplay();
#endif
  relayOn(pwrLight1);
}
void Lights1Off() {
#if debug
  Serial.println(F("Lights1 off"));
  digitalClockDisplay();
#endif
  relayOff(pwrLight1);
}
void Lights2On() {
#if debug
  Serial.println(F("Lights2 on"));
  digitalClockDisplay();
#endif
  relayOn(pwrLight2);
}
void Lights2Off() {
#if debug
  Serial.println(F("Lights2 off"));
  digitalClockDisplay();
#endif
  relayOff(pwrLight2);
}
void FogOn() {                                                        //
#if debug
  Serial.println(F("Fog On"));
  digitalClockDisplay();
#endif
  relayOn(pwrFogger1);
}
void FogOff() {                                                     //
#if debug
  Serial.println(F("Fog Off"));
  digitalClockDisplay();
#endif
  relayOff(pwrFogger1);
}
void FanOn() {                                                      //
#if debug
  Serial.println(F("Fan On"));
  digitalClockDisplay();
#endif
  relayOn(pwrFan1);
}
void FanOff() {                                                   //
#if debug
  Serial.println(F("Fan Off"));
  digitalClockDisplay();
#endif
  relayOff(pwrFan1);
}
void MistOn() {                                                      //
#if debug
  Serial.println(F("Mist On"));
  digitalClockDisplay();
#endif
  relayOn(pwrMisting1);
}
void MistOff() {                                                   //
#if debug
  Serial.println(F("Mist Off"));
  digitalClockDisplay();
#endif
  relayOff(pwrMisting1);
}
void HeaterOn() {                                                      //
#if debug
  Serial.println(F("Heater On"));
  digitalClockDisplay();
#endif
  relayOn(pwrTemp1);
}
void HeaterOff() {                                                   //
#if debug
  Serial.println(F("Heater Off"));
  digitalClockDisplay();
#endif
  relayOff(pwrTemp1);
}
//////////////////////////////////////////////////////////////////////////////////////////////

