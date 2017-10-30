//
//  main.cpp
//  FaceRecognizer1
//
//  Created by 阿凡树 on 2017/10/23.
//  Copyright © 2017年 阿凡树. All rights reserved.
//
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;
int main(int argc, const char * argv[]) {
    
    VideoCapture cap(0);    //打开默认摄像头
    if (!cap.isOpened())
    {
        return -1;
    }
    Mat frame;
    Mat edges;
    Mat gray;
    
    CascadeClassifier eyeCascade;
    CascadeClassifier faceCascade;
    CascadeClassifier noseCascade;
    CascadeClassifier mouthCascade;
    bool stop = false;
    //训练好的文件名称，放置在可执行文件同目录下
    eyeCascade.load("/Volumes/extension/githubmy/learnopencv/opencv/FaceRecognizer1/haarcascade_eye.xml");
    faceCascade.load("/Volumes/extension/githubmy/learnopencv/opencv/FaceRecognizer1/haarcascade_frontalface_alt.xml");
    noseCascade.load("/Volumes/extension/githubmy/learnopencv/opencv/FaceRecognizer1/haarcascade_mcs_nose.xml");
    mouthCascade.load("/Volumes/extension/githubmy/learnopencv/opencv/FaceRecognizer1/haarcascade_mcs_mouth.xml");
    while (!stop)
    {
        cap >> frame;
        //建立用于存放人脸的向量容器
        vector<Rect> faces(0);
        vector<Rect> eyes(0);
        vector<Rect> nose(0);
        vector<Rect> mouth(0);
        cvtColor(frame, gray, CV_BGR2GRAY);
        //变换后的图像进行直方图均值化处理
        equalizeHist(gray, gray);
        
        faceCascade.detectMultiScale(gray, faces, 1.1, 2,0
                                     | CV_HAAR_FIND_BIGGEST_OBJECT
                                     | CV_HAAR_DO_ROUGH_SEARCH
                                     | CV_HAAR_SCALE_IMAGE,
                                     Size(200, 200));
        eyeCascade.detectMultiScale(gray, eyes, 1.1, 2,0
                                    | CV_HAAR_FIND_BIGGEST_OBJECT
                                    | CV_HAAR_DO_ROUGH_SEARCH
                                    | CV_HAAR_SCALE_IMAGE,
                                    Size(100, 100));
        noseCascade.detectMultiScale(gray, nose, 1.1, 2,0
                                    | CV_HAAR_FIND_BIGGEST_OBJECT
                                    | CV_HAAR_DO_ROUGH_SEARCH
                                    | CV_HAAR_SCALE_IMAGE,
                                    Size(50, 50));
        mouthCascade.detectMultiScale(gray, mouth, 1.1, 2,0
                                    | CV_HAAR_FIND_BIGGEST_OBJECT
                                    | CV_HAAR_DO_ROUGH_SEARCH
                                    | CV_HAAR_SCALE_IMAGE,
                                    Size(50, 50));
        // 用红色的线画出眼睛的框
        for (size_t i = 0;i<eyes.size(); i++)
        {
            if (eyes[i].height > 0 && eyes[i].width > 0)
            {
                rectangle(frame, eyes[i], Scalar(0, 0, 255), 1, 8, 0);
            }
        }
        // 用绿色的线画出脸的框
        for (size_t i = 0;i<faces.size(); i++)
        {
            if (faces[i].height > 0 && faces[i].width > 0)
            {
                rectangle(frame, faces[i], Scalar(0, 255, 0), 1, 8, 0);
            }
        }
        // 用青色的线画出脸的框
        for (size_t i = 0;i<nose.size(); i++)
        {
            if (nose[i].height > 0 && nose[i].width > 0)
            {
                rectangle(frame, nose[i], Scalar(255, 255, 0), 1, 8, 0);
            }
        }
        // 用黄色的线画出脸的框
        for (size_t i = 0;i<mouth.size(); i++)
        {
            if (mouth[i].height > 0 && mouth[i].width > 0)
            {
                rectangle(frame, mouth[i], Scalar(0, 255, 255), 1, 8, 0);
            }
        }
        imshow("face", frame);
        if (waitKey(50) >= 0)
            stop = true;
    }
    
    return 0;
}
