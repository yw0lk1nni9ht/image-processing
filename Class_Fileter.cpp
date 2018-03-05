#include "stdafx.h"
#include "Class_Fileter.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

/*************************************
//									//
//									//
//			ͼ��ƽ������			//
//									//
//									//
*************************************/

Mat src_temp;
int Filter_i;
int max_value;
Mat dst_blur2;	//��ֵ
Mat dst_GaussianBlur2;//��˹
Mat dst_medianBlur2;//��ֵ
Mat dst_bilateralFilter2;//˫��

void Filter(int, void *) {
	if ((Filter_i % 2) != 0)
	{
		//��һ���˲���
		blur(src_temp, dst_blur2, Size(Filter_i, Filter_i));
		//��˹�˲���
		GaussianBlur(src_temp, dst_GaussianBlur2, Size(Filter_i, Filter_i), 0, 0);
		//��ֵ�˲���
		medianBlur(src_temp, dst_medianBlur2, Filter_i);
		//˫���˲�
		bilateralFilter(src_temp, dst_bilateralFilter2, Filter_i, Filter_i * 2, Filter_i / 2);
		imshow("ԭͼ", src_temp);
		imshow("��ֵģ��", dst_blur2);
		imshow("��˹ģ��", dst_GaussianBlur2);
		imshow("��ֵģ��", dst_medianBlur2);
		imshow("˫��ģ��", dst_bilateralFilter2);
	}
}

Class_Fileter::Class_Fileter()
{
	

}

void Class_Fileter::first(){
	src_temp = Class_Fileter::_src;
	Filter_i = Class_Fileter::_i;
	max_value = Class_Fileter::_max_value;
	namedWindow("���ƻ���", 0);
	createTrackbar("ģ���̶�", "���ƻ���", &Filter_i, max_value,Filter);
	namedWindow("ԭͼ", 0);
	namedWindow("��ֵģ��", 0);
	namedWindow("��˹ģ��", 0);
	namedWindow("��ֵģ��", 0);
	namedWindow("˫��ģ��", 0);
	Filter(0, 0);
}

Class_Fileter::~Class_Fileter()
{
}

