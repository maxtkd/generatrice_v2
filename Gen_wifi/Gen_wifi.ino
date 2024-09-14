#include <TM1637Display.h>
#define CLK 8
#define DIO 6

int max_runtime = 1800; // 30 mins
int runtime = 0; // en seconde
int sec, h, m, s;
int last_raw_sec = 0;
int raw_sec = 0;
int cmd_raw_sec = 0; // time from app
int timeThreshold = 15;
int failCode = -1;

int last_m;

bool failBlinkOn = true;

TM1637Display display(CLK, DIO);

// Manual command
bool manual_engine_start = false;
bool manual_engine_stop = false;

// Process variables
bool start_process = false;
bool engine_started = false;
bool safe_switch = true;
bool fail = false;
bool key_is_off_due_to_fail = false;
float timer = 0.0;

// Pins
int pin_start_process = 2;
int pin_engine_started = 3;
int pin_key_on = 10;
int pin_key_start = 12;
int pin_relay_fail = 7;

// Debug Var
int start_count = 0;
int runing_time = 0;


void setup() {
  // put your setup code here, to run once:
  display.setBrightness(0x0f);
  

  // Map the pins
  pinMode(pin_start_process, INPUT); //start_process
  pinMode(pin_key_on, OUTPUT); //KEY_ON
  pinMode(pin_key_start, OUTPUT); //KEY_START
  pinMode(pin_engine_started, INPUT); //ENGINE_STARTED
  pinMode(pin_relay_fail, OUTPUT); // ENGINE FAIL
  pinMode(A3, INPUT); //TIMER
  pinMode(A5, INPUT); //VoltSensor 
  Serial.begin(9600); // Communication with ESP32

  digitalWrite(pin_relay_fail, LOW);
  digitalWrite(pin_key_on, HIGH);
  digitalWrite(pin_key_start, HIGH);

  //Create 5v Output
  analogWrite(13, 255); //relay
  analogWrite(12, 255); //switch
  analogWrite(11, 255); //
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  readSerial();
  Serial.println("cmd:Fail:None");

  // If low battery, pin_start_process is true
  start_process = !digitalRead(pin_start_process);
  //SerialUpdateVars();
  if((start_process || manual_engine_start) && !fail){
    manual_engine_start = false;

    //TURN KEY ON
    digitalWrite(pin_key_on,LOW);
    Serial.println("cmd:State:KeyOn");
    delay(3000);
    
    int essaie = 0;
    int essaie_maximum = 4;

    //Step 1 - Start the engine
    while(engine_started == false && essaie < essaie_maximum ){
      /* Wait 2 sec
       * Turn key on Start position 5 sec
       * Turn key back to On position
       * Verify if the engine is started
       */
      delay(2000);
      digitalWrite(pin_key_start, LOW);
      Serial.println("cmd:State:KeyStart");
      delay(5000);
      digitalWrite(pin_key_start, HIGH); //RESET KEY TO ON STATE
      Serial.println("cmd:State:KeyStartOff");
  
      //VERIFY IF ENGINE IS STARTED
      engine_started = !digitalRead(pin_engine_started);
      if (!engine_started){
        essaie = essaie + 1;
      }

      start_count = essaie;
    }

    Serial.println("cmd:State:KeyStartOver");

    //Step 2 - Let the engine run for the given duration
    if(essaie < essaie_maximum && engine_started) {
      /* Run while the timer is not over
       * Update the remaining time display
       * Turn key off
       */
      Serial.println("cmd:State:TimerStarting");
      last_m = millis()/1000;
      int reverifyFailCount = 0;
      
      //Tant que temps pas ecouler et pas fail
      while(runtime < max_runtime && !fail && !manual_engine_stop){
        runtime = (millis()/1000) - last_m;
        Serial.println("cmd:RemainingTime:");
        adjustTime(true);
        delay(1000); //1 sec
        engine_started = !digitalRead(pin_engine_started);

        // Read commands while running the engine
        readSerial();

        if(!engine_started)
          reverifyFailCount ++;
      
        // Error 2: Engine stopped while running
        if(fail || reverifyFailCount == 3) 
        {
          fail = true;
          failCode = 2;
        }
        runing_time = runtime;
      }
      Serial.println("cmd:State:TimerOver");
    }
    else {
      // Error 1: Engine never started
      fail = true;
      failCode = 1;
    }
    TurnKeyOFF();
    runtime = 0;
    manual_engine_stop = false;
  }

  
  if(!fail) adjustTime(false);
  else {
    displayFail(failCode);
    /*if (!key_is_off_due_to_fail){
      TurnKeyOFF();
      key_is_off_due_to_fail = true;
    }*/
  }
}

void TurnKeyOFF(){
  // TURN KEY TO OFF 

  int try_off = 0;
  bool key_is_off = false;
  while(try_off < 5)
  {
    digitalWrite(pin_key_on, HIGH); // TURN KEY OFF
    delay(3000);
    //VERIFY IF ENGINE IS TURNED OFF
    engine_started = !digitalRead(pin_engine_started);

    if ( !engine_started ) {
      key_is_off=true;
      try_off = 5;
    }
    try_off++;
  }

  if(!key_is_off)
  {
    fail = true;
    failCode = 3;
  }
  else
  {
    Serial.println("cmd:State:EngineStarted");
  }
}