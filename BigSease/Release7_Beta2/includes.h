//////////////////////////////////////////////////////////////////////////////////////////////////////
//                    Ubuntubold In Red Is Commonly Used So Do It In A Function                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t Ubuntubold[];                                                                        //
extern UTFT myGLCD;                                                                                 //
extern byte tempUnits;
//////////////////////////////////////////////////////////////////////////////////////////////////////
void ubuntuRed()                                                                                    //
{ //                                                                                                //
  myGLCD.setColor(255, 77, 0);                                                                      //
  myGLCD.setFont(Ubuntubold);                                                                       //
} //                                                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                        Main Structure To Hold Our Scheduling Information                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct { //                                                                                 //
  byte Enable;   // If Lights, not used                                                             //
  byte On1Hr;                                                                                       //
  byte On1Min;                                                                                      //
  byte Dur1Min;  // If Lights, then Lights1 Off1Hr                                                  //
  byte Dur1Sec;  // If Lights, then Lights1 Off1Min                                                 //
  byte On2Hr;                                                                                       //
  byte On2Min;                                                                                      //
  byte Dur2Min;  // If Lights, then Lights2 Off2Hr                                                  //
  byte Dur2Sec;  // If Lights, then Lights2 Off2Min                                                 //
  byte OnDay;    // If Lights, not used  On1   ( 0x7F = All week; 0/Sat/Fri/Thu/Wed/Tue/Mon/Sun )   //
  byte OnDay2;   //                      On2   ( 0x7F = All week; 0/Sat/Fri/Thu/Wed/Tue/Mon/Sun )   //
} SCHEDULE ;                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   FIRST RUN SETUP                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////
void firstRunSetup() //                                                                             //
{ //                                                                                                //
  if (EEPROM.read(2000) != 58)   // If this is the first run, store all the default values          //
  { //                                                                                              //
    EEPROM.write(2000, 58);      // write to EEPROM so this never runs again                        //
    EEPROM.write(1, 2);          //       <- HomeDelay                                              //
    EEPROM.write(2, 16);         //       <- BackLight                                              //
    EEPROM.write(3, 1);          //       <- AutoDim                                                //
    EEPROM.write(4, 60);         //       <- TouchDelay                                             //
    EEPROM.writeFloat(5, 55.0);  //       <- Heater                                                 //
    EEPROM.writeFloat(9, 79.9);  //       <- Fan                                                    //
    EEPROM.write(13, 8);         //       <- Light.On1Hr                                            //
    EEPROM.write(14, 15);        //       <- Light.On1Min                                           //
    EEPROM.write(15, 20);        //       <- Light.Dur1Min                                          //
    EEPROM.write(16, 30);        //       <- Light.Dur1Sec                                          //
    EEPROM.write(17, 20);        //       <- Light.On2Hr                                            //
    EEPROM.write(18, 00);        //       <- Light.On2Min                                           //
    EEPROM.write(19, 8);         //       <- Light.Dur2Min                                          //
    EEPROM.write(20, 00);        //       <- Light.Dur2Sec                                          //
    EEPROM.write(21, 0xFF);      //       <- Fog.Enable (F0 = 1enable, 2disable, FF = 1&2 enable)   //
    EEPROM.write(22, 7);         //       <- Fog.On1Hr                                              //
    EEPROM.write(23, 15);        //       <- Fog.On1Min                                             //
    EEPROM.write(24, 01);        //       <- Fog.Dur1Min                                            //
    EEPROM.write(25, 30);        //       <- Fog.Dur1Sec                                            //
    EEPROM.write(26, 16);        //       <- Fog.On2Hr                                              //
    EEPROM.write(27, 30);        //       <- Fog.On2Min                                             //
    EEPROM.write(28, 00);        //       <- Fog.Dur2Min                                            //
    EEPROM.write(29, 45);        //       <- Fog.Dur2Sec                                            //
    EEPROM.write(30, 0x7F);      //       <- Fog.OnDay                                              //
    EEPROM.write(31, 0x7F);      //       <- Fog.OnDay2                                             //
    EEPROM.write(32, 0xFF);      //       <- Mist.Enable (F0 = 1enable, 2disable, FF = 1&2 enable)  //
    EEPROM.write(33, 8);         //       <- Mist.On1Hr                                             //
    EEPROM.write(34, 15);        //       <- Mist.On1Min                                            //
    EEPROM.write(35, 0);         //       <- Mist.Dur1Min                                           //
    EEPROM.write(36, 30);        //       <- Mist.Dur1Sec                                           //
    EEPROM.write(37, 16);        //       <- Mist.On2Hr                                             //
    EEPROM.write(38, 0);         //       <- Mist.On2Min                                            //
    EEPROM.write(39, 0);         //       <- Mist.Dur2Min                                           //
    EEPROM.write(40, 45);        //       <- Mist.Dur2Sec                                           //
    EEPROM.write(41, 0x7F);      //       <- Mist.OnDay                                             //
    EEPROM.write(42, 0x7F);      //       <- Mist.OnDay2                                            //
    EEPROM.write(43, 0xFF);      //       <- Mist2.Enable (F0 = 3enable, 4disable, FF = 3&4 enable) //
    EEPROM.write(44, 8);         //       <- Mist2.On1Hr                                            //
    EEPROM.write(45, 15);        //       <- Mist2.On1Min                                           //
    EEPROM.write(46, 0);         //       <- Mist2.Dur1Min                                          //
    EEPROM.write(47, 30);        //       <- Mist2.Dur1Sec                                          //
    EEPROM.write(48, 16);        //       <- Mist2.On2Hr                                            //
    EEPROM.write(49, 50);        //       <- Mist2.On2Min                                           //
    EEPROM.write(50,  0);        //       <- Mist2.Dur2Min                                          //
    EEPROM.write(51, 45);        //       <- Mist2.Dur2Sec                                          //
    EEPROM.write(52, 0x7F);      //       <- Mist2.OnDay                                            //
    //                                          (0x7F = All week; 0/Sat/Fri/Thu/Wed/Tue/Mon/Sun)    //
    EEPROM.write(53, 0x7F);      //       <- Mist2.OnDay2                                           //
    EEPROM.write(54, 0xFF);      //       <- Fan.Enable (F0 = 1enable, 2disable, FF = 1&2 enable)   //
    EEPROM.write(55, 21);        //       <- Fan.On1Hr                                              //
    EEPROM.write(56, 50);        //       <- Fan.On1Min                                             //
    EEPROM.write(57, 21);        //       <- Fan.Dur1Min                                            //
    EEPROM.write(58, 55);        //       <- Fan.Dur1Sec                                            //
    EEPROM.write(59, 22);        //       <- Fan.On2Hr                                              //
    EEPROM.write(60, 0);         //       <- Fan.On2Min                                             //
    EEPROM.write(61, 22);        //       <- Fan.Dur2Min                                            //
    EEPROM.write(62, 05);        //       <- Fan.Dur2Sec                                            //
    EEPROM.write(63, 0xFF);      //       <- Fan.OnDay                                              //
    EEPROM.write(64, 0xFF);      //       <- Fan.OnDay2                                             //
    EEPROM.writeWord(65, 65535); //       <- Hi/Lo temp1Lo                                          //
    EEPROM.writeWord(67, 0);     //       <- Hi/Lo temp1Hi                                          //
    EEPROM.writeWord(69, 65535); //       <- Hi/Lo temp2Lo                                          //
    EEPROM.writeWord(71, 0);     //       <- Hi/Lo temp2Hi                                          //
    EEPROM.writeWord(73, 65535); //       <- Hi/Lo temp3Lo                                          //
    EEPROM.writeWord(75, 0);     //       <- Hi/Lo temp3Hi                                          //
    EEPROM.writeWord(77, 65535); //       <- Hi/Lo hum1Lo                                           //
    EEPROM.writeWord(79, 0);     //       <- Hi/Lo hum1Hi                                           //
    EEPROM.write(81, 0xF0);      //       <- TempUnit ( 0xF0 = F, 0x0F = C)                         //
  } //                                                                                              //
} //                                                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                Format Clock For Serial Monitor                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////
void digitalClockDisplay() //                                                                       //
{ //                                                                                                //
  Serial.print(hour());                                                                             //
  if (minute() < 10)                                                                                //
    Serial.print('0');                                                                              //
  Serial.print(minute());                                                                           //
  if (second() < 10)                                                                                //
    Serial.print('0');                                                                              //
  Serial.print(second());                                                                           //
  if (day() < 10)                                                                                   //
    Serial.print('0');                                                                              //
  Serial.print(day());                                                                              //
  if (month() < 10)                                                                                 //
    Serial.print('0');                                                                              //
  Serial.print(month());                                                                            //
  Serial.print(year());                                                                             //
  Serial.println();                                                                                 //
} //                                                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                        Simple Conversion Of Day And Time To Seconds                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long mySecs(byte days, byte hours, byte mins, byte secs)                                   //
{ //                                                                                                //
  return ((days * 86400UL) + (hours * 3600UL) + (mins * 60UL) + secs);                              //
} //                                                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////
void tempfc(float temps, char bufs[6] )
{
  if (tempUnits == 0xF0)
  {
    dtostrf(temps, 4, 1, bufs);
  }
  else
  {
    dtostrf((((temps - 32.0) / 9.0) * 5.0), 4, 1, bufs);
  }
#if debug
  Serial.print(F("tempfc "));
  Serial.print(temps);
  Serial.print(F(" "));
  Serial.println(bufs);
#endif
}
