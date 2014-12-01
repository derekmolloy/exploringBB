/* A program for a GTK application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<gtk/gtk.h>

int main(int argc, char *argv[]){
   // This application will have a window and a single label
   GtkWidget *window, *label;
   // Initialize the toolkit, pass the command line arguments
   gtk_init(&argc, &argv);
   // Create the top-level window (not yet visible)
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   // Set the title of the window to Exploring BB
   gtk_window_set_title ( GTK_WINDOW (window), "Exploring BB");

   // Create a label
   label = gtk_label_new ("Hello BeagleBone");
   // Add the label to the window
   gtk_container_add(GTK_CONTAINER (window), label);
   // Make the label visible (must be done for every widget)
   gtk_widget_show(label);
   // Make the window visible
   gtk_widget_show(window);
   // Runs the main loop until gtk_main_quit() is called  (hit Ctrl C)
   gtk_main();
   return 0;
}
