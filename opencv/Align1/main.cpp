//
//  main.cpp
//  Align1
//
//  Created by 阿凡树 on 2017/11/10.
//  Copyright © 2017年 阿凡树. All rights reserved.
//

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;
using namespace cv;

////根据眼睛坐标对图像进行仿射变换
////src - 原图像
////landmarks - 原图像中68个关键点
Mat getwarpAffineImg(Mat &src, std::vector<Point2f> &landmarks)
{
    Mat oral;src.copyTo(oral);
    //for (int j = 0; j < landmarks.size(); j++)
    //{
    //    circle(oral, landmarks[j], 2, Scalar(255, 0, 0));
    //}
    Point2f leftEye = Point2f( (landmarks[36].x + landmarks[39].x) * 0.5f, (landmarks[36].y + landmarks[39].y) * 0.5f );
    Point2f rightEye = Point2f( (landmarks[42].x + landmarks[45].x) * 0.5f, (landmarks[42].y + landmarks[45].y) * 0.5f );
    //计算两眼中心点，按照此中心点进行旋转， 第31个为左眼坐标，36为右眼坐标
    Point2f eyesCenter = Point2f( (leftEye.x + rightEye.x) * 0.5f, (leftEye.y + rightEye.y) * 0.5f );
    // 计算两个眼睛间的角度
    double dy = (rightEye.y - leftEye.y);
    double dx = (rightEye.x - leftEye.x);
    double angle = atan2(dy, dx) * 180.0/CV_PI; // Convert from radians to degrees.
    //由eyesCenter, andle, scale按照公式计算仿射变换矩阵，此时1.0表示不进行缩放
    Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, 0.5);
    Mat rot;
    // 进行仿射变换，变换后大小为src的大小
    warpAffine(src, rot, rot_mat, src.size());
    std::vector<Point2f> marks;
    //按照仿射变换矩阵，计算变换后各关键点在新图中所对应的位置坐标。
    for (int n = 0; n<landmarks.size(); n++)
    {
        Point2f p = Point2f(0, 0);
        p.x = rot_mat.ptr<double>(0)[0] *landmarks[n].x + rot_mat.ptr<double>(0)[1] * landmarks[n].y + rot_mat.ptr<double>(0)[2];
        p.y = rot_mat.ptr<double>(1)[0] * landmarks[n].x + rot_mat.ptr<double>(1)[1] * landmarks[n].y + rot_mat.ptr<double>(1)[2];
        marks.push_back(p);
    }
    //标出关键点
    //for (int j = 0; j < landmarks.size(); j++)
    //{
    //    circle(rot, marks[j], 2, Scalar(0, 0, 255));
    //}
    float x = marks[0].x;
    float y = min(marks[24].y, marks[19].y);
    float w = marks[16].x-marks[0].x;
    float h = marks[8].y-y;
    Rect rect(x,y,w,h);
    return Mat(rot, rect);
}


int main(int argc, const char * argv[]) {
    // Load face detection and pose estimation models.
    frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor pose_model;
    // 模型下载地址：http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
    deserialize("/Volumes/extension/githubmy/learnopencv/opencv/Align1/shape_predictor_68_face_landmarks.dat") >> pose_model;
    
    cv::Mat temp = cv::imread("/Volumes/extension/githubmy/learnopencv/opencv/Align1/d9c695da852946a9127a08b6d301da7f.jpg");
    
    cv_image<bgr_pixel> cimg(temp);
    // Detect faces
    std::vector<dlib::rectangle> faces = detector(cimg);
    // Find the pose of each face.
    std::vector<full_object_detection> shapes;
    for (unsigned long i = 0; i < faces.size(); ++i)
        shapes.push_back(pose_model(cimg, faces[i]));
    
    Mat result;
    if (!shapes.empty()) {
        for (int j = 0; j < faces.size(); j++) {
            std::vector<Point2f> landmarks;
            for (int i = 0; i < 68; i++) {
                if (i <= 70) {
                    //putText(temp, to_string(i), cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 1, FONT_HERSHEY_PLAIN, cv::Scalar(0, 0, 255));
                    circle(temp, cvPoint(shapes[j].part(i).x(), shapes[j].part(i).y()), 2, cv::Scalar(0, 0, 255), -1);
                }
                Point2f f = Point2f(shapes[j].part(i).x(), shapes[j].part(i).y());
                landmarks.push_back(f);
            }
            result = getwarpAffineImg(temp, landmarks);
            char c[8];
            sprintf(c, "%d", j);
            imshow("After"+std::string(c), result);
        }
    }
    imshow("Before", temp);
    cv::waitKey(0);
}

