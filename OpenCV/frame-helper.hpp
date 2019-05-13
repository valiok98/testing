//
//  frame-helper.hpp
//  OpenCV
//
//  Created by Valentin Kostadinov on 13.05.19.
//  Copyright © 2019 Valentin Kostadinov. All rights reserved.
//

#pragma once
#include <opencv2/opencv.hpp>

volatile class FrameHelper {
    
    const int THRESHOLDING_MAX_VALUE = 255;
    const int ADAPTIVE_THRESHOLDING_MAX_VALUE = 255;
    
    int mode = 0;
    int threshold_value = 0;
    int threshold_type = 0;
    int threshold_method = 0;
    cv::Mat frame;
public:
    FrameHelper(int);
    int* getThresholdValue();
    int* getThresholdType();
    int* getThresholdMethod();
    cv::Mat getFrame() const;
    void setThresholdValue(int);
    void setThresholdType(int);
    void setThresholdMethod(int);
    void setFrame(cv::Mat&);
    void thresholding();
};
