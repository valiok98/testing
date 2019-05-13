//
//  window.cpp
//  OpenCV
//
//  Created by Valentin Kostadinov on 13.05.19.
//  Copyright © 2019 Valentin Kostadinov. All rights reserved.
//

#include "window.hpp"

Window::Window(std::string name) {
    this->name = name.length() ? name : "Video capture";
}

Window::~Window() {
    cv::destroyWindow(this->name);
}

std::string Window::getName() {
    return this->name;
}
