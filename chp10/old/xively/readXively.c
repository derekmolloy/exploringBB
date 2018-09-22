/* An Xively example application that reads from a stream
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <xively.h>
#include <xi_helpers.h>
#include <stdio.h>
#include <stdlib.h>

#define DEVICE_KEY "EBBQLU5KOS50UZjnUXsWsLH4jHozyhwKHH8UwAxeaXdy1abc"
#define FEED_ID 55555555

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
   feed.datastream_count  = 1;  //only receiving a single stream, datastream 0
   // Get a reference to the datastream
   xi_datastream_t* data  = &feed.datastreams[0];
   // Only going to send a single data point in the stream
   data->datapoint_count = 1;
   // Set up the data stream identifier "tempSensor"
   int size = sizeof(data->datastream_id);
   xi_str_copy_untiln(data->datastream_id, size, "tempSensor", '\0' );
   // Get the feed and then the datapoint
   xi_feed_get(xi_context, &feed);
   xi_datapoint_t* point = &data->datapoints[0];
   printf("The current data point is: %f\n", point->value.f32_value);
   xi_delete_context(xi_context);
   return 0;
}
