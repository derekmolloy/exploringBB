int ledPin = 13;                    // the Arduino on-board LED

void setup() {
  Serial.begin(9600, SERIAL_8N1);   // set up 8-bit with no parity and 1 stop bit
  pinMode(ledPin, OUTPUT);          // the ledPin is an output
  Serial.write("Hello from the Arduino");
}

// this function loops as quickly as possible, forever
void loop() {
  digitalWrite(ledPin, HIGH);       // set the LED pin high
  delay(500);                       // high and low every second (flash at 1Hz)
  digitalWrite(ledPin, LOW);        // set the LED pin low
  delay(500);                   
}

