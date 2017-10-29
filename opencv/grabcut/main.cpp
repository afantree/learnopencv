//
//  main.cpp
//  grabcut
//
//  Created by 阿凡树 on 2017/6/19.
//  Copyright © 2017年 阿凡树. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;

//自己本地的图片
string filename = "/Volumes/extension/githubmy/learnopencv/opencv/grabcut/3.jpg";
Mat image;
string winName = "show";
enum{NOT_SET = 0, IN_PROCESS = 1, SET = 2};
uchar rectState;
Rect rect;
Mat mask;
const Scalar GREEN = Scalar(0,255,0);
Mat bgdModel, fgdModel;

static void getBinMask( const Mat& comMask, Mat& binMask ){
    binMask.create( comMask.size(), CV_8UC1 );
    binMask = comMask & 1;
}

void on_mouse( int event, int x, int y, int flags, void* )
{
    Mat res;
    Mat binMask;
    
    switch( event ){
        case CV_EVENT_LBUTTONDOWN:
            if( rectState == NOT_SET){
                rectState = IN_PROCESS;
                rect = Rect( x, y, 1, 1 );
            }
            break;
        case CV_EVENT_LBUTTONUP:
            if( rectState == IN_PROCESS ){
                rect = Rect( Point(rect.x, rect.y), Point(x,y) );
                rectState = SET;
                (mask(rect)).setTo( Scalar(GC_PR_FGD));
                image = imread( filename, 1 );
                grabCut(image, mask, rect, bgdModel, fgdModel, 2, GC_INIT_WITH_RECT);
                getBinMask( mask, binMask );
                image.copyTo(res, binMask );
                Mat res1;
                res(rect).copyTo(res1);
                namedWindow("Cut",0);
                imshow("Cut", res1);
                moveWindow("Cut", 200,200);
            }
            break;
        case CV_EVENT_MOUSEMOVE:
            if( rectState == IN_PROCESS ){
                rect = Rect( Point(rect.x, rect.y), Point(x,y) );
                image = imread( filename, 1 );
                // 用鼠标画一个框
                Point temp(rect.x + rect.width, rect.y + rect.height );
                rectangle(image, Point( rect.x, rect.y ), temp, GREEN, 2);
                imshow(winName, image);
            }
            break;
    }
}

int main(int argc, char* argv[]){
    // 加载并显示图片
    image = imread( filename, 1 );
    imshow(winName, image);
    // 设置掩码，首先创建了一个和源图片一样大小的掩码空间。
    mask.create(image.size(), CV_8UC1);
    // 接着将整个掩码空间设置为背景：GC_BGD。
    mask.setTo(GC_BGD);
    rectState = NOT_SET;
    // 设置鼠标回调
    setMouseCallback(winName, on_mouse, 0);
    waitKey(0);
    return 0;
}
