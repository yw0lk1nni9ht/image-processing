#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include <iostream>
using namespace std;
using namespace cv;
using namespace ml;

class MachineLearning
{
public:
	MachineLearning();

	//������㷨
	void KNN_test();

	//֧��������
	void SVM_test();


	~MachineLearning();
};

