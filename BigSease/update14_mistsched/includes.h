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

