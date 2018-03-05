#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
class Class_Eroding_and_Dilating
{
public:
	Class_Eroding_and_Dilating(Mat _src);
	~Class_Eroding_and_Dilating();
	//腐蚀和膨胀
	void E_and_D();
	//开闭运算，形态梯度，顶帽黑帽
	void Morphology();
	//放大缩小
	void Up_and_Down();
	//阈值
	void Threshold();
	/*定制滤波器*/
	void Self_Filter();
};

