/* A program to test the performance of the BeagleBone for Image Processing
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>
using namespace std;
using namespace cv;

int main()
{
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    if(!capture.isOpened()){
	    cout << "Failed to connect to the camera." << endl;
    }
    Mat frame, edges;

    struct timespec start, end;
    clock_gettime( CLOCK_REALTIME, &start );

    int frames=100;
    for(int i=0; i<frames; i++){
    	capture >> frame;
    	if(frame.empty()){
		cout << "Failed to capture an image" << endl;
		return -1;
    	}
    	cvtColor(frame, edges, CV_BGR2GRAY);
    	Canny(edges, edges, 0, 30, 3);
    }

    clock_gettime( CLOCK_REALTIME, &end );
    double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0d;
    cout << "It took " << difference << " seconds to process " << frames << " frames" << endl;
    cout << "Capturing and processing " << frames/difference << " frames per second " << endl;

    imwrite("edges.png", edges);
    imwrite("capture.png", frame);
    return 0;
}
