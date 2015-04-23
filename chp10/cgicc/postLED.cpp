/* C++ CGI BeagleBone POST example -- Written by Derek Molloy (www.derekmolloy.ie)
	You must set the setuid bit for this script in order that it can
	access the on-board LED sysfs file system. This example integrates the form
	for ease of use -- i.e., it is a form that calls itself. It could just as
	easily receive the input from a regular HTML page.
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
   Cgicc form;                                     // the CGI form object
   LED *led3 = new LED(3);                         // the LED object -- USR3
   string flashPeriod, command;                    // default values

   bool isStatus = form.queryCheckbox("status");   // get the state of the status checkbox
   form_iterator it = form.getElement("period");   // get the period text value
   if (it == form.getElements().end() || it->getValue()==""){
      flashPeriod = "100";                         // if it is invalid use 100
   }
   else { flashPeriod = it->getValue(); }          // otherwise use submitted value
   it = form.getElement("command");                // get the radio command chosen
   if (it == form.getElements().end() || it->getValue()==""){
      command = "off";                             // if it is invalid use "off"
   }
   else { command = it->getValue(); }              // otherwise use submitted value
   char *value = getenv("REMOTE_ADDR");	           // The remote address CGI env. variable

   // Generate the form but use states that are set in the form that was submitted
   cout << HTTPHTMLHeader() << endl;               // Generate the HTML form using cgicc
   cout << html() << head() << title("EBB C++ Post LED Example") << head() << endl;
   cout << body() << h1("BeagleBone POST LED Example") << endl;;
   cout << "<form action=\"/cgi-bin/postLED.cgi\" method=\"POST\">\n";
   cout << "<div>LED state: <input type=\"radio\" name=\"command\" value=\"on\""
        << ( command=="on" ? "checked":"") << "/> On ";	// is the command="on"?
   cout << "<input type=\"radio\" name=\"command\" value=\"off\""
        << ( command=="off" ? "checked":"") << "/> Off ";
   cout << "<input type=\"radio\" name=\"command\" value=\"flash\""
        << ( command=="flash" ? "checked":"") << "/> Flash ";
   cout << "<input type=\"checkbox\" name=\"status\" " << (isStatus ? "checked":"")
        << " /> Display Status </div>";
   cout << "<div>Flash period: <input type=\"text\" name=\"period\" size=\"6\" value=\""
        << flashPeriod << "\"> ms   ";   // populate the text field
   cout << "<input type=\"submit\" value=\"Execute Command\" />";
   cout << "</div></form>";

   // Process the form data to trigger the LED state
   if (command=="on") led3->turnOn();                   // turn the LED on?
   else if (command=="off") led3->turnOff();            // off?
   else if (command=="flash") led3->flash(flashPeriod);	// flash with the period above
   else cout << "<div> Invalid command! </div>";        // not possible at the moment
   // If the Display Status checkbox is checked then display the status now
   // this should happen after the command is set, otherwise it is old data
   if (isStatus){
      cout << "<div>";
      led3->outputState();
      cout << "</div>";
   }
   cout << "<div> The CGI REMOTE_ADDR environment variable is " << value << "</div>";
   cout << body() << html();
   return 0;
}
