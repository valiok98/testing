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
            cv::cvtColor(threshold_frame, threshold_frame_copy, CV_BGR2GRAY);
            // Set frame.
            threshold_fr_hp->setFrame(threshold_frame_copy);
            // Apply thresholding.
            threshold_fr_hp->thresholding();
            // Find contours.
            cv::findContours(threshold_frame_copy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
            // Draw contours.
            for( size_t i = 0; i < contours.size(); i++ ) {
            
                std::vector<cv::Point> approx_contour;
                
                cv::approxPolyDP(contours[i], approx_contour, arcLength(contours[i], true) * 0.02, true);
                
                cv::Scalar QUADRILATERAL_COLOR(0, 0, 255);
                cv::Scalar colour;
                
                cv::Rect r = cv::boundingRect(approx_contour);
                
                if (approx_contour.size() == 4) {
                    colour = QUADRILATERAL_COLOR;
                }
                else {
                    continue;
                }
                
                if (r.height < 20 || r.width < 20 || r.width > threshold_frame.cols - 10 || r.height > threshold_frame.rows - 10) {
                    continue;
                }
                
                polylines(threshold_frame, approx_contour, true, colour, 4);
                
                // -----------------------------
                
                // --- Process Corners ---
                
                for (size_t j = 0; j < approx_contour.size(); ++j) {
                    circle(threshold_frame, approx_contour[j], 3, CV_RGB(0, 255, 0), -1);
                    
                    double dx = ((double)approx_contour[(j + 1) % 4].x - (double)approx_contour[j].x) / 7.0;
                    double dy = ((double)approx_contour[(j + 1) % 4].y - (double)approx_contour[j].y) / 7.0;
                    
                    for (int k = 1; k < 7; ++k) {
                        double px = (double)approx_contour[j].x + (double)k * dx;
                        double py = (double)approx_contour[j].y + (double)k * dy;
                        
                        cv::Point p;
                        p.x = (int)px;
                        p.y = (int)py;
                        circle(threshold_frame, p, 2, CV_RGB(0, 0, 255), -1);
                    }
                }
            }
                
                // -----------------------------
                
                
        } catch(cv::Exception& err) {
            // Let it slide, happens on first try. Continue with the safe copy instead.
            threshold_frame = threshold_frame_copy;
        }
		//hello
        
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
