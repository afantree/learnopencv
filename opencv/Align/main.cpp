//
//  main.cpp
//  Align
//
//  Created by 阿凡树 on 2017/11/9.
//  Copyright © 2017年 阿凡树. All rights reserved.
//

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;
//原始图像大小
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
//输出的图像大小（裁剪后的）
#define IMAGE_WIDTH_STD 90
#define IMAGE_HEIGHT_STD 90

#define LANDMARK_SIZE 8//对齐点的个数
#define LANDMARK_SIZE_DOUBLE 16//对齐点个数的两倍

Point2f srcTri[LANDMARK_SIZE];//对齐点的Point2f数组,检测到的人脸对齐点，注意这里是基于原始图像的坐标点
Point2f destTri[LANDMARK_SIZE];//对齐点的Point2f数组,模板的Landmarks，注意这是一个基于输出图像大小尺寸的坐标点
//对齐点的double数组
double template_landmark[LANDMARK_SIZE_DOUBLE*2] = {
    0.0792396913815, 0.339223741112, 0.0829219487236, 0.456955367943,
    0.0967927109165, 0.575648016728, 0.122141515615, 0.691921601066,
    0.168687863544, 0.800341263616, 0.239789390707, 0.895732504778,
    0.325662452515, 0.977068762493, 0.422318282013, 1.04329000149,
    0.531777802068, 1.06080371126, 0.641296298053, 1.03981924107,
    0.738105872266, 0.972268833998, 0.824444363295, 0.889624082279,
    0.894792677532, 0.792494155836, 0.939395486253, 0.681546643421,
    0.96111933829, 0.562238253072, 0.970579841181, 0.441758925744
};

////根据眼睛坐标对图像进行仿射变换
////src - 原图像
////landmarks - 原图像中68个关键点
//Mat getwarpAffineImg(Mat &src, vector<Point2f> &landmarks)
//{
//    Mat oral;src.copyTo(oral);
//    for (int j = 0; j < landmarks.size(); j++)
//    {
//        circle(oral, landmarks[j], 2, Scalar(255, 0, 0));
//    }
//    //计算两眼中心点，按照此中心点进行旋转， 第31个为左眼坐标，36为右眼坐标
//    Point2f eyesCenter = Point2f( (landmarks[31].x + landmarks[36].x) * 0.5f, (landmarks[31].y + landmarks[36].y) * 0.5f );
//    // 计算两个眼睛间的角度
//    double dy = (landmarks[36].y - landmarks[31].y);
//    double dx = (landmarks[36].x - landmarks[31].x);
//    double angle = atan2(dy, dx) * 180.0/CV_PI; // Convert from radians to degrees.
//    //由eyesCenter, andle, scale按照公式计算仿射变换矩阵，此时1.0表示不进行缩放
//    Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, 1.0);
//    Mat rot;
//    // 进行仿射变换，变换后大小为src的大小
//    warpAffine(src, rot, rot_mat, src.size());
//    vector<Point2f> marks;
//    //按照仿射变换矩阵，计算变换后各关键点在新图中所对应的位置坐标。
//    for (int n = 0; n<landmarks.size(); n++)
//    {
//        Point2f p = Point2f(0, 0);
//        p.x = rot_mat.ptr<double>(0)[0] *landmarks[n].x + rot_mat.ptr<double>(0)[1] * landmarks[n].y + rot_mat.ptr<double>(0)[2];
//        p.y = rot_mat.ptr<double>(1)[0] * landmarks[n].x + rot_mat.ptr<double>(1)[1] * landmarks[n].y + rot_mat.ptr<double>(1)[2];
//        marks.push_back(p);
//    }
//    //标出关键点
//    for (int j = 0; j < landmarks.size(); j++)
//    {
//        circle(rot, marks[j], 2, Scalar(0, 0, 255));
//    }
//    return rot;
//}

int main(int argc, const char * argv[]) {
    VideoCapture vcap(0);    //打开默认摄像头
    if (!vcap.isOpened())
    {
        return -1;
    }
    
    for (int i = 0; i < LANDMARK_SIZE; i++)
    {
        srcTri[i] = Point2f(template_landmark[i * 2] * 90 + IMAGE_HEIGHT / 2, template_landmark[i * 2 + 1] * 90 + IMAGE_WIDTH / 2);
        destTri[i] = Point2f(template_landmark[i * 2] * IMAGE_HEIGHT_STD, template_landmark[i * 2 + 1] * IMAGE_WIDTH_STD);
    }
    Mat warp_mat = getAffineTransform( srcTri, destTri );//使用仿射变换，计算H矩阵
//    Mat warp_mat = estimateRigidTransform(srcTri, destTri, false);//使用相似变换，不适合使用仿射变换，会导致图像变形
    Mat frame;
    Mat warp_frame(200, 200, CV_8UC3);
    while (1)
    {
        vcap >> frame;
        warpAffine(frame, warp_frame, warp_mat, warp_frame.size());//裁剪图像

        imshow("frame", frame);//显示原图像
        imshow("warp_frame", warp_frame);//显示裁剪后得到的图像

        waitKey(10);
    }
    return 0;
}
