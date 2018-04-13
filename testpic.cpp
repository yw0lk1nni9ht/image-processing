// testpic.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include"Class_Fileter.h"
#include"Class_Eroding_and_Dilating.h"
#include "Blur.h"
#include "Arithmetic.h"
#include "Histogram.h"
#include "Contour.h"
#include "Feature.h"
#include "MachineLearning.h"

#include "集成测试.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

//声明区域
Mat cut(Mat src, Rect test);


int main()
{
	for (int i = 11; i < 81; i++)
	{
		// 载入原图像
		//Mat src = imread("testpicture\\num2.jpg", 1);
		Mat src = imread("100张样本\\" + std::to_string(i) + ".jpg", 1);
		//Mat src = imread("piaohao.jpg", 1);
		//Mat src = imread("pic2.jpg", 1);


		//平滑处理
		/*Class_Fileter a;
		a._src = src;
		a._i = 1;
		a._max_value = 71;
		a.first();*/

		//膨胀腐蚀等形态变化
		/*Class_Eroding_and_Dilating b(src);
		//膨胀
		b.E_and_D();
		//更多形态变化
		b.Morphology();
		//放大和缩小
		b.Up_and_Down();
		//阈值操作
		b.Threshold();
		//自制滤波
		b.Self_Filter();
		*/

		//降噪处理
		/*Blur b(src);
		b.View();
		b.BilateralFilter();
		b.Blur_junzhi();
		b.GaussianBlur();
		b.MedianBlur();
		*/

		//获取轮廓算法 
		/*
		Arithmetic c(src);
		c.Arithmetic_Sobel();
		c.Arithmetic_Laplacian();
		c.Arithmetic_Canny();
		c.Arithmetic_HoughLines();
		c.Arithmetic_HoughLinesP();
		*/

		//直方图
		/*
		Histogram d(src);
		d.Histogram_EqualizeHist();
		d.Histogram_Calc();
		Mat src2 = imread("piaohao2.jpg", 1);
		d.Histogram_Compare(src,src2 );
		d.Histogram_BackProject();
		*/

		//匹配相关
		/*src = imread("单字\\第11张：0.jpg");
		Contour e(src);
		Mat temp = imread("模板\\0\\0.jpg");
		namedWindow("样本", 0);
		imshow("样本", temp);
		e.Match(temp);
		//e.FindContours();
		//e.ConvexHull();*/

		//特征寻找，匹配
		/*
		//Feature a;
		//a.FindFeature();
		//a.MatchFeature();
		*/

		//机器学习
		/*
		//MachineLearning ML;
		//ML.KNN_test();
		//ML.SVM_test();
		*/
		
		
		集成测试 a(src);
		Mat newpic = cut(src, a.Need);		//裁剪到剩下票号得矩阵图
		//newpic = imread("14110657.jpg", 1);
		a.pichandle(newpic);				//获取并筛选大部分轮廓
		a.clean();							//对轮廓清洗和放大，留下最精确的8个票号轮廓
		
		for (int i = 0; i < a.saved_contours.size(); i++) {
			rectangle(newpic, a.saved_contours[i], Scalar(255, 64, 64));
		}
		imshow("new", newpic);

		/*显示每个数字*/
		cvtColor(newpic, newpic, 7);	//转成灰度
		threshold(newpic, newpic, 130, 255, THRESH_BINARY);		//二值化
																//2.去背景干扰
		Mat kernel = getStructuringElement(0, Size(3, 3));	//膨胀
		dilate(newpic, newpic, kernel);
		cout << "第" << i - 10 << "张票号：";
		for (int j = 0; j < a.saved_contours.size(); j++) {
			Mat test = newpic.clone();
			test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
			namedWindow(to_string(j), 0);
			imshow(to_string(j), test);
			//imwrite("单字\\第"+to_string(i)+"张："+ to_string(j) + ".jpg", test);
			
			//Ptr<SVM>svm = Algorithm::load<SVM>("svm.xml"); 
			///*Ptr<SVM> svm = SVM::create();
			//svm->load("svm.xml");*/
			//test.rows = 47;
			//test.cols = 32;
			//test.convertTo(test, CV_32FC1);
			//test = test.reshape(1, 1);
			//float response2 = svm->predict(test);  //进行预测，返回1或-1,返回类型为float
			//cout << response2 ;

			
			#pragma region 注释的模板匹配雏形
			//Mat _test = test.clone();
			//cvtColor(test, _test, 8);
			//for (int method_num = 0 ; method_num < 5 ; method_num++) 
			//{
			//	vector<Point> percent;		
			//	Mat result;
			//	for (int moban = 0; moban < 10; moban++) 
			//	{		
			//		try
			//		{
			//			Mat temp = imread("模板\\" + to_string(moban) + "\\0.jpg");
			//			matchTemplate(_test, temp, result, method_num); //0-5
			//														   ///归一化
			//			normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
			//			/// 通过函数 minMaxLoc 定位最匹配的位置
			//			double minVal; double maxVal; Point minLoc; Point maxLoc;
			//			Point matchLoc;
			//			minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
			//			if (method_num == CV_TM_SQDIFF || method_num == CV_TM_SQDIFF_NORMED)
			//			{
			//				matchLoc = minLoc;
			//			}
			//			else
			//			{
			//				matchLoc = maxLoc;
			//			}
			//			percent.push_back(matchLoc);
			//		}
			//		catch (exception e)
			//		{
			//			continue;
			//			// 处理 ExceptionName 异常的代码
			//		}
			//		
			//	}
			//}
#pragma endregion
		}
		cout << "\t正确号码" << "678910" << i;
		cout << endl << endl;
		waitKey();
	}

	waitKey();
	return 0;
}

//裁剪图片
Mat cut(Mat src, Rect test){
	Mat src2 = src.clone();
	int y = test.y + test.height - 90;
	int height = test.y + test.height - y;
	test.y = y;
	test.height = height;
	//rectangle(src2, test, Scalar(148, 0, 211), 2);
	//imshow("最终", src2);

	Mat newpic = src(Range(test.y, test.y + height), Range(test.x, test.x + test.width));
	//imshow("裁剪", newpic);


	return newpic;
}
