//
//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \
//                / _||||| -:- |||||- \
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//               佛祖保佑         永无BUG
//
//
//

#include "stdafx.h"
#include "Feature.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"

Feature::Feature()
{
}

void Feature::FindFeature() {
	//Mat src22 = imread("单字\\第11张：0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat src22 = imread("match.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("src", 0);
	imshow("src", src22);
	Mat out_src;

	vector<KeyPoint> keypoints;
	//AGAST(src22,keypoints,50);
	//FAST(src22, keypoints, 100);
	Ptr<AgastFeatureDetector> testdetector = AgastFeatureDetector::create();
	testdetector->detect(src22, keypoints);
	
	drawKeypoints(src22, keypoints, out_src);
	namedWindow("look", 0);
	imshow("look", out_src);
}

void Feature::MatchFeature() {
	Mat src_1 = imread("单字\\第11张：0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat src_2 = imread("模板\\6\\0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	vector<KeyPoint> keypoints_1, keypoints_2;
	Mat descriptors_1, descriptors_2;
	vector<DMatch>match;
	//vector<vector<DMatch>> match;
	vector<DMatch>GoodMatchePoints;
	Mat result;
	
	Ptr<FeatureDetector> detector = Ptr<FeatureDetector>();
	detector = KAZE::create();
	detector->detect(src_1, keypoints_1);
	detector->detect(src_2, keypoints_2);
	//Ptr<BRISK>detector2 = BRISK::create();
	detector->compute(src_1, keypoints_1, descriptors_1);
	detector->compute(src_2, keypoints_2, descriptors_2);
	//
	Ptr<BFMatcher> bfmatcher = BFMatcher::create();
	bfmatcher->match(descriptors_1, descriptors_2, match);
	//bfmatcher->knnMatch(descriptors_1, descriptors_2, match,3);
	

	//获取优秀匹配点
	sort(match.begin(), match.end());  //筛选匹配点  
	for (int i = 0; i < match.size(); i++)
	{
		GoodMatchePoints.push_back(match[i]);
		drawMatches(src_1, keypoints_1, src_2, keypoints_2, GoodMatchePoints, result);
	}
	float k = 0;
	for (int num = 0; num < GoodMatchePoints.size(); num++) {
		if (GoodMatchePoints[num].distance < 100)
		{
			k += 1;
		}
	}
	cout << k / GoodMatchePoints.size();


	//ratio = 0. 4：对于准确度要求高的匹配；
	//ratio = 0. 6：对于匹配点数目要求比较多的匹配；
	//ratio = 0. 5：一般情况下。
	/*for (int i = 0; i < match.size(); i++)
	{
		if (match[i][0].distance < 0.5 * match[i][1].distance)
		{
			GoodMatchePoints.push_back(match[i][0]);
		}
	}
*/
	drawMatches(src_1, keypoints_1, src_2, keypoints_2, GoodMatchePoints, result);
	namedWindow("匹配结果图", 0);
	imshow("匹配结果图", result);
}


Feature::~Feature()
{
}
