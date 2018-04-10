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

	//最近邻算法
	void KNN_test();

	//支持向量机
	void SVM_test();


	~MachineLearning();
};

