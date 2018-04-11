#include "stdafx.h"
#include "MachineLearning.h"


MachineLearning::MachineLearning()
{
}

void MachineLearning::KNN_test()
{
	int K = 3;
	//knn�㷨 ���Գ�ʼ��
	Ptr<KNearest> knn = KNearest::create();
	knn->setAlgorithmType(knn->BRUTE_FORCE);

	//ѵ������
	int rows;	//ѵ����ͼƬ����
	int cols;	//����ͼƬ�����أ�tmp.cols*tmp.rows
	Mat tmp = imread("����\\��11�ţ�0.jpg", CV_32FC1);
	rows = tmp.rows;
	cols = tmp.cols * tmp.rows;
	Mat train_data(rows, cols, CV_32FC1);
	Mat train_labels(rows, 1, CV_32FC1);
	
	for (int i = 0; i < 5; i++) {
		Mat input = imread("ģ��\\0\\" + to_string(i) + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
		input.reshape(0, 1);
		tmp = train_data.rowRange(i, i + 1);
		input.copyTo(tmp);
	}
	knn->train(train_data, ROW_SAMPLE, train_labels);

	Mat predict_data(rows, cols, CV_32FC1);
	Mat predict_labels(5, 1, CV_32FC1);
	for (int i = 0; i < 5; i++)
	{
		Mat input = imread("����\\��11�ţ�" + to_string(i) + ".jpg", CV_32FC1);
		input.reshape(0, 1);
		tmp = predict_data.rowRange(i, i + 1);
		input.copyTo(tmp);
	}
	
	Mat result;
	knn->findNearest(predict_data, K, result);

	int count = 0;
	for (int i = 0; i < 5; ++i) {
		float value1 = ((float*)predict_labels.data)[i];
		float value2 = ((float*)result.data)[i];
		fprintf(stdout, "expected value: %f, actual value: %f\n", value1, value2);

		if (int(value1) == int(value2)) ++count;
	}
	fprintf(stdout, "when K = %d, accuracy: %f\n", K, count * 1.f / 5);

}


void MachineLearning::SVM_test() {

	Mat train_data, train_label;
	Mat targetData,backData;	//������/������

	Mat muban = imread("ģ��\\0\\0.jpg", 0);
	int _rows = muban.rows;	//��׼��
	int _cols = muban.cols;	//��׼��
	int plus_num = 30;	//����������
	int minus_num = 36;	//����������
	/*Mat targetData(plus_num, _rows*_cols, CV_32FC1);
	Mat backData(minus_num, _rows*_cols, CV_32FC1);*/

	//������
	for (int i = 0; i < plus_num; i++) {
		Mat tmp = imread("ģ��\\0\\" + to_string(i) + ".jpg", 0);
		
		tmp.rows = _rows;
		tmp.cols = _cols;
		
		//tmp.reshape(1, 1);
		/*Mat _tmp = targetData.rowRange(i, i + 1);
		_tmp = tmp.reshape(1, 1);*/
		//tmp.copyTo(_tmp);
		Mat _tmp;
		tmp.convertTo(_tmp, CV_32FC1);
		_tmp = _tmp.reshape(1, 1);
		targetData.push_back(_tmp);
		train_label.push_back(0);
	}

	//������
	for (int i = 1; i < 10; i++) {
		for (int j = 0; j < 4; j++) {
			Mat tmp = imread("ģ��\\" + to_string(i) + "\\" + to_string(j) +".jpg", 0);
			tmp.rows = _rows;
			tmp.cols = _cols;
			/*tmp.reshape(1, 1);
			Mat _tmp = backData.rowRange(i - 1, i);
			tmp.copyTo(_tmp);*/

			Mat _tmp;
			tmp.convertTo(_tmp, CV_32FC1); 
			_tmp = _tmp.reshape(1, 1);
			backData.push_back(_tmp);
			train_label.push_back(i);
		}
	}
	vconcat(targetData, backData, train_data); //�ϲ����е������㣬��Ϊѵ������
	//train_label = Mat(plus_num + minus_num, 1, CV_32S, Scalar::all(1)); //��ʼ����ע
	//train_label = Mat(plus_num + minus_num, 1, CV_32F, Scalar::all(1)); //��ʼ����ע
	/*for (int i = plus_num; i < plus_num + minus_num; i++) {
		train_label.at<int>(i, 0) = 0;
	}*/


	Ptr<SVM> svm = SVM::create();
	svm->setType(100);	//100-104
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	
	// ѵ��������
	Ptr<TrainData> traindata = TrainData::create(train_data, ROW_SAMPLE, train_label);
	//svm->train(traindata);
	svm->trainAuto(traindata, 10);
	svm->save("svm.xml");

	
	Mat sampleMat = imread("����\\��11�ţ�0.jpg", 0);
	sampleMat.rows = _rows;
	sampleMat.cols = _cols;
	Mat test;
	sampleMat.convertTo(test, CV_32FC1);
	test = test.reshape(1, 1);
	float response = svm->predict(test);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat2 = imread("����\\��17�ţ�5.jpg", CV_32FC1);
	sampleMat2.rows = _rows;
	sampleMat2.cols = _cols;
	sampleMat.convertTo(test, CV_32FC1);
	test = test.reshape(1, 1);
	float response2 = svm->predict(test);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat3 = imread("����\\��12�ţ�5.jpg", CV_32FC1);
	sampleMat3.rows = _rows;
	sampleMat3.cols = _cols;
	sampleMat3 = sampleMat.reshape(1, 1);
	test = Mat(sampleMat3.rows, sampleMat3.cols, CV_32F);
	sampleMat3.convertTo(test, CV_32F);
	float response3 = svm->predict(test);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat4 = imread("����\\��13�ţ�5.jpg", CV_32FC1);
	sampleMat4.rows = _rows;
	sampleMat4.cols = _cols;
	sampleMat4 = sampleMat.reshape(1, 1);
	test = Mat(sampleMat4.rows, sampleMat2.cols, CV_32F);
	sampleMat4.convertTo(test, CV_32F);
	float response4 = svm->predict(test);  //����Ԥ�⣬����1��-1,��������Ϊfloat
}


MachineLearning::~MachineLearning()
{
}
