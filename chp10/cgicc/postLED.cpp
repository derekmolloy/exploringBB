/* C++ CGI BeagleBone POST example -- Written by Derek Molloy (www.derekmolloy.ie)
	You must set the sticky bit for this script in order that it can
	access the on-board LED sysfs file system. This example integrates the form
	for ease of use -- i.e., it calls itself, but it could just as easily receive
	the input from a regular HTML page.
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
	Cgicc form;
	LED *led3 = new LED(3);
	string flashPeriod = "100", command = "off";   // default values
	bool isStatus = form.queryCheckbox("status");
	form_iterator it = form.getElement("command");
	if (!it->isEmpty() && it!=(*form).end()) {
		string resp(**it);
		command = resp;
	}
	it = form.getElement("period");
	if (!it->isEmpty() && it!=(*form).end()) {
		string resp(**it);
		flashPeriod = resp;
	}
	char *value = getenv("REMOTE_ADDR");           // The remote address CGI environment variable
	cout << "Content-type:text/html\r\n\r\n";      // Generate the HTML output
	cout << "<html><head>\n";
	cout << "<title>EBB C++ Post LED Example</title>\n";
	cout << "</head><body>\n";
	cout << "<h1>BeagleBone POST LED Example</h1>\n";
	cout << "<form action=\"/cgi-bin/postLED.cgi\" method=\"POST\">\n";
	cout << "<div>LED state: <input type=\"radio\" name=\"command\" value=\"on\""
	     << ( command=="on" ? "checked":"") << "/> On ";
	cout << "<input type=\"radio\" name=\"command\" value=\"off\""
	     << ( command=="off" ? "checked":"") << "/> Off ";
	cout << "<input type=\"radio\" name=\"command\" value=\"flash\""
	     << ( command=="flash" ? "checked":"") << "/> Flash ";
	cout << "<input type=\"checkbox\" name=\"status\" " << (isStatus ? "checked":"") << " /> Display Status </div>";
	cout << "<div>Flash period: <input type=\"text\" name=\"period\" size=\"6\" value=\"" << flashPeriod << "\"> ms   ";
	cout << "<input type=\"submit\" value=\"Execute Command\" />";
	cout << "</div></form>";

	// If the Display Status checkbox is checked then display the status now
	if (isStatus){
		cout << "<div>";
		led3->outputState();
		cout << "</div>";
	}
	if (command=="on") led3->turnOn();
	else if (command=="off") led3->turnOff();
	else if (command=="flash") led3->flash(flashPeriod);
	else cout << "<div> Invalid command! </div>";

	cout << "<div> The CGI REMOTE_ADDR environment variable is " << value << "</div>";
	cout << "</body></html>\n";
	return 0;
}
