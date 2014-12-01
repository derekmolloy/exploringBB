/* Basic Image Processing Example using OpenCV
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<opencv2/opencv.hpp>   // C++ OpenCV include file
using namespace std;
using namespace cv;            // using the cv namespace too

int main()
{
    VideoCapture capture(0);   // capturing from /dev/video0

    cout << "Started Processing - Capturing Image" << endl;
    // set any  properties in the VideoCapture object
    capture.set(CV_CAP_PROP_FRAME_WIDTH,1280);   // width pixels
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,720);   // height pixels
    capture.set(CV_CAP_PROP_GAIN, 0);            // Enable auto gain etc.
    if(!capture.isOpened()){   // connect to the camera
       cout << "Failed to connect to the camera." << endl;
    }

    Mat frame, gray, edges;    // images for orignal, grayscale and edge image
    capture >> frame;          // capture the image to the frame
    if(frame.empty()){         // did the capture succeed?
       cout << "Failed to capture an image" << endl;
       return -1;
    }
    cout << "Processing - Performing Image Processing" << endl;
    cvtColor(frame, gray, CV_BGR2GRAY); // convert to grayscale
    blur(gray, edges, Size(3,3));       // blur grayscale image 3x3 kernel
    // use Canny edge detector that outputs to the same image
    // low threshold = 10, high threshold = 30, kernel size = 3
    Canny(edges, edges, 10, 30, 3);      // Run Canny edge detector
    cout << "Finished Processing - Saving images" << endl;

    imwrite("capture.png", frame);
    imwrite("grayscale.png", gray);
    imwrite("edges.png", edges);
    return 0;
}
