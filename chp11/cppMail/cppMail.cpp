#include <iostream>
#include <sstream>
#include <stdlib.h>
using namespace std;

int main(){
   string to("xxx@yyy.com");
   string subject("Hello Derek");
   string body("Test Message body...");
   stringstream command;
   command << "echo \""<< body <<"\" | mail -s \""<< subject <<"\" "<< to;
   int result = system(command.str().c_str());
   cout << "Command: " << command.str() << endl;
   cout << "The return value was " << result << endl;
   return result;
}
