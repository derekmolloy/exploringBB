/* C++ CGI BeagleBone GET example -- Written by Derek Molloy (www.derekmolloy.ie)
	You must set the setuid bit for this script in order that it can
	access the on-board LED sysfs file system. See the web page for instructions.
*/

#include <iostream>         // for the input/output
#include <stdlib.h>         // for the getenv call
#include <sys/sysinfo.h>    // for the system uptime call
#include <cgicc/Cgicc.h>    // the cgicc headers
#include <cgicc/CgiDefs.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "LED.h"            // the LED class from Chapter 5 of the book
using namespace std;
using namespace cgicc;

int main(){
   Cgicc form;                                      // The Cgicc object
   LED *led3 = new LED(3);                          // The LED object -- USR3

   // Generate the response HTML page
   char *value = getenv("REMOTE_ADDR");	            // the remote address CGI env. variable
   cout << "Content-type:text/html\r\n\r\n";        // generate the HTML output
   cout << "<html><head>\n";
   cout << "<title>EBB C++ GET Example</title>\n";
   cout << "</head><body>\n";
   cout << "<h1>BeagleBone GET Example</h1>\n";

   form_iterator it = form.getElement("command");   // read the URL get command string
   if (it == form.getElements().end() || it->getValue()==""){
      cout << "<div> The LED command is missing or invalid.</div>";
      cout << "<div> Valid commands are on, off, flash, and status </div>";
   }
   else{
      string cmd(**it);
      cout << "<div> The LED command is " << cmd << ".</div>";
      /** This code sets the USR3 LED state using the LED class **/
      if(cmd=="on") led3->turnOn();
      else if(cmd=="off") led3->turnOff();
      else if(cmd=="flash") led3->flash("100");
      else if(cmd=="status"){
         cout << "<div>";
         led3->outputState();
         cout << "</div>";
      }
      else cout << "<div> Invalid command! </div>";
   }
   cout << "<div> The CGI REMOTE_ADDR environment variable is " << value << "</div>";
   cout << "</body></html>\n";
   return 0;
}
