#include <iostream>
#include <sstream>
#include <fstream>
#include "network/SocketClient.h"
#define ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
#define ADC 0
#define IO_USERNAME  "molloyd"
#define IO_KEY       "feb8ea6179c64b638b35b93ef87d4b42"
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

// https://io.adafruit.com/api/groups/weather/send.json?x-aio-key=a052ecc32b2de1c80abc03bd471acd1d6b218e5c&temperature=13&humidity=12&wind=45

int main() {
   ostringstream head, data;
   cout << "Starting Adafruit Example" << endl;
   SocketClient sc("io.adafruit.com",443);
   data << "temperature=" << getTemperature(readAnalog(ADC)) << endl;
   sc.connectToServer();
   head << "GET /api/groups/weather/send.json HTTP/1.1\n"
        << "Host: io.adafruit.com\n"
        << "Connection: close\n"
        << "x-aio-key: feb8ea6179c64b638b35b93ef87d4b42\n"
        << "Content-Type: application/x-www-form-urlencoded\n"
        << "Content-Length:" << string(data.str()).length() << "\n\n";
   sc.send(string(head.str()));
   sc.send(string(data.str()));
   string rec = sc.receive(1024);
   cout << "[" << rec << "]" << endl;
   cout << "End of Adafruit Example" << endl;
}
