#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
class Class_Eroding_and_Dilating
{
public:
	Class_Eroding_and_Dilating(Mat _src);
	~Class_Eroding_and_Dilating();
	//��ʴ������
	void E_and_D();
	//�������㣬��̬�ݶȣ���ñ��ñ
	void Morphology();
	//�Ŵ���С
	void Up_and_Down();
	//��ֵ
	void Threshold();
	/*�����˲���*/
	void Self_Filter();
};

