#include "stdafx.h"
#include "���ɲ���.h"

//Mat test_src2;


//��ȡƱ�ŵ������
���ɲ���::���ɲ���(Mat _src, int type)
{
	/*��һ�ַ���*/
	//Mat dst;
	//Mat dst2;
	///*�Ҷ�*/
	//Mat src = imread("piaohao.jpg", 0);
	////imshow("ͼ1", src);
	//threshold(src, dst, 100, 255, THRESH_BINARY);
	//imshow("1.ͼ", dst);
	//medianBlur(dst, dst2, 5);
	////GaussianBlur(dst, dst, Size(43, 43), 0, 0);
	//imshow("1.2ͼ", dst2);


	/*�ڶ��ַ���*/
	//test_src2 = imread("piaohao2.jpg", 1);
	Mat test_src2 = _src;
	Mat test_dst;

	//�㷨		
	//1.�����ȡ����
	GaussianBlur(test_src2, test_dst, Size(9, 9), 0, 0);
	Mat test_dst2, test_dst3;
	cvtColor(test_dst, test_dst2, 7);
	Laplacian(test_dst2, test_dst3, 2, 3);
	convertScaleAbs(test_dst3, test_dst);
	//imshow("2.1��˹ͼ", test_dst);

	//Mat test_dst2, test_dst3;
	//test_dst2 = test_src2;
	//cvtColor(test_dst2, test_dst, 7);

	//��ֵ���ڰ�
	//2.��ֵ�ڰ�ͼ��
	threshold(test_dst, test_src2, 10, 255, THRESH_BINARY);
	//threshold(test_dst, test_src2, 10, 255, THRESH_BINARY_INV);
	//imshow("2.2��ֵͼ", test_src2);

	////����
	//blur(test_src2, test_dst, Size(49, 49), Point(-1, -1));
	//GaussianBlur(src, dst, Size(3, 3), 0, 0);
	//imshow("2.3����ͼ", test_dst);

	//����ֻ�������
	//3.͹����������
	/************************�㶫ͨ�û���Ʊ**********************************/
	if (type == 0)
	{
		Mat kernel = getStructuringElement(0, Size(10, 10));		//1000������
		dilate(test_src2, test_src2, kernel);
	}
	//���ְ׵�
	else if (type == 2 || type ==3)
	{
		Mat kernel = getStructuringElement(0, Size(50, 50));		//other_invoke
		dilate(test_src2, test_src2, kernel);
	}
	//imshow("2.4����", test_src2);

	//��������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int thresh = 10;
	/// ��Canny���Ӽ���Ե
	Canny(test_src2, test_dst, thresh, thresh * 2, 3);
	//imshow("Canny����", test_dst);
	/// Ѱ������
	findContours(test_dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	int Max  = 0;
	//Rect Need;
	for (int i= 0 ; i < contours.size(); i++) {
		Rect a = boundingRect(contours[i]);
		if (type == 3)
		{
			if (a.width > 500)
				continue;
			if (a.y < test_dst.rows*0.25 || a.y>test_dst.rows*0.75)
				continue;
		}
		/********************�ڶ���Ʊ�ݵ�ɸѡ*****************************/
		if (type == 0 || type == 2)
		{
			if (a.width > 450)
			{
				continue;
			}
		}
		/***************************************************************/
		if ((a.width * a.height) > Max)
		{
			Max = a.width * a.height;
			this->Need = a;
		}
	}
	rectangle(test_dst, this->Need, Scalar(148, 0, 211));
	//imshow("drawing", test_dst);
}

//�Բü���ͼƬ��һ�д�����ȡ�����ֿ�
void ���ɲ���::pichandle(Mat handle_src) {
	Mat save = handle_src.clone();

	

#pragma region 1.��ʼ������

	cvtColor(handle_src, handle_src, 7);	//ת�ɻҶ�
	threshold(handle_src, handle_src, 130, 255, THRESH_BINARY);		//��ֵ��
	//imshow("1��ֵ", handle_src);

	#pragma endregion


#pragma region 2.ȥ�������� �����͸�ʴʹƱ�Ÿ�����

	Mat kernel = getStructuringElement(0, Size(3, 3));	//����
	dilate(handle_src, handle_src, kernel);
	//imshow("2����", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(4, 4));	//��ʴ
	erode(handle_src, handle_src, kernel2);
	//imshow("2��ʴ", handle_src);
	Mat kernel3 = getStructuringElement(0, Size(5, 5));	//����
	dilate(handle_src, handle_src, kernel3);
	//imshow("3����", handle_src);
	Mat kernel4 = getStructuringElement(0, Size(3, 3));	//��ʴ
	erode(handle_src, handle_src, kernel4);
	////imshow("3��ʴ", handle_src);

	#pragma endregion


#pragma region 3.����Ե

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	 //��Canny���Ӽ���Ե
	Canny(handle_src, canny_output, 10, 10 * 2, 3);
	//imshow("Canny����", canny_output);

	#pragma endregion

	 
#pragma region 4.Ѱ������
	 
	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	#pragma	endregion


#pragma region 5.ɸѡ����

	for (int i = 0; i < contours.size() ; i++) {
		Rect a = boundingRect(contours[i]);
		if (a.area() > 80)
		{
			rectangle(save, a, Scalar(255, 64, 64));
			saved_contours.push_back(a);
		}
	}

	//for (int i = 0; i < contours.size() - 1; i++) {
	//	Rect a = boundingRect(contours[i]);
	//	if ((a.width * a.height) > 1 && (a.width * a.height)<1800)
	//	{
	//		a.x = a.x - 5;
	//		a.y = a.y - 5;
	//		a.width = a.width + 10;
	//		a.height = a.height + 10;
	//		rectangle(save, a, Scalar(255, 64, 64));
	//		saved_contours.push_back(a);
	//	}
	//}
	//imshow("Contours", save);
	/// �������
	#pragma endregion 




}

//�������˲�����
void ���ɲ���::clean() {

	//����x���꣬����
	int _contours = saved_contours.size();
	for (int i = 0; i < _contours - 1 ; i++)
	{
		int saveindex = i;
		int min = saved_contours[i].x;
		for (int j = i + 1; j < _contours; j++)
		{
			if (min > saved_contours[j].x)
			{
				min = saved_contours[j].x;
				saveindex = j;
			}
		}
		if (saveindex != i) {
			Rect temp = saved_contours[i];
			saved_contours[i] = saved_contours[saveindex];
			saved_contours[saveindex] = temp;
		}
	}


	//���ͬһ�����¶�������Ĳ���ȷ������ֻ���������Ͻǵ�
	vector<Rect>::iterator element_to_delete = saved_contours.begin(); 
	for (int i = 0; i < _contours - 1; i++) {
		//��һ��������x�������������x��Χ�֤����������Ҫ����һ�����������Ծ�ɾ��
		//������°���������yֵ���ó��ϰ�������yֵ
		if ((saved_contours[i + 1].x <= saved_contours[i].x + saved_contours[i].width)
			&& (saved_contours[i + 1].x >= saved_contours[i].x))
		{
			if (saved_contours[i].y > saved_contours[i + 1].y)
			{
				saved_contours[i].y = saved_contours[i + 1].y;
			}
			element_to_delete + 1 = saved_contours.erase(element_to_delete + 1);
			_contours--;
			i--;
			continue;
		}
		element_to_delete++;
	}

	//��ȡ�������Ͻ����꣬�ػ�������С
	for (int i = 0; i < _contours; i++) {
		saved_contours[i].x -= 5;
		saved_contours[i].y -= 5;
		saved_contours[i].width = 32;
		saved_contours[i].height = 50;


	}

	//Ʊ��8��
	if (saved_contours.size() == 8) {

	}
	else if (saved_contours.size() > 8) {
		while (saved_contours.size() > 8) {
			saved_contours.pop_back();
		}
	}

}



���ɲ���::~���ɲ���()
{
}
