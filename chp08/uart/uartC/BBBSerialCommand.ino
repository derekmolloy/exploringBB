/* Arduino Serial Command Program - Derek Molloy (www.derekmolloy.ie)
*   for the book Exploring BeagleBone */

// There is an on-board LED attached to Pin 13
int ledPin=13;

void setup() {
   // Baud of 57600 works fine over short distances (twist cables)
   Serial.begin(57600, SERIAL_8N1);
   // The ledPin is and output pin
   pinMode(ledPin, OUTPUT);
}

// This function will loop forever.
void loop() {
   String command, inChar;          
   if (Serial.available()>0){     // A byte has been received
      command = Serial.readStringUntil('\0');
      if(command=="on"){          // Turn on the on-board LED
         digitalWrite(ledPin, HIGH);
         Serial.print("LED On");
      }
      else if (command=="off") {  // Turn off the on-board LED
         digitalWrite(ledPin, LOW);
         Serial.print("LED Off");
      }
      else {                      // Otherwise return string passed
         Serial.print(command);
      }
   }
}
