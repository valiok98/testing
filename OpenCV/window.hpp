//
//  window.hpp
//  OpenCV
//
//  Created by Valentin Kostadinov on 13.05.19.
//  Copyright © 2019 Valentin Kostadinov. All rights reserved.
//

#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

class Window {
    std::string name;
public:
    Window(std::string);
    ~Window();
    std::string getName();
};
