/* An Xively sensor example
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <xively.h>
#include <xi_helpers.h>
#include <stdio.h>
#include <stdlib.h>

#define ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
#define DEVICE_KEY "EBBQLU5KOS50UZjnUXsWsLH4jHozyhwKHH8UwAxeaXdy1abc"
#define FEED_ID 55555555

int readAnalog(int number){
   int value;
   FILE* fp;
   char fullFileName[100];
   sprintf(fullFileName, ADC_PATH "%d_raw", number);
   fp = fopen(fullFileName, "r");
   fscanf(fp, "%d", &value);
   fclose(fp);
   return value;
}

float getTemperature(int adc_value){
   float cur_voltage = adc_value * (1.80f/4096.0f);
   float diff_degreesC = (cur_voltage-0.75f)/0.01f;
   return (25.0f + diff_degreesC);
}

int main()
{
   // Create the Xively context
   xi_context_t* xi_context = xi_create_context(XI_HTTP, DEVICE_KEY, FEED_ID);
   if(xi_context==NULL){
      perror("XIVELY: Problem in creating the context");
      return 1;
   }
   // Create the feed and clear its contents
   xi_feed_t feed;
   memset(&feed, 0, sizeof(xi_feed_t));
   // Set the properties of the feed (id, number of streams and datstream)
   feed.feed_id           = FEED_ID;
   feed.datastream_count  = 1;  //only sending a single stream, datastream 0
   // Get a reference to the datastream
   xi_datastream_t* data  = &feed.datastreams[0];
   // Only going to send a single data point in the stream
   data->datapoint_count = 1;
   // Set up the data stream identifier "tempSensor"
   int size = sizeof(data->datastream_id);
   xi_str_copy_untiln(data->datastream_id, size, "tempSensor", '\0' );
   // Set up the data point and setting a floating point value
   xi_datapoint_t* point = &data->datapoints[0];
   xi_set_value_f32(point, getTemperature(readAnalog(0)));
   // Update the feed
   xi_feed_update(xi_context, &feed);
   xi_delete_context(xi_context);
   return 0;
}
