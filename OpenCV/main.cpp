#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv_modules.hpp>

int threshold_value = 0;
int threshold_type = 3;;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

void threshold_demo(int, void*);

class FrameObject {
    Mat frame;
};

int main(){
    FrameObject* frame_instance = new
    Mat frame;
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cout << "NO capture" << endl;
        return -1;
    }
    string w_name = "Frame Capture";
    
    namedWindow(w_name);
    
    createTrackbar("Type", w_name, &threshold_type,
                   max_type, threshold_demo);
    
    createTrackbar("Value", w_name, &threshold_value,
                   max_value, threshold_demo);
    
    while (true)
    {
        cap >> frame;
        // Change frame color scheme to Black&White.
        cvtColor(frame, frame, CV_BGR2GRAY);
        // Apply thresholding.
        imshow(w_name, frame);
        int key = waitKey(10);
        if (key == 27) {
            break;
        }
    }
    
    destroyWindow(w_name);
    return 0;
}

void threshold_demo(int, void*) {
    threshold(frame, frame, threshold_value, max_BINARY_value, threshold_type);
}
