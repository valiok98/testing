#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
// Custom header files
#include "window.hpp"
#include "frame-helper.hpp"

#define MODE 0 // This is the mode of thresholding. Currently supported : 0 | 1

cv::RNG rng(12345);

int main() {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    Window* threshold_window = new Window(MODE ? "Adaptive thresholding" : "Normal thresholding"),
        * trackbar_window = new Window("Trackbar window");
    
    FrameHelper thrs_fr_hp(MODE),
        * threshold_fr_hp = &thrs_fr_hp;
    
    cv::VideoCapture cap(0);
    
    if (!cap.isOpened()) {
        std::cerr << "Video capturing failed!\n";
        return -1;
    }
    /**
     * Open 3 windows.
     * 1. Threshold window with camera feed.
     * 2. Adaptive threshold window with camera feed.
     * 3. Trackbar window with controls for both thresholding types.
     */
    cv::namedWindow(threshold_window->getName());
    cv::namedWindow(trackbar_window->getName());
    
    // Create the trackbars
    threshold_fr_hp->create_trackbars(trackbar_window);
    
    while (1) {
        cv::Mat threshold_frame,
            threshold_frame_copy;
        
        cap >> threshold_frame;
        cap >> threshold_frame_copy;
        
        try {
            cv::cvtColor(threshold_frame, threshold_frame, CV_BGR2GRAY);
            // Set frame.
            threshold_fr_hp->setFrame(threshold_frame);
            // Apply thresholding.
            threshold_fr_hp->thresholding();
            // Find contours.
            cv::findContours(threshold_frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
            // Draw contours.
            for( int i = 0; i < contours.size(); i++ ) {
                    contours[i] = cv::boundingRect(contours[i]);
                    cv::approxPolyDP(contours[i], contours[i], cv::arcLength(contours[i], true) * 0.02, true);
                    cv::drawContours(threshold_frame, contours, i, CV_BGR2HLS, 2, 8, hierarchy);
            }
        } catch(cv::Exception& err) {
            // Let it slide, happens on first try. Continue with the safe copy instead.
            threshold_frame = threshold_frame_copy;
        }
        
        // Show the thresholded image.
        cv::imshow(threshold_window->getName(), threshold_frame);
    
        if (cv::waitKey(10) == 27) {
            break;
        }
    }
    
    delete trackbar_window;
    delete threshold_window;
    return 0;
}
