#include "stdafx.h"
#include "Histogram.h"


/*************************************
//									//
//									//
//			直方图相关				//
//									//
//									//
*************************************/

//声明全局变量
Mat Histogram_Src;

//反向投影
Mat HSV;
Mat Hue;
int bins;

Histogram::Histogram(Mat src)
{
	Histogram_Src = src;
}


//直方图均衡化
void Histogram::Histogram_EqualizeHist() {
	Mat Histogram_Dst;
	Mat dst;
	cvtColor(Histogram_Src, Histogram_Dst, CV_BGR2GRAY);
	equalizeHist(Histogram_Dst, dst);
	namedWindow("equalizeHist", 0);
	imshow("equalizeHist", dst);
	imshow("equalizeHist2", Histogram_Src);
}


//直方图计算
void Histogram::Histogram_Calc() {
	/// 分割成3个单通道图像 ( R, G 和 B )
	vector<Mat> rgb_planes;
	split(Histogram_Src, rgb_planes);

	// 设定bin数目
	int histSize = 255;

	/// 设定取值范围 ( R,G,B) )
	float range[] = { 0, 255 };
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;

	Mat r_hist, g_hist, b_hist;
	/// 计算直方图:
	calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);

	// 创建直方图画布
	int hist_w = 400; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

	/// 将直方图归一化到范围 [ 0, histImage.rows ]
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// 在直方图画布上画出直方图
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	/// 显示直方图
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);
}


//直方图对比
void Histogram::Histogram_Compare(Mat pic1 , Mat pic2) {
	//转换为HSV
	cvtColor(pic1, pic1, CV_BGR2HSV);
	Mat pic1_half = pic1(Range(pic1.rows / 2, pic1.rows - 1), Range(0, pic1.cols - 1));
	cvtColor(pic2, pic2, CV_BGR2HSV);
	Mat pic2_half = pic2(Range(pic2.rows / 2, pic2.rows - 1), Range(0, pic2.cols - 1));
	/// 对hue通道使用30个bin,对saturatoin通道使用32个bin
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue的取值范围从0到256, saturation取值范围从0到180
	float h_ranges[] = { 0, 256 };
	float s_ranges[] = { 0, 180 };

	const float* ranges[] = { h_ranges, s_ranges };

	// 使用第0和第1通道
	int channels[] = { 0, 1 };

	/// 直方图
	MatND hist_pic1;
	MatND hist_half_pic1;
	MatND hist_pic2;
	MatND hist_half_pic2;

	/// 计算HSV图像的直方图
	calcHist(&pic1, 1, channels, Mat(), hist_pic1, 2, histSize, ranges, true, false);
	normalize(hist_pic1, hist_pic1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&pic1_half, 1, channels, Mat(), hist_half_pic1, 2, histSize, ranges, true, false);
	normalize(hist_half_pic1, hist_half_pic1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&pic2, 1, channels, Mat(), hist_pic2, 2, histSize, ranges, true, false);
	normalize(hist_pic2 ,hist_pic2, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&pic2_half, 1, channels, Mat(), hist_half_pic2, 2, histSize, ranges, true, false);
	normalize(hist_half_pic2, hist_half_pic2, 0, 1, NORM_MINMAX, -1, Mat());

	for (int i = 0; i < 4; i++)
	{
		int compare_method = i;
		double pic1_pic1 = compareHist(hist_pic1, hist_pic1, compare_method);
		double pic1_half_pic1 = compareHist(hist_pic1, hist_half_pic1, compare_method);
		double pic1_pic2 = compareHist(hist_pic1, hist_pic2, compare_method);
		double pic1_half_pic2 = compareHist(hist_pic1, hist_half_pic2, compare_method);


		printf(" Method [%d] Perfect, Base-Half, Base-Test(1), Base-Test(2) : %f, %f, %f, %f \n", i, pic1_pic1, pic1_half_pic1, pic1_pic2, pic1_half_pic2);
	}
	namedWindow("pic1", 0);
	namedWindow("hlf_pic1", 0);
	namedWindow("pic2", 0);
	namedWindow("half_pic2", 0);
	imshow("pic1", hist_pic1);
	imshow("hlf_pic1", hist_half_pic1);
	imshow("pic2", hist_pic2);
	imshow("half_pic2", hist_half_pic2);
}


void Hist_and_Backproj(int, void *) {
	MatND hist;
	int histSize = MAX(bins, 2);
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };

	/// 计算直方图并归一化
	calcHist(&Hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	/// 计算反向投影
	MatND backproj;
	calcBackProject(&Hue, 1, 0, hist, backproj, &ranges, 1, true);

	/// 显示反向投影
	imshow("BackProj", backproj);

	/// 显示直方图
	int w = 400; int h = 400;
	int bin_w = cvRound((double)w / histSize);
	Mat histImg = Mat::zeros(w, h, CV_8UC3);

	for (int i = 0; i < bins; i++)
	{
		rectangle(histImg, Point(i*bin_w, h), Point((i + 1)*bin_w, h - cvRound(hist.at<float>(i)*h / 255.0)), Scalar(0, 0, 255), -1);
	}

	imshow("Histogram", histImg);
}
void Histogram::Histogram_BackProject() {
	
	//转换到HSV
	cvtColor(Histogram_Src, HSV, CV_BGR2HSV);
	Hue.create(HSV.size(), HSV.depth());
	int ch[] = { 0, 0 };
	mixChannels(&HSV, 1, &Hue, 1, ch, 1);

	/// 创建 Trackbar 来输入bin的数目
	namedWindow("window_image", CV_WINDOW_AUTOSIZE);
	createTrackbar("* Hue  bins: ", "window_image", &bins, 180, Hist_and_Backproj);
	Hist_and_Backproj(0, 0);
}


Histogram::~Histogram()
{
}
