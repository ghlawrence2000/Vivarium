#include <SPI.h>
// SdFat lib from here :-
// https://github.com/greiman/SdFat/archive/master.zip
#include <SdFat.h>
#include <SdStream.h>
#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
#include <UTFT_SdRaw.h>
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

#define SD_CHIP_SELECT  53  // SD chip select pin
// file system object
SdFat sd;
// print stream
ArduinoOutStream cout(Serial);

int wait = 2, pressed_button;

// Initialize display
// ------------------
// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino Uno/2009 Shield            : <display model>,19,18,17,16
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!

UTFT myGLCD(ITDB32S, 38, 39, 40, 41);




// Initialize touchscreen
// ----------------------
// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino Uno/2009 Shield            : 15,10,14, 9, 8
// Standard Arduino Mega/Due shield            :  6, 5, 4, 3, 2
// CTE TFT LCD/SD Shield for Arduino Due       :  6, 5, 4, 3, 2
// CTE TFT LCD/SD Shield for Arduino Due (JP10):  6, 5,32, 3, 2
// Teensy 3.x TFT Test Board                   : 26,31,27,28,29
// ElecHouse TFT LCD/SD Shield for Arduino Due : 25,26,27,29,30
//

UTouch  myTouch( 6, 5, 4, 3, 2);

UTFT_SdRaw myFiles(&myGLCD);
UTFT_Buttons  myButtons(&myGLCD, &myTouch);

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println(F("Initialising SD card..."));
  bool mysd = 0;
  // see if the card is present and can be initialized:
  while (!mysd)
  {
    if (!sd.begin(SD_CHIP_SELECT, SPI_FULL_SPEED)) {
      Serial.println(F("Card failed, or not present"));
      Serial.println(F("Retrying...."));
    }
    else
    {
      mysd = 1;
      Serial.println(F("Card initialised."));
    }
  }
  Serial.println(F("Initialising LCD."));
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  myButtons.setTextFont(BigFont);
  int butskip = myButtons.addButton( 85,  219 , 70,  20, "Skip");
  Serial.println(F("LCD initialised."));
  // ////////////////////////////////////////////////////////////////////////////////
  //             Wait for GPS screen  or skip button
  ///////////////////////////////////////////////////////////////////////////////////
  Serial.println(F("Initialising GPS."));
  long mytime = millis();
  myFiles.load(0, 0, 240, 240, "wait4gps.RAW", 1 , 1);
  long mytime1 = millis() - mytime;
  cout << F("Time to draw 240x240 raw ") << mytime1 << F(" ms") << endl;
  myButtons.drawButton(butskip);
  while (wait == 2) {
    pressed_button = myButtons.checkButtons();
    if (pressed_button == butskip)
    {
      cout << F("Skip button detected (No GPS :( )\n");
      wait = true;
    }
  }
  char fname240[] = "SK45/SK400500.RAW";
  myGLCD.clrScr();
  mytime = millis();
  myFiles.load(0, 0, 240, 240, fname240, 1);
  mytime1 = millis() - mytime;
  cout << F("Time to draw 240x240 raw (no extra buffer) (non inverted colour) ") << mytime1 << F(" ms") << endl;
  delay(5000);
  mytime = millis();
  myFiles.load(0, 0, 240, 240, fname240, 8, 1);
  mytime1 = millis() - mytime;
  cout << F("Time to draw 240x240 raw (8*buffer) (inverted colour) ") << mytime1 << F(" ms") << endl;
  delay(5000);
  mytime = millis();
  myFiles.pan(0, 0, 240, 240, 0, 0, 4040, 4040, "SK45/SK45.RAW");
  mytime1 = millis() - mytime;
  cout << F("Time to draw 240x240 portion from 4040x4040 raw image (non inverted colour) ") << mytime1 << F(" ms") << endl;
  delay(5000);
  mytime = millis();
  myFiles.pan(0, 0, 240, 240, 0, 0, 4040, 4040, "SK45/SK45.RAW", 1);
  mytime1 = millis() - mytime;
  cout << F("Time to draw 240x240 portion from 4040x4040 raw image (inverted colour) ") << mytime1 << F(" ms") << endl;
  delay(5000);
  myGLCD.clrScr();
  int xad, yad;
  int tmpx = 0, tmpy = 0;
  for (int yx = 0; yx < 6; yx++)
  {
    int xd = 0, yd = 0;
    mytime = millis();
    for (int z = 0 + (yx * 35); z < 35 + (yx * 35); z++)
    {
      tmpy = (z / 22);
      tmpx = z - (tmpy * 22);
      yad = (tmpy * 69) + 10;
      xad = (tmpx * 108) + 44;
      myFiles.pan(xd, yd, 42, 42, xad, yad, 2374, 696, "icons.raw");
      xd += 42;
      if (xd > 277)
      {
        xd = 0;
        yd += 42;
      }
    }
    mytime1 = millis();
    cout << F("Time to draw 35 42x42 icons (") << (0 + (yx * 35)) << "-" << (34 + (yx * 35)) << F(") from 2374x696 image ") << (mytime1 - mytime) << F(" ms") << endl;
    delay(5000);
  }
  cout << F("Thanks for watching!") << endl;
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.print("Thanks for watching!", CENTER, 111);
}

void loop()
{
}
