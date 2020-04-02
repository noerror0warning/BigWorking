#pragma once
#include<opencv2\opencv.hpp>
#include<opencv2\xfeatures2D.hpp>
#include<algorithm>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;


void pointMappingProcess(Mat &image1,Mat &image2,Mat &image1Out,Mat &image2Out,Mat &out,int detection,int mapping);