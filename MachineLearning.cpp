#include "stdafx.h"
#include "MachineLearning.h"


MachineLearning::MachineLearning()
{
}

void MachineLearning::KNN_test()
{
	int K = 3;
	//knn算法 属性初始化
	Ptr<KNearest> knn = KNearest::create();
	knn->setAlgorithmType(knn->BRUTE_FORCE);

	//训练矩阵
	int rows;	//训练的图片数量
	int cols;	//单张图片的像素，tmp.cols*tmp.rows
	Mat tmp = imread("单字\\第11张：0.jpg", CV_32FC1);
	rows = tmp.rows;
	cols = tmp.cols * tmp.rows;
	Mat train_data(rows, cols, CV_32FC1);
	Mat train_labels(rows, 1, CV_32FC1);
	
	for (int i = 0; i < 5; i++) {
		Mat input = imread("模板\\0\\" + to_string(i) + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
		input.reshape(0, 1);
		tmp = train_data.rowRange(i, i + 1);
		input.copyTo(tmp);
	}
	knn->train(train_data, ROW_SAMPLE, train_labels);

	Mat predict_data(rows, cols, CV_32FC1);
	Mat predict_labels(5, 1, CV_32FC1);
	for (int i = 0; i < 5; i++)
	{
		Mat input = imread("单字\\第11张：" + to_string(i) + ".jpg", CV_32FC1);
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
	Mat targetData, backData;	//正样本/负样本

	Mat muban = imread("模板\\0\\0.jpg", 0);
	//正样本
	for (int i = 0; i < 5; i++) {
		Mat tmp = imread("模板\\0\\" + to_string(i) + ".jpg", CV_32FC1);
		normalize(muban, tmp);
		tmp.reshape(0, 1);
		Mat _tmp = targetData.rowRange(i, i + 1);
		tmp.copyTo(_tmp);
		
		targetData.push_back(_tmp);
	}

	//负样本
	for (int i = 1; i < 10; i++) {
		Mat tmp = imread("模板\\" + to_string(i) + "\\0.jpg", CV_32FC1);
		normalize(muban, tmp);
		backData.push_back(tmp);
	}
	vconcat(targetData, backData, train_data); //合并所有的样本点，作为训练数据
	train_label = Mat(14, 1, CV_32S, Scalar::all(1)); //初始化标注
	for (int i = 5; i < 14; i++) {
		train_label.at<int>(i, 0) = -1;
	}


	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	
	// 训练分类器
	Ptr<TrainData> traindata = ml::TrainData::create(train_data, ROW_SAMPLE, train_label);
	svm->trainAuto(traindata, 10);
	svm->save("svm.xml");

	Mat sampleMat = imread("单字\\第11张：0.jpg", CV_32FC1);
	float response = svm->predict(sampleMat);  //进行预测，返回1或-1,返回类型为float
	Mat sampleMat2 = imread("单字\\第11张：5.jpg", CV_32FC1);
	float response2 = svm->predict(sampleMat);  //进行预测，返回1或-1,返回类型为float
}


MachineLearning::~MachineLearning()
{
}
