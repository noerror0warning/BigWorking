#include "pch.h"
#include "ImageProcess.h"


Point2f getTransformPoint(const Point2f originalPoint, const Mat &transformMaxtri)
{
	Mat originalP, targetP;
	originalP = (Mat_<double>(3, 1) << originalPoint.x, originalPoint.y, 1.0);
	targetP = transformMaxtri * originalP;
	float x = targetP.at<double>(0, 0) / targetP.at<double>(2, 0);
	float y = targetP.at<double>(1, 0) / targetP.at<double>(2, 0);
	return Point2f(x, y);
}


void pointMappingProcess(Mat& image1, Mat& image2, Mat& image1Out, Mat& image2Out, Mat& out, int detection, int mapping){
	vector<KeyPoint>keypoints1, keypoints2;
	Mat discriptions1, discriptions2;
	//���ּ�ⷽ��
	if (detection == 0) {
		Ptr<SIFT>sift = SIFT::create(200,3,0.04,20.0);
		//���ؼ���
		sift->detect(image1, keypoints1);
		sift->detect(image2, keypoints2);
		//��ͼ�л��ƹؼ��㣨������ԭͼ
		drawKeypoints(image1, keypoints1, image1Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		drawKeypoints(image2, keypoints2, image2Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		//������������
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
	//��¼
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
	//����ƥ��ͼ
	drawMatches(image1, keypoints1, image2, keypoints2, match, out);
}

void pointMappingProcess(Mat& image1, Mat& image2, Mat& image1Out, Mat& image2Out, Mat& out, int detection, int mapping, vector<KeyPoint>& key1, vector<KeyPoint>& key2, vector<DMatch>& matches)
{
	Mat discriptions1, discriptions2;
	//���ּ�ⷽ��
	if (detection == 0) {
		Ptr<SIFT>sift = SIFT::create(1000, 3, 0.04, 20.0);
		//���ؼ���
		sift->detect(image1, key1);
		sift->detect(image2, key2);
		//��ͼ�л��ƹؼ��㣨������ԭͼ
		drawKeypoints(image1, key1, image1Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		drawKeypoints(image2, key2, image2Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		//������������
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
	//��¼
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
	//sort(matches.begin(), matches.end());
	////vector<DMatch> match(matches.begin(), matches.begin() + int(matches.size() / 2));
	//vector<DMatch> match(matches.begin(), matches.begin() + min(50, int(matches.size() / 2)));
	//����ƥ��ͼ

	// ����ؼ����������С����
	double maxDist = 0;
	double minDist = 100;
	for (int i = 0; i < discriptions1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < minDist)
			minDist = dist;
		if (dist > maxDist)
			maxDist = dist;
	}
	//�����ϺõĹؼ��� С��������С����
	vector< DMatch > goodMatches;

	for (int i = 0; i < discriptions1.rows; i++)
	{
		if (matches[i].distance < 2 * minDist)
			goodMatches.push_back(matches[i]);
	}

	matches = goodMatches;

	////drawMatches(image1, key1, image2, key2, goodMatches, out,
	////	Scalar::all(-1), Scalar::all(-1), vector<char>(),
	////	DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS //����ʾδƥ��ĵ�
	////);

	drawMatches(image1, key1, image2, key2, matches, out);
}

//���ν���
void geometricCorrectionProcess(Mat &image1, Mat &image2, Mat &image1Out, Mat &image2Out, Mat &out, int detection, int mapping)
{
	vector<KeyPoint>keypoints1, keypoints2;
	vector<DMatch>matches;
	pointMappingProcess(image1, image2, image1Out, image2Out, out, detection, mapping, keypoints1, keypoints2, matches);

	//����ת��Ϊfloat����
	vector<Point2f> points1, points2;

	for (size_t i = 0; i < matches.size(); i++)
	{
		points1.push_back(keypoints1[matches[i].queryIdx].pt);
		points2.push_back(keypoints2[matches[i].trainIdx].pt);
	}
	cout << matches.size();

	Mat homo = findHomography(points2, points1, RANSAC);

	warpPerspective(image2, out, homo, image1.size());
}

//ͼ��ƴ��
void imageMosaicProcess(Mat &image1, Mat &image2, Mat &image1Out, Mat &image2Out, Mat &out, int detection, int mapping)
{
	vector<KeyPoint>keypoints1, keypoints2;
	vector<DMatch>matches;

	pointMappingProcess(image1, image2, image1Out, image2Out, out, detection, mapping, keypoints1, keypoints2, matches);

	vector<Point2f> points1, points2;

	//���ǰN������ƥ��������
	for (int i = 0; i < 10; i++)
	{
		points1.push_back(keypoints1[matches[i].queryIdx].pt);
		points2.push_back(keypoints2[matches[i].trainIdx].pt);
	}

	Mat homo = findHomography(points1, points2, RANSAC);//��Ӧ�;���
	Mat adjustMat = (Mat_<double>(3, 3) << 1.0, 0, image1.cols, 0, 1.0, 0, 0, 0, 1.0);
	Mat adjustHomo = adjustMat * homo;

	//����任
	Point2f oriLinkePoint, tarLinkPoint, baseImagePoint;
	oriLinkePoint = keypoints1[matches[0].queryIdx].pt;
	tarLinkPoint = getTransformPoint(oriLinkePoint, adjustHomo);
	baseImagePoint = keypoints2[matches[0].trainIdx].pt;

	//ͼ����׼
	warpPerspective(image1, out, adjustHomo, Size(image2.cols + image1.cols + 10, image2.rows));

	//����ͻ��
	Mat image1Overlap, image2Overlap;
	image1Overlap = out(Rect(Point(tarLinkPoint.x - baseImagePoint.x, 0), Point(tarLinkPoint.x, image2.rows)));
	image2Overlap = image2(Rect(0, 0, image1Overlap.cols, image1Overlap.rows));
	Mat image1ROICopy = image1Overlap.clone();  
	for (int i = 0; i < image1Overlap.rows; i++)
	{
		for (int j = 0; j < image1Overlap.cols; j++)
		{
			double weight;
			weight = (double)j / image1Overlap.cols;  
			image1Overlap.at<Vec3b>(i, j)[0] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[0] + weight * image2Overlap.at<Vec3b>(i, j)[0];
			image1Overlap.at<Vec3b>(i, j)[1] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[1] + weight * image2Overlap.at<Vec3b>(i, j)[1];
			image1Overlap.at<Vec3b>(i, j)[2] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[2] + weight * image2Overlap.at<Vec3b>(i, j)[2];
		}
	}

	//�ν�
	Mat ROIMat = image2(Rect(Point(baseImagePoint.x, 0), Point(image2.cols, image2.rows)));
	ROIMat.copyTo(Mat(out, Rect(tarLinkPoint.x, 0, image2.cols - baseImagePoint.x + 1, image2.rows)));
	
}
//Ŀ����
void targetDetectionProcess(Mat& image1, Mat& image2, Mat& image1Out, Mat& image2Out, Mat& out, int detection, int mapping)
{

	vector<KeyPoint>keypoints1, keypoints2;
	vector<DMatch>matches;

	pointMappingProcess(image1, image2, image1Out, image2Out, out, detection, mapping, keypoints1, keypoints2, matches);



	//ʹ��findHomography�ҳ���Ӧ��͸�ӱ任
	vector<Point2f> points1, points2;
	for (size_t i = 0; i < matches.size(); i++)
	{
		points1.push_back(keypoints1[matches[i].queryIdx].pt);
		points2.push_back(keypoints2[matches[i].trainIdx].pt);
	}
	//cout << matches.size();
	Mat homo = findHomography(points1, points2, RANSAC);

	//ʹ��perspectiveTransformӳ���Ⱥ���ڳ����л�ȡĿ��λ��

	vector<Point2f> objCorners(4);
	objCorners[0] = cvPoint(0, 0);
	objCorners[1] = cvPoint(image1.cols, 0);
	objCorners[2] = cvPoint(image1.cols, image1.rows);
	objCorners[3] = cvPoint(0, image1.rows);
	vector<Point2f> sceneCorners(4);
	perspectiveTransform(objCorners, sceneCorners, homo);

	//-- �ڱ���⵽��Ŀ���ĸ���֮�仮��
	line(out, sceneCorners[0] + Point2f(image1.cols, 0), sceneCorners[1] + Point2f(image1.cols, 0), Scalar(0, 255, 0), 4);
	line(out, sceneCorners[1] + Point2f(image1.cols, 0), sceneCorners[2] + Point2f(image1.cols, 0), Scalar(0, 255, 0), 4);
	line(out, sceneCorners[2] + Point2f(image1.cols, 0), sceneCorners[3] + Point2f(image1.cols, 0), Scalar(0, 255, 0), 4);
	line(out, sceneCorners[3] + Point2f(image1.cols, 0), sceneCorners[0] + Point2f(image1.cols, 0), Scalar(0, 255, 0), 4);


}

