#pragma once
#include<opencv2\opencv.hpp>
#include<opencv2\xfeatures2D.hpp>
#include<opencv2\core\types_c.h>
#include<algorithm>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;


void pointMappingProcess(Mat &image1,Mat &image2,Mat &image1Out,Mat &image2Out,Mat &out,int detection,int mapping);
void pointMappingProcess(Mat &image1, Mat &image2, Mat &image1Out, Mat &image2Out, Mat &out, int detection, int mapping, vector<KeyPoint> &key1, vector<KeyPoint> &key2, vector<DMatch> &match);
void geometricCorrectionProcess(Mat &image1, Mat &image2, Mat &image1Out, Mat &image2Out, Mat &out, int detection, int mapping);
void imageMosaicProcess(Mat &image1, Mat &image2, Mat &image1Out, Mat &image2Out, Mat &out, int detection, int mapping);
void targetDetectionProcess(Mat& image1, Mat& image2, Mat& image1Out, Mat& image2Out, Mat& out, int detection, int mapping);