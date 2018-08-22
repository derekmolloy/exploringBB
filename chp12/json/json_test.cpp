#include "json/json.h"
#include<iostream>
#include<fstream>
using namespace std;

int main(){
   Json::Value root;           // the parsed data is at the root
   Json::CharReaderBuilder builder;
   ifstream data("data.json",  ifstream::binary);
   string error;
   bool success = parseFromStream(builder, data, &root, &error);
   if(!success){               // has the parsing failed?
      cout << "Failed: " << error << endl;
   }
   // the deserialized data can be converted to a float and a bool
   float temperature = root.get("temperature", "UTF-8").asFloat();
   bool button = root.get("button", "UTF-8").asBool();
   cout << "The temperature is " << temperature << "°C" << endl;
   cout << "The button is " << (button ? "pressed":"not pressed") << endl;
   return 0;
}

