int ledPin = 11;          // LED with PWM brightness control

void setup() {            // called once on start up
   // A baud rate of 9600 (8-bit with No parity and 1 stop bit)
   Serial.begin(9600, SERIAL_8N1);
   pinMode(ledPin, OUTPUT);         // the LED is an output
}

void loop() {              // loops forever
   String command;
   char buffer[100];       // stores the return buffer on each loop   
   if (Serial.available()>0){                 // bytes received
      command = Serial.readStringUntil('\0'); // C strings end with \0
      if(command.substring(0,4) == "LED "){   // begins with "LED "?
         String intString = command.substring(4, command.length());
         int level = intString.toInt();       // extract the int
         if(level>=0 && level<=255){          // is it in range?
            analogWrite(ledPin, level);       // yes, write out
            sprintf(buffer, "Set brightness to %d", level);
         }
         else{                                // no, error message back
            sprintf(buffer, "Error: %d is out of range", level);
         } 
      }                                       // otherwise, unknown cmd
      else{ sprintf(buffer, "Unknown command: %s", command.c_str()); }
      Serial.print(buffer);              // send buffer to PocketBeagle
   }
}
