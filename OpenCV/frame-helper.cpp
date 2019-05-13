//
//  frame-helper.cpp
//  OpenCV
//
//  Created by Valentin Kostadinov on 13.05.19.
//  Copyright © 2019 Valentin Kostadinov. All rights reserved.
//

#include "frame-helper.hpp"

FrameHelper::FrameHelper(int mode) : mode(mode) {}

int* FrameHelper::getThresholdValue() {
    return &(this->threshold_value);
}

int* FrameHelper::getThresholdType() {
    return &(this->threshold_type);
}

int* FrameHelper::getThresholdMethod() {
    return &(this->threshold_method);
}

cv::Mat FrameHelper::getFrame() const {
    return this->frame;
}

void FrameHelper::setThresholdValue(int threshold_value) {
    this->threshold_value = threshold_value;
}

void FrameHelper::setThresholdType(int threshold_type) {
    this->threshold_type = threshold_type;
}

void FrameHelper::setThresholdMethod(int threshold_method) {
    this->threshold_method = threshold_method;
}

void FrameHelper::setFrame(cv::Mat& frame) {
    this->frame = frame;
}

void FrameHelper::thresholding() {
    switch(this->mode) {
        // Apply normal thresholding.
        case 0:
            cv::threshold(this->getFrame(), this->getFrame(), *(this->getThresholdValue()), this->THRESHOLDING_MAX_VALUE, *(this->getThresholdType()));
            break;
        // Apply adaptive thresholding.
        case 1:
            cv::adaptiveThreshold(this->getFrame(), this->getFrame(), this->ADAPTIVE_THRESHOLDING_MAX_VALUE, *(this->getThresholdMethod()), *(this->getThresholdType()), 3, 0.1);
            break;
        // Let default case be normal thresholding.
        default:
            cv::threshold(this->getFrame(), this->getFrame(), *(this->getThresholdValue()), this->THRESHOLDING_MAX_VALUE, *(this->getThresholdType()));
            break;
    }
}
