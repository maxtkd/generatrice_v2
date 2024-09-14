void readSerial () {
  // Check for commands from ESP32 (Raspberry Pi)
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');

    if (command == "Manual Engine Start") {
      manual_engine_start = true;
    } 
    else if (command == "Manual Engine Stop") {
      manual_engine_stop = true;
    }
    else if (command == "Send Remaining Time") {
      manual_engine_stop = true;
    }
    else if (command.startsWith("Set Time ")) {
      cmd_raw_sec = command.substring(9).toInt();
    }
  }
}

/*
void SerialUpdateVars()
{
  if(Serial)
  {
    String serialData = "";
    while(Serial.available() > 0)
    {
      serialData += Serial.readString();

      if (serialData.startsWith("cmd:StartGen")){ start_process = true; }
      if (serialData.startsWith("cmd:StopGen")){ manual_cleanEngStop = true; }
      if (serialData.startsWith("cmd:ChangeTimer")) 
      {
        Serial.println("-------------------------------");
        Serial.println(serialData.substring(16));
        int newTime = serialData.substring(16).toInt();
        cmd_raw_sec = newTime;
      }
    
    }
  }
}*/
