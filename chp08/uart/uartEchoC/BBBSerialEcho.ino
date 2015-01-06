// Arduino echo program, written by Derek Molloy to test the BBB UART4

// This function is called once when the program starts. 
void setup() {
   // Choose a baud rate and configuration. 115200
   // Default is 8-bit with No parity and 1 stop bit
   Serial.begin(115200, SERIAL_8N1);
}

// This function will loop as quickly as possible, forever.
void loop() {
   byte charIn;
   if(Serial.available()){          // A byte has been received
      charIn = Serial.read();       // Read the character in from the BBB
      Serial.write(charIn);         // Send the character back to the BBB
   }
}

