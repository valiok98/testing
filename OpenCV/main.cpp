#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
#include "window.hpp"

#define MAX_VALUE 255
#define MAX_TYPE 4
#define MAX_BINARY_VALUE 255

int threshold_value = 0,
    threshold_type = 3;

cv::Mat frame;

void threshold_demo(int, void*);

int main() {
    
    Window* camera_window = new Window("Video capture"),
        * trackbar_window = new Window("Trackbar window");
    
    cv::VideoCapture cap(0);
    
    if (!cap.isOpened()) {
        std::cerr << "Video capturing failed!\n";
        return -1;
    }
    
    cv::namedWindow(camera_window->getName());
    cv::namedWindow(trackbar_window->getName());

    cv::createTrackbar("Type", trackbar_window->getName(), &threshold_type,
                   MAX_TYPE, threshold_demo);
    
    cv::createTrackbar("Value", trackbar_window->getName(), &threshold_value,
                   MAX_VALUE, threshold_demo);
    
    while (1) {
        cap >> frame;
        // Change frame color scheme to Black&White.
        cv::cvtColor(frame, frame, CV_BGR2GRAY);
        // Apply thresholding.
        cv::imshow(camera_window->getName(), frame);
        
        if (cv::waitKey(10) == 27) {
            break;
        }
    }
    
    delete trackbar_window;
    delete camera_window;
    return 0;
}

void threshold_demo(int, void*) {
    cv::threshold(frame, frame, threshold_value, MAX_BINARY_VALUE, threshold_type);
}
