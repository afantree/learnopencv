//
//  main.cpp
//  opencv1
//
//  Created by 阿凡树 on 2017/5/8.
//  Copyright © 2017年 阿凡树. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main(int argc, char *argv[]) {
    Mat img = imread("/Volumes/extension/githubmy/learnopencv/opencv/opencv1/1.jpg");
    cout << format(img,"python") << endl;
    cout << "dims:" << img.dims << endl;
    cout << "rows:" << img.rows << endl;
    cout << "cols:" << img.cols << endl;
    cout << "channels:" << img.channels() << endl;
    cout << "type:" << img.type() << endl;
    cout << "depth:" << img.depth() << endl;
    cout << "elemSize:" << img.elemSize() << endl;
    cout << "elemSize1:" << img.elemSize1() << endl;
    cout << "step[0]" << img.step[0] << endl;
    cout << "step[1]" << img.step[1] << endl;
    cout << "step1[0]" << img.step1(0) << endl;
    cout << "step1[1]" << img.step1(1) << endl;
    imshow("载入的图片", img);
    waitKey(0);
}
