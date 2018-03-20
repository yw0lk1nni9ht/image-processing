#include "stdafx.h"
#include "Histogram.h"


/*************************************
//									//
//									//
//			ֱ��ͼ���				//
//									//
//									//
*************************************/

//����ȫ�ֱ���
Mat Histogram_Src;

//����ͶӰ
Mat HSV;
Mat Hue;
int bins;

Histogram::Histogram(Mat src)
{
	Histogram_Src = src;
}


//ֱ��ͼ���⻯
void Histogram::Histogram_EqualizeHist() {
	Mat Histogram_Dst;
	Mat dst;
	cvtColor(Histogram_Src, Histogram_Dst, CV_BGR2GRAY);
	equalizeHist(Histogram_Dst, dst);
	namedWindow("equalizeHist", 0);
	imshow("equalizeHist", dst);
	imshow("equalizeHist2", Histogram_Src);
}


//ֱ��ͼ����
void Histogram::Histogram_Calc() {
	/// �ָ��3����ͨ��ͼ�� ( R, G �� B )
	vector<Mat> rgb_planes;
	split(Histogram_Src, rgb_planes);

	// �趨bin��Ŀ
	int histSize = 255;

	/// �趨ȡֵ��Χ ( R,G,B) )
	float range[] = { 0, 255 };
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;

	Mat r_hist, g_hist, b_hist;
	/// ����ֱ��ͼ:
	calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);

	// ����ֱ��ͼ����
	int hist_w = 400; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

	/// ��ֱ��ͼ��һ������Χ [ 0, histImage.rows ]
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// ��ֱ��ͼ�����ϻ���ֱ��ͼ
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

	/// ��ʾֱ��ͼ
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);
}


//ֱ��ͼ�Ա�
void Histogram::Histogram_Compare(Mat pic1 , Mat pic2) {
	//ת��ΪHSV
	cvtColor(pic1, pic1, CV_BGR2HSV);
	Mat pic1_half = pic1(Range(pic1.rows / 2, pic1.rows - 1), Range(0, pic1.cols - 1));
	cvtColor(pic2, pic2, CV_BGR2HSV);
	Mat pic2_half = pic2(Range(pic2.rows / 2, pic2.rows - 1), Range(0, pic2.cols - 1));
	/// ��hueͨ��ʹ��30��bin,��saturatoinͨ��ʹ��32��bin
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue��ȡֵ��Χ��0��256, saturationȡֵ��Χ��0��180
	float h_ranges[] = { 0, 256 };
	float s_ranges[] = { 0, 180 };

	const float* ranges[] = { h_ranges, s_ranges };

	// ʹ�õ�0�͵�1ͨ��
	int channels[] = { 0, 1 };

	/// ֱ��ͼ
	MatND hist_pic1;
	MatND hist_half_pic1;
	MatND hist_pic2;
	MatND hist_half_pic2;

	/// ����HSVͼ���ֱ��ͼ
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

	/// ����ֱ��ͼ����һ��
	calcHist(&Hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	/// ���㷴��ͶӰ
	MatND backproj;
	calcBackProject(&Hue, 1, 0, hist, backproj, &ranges, 1, true);

	/// ��ʾ����ͶӰ
	imshow("BackProj", backproj);

	/// ��ʾֱ��ͼ
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
	
	//ת����HSV
	cvtColor(Histogram_Src, HSV, CV_BGR2HSV);
	Hue.create(HSV.size(), HSV.depth());
	int ch[] = { 0, 0 };
	mixChannels(&HSV, 1, &Hue, 1, ch, 1);

	/// ���� Trackbar ������bin����Ŀ
	namedWindow("window_image", CV_WINDOW_AUTOSIZE);
	createTrackbar("* Hue  bins: ", "window_image", &bins, 180, Hist_and_Backproj);
	Hist_and_Backproj(0, 0);
}


Histogram::~Histogram()
{
}
