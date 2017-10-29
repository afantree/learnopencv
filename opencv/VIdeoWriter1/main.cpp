//
//  main.cpp
//
//  Created by 阿凡树 on 2017/6/12.
//  Copyright © 2017年 阿凡树. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

int main(int, char**)
{
    cv::VideoCapture cap(0); // 打开相机
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    if(!cap.isOpened())  // 检查是否打开成功
        return -1;
    
    Mat edges;
    namedWindow("before",1);
    namedWindow("lines",1);
    namedWindow("fill",1);
    for(;;)
    {
        Mat img;
        cap >> img;
        imshow("before", img);
        
        Point root_points[1][9];
        root_points[0][0] = Point(20,40);
        root_points[0][1] = Point(img.cols-30,10);
        root_points[0][2] = Point(img.cols-20,img.rows-40);
        root_points[0][3] = Point(img.cols/2.0+20,img.rows-25);
        root_points[0][4] = Point(img.cols-50,img.rows/2.0);
        
        root_points[0][5] = Point(img.cols*0.75,100);
        root_points[0][6] = Point(img.cols/2.0+20,img.rows/2.0-20);
        root_points[0][7] = Point(img.cols/2.0,50);
        root_points[0][8] = Point(img.cols/2.0-20,img.rows/2.0-20);
        root_points[0][9] = Point(img.cols*0.25,160);
        
        root_points[0][10] = Point(50,img.rows/2.0);
        root_points[0][11] = Point(img.cols/2.0-20,img.rows-20);
        root_points[0][12] = Point(30,img.rows-30);
        
        const Point* ppt[1] = {root_points[0]};
        int npt[] = {13};
        // 画一个多边形
        polylines(img, ppt, npt, 1, 1, Scalar(255),1,8,0);
        imshow("lines", img);
        // 填充多边形
        fillPoly(img, ppt, npt, 1, Scalar(0,255,0));
        imshow("fill", img);
        
        if(waitKey(30) >= 0) break;
    }
    cap.release();
    return 0;
}
