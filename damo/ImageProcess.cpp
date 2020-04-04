#include "pch.h"
#include "ImageProcess.h"



void pointMappingProcess(Mat& image1, Mat& image2, Mat& image1Out, Mat& image2Out, Mat& out, int detection, int mapping){
	vector<KeyPoint>keypoints1, keypoints2;
	Mat discriptions1, discriptions2;
	//两种检测方法
	if (detection == 0) {
		Ptr<SIFT>sift = SIFT::create(200,3,0.04,20.0);
		//检查关键点
		sift->detect(image1, keypoints1);
		sift->detect(image2, keypoints2);
		//在图中绘制关键点（并不是原图
		drawKeypoints(image1, keypoints1, image1Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		drawKeypoints(image2, keypoints2, image2Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		//计算特征向量
		sift->compute(image1, keypoints1, discriptions1);
		sift->compute(image2, keypoints2, discriptions2);
	}
	else {
		Ptr<SURF>surf = SURF::create(1000.0);
		surf->detect(image1, keypoints1);
		surf->detect(image2, keypoints2);
		drawKeypoints(image1, keypoints1, image1Out);
		drawKeypoints(image2, keypoints2, image2Out);
		surf->compute(image1, keypoints1, discriptions1);
		surf->compute(image2, keypoints2, discriptions2);
	}
	//记录
	vector<DMatch>matches;
	Ptr<DescriptorMatcher>matcher;
	switch (mapping){
	case 0: {
		matcher = DescriptorMatcher::create("BruteForce");
		break;
	}
	case 1: {
		matcher = DescriptorMatcher::create("FlannBased");
		break;
	}
	default:
		break;
	}
	matcher->match(discriptions1, discriptions2, matches);
	sort(matches.begin(), matches.end());
	//vector<DMatch> match(matches.begin(), matches.begin() + int(matches.size() / 2));
	vector<DMatch> match(matches.begin(), matches.begin() + min(50,int(matches.size()/2)));
	//绘制匹配图
	drawMatches(image1, keypoints1, image2, keypoints2, match, out);
}

//几何矫正中的特征点检测
void pointMappingProcess(Mat &image1, Mat &image2, Mat &image1Out, Mat &image2Out, Mat &out, int detection, int mapping, vector<KeyPoint> &key1, vector<KeyPoint> &key2, vector<DMatch> &matches)
{
	Mat discriptions1, discriptions2;
	//两种检测方法
	if (detection == 0) {
		Ptr<SIFT>sift = SIFT::create(200, 3, 0.04, 20.0);
		//检查关键点
		sift->detect(image1, key1);
		sift->detect(image2, key2);
		//在图中绘制关键点（并不是原图
		drawKeypoints(image1, key1, image1Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		drawKeypoints(image2, key2, image2Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		//计算特征向量
		sift->compute(image1, key1, discriptions1);
		sift->compute(image2, key2, discriptions2);
	}
	else {
		Ptr<SURF>surf = SURF::create(1000.0);
		surf->detect(image1, key1);
		surf->detect(image2, key2);
		drawKeypoints(image1, key1, image1Out);
		drawKeypoints(image2, key2, image2Out);
		surf->compute(image1, key1, discriptions1);
		surf->compute(image2, key2, discriptions2);
	}
	//记录
	Ptr<DescriptorMatcher>matcher;
	switch (mapping) {
	case 0: {
		matcher = DescriptorMatcher::create("BruteForce");
		break;
	}
	case 1: {
		matcher = DescriptorMatcher::create("FlannBased");
		break;
	}
	default:
		break;
	}
	matcher->match(discriptions1, discriptions2, matches);
	sort(matches.begin(), matches.end());
	//vector<DMatch> match(matches.begin(), matches.begin() + int(matches.size() / 2));
	vector<DMatch> match(matches.begin(), matches.begin() + min(50, int(matches.size() / 2)));
	//绘制匹配图
	//drawMatches(image1, key1, image2, key2, matches, out);
}

//几何矫正
void geometricCorrectionProcess(Mat &image1, Mat &image2, Mat &image1Out, Mat &image2Out, Mat &out, int detection, int mapping)
{
	vector<KeyPoint>keypoints1, keypoints2;
	vector<DMatch>matches;
	pointMappingProcess(image1, image2, image1Out, image2Out, out, detection, mapping, keypoints1, keypoints2, matches);

	//坐标转化为float类型
	vector<Point2f> points1, points2;

	for (size_t i = 0; i < matches.size(); i++)
	{
		points1.push_back(keypoints1[matches[i].queryIdx].pt);
		points2.push_back(keypoints2[matches[i].trainIdx].pt);
	}

	Mat homo = findHomography(points2, points1, RANSAC);

	warpPerspective(image2, out, homo, image1.size());
}
