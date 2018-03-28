#include "stdafx.h"
#include "Feature.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"

Feature::Feature()
{
}

void Feature::FindFeature() {
	Mat src22 = imread("µ¥×Ö\\µÚ12ÕÅ£º0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("src", 0);
	imshow("src", src22);
	Mat out_src;

	vector<KeyPoint> keypoints;
	AGAST(src22,keypoints,50);
	//FAST(src22, keypoints, 100);

	
	drawKeypoints(src22, keypoints, out_src);
	namedWindow("look", 0);
	imshow("look", out_src);
}

Feature::~Feature()
{
}
