//
//  subfix.h
//  OpenCV
//
//  Created by Valentin Kostadinov on 14.05.19.
//  Copyright © 2019 Valentin Kostadinov. All rights reserved.
//

#pragma once
#include <opencv2/opencv.hpp>

int subpixSampleSafe ( const cv::Mat &pSrc, const cv::Point2f &p ){
    int x = int( floorf ( p.x ) );
    int y = int( floorf ( p.y ) );
    if ( x < 0 || x >= pSrc.cols - 1 ||
        y < 0 || y >= pSrc.rows - 1 ) {
        return 127;
    }
    
    int dx = int (256 * (p.x - floorf (p.x)));
    int dy = int (256 * (p.y - floorf (p.y)));
    unsigned char* i = ( unsigned char*) ((pSrc.data + y * pSrc.step) + x);
    int a = i[0] + ((dx * (i[ 1 ] - i[0])) >> 8);
    i += pSrc.step;
    int b = i[ 0 ] + ( ( dx * ( i[1] - i[0])) >> 8 );
    return a + (( dy * (b - a)) >> 8);
}
