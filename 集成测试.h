#pragma once

#include "stdafx.h"
#include <iostream>
#include"Class_Fileter.h"
#include"Class_Eroding_and_Dilating.h"
#include "Blur.h"
#include "Arithmetic.h"
#include "Histogram.h"
#include "Contour.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class ���ɲ���
{
public:
	//��ȡƱ�����򲢷��ؾ���
	���ɲ���(Mat _src, int type);	//type����2��Ʊ��Ӧ���ں�ѡ��
		
	//һϵ�д����ȡ�����
	void pichandle(Mat handle_src);
	//�������˲�����
	void clean();
	

	Rect Need;		//����Ŀ�
	vector<Rect> saved_contours;		//�������Ҫ�Ŀ���




	~���ɲ���();
	
	
};

