int ledPin = 11;   // LED that flashes when a key is pressed

void setup() {                      // called once on start up
   // A baud rate of 115200 (8-bit with No parity and 1 stop bit)
   Serial.begin(115200, SERIAL_8N1);
   pinMode(ledPin, OUTPUT);         // the LED is an output
}

void loop() {                       // Loops forever!
   byte charIn;
   digitalWrite(ledPin, LOW);       // set the LED to be off
   if(Serial.available()){          // a byte has been received
      charIn = Serial.read();       // read character in from the Beagle
      Serial.write(charIn);         // send character back to the Beagle
      digitalWrite(ledPin, HIGH);   // light the LED
      delay(100);                   // delay so the LED is visible
   }
}
