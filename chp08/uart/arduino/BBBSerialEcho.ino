/* Arduino Serial Test Program - Derek Molloy (www.derekmolloy.ie)
*   for the book Exploring BeagleBone */

// There is an on-board LED attached to Pin13
int ledPin = 13;

// This function is called once when the program starts. 
void setup() {
   // Choose a baud rate and configuration. 9600 
   // Default is 8-bit with No parity and 1 stop bit
   Serial.begin(9600, SERIAL_8N1);
   // The ledPin is an output pin
   pinMode(ledPin, OUTPUT); 
}

// This function will loop as quickly as possible, forever.
void loop() {
   byte charIn;
   if(Serial.available()){          // A byte has been received
      digitalWrite(ledPin, HIGH);   // Turn the LED on, character received
      charIn = Serial.read();       // Read the character in from the BBB
      Serial.write(charIn);         // Send the character back to the BBB
      delay(100);                   // Wait for a 100ms so the LED is visible
      digitalWrite(ledPin, LOW);    // Turn the LED off, wait for next character
   }
}
