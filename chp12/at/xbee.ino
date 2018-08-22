const int analogInPin = A0;          // analog input for the TMP36 
 
void setup(){
   pinMode(13, OUTPUT);
   Serial.begin(115200, SERIAL_8N1);  
}

void loop(){                         // update registers every five secs
  digitalWrite(13, HIGH);                          // LED briefly on
  delay(100);                                      // 100ms + processing
  int adcValue = analogRead(analogInPin);          // using a 10-bit ADC
  float curVoltage = adcValue * (3.3f/1024.0f);    // Vcc = 5.0V, 10-bit
  float tempC = 25.0 + ((curVoltage-0.75f)/0.01f); // from datasheet
  float tempF = 32.0 + ((tempC * 9)/5);            // deg. C to F
  Serial.print("{ \"Temperature\" : ");            // Send as JSON msg
  Serial.print(tempC);                             // The temperature
  Serial.println(" }");                            // close JSON message
  digitalWrite(13, LOW);                           // LED off
  delay(4900);                                     // delay ~5 secs total
}

