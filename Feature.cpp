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
//               ���汣��         ����BUG
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
	//Mat src22 = imread("����\\��12�ţ�0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
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
	Mat src_1 = imread("piaohao.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat src_2 = imread("match2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	vector<KeyPoint> keypoints_1, keypoints_2;
	Mat descriptors_1, descriptors_2;
	vector<DMatch>match;
	//vector<vector<DMatch>> match;
	vector<DMatch>GoodMatchePoints;
	Mat result;
	
	Ptr<AKAZE> detector = AKAZE::create();
	detector->detect(src_1, keypoints_1);
	detector->detect(src_2, keypoints_2);
	detector->compute(src_1, keypoints_1, descriptors_1);
	detector->compute(src_2, keypoints_2, descriptors_2);
	
	Ptr<BFMatcher> bfmatcher = BFMatcher::create();
	bfmatcher->match(descriptors_1, descriptors_2, match);
	//bfmatcher->knnMatch(descriptors_1, descriptors_2, match,3);
	

	//��ȡ����ƥ���
	sort(match.begin(), match.end());  //ɸѡƥ���  
	//int ptsPairs = std::min(50, (int)(match.size() * 0.15));
	//cout << ptsPairs << endl;
	for (int i = 0; i < 480; i++)
	{
		GoodMatchePoints.push_back(match[i]);
	}
	//ratio = 0. 4������׼ȷ��Ҫ��ߵ�ƥ�䣻
	//ratio = 0. 6������ƥ�����ĿҪ��Ƚ϶��ƥ�䣻
	//ratio = 0. 5��һ������¡�
	/*for (int i = 0; i < match.size(); i++)
	{
		if (match[i][0].distance < 0.5 * match[i][1].distance)
		{
			GoodMatchePoints.push_back(match[i][0]);
		}
	}
*/
	drawMatches(src_1, keypoints_1, src_2, keypoints_2, GoodMatchePoints, result);
	imshow("ƥ����ͼ", result);
}


Feature::~Feature()
{
}
