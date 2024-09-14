/*void adjustTime(bool display_logs) 
{
  raw_sec = analogRead(A3);

  //threshold
  //if( cmd_raw_sec > 0 )
  //{ 
  //  sec = cmd_raw_sec;
    
  //}
  
  if(abs(raw_sec - last_raw_sec) > timeThreshold)
  {
    last_raw_sec = raw_sec;
    sec = last_raw_sec *((13580.0/1023.0))+900; // 13580+900 = 3h45 +15min
    cmd_raw_sec = 0;
  }
  
  max_runtime = sec;
  
  sec = sec-runtime;
  
  h = (sec/3600); 
  m = (sec -(3600*h))/60;
  s = (sec -(3600*h)-(m*60));
  if(h == 0 && m < 10)
  {
    h = m;
    m = s;
  }
  String min = String(m);
  int m0 = (int)min.charAt(0)-48;
  int m1 = (int)min.charAt(1)-48;
   int test = s;
  if( m1 < 0 ) m1 = 0;
  if( m0 < 0 ) m0 = 0; 
  if( m < 10 ) 
  { 
    m1 = m0;
    m0 = 0;
  }
  int disp = (h*100)+(m0*10);
  int fullDisp = disp + m1;

  if(display_logs){Serial.println("H:" + (String)h + " M:" + (String)m + " S:" + (String)s);}
  display.showNumberDecEx(fullDisp,0b11100000,true,4,0);
}*/


void adjustTime(bool display_logs) 
{
  if (cmd_raw_sec != 0) {
    raw_sec = cmd_raw_sec;
    cmd_raw_sec = 0;
  }
  else {
    raw_sec = analogRead(A3);
  }
  
  // If raw_sec changes significantly, recalculate the time
  if(abs(raw_sec - last_raw_sec) > timeThreshold)
  {
    last_raw_sec = raw_sec;
    sec = raw_sec * (13580.0 / 1023.0) + 900; // Calculate seconds based on input
    cmd_raw_sec = 0;
  }

  max_runtime = sec;         // Set maximum runtime
  sec = sec - runtime;       // Remaining time calculation

  // Convert seconds to hours, minutes, and seconds
  h = sec / 3600;
  m = (sec % 3600) / 60;
  s = sec % 60;

  // Adjust if hours are zero and minutes are less than 10
  if (h == 0 && m < 10) {
    h = m;
    m = s;
  }

  // Directly calculate m0 and m1 from minute values
  int m0 = m / 10;
  int m1 = m % 10;

  // Build the display value
  int fullDisp = (h * 100) + (m0 * 10) + m1;

  // Display the time
  display.showNumberDecEx(fullDisp, 0b11100000, true, 4, 0);
}
