void displayFail(int code)
{

  /*
   *   A
   *  E  C
   *   G
   *  F  D
   *   B
   */  
  
  const uint8_t fail[] = {
    SEG_A | SEG_E | SEG_F | SEG_G,           // F
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // A
    SEG_B | SEG_C,                           // I
    SEG_D | SEG_E | SEG_F            // L
  };

  const uint8_t err1[] = {
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
    SEG_E | SEG_G,   // r
    SEG_E | SEG_G,   // r
    SEG_B | SEG_C    // 1
  };

  const uint8_t err2[] = {
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
    SEG_E | SEG_G,   // r
    SEG_E | SEG_G,   // r
    SEG_A | SEG_B | SEG_D | SEG_E | SEG_G    // 2
  };

  const uint8_t err3[] = {
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
    SEG_E | SEG_G,   // r
    SEG_E | SEG_G,   // r
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_G    // 3
  };

  const uint8_t err4[] = {
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
    SEG_E | SEG_G,   // r
    SEG_E | SEG_G,   // r
    SEG_B | SEG_C | SEG_F | SEG_G    // 4
  };

  const uint8_t err5[] = {
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
    SEG_E | SEG_G,   // r
    SEG_E | SEG_G,   // r
    SEG_A | SEG_C | SEG_D | SEG_F | SEG_G    // 5
  };
 
  
  if(failBlinkOn)
  { 
    display.setBrightness(0,false);
    failBlinkOn = false;
    switch(code){
       case 0: display.setSegments(fail);
          break;
       case 1: display.setSegments(err1);
          break;
       case 2: display.setSegments(err2);
          break;
       case 3: display.setSegments(err3);
        break;
       case 4: display.setSegments(err4);
        break;
       case 5: display.setSegments(err5);
        break;
    }
  }
  else
  {
    display.setBrightness(7);
    switch(code){
       case 0: display.setSegments(fail);
          break;
       case 1: display.setSegments(err1);
          break;
       case 2: display.setSegments(err2);
          break;
       case 3: display.setSegments(err3);
        break;
       case 4: display.setSegments(err4);
        break;
       case 5: display.setSegments(err5);
        break;
    }
    failBlinkOn = true;
  }

  Serial.println("cmd:Fail:" + failCode);
  delay(500);
}