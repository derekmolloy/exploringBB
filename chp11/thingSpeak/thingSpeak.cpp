#include <iostream>
#include <sstream>
#include <fstream>
#include "network/SocketClient.h"
#define ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
#define ADC 0
using namespace std;
using namespace exploringBB;

float getTemperature(int adc_value) {     // from the TMP36 datasheet
   float cur_voltage = adc_value * (1.80f/4096.0f); // Vcc = 1.8V, 12-bit
   float diff_degreesC = (cur_voltage-0.75f)/0.01f;
   return (25.0f + diff_degreesC);
}

int readAnalog(int number){
   stringstream ss;
   ss << ADC_PATH << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

int main() {
   ostringstream head, data;
   cout << "Starting ThingSpeak Example" << endl;
   SocketClient sc("api.thingspeak.com",80);
   data << "field1=" << getTemperature(readAnalog(ADC)) << endl;
   cout << "Sending the temperature: " <<  getTemperature(readAnalog(ADC)) << endl;
   sc.connectToServer();
   head << "POST /update HTTP/1.1\n"
        << "Host: api.thingspeak.com\n"
        << "Connection: close\n"
        << "X-THINGSPEAKAPIKEY: G6MTQ21IVBFGYJG7\n"
        << "Content-Type: application/x-www-form-urlencoded\n"
        << "Content-Length:" << string(data.str()).length() << "\n\n";
   sc.send(string(head.str()));
   sc.send(string(data.str()));
   string rec = sc.receive(1024);
   cout << "[" << rec << "]" << endl;
   cout << "End of ThingSpeak Example" << endl;
}
