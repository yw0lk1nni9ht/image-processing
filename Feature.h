#pragma once

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
using namespace std;
using namespace cv;

class Feature
{
public:
	Feature();

	//��������
	void FindFeature();
	//����ƥ��
	void MatchFeature();

	~Feature();
};

