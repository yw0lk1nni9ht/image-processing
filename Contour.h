#pragma once
#include <opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

class Contour
{
public:
	Contour(Mat _src);
	~Contour();

	///ƥ��
	void Match(Mat temp);
	
	///Ѱ������
	void FindContours();

	//���������͹��
	void ConvexHull();
};

