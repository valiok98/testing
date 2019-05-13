#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
// Custom header files
#include "window.hpp"
#include "frame-helper.hpp"

#define THRESHOLDING_MAX_VALUE 255
#define THRESHOLDING_MAX_TYPE 4
#define ADAPTIVE_THRESHOLDING_MAX_VALUE 255
#define ADAPTIVE_THRESHOLDING_METHOD 1
#define ADAPTIVE_THRESHOLDING_MAX_TYPE 1

#define MAX_BINARY_VALUE 255

void on_change_threshold(void*);
void on_change_adaptive_threshold(void*);

int main() {
    Window* threshold_window = new Window("Normal thresholding"),
        * adaptive_threshold_window = new Window("Adaptive thresholding"),
        * trackbar_window = new Window("Trackbar window");
    
    FrameHelper thrs_fr_hp,
        adpt_thrs_fr_hp,
        * threshold_fr_hp = &thrs_fr_hp,
        * adaptive_threshold_fr_hp = &adpt_thrs_fr_hp;
    
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
    cv::namedWindow(adaptive_threshold_window->getName());
    cv::namedWindow(trackbar_window->getName());
    
    // Create the 4 trackbars
    
    cv::createTrackbar("Thresholding value", trackbar_window->getName(), threshold_fr_hp->getThresholdValue(),
                   THRESHOLDING_MAX_VALUE);
    
    cv::createTrackbar("Thresholding type", trackbar_window->getName(), threshold_fr_hp->getThresholdType(),
                       THRESHOLDING_MAX_TYPE);
    
    cv::createTrackbar("Adaptive thresholding method", trackbar_window->getName(), adaptive_threshold_fr_hp->getThresholdMethod(),
                       ADAPTIVE_THRESHOLDING_METHOD);
    
    cv::createTrackbar("Adaptive thresholding type", trackbar_window->getName(), adaptive_threshold_fr_hp->getThresholdType(),
                       ADAPTIVE_THRESHOLDING_MAX_TYPE);

    while (1) {
        cv::Mat threshold_frame,
            threshold_frame_copy,
            adaptive_threshold_frame,
            adaptive_threshold_frame_copy;
        
        cap >> threshold_frame;
        cap >> threshold_frame_copy;
        cap >> adaptive_threshold_frame;
        cap >> adaptive_threshold_frame_copy;
        
        try {
            cv::cvtColor(threshold_frame, threshold_frame, CV_BGR2GRAY);
        } catch(cv::Exception& err) {
            // Let it slide, happens on first try. Continue with the safe copy instead.
            threshold_frame = threshold_frame_copy;
        }
        
        try {
            cv::cvtColor(adaptive_threshold_frame, adaptive_threshold_frame, CV_BGR2GRAY);
        } catch(cv::Exception& err) {
            // Let it slide, happens on first try. Continue with the safe copy instead.
            adaptive_threshold_frame = adaptive_threshold_frame_copy;
        }
        
        threshold_fr_hp->setFrame(threshold_frame);
        adaptive_threshold_fr_hp->setFrame(adaptive_threshold_frame);
        // Apply thresholding.
        on_change_threshold((void*)threshold_fr_hp);
        on_change_adaptive_threshold((void*)adaptive_threshold_fr_hp);
        // Show the thresholded image.
        cv::imshow(threshold_window->getName(), threshold_frame);
        cv::imshow(adaptive_threshold_window->getName(), adaptive_threshold_frame);
        
        if (cv::waitKey(10) == 27) {
            break;
        }
    }
    
    delete trackbar_window;
    delete adaptive_threshold_window;
    delete threshold_window;
    return 0;
}

void on_change_threshold(void* user_data) {
    FrameHelper* fr_helper = (FrameHelper*) user_data;
    if(fr_helper->getFrame().empty()) {
        std::cout<<"Frame exists ;)\n";
    }
    cv::threshold(fr_helper->getFrame(), fr_helper->getFrame(), *(fr_helper->getThresholdValue()), THRESHOLDING_MAX_VALUE, *(fr_helper->getThresholdType()));
}

void on_change_adaptive_threshold(void* user_data) {
    FrameHelper* fr_helper = (FrameHelper*) user_data;
    cv::adaptiveThreshold(fr_helper->getFrame(), fr_helper->getFrame(), ADAPTIVE_THRESHOLDING_MAX_VALUE, *(fr_helper->getThresholdMethod()), *(fr_helper->getThresholdType()), 3, 0.1);
}
