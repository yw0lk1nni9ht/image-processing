#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
using namespace std;
using namespace cv;
using namespace ml;

class ReconizedImage
{
public:
	ReconizedImage();
	~ReconizedImage();
	void Reconized(Mat pic,int type, Ptr<SVM>svm);
};

