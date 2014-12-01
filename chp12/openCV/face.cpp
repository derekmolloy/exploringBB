/* Haar Classifier Cascade Face Detection Example. Based on the sample
*  classifer from http://docs.opencv.org/
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

int main(int argc, char *args[])
{
   Mat frame;
   VideoCapture *capture;   // capture needs full scope of main(), using ptr
   cout << "Starting face detection application" << endl;
   if(argc==2){  // loading image from a file
      cout << "Loading the image " << args[1] << endl;
      frame = imread(args[1], CV_LOAD_IMAGE_COLOR);
   }
   else {
      cout << "Capturing from the webcam" << endl;
      capture = new VideoCapture(0);
      // set any  properties in the VideoCapture object
      capture->set(CV_CAP_PROP_FRAME_WIDTH,1280);   // width pixels
      capture->set(CV_CAP_PROP_FRAME_HEIGHT,720);   // height pixels
      if(!capture->isOpened()){   // connect to the camera
         cout << "Failed to connect to the camera." << endl;
         return 1;
      }
      *capture >> frame;          // populate the frame with captured image
      cout << "Successfully captured a frame." << endl;
   }
   if (!frame.data){
      cout << "Invalid image data... exiting!" << endl;
      return 1;
   }
   // loading the classifier from a file (standard OpenCV example classifier)
   CascadeClassifier faceCascade;
   faceCascade.load("haarcascade_frontalface.xml");

   // faces is a STL vector of faces - will store the detected faces
   std::vector<Rect> faces;
   // detect objects in the scene using the classifier above
   // (frame, faces, scale factor, min neighbors, flags, min size, max size)
   faceCascade.detectMultiScale(frame, faces, 1.1, 3,
                      0 | CV_HAAR_SCALE_IMAGE, Size(50,50));
   if(faces.size()==0){
      cout << "No faces detected!" << endl;     // display the image anyway
   }
   // draw oval around the detected faces in the faces vector
   for(int i=0; i<faces.size(); i++)
   {
      // Using the center point and a rectangle to create an ellipse
      Point cent(faces[i].x+faces[i].width*0.5, faces[i].y+faces[i].height*0.5);
      RotatedRect rect(cent, Size(faces[i].width,faces[i].width),0);
      // image, rectangle, color=green, thickness=3, linetype=8
      ellipse(frame, rect, Scalar(0,255,0), 3, 8);
      cout << "Face at: (" << faces[i].x << "," <<faces[i].y << ")" << endl;
   }
   imshow("EBB OpenCV face detection", frame);  // display image results
   imwrite("faceOutput.png", frame);            // save image too
   waitKey(0);                                  // dislay image until key press
   return 0;
}
