/* A program for a GTK Temperature application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<gtk/gtk.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<stdlib.h>
using namespace std;

#define LDR_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

// Same function as in Chapter 10 to read the ADC value
int readAnalog(int number){
   stringstream ss;
   ss << LDR_PATH << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

// The callback function that is associated with the button
//  it passes a pointer to the label, so that it can be changed
//  when the button is pressed
static void getTemperature(GtkWidget *widget, gpointer temp_label){
   // cast the generic gpointer into a GtkWidget label
   GtkWidget *temperature_label = (GtkWidget *) temp_label;
   int adc_value = readAnalog(0);
   float cur_voltage = adc_value * (1.80f/4096.0f);
   float diff_degreesC = (cur_voltage-0.75f)/0.01f;
   float temperature = 25.0f + diff_degreesC;
   stringstream ss;
   ss << "Temperature: "  << temperature << " degrees C";
   // set the text in the label
   gtk_label_set_text( GTK_LABEL(temp_label), ss.str().c_str());
   ss << endl;  // add a \n to the string for the standard output
   g_print(ss.str().c_str());  //output to the terminal (std out)
}

int main(int argc, char *argv[]){
   GtkWidget *window, *temp_label, *button, *button_label;
   gtk_init(&argc, &argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW (window), "Exploring BB");

   // Fix the size of the window so that it cannot be resized
   gtk_widget_set_size_request(window, 220, 50);
   gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
   // Place a border of 5 pixels around the inner window edge
   gtk_container_set_border_width (GTK_CONTAINER (window), 5);

   // Quit application if X button is pressed
   g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

   // Set up the window to contain two vertically stacked widgets using a vbox
   GtkWidget *vbox = gtk_vbox_new(FALSE, 5);  //spacing of 5
   gtk_container_add (GTK_CONTAINER (window), vbox); //add vbox to window
   gtk_widget_show (vbox); // set the vbox visible

   // This is the label in which to display the temperature
   temp_label = gtk_label_new ("Temperature is Undefined");
   gtk_widget_show(temp_label);  // make it visible
   gtk_label_set_justify( GTK_LABEL(temp_label), GTK_JUSTIFY_LEFT);
   // Add the label to the vbox
   gtk_box_pack_start (GTK_BOX (vbox), temp_label, FALSE, FALSE, 0);

   // Create a button and connect it to the getTemperature() callback function
   button = gtk_button_new();
   button_label = gtk_label_new ("Get Temperature"); //label for text on button
   gtk_widget_show(button_label);  // show label
   gtk_widget_show(button);        // show button
   gtk_container_add(GTK_CONTAINER (button), button_label);  // add label to button
   // Connect the callback function getTemperature() to the button press
   g_signal_connect(button, "clicked", G_CALLBACK (getTemperature), (gpointer) temp_label);
   // Add the button to the vbox
   gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

   gtk_widget_show(window);
   gtk_main();
   return 0;
}
