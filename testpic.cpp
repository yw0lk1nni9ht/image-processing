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


//test
Mat img, image;
Mat targetData, backData;
bool flag = true;
string wdname = "image";

void on_mouse(int event, int x, int y, int flags, void* ustc); //鼠标取样本点
void getTrainData(Mat &train_data, Mat &train_label);  //生成训练数据 
void svm(); //svm分类


int main()
{
	/*string path = "模板\\0\\0.jpg";
	img = imread(path);
	img.copyTo(image);
	if (img.empty())
	{
		cout << "Image load error";
		return 0;
	}
	namedWindow(wdname);
	setMouseCallback(wdname, on_mouse, 0);

	for (;;)
	{
		imshow("image", img);

		int c = waitKey(0);
		if ((c & 255) == 27)
		{
			cout << "Exiting ...\n";
			break;
		}
		if ((char)c == 'c')
		{
			flag = false;
		}
		if ((char)c == 'q')
		{
			destroyAllWindows();
			break;
		}
	}
	svm();
	return 0;
	*/


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

		//Class_Eroding_and_Dilating b(src);
		//膨胀
		//b.E_and_D();
		//更多形态变化
		//b.Morphology();
		//放大和缩小
		//b.Up_and_Down();
		//阈值操作
		//b.Threshold();
		//自制滤波
		//b.Self_Filter();

		//Blur b(src);
		//b.View();
		//b.BilateralFilter();
		//b.Blur_junzhi();
		//b.GaussianBlur();
		//b.MedianBlur();

		//Arithmetic c(src);
		//c.Arithmetic_Sobel();
		//c.Arithmetic_Laplacian();
		//c.Arithmetic_Canny();
		//c.Arithmetic_HoughLines();
		//c.Arithmetic_HoughLinesP();

		//Histogram d(src);
		//d.Histogram_EqualizeHist();
		//d.Histogram_Calc();
		//Mat src2 = imread("piaohao2.jpg", 1);
		//d.Histogram_Compare(src,src2 );
		//d.Histogram_BackProject();

	/*	src = imread("单字\\第11张：0.jpg");
		Contour e(src);
		Mat temp = imread("模板\\0\\0.jpg");
		namedWindow("样本", 0);
		imshow("样本", temp);
		e.Match(temp);*/
		//e.FindContours();
		//e.ConvexHull();

		//Feature a;
		//a.FindFeature();
		//a.MatchFeature();

		MachineLearning ML;
		//ML.KNN_test();
		//ML.SVM_test();


		集成测试 a(src);
		Mat newpic = src(Range(a.Need.y, a.Need.y + a.Need.height), Range(a.Need.x, a.Need.x + a.Need.width));
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat test;
		Canny(newpic, test, 10, 10 * 2, 3);
		/// 寻找轮廓
		findContours(test, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		for (int i = 0; i < contours.size() - 1; i++) {
			Rect a = boundingRect(contours[i]);
			rectangle(test, a, Scalar(148, 0, 211));
		}
		
		/*Mat newpic = cut(src, a.Need);
		a.pichandle(newpic);
		a.clean();*/
		

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
			imshow(to_string(j), test);
			//imwrite("单字\\第"+to_string(i)+"张："+ to_string(j) + ".jpg", test);
			
			Ptr<SVM>svm = Algorithm::load<SVM>("svm.xml"); 
			/*Ptr<SVM> svm = SVM::create();
			svm->load("svm.xml");*/
			test.rows = 47;
			test.cols = 32;
			test.convertTo(test, CV_32FC1);
			test = test.reshape(1, 1);
			float response2 = svm->predict(test);  //进行预测，返回1或-1,返回类型为float
			cout << response2 ;

			
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

//鼠标在图像上取样本点，按q键退出
void on_mouse(int event, int x, int y, int flags, void* ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		Point pt = Point(x, y);
		Vec3b point = img.at<Vec3b>(y, x);  //取出该坐标处的像素值，注意x,y的顺序
		Mat tmp = (Mat_<float>(1, 3) << point[0], point[1], point[2]);
		if (flag)
		{
			targetData.push_back(tmp); //加入正样本矩阵
			circle(img, pt, 2, Scalar(0, 255, 255), -1, 8); //画出点击的点 

		}

		else
		{
			backData.push_back(tmp); //加入负样本矩阵
			circle(img, pt, 2, Scalar(255, 0, 0), -1, 8);

		}
		imshow(wdname, img);
	}
}

void getTrainData(Mat &train_data, Mat &train_label)
{
	int m = targetData.rows;
	int n = backData.rows;
	cout << "正样本数：:" << m << endl;
	cout << "负样本数：" << n << endl;
	vconcat(targetData, backData, train_data); //合并所有的样本点，作为训练数据
	train_label = Mat(m + n, 1, CV_32S, Scalar::all(1)); //初始化标注
	for (int i = m; i < m + n; i++)
		train_label.at<int>(i, 0) = -1;
}

void svm()
{
	Mat train_data, train_label;
	getTrainData(train_data, train_label); //获取鼠标选择的样本训练数据

										   // 设置参数
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);

	// 训练分类器
	Ptr<TrainData> tData = TrainData::create(train_data, ROW_SAMPLE, train_label);
	svm->train(tData);

	Vec3b color(0, 0, 0);
	// Show the decision regions given by the SVM
	for (int i = 0; i < image.rows; ++i)
		for (int j = 0; j < image.cols; ++j)
		{
			Vec3b point = img.at<Vec3b>(i, j);  //取出该坐标处的像素值
			Mat sampleMat = (Mat_<float>(1, 3) << point[0], point[1], point[2]);
			float response = svm->predict(sampleMat);  //进行预测，返回1或-1,返回类型为float
			if ((int)response != 1)
				image.at<Vec3b>(i, j) = color;  //将背景设置为黑色
		}

	imshow("SVM Simple Example", image);
	waitKey(0);
}