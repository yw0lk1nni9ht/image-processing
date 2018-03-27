#include "stdafx.h"
#include "Feature.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"

Feature::Feature()
{
}

void Feature::FindFeature() {
	Mat src22 = imread("piaohao.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	vector<KeyPoint> keypoints;
	//int minHessian = 400;
	//DescriptorExtractor a;
	AGAST(src22,keypoints,10);
	//FAST(src22, keypoints, 100);
	//AgastFeatureDetector c;
	//c.detect(src22, keypoints);
	Mat out_src;
	drawKeypoints(src22, keypoints, out_src);
	imshow("look", out_src);
}

Feature::~Feature()
{
}
