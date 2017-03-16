

#include "ImageContrast.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

using namespace std;

int main()
{
	clock_t tim_str, tim_end;
	tim_str = clock();

	cv::Mat m0, m1;
	
	m0 = cv::imread("D:\\csimg\\毁伤检测0809\\ap0.tif");
	m1 = cv::imread("D:\\csimg\\毁伤检测0809\\ap1.tif");

	//m0 = cv::imread("D:\\csimg\\毁伤检测0809\\ft0.tif");
	//m1 = cv::imread("D:\\csimg\\毁伤检测0809\\ft1.tif");

	//m0 = cv::imread("D:\\csimg\\毁伤检测0809\\hs0.bmp");
	//m1 = cv::imread("D:\\csimg\\毁伤检测0809\\hs2.bmp");
	
	cv::medianBlur(m0, m0, 3);
	cv::medianBlur(m1, m1, 3);
	ImgCon ic(&m0, &m1);
		
	//haar or lbp
	ic.integral_img();
	ic.compute_h_w_bin(1);		

	int h = m0.size().height;
	int w = m0.size().width;
	//得分图
	int **scmp;
	int **scmp1;
	int **scmp2;
	int **scmp3;
	int **scmp4;
	int **scmp_;	//平均
	scmp = new int *[h];		//new完没有写释放代码，暂时不影响
	for (int i = 0; i < h; i++)
	{
		scmp[i] = new int[w];
	}
	scmp1 = new int *[h];
	for (int i = 0; i < h; i++)
	{
		scmp1[i] = new int[w];
	}
	scmp2 = new int *[h];
	for (int i = 0; i < h; i++)
	{
		scmp2[i] = new int[w];
	}
	scmp3 = new int *[h];
	for (int i = 0; i < h; i++)
	{
		scmp3[i] = new int[w];
	}
	scmp4 = new int *[h];
	for (int i = 0; i < h; i++)
	{
		scmp4[i] = new int[w];
	}
	scmp_ = new int *[h];
	for (int i = 0; i < h; i++)
	{
		scmp_[i] = new int[w];
	}
	cv::Mat res =  ic.compare(6, scmp,  true, 9);
	cv::Mat res1 = ic.compare(6, scmp1, true, 10);
	cv::Mat res2 = ic.compare(6, scmp2, true, 11);
	cv::Mat res3 = ic.compare(6, scmp3, true, 12);
	//cv::Mat res4 = ic.compare(6, scmp3, true, 1);
	//cv::Mat res5 = ic.compare(6, scmp3, true, 2);
	//cv::Mat res6 = ic.compare(6, scmp3, true, 3);
	//cv::Mat res7 = ic.compare(6, scmp3, true, 4);
	//cv::Mat res8 = ic.compare(6, scmp3, true, 5);
	//cv::Mat res9 = ic.compare(6, scmp3, true, 6);
	//cv::Mat res10 = ic.compare(6, scmp3, true, 7);
	//cv::Mat res11 = ic.compare(6, scmp3, true, 8);
	//cv::Mat res12 = ic.compare(6, scmp3, true, 13);
	//cv::Mat res13 = ic.compare(6, scmp3, true, 14);
	cv::Mat res4 = ic.compare(6, scmp4, true, 0);
	
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			scmp_[i][j] = (scmp[i][j] + scmp1[i][j] + scmp2[i][j] + scmp3[i][j] + scmp4[i][j]) / 5;
		}
	}//平均得分图即毁伤评估
	
	cv::Mat fa;
	fa = ic.feature_and(&res, &res1);
	fa = ic.feature_and(&fa, &res2);
	fa = ic.feature_and(&fa, &res3);
	fa = ic.feature_and(&fa, &res4);
	//fa = ic.feature_and(&fa, &res5);
	//fa = ic.feature_and(&fa, &res6);
	//fa = ic.feature_and(&fa, &res7);
	//fa = ic.feature_and(&fa, &res8);
	//fa = ic.feature_and(&fa, &res9);
	//fa = ic.feature_and(&fa, &res10);
	//fa = ic.feature_and(&fa, &res11);
	//fa = ic.feature_and(&fa, &res12);
	//fa = ic.feature_and(&fa, &res13);
	cv::medianBlur(fa, fa, 3);
	cv::imshow("zuichu", fa);

	//腐蚀
	cv::Mat mk = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));///////
	cv::erode(fa, fa, mk);
	cv::imshow("fushi", fa);

	//去掉面积小的连通区域
	std::vector<std::vector<CvPoint>> rs;
	ic.findconnectedregions(&fa, rs);
	std::vector<std::vector<CvPoint>> rs2;
	for (int i = 0; i < rs.size(); i++)
	{
		if (rs[i].size() > 10)//////
			rs2.push_back(rs[i]);
	}
	fa = ic.vecs2binimg(rs2);
	cout << rs.size() << " " << rs2.size() << endl;
	cv::imshow("quchuxiaoquyu", fa);

	//膨胀
	cv::Mat mj = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15,15));///////
	cv::dilate(fa, fa, mj);
	cv::imshow("pengzhang", fa);

	//去边缘黑边的影响
	std::vector<std::vector<CvPoint>> rs3;
	ic.findconnectedregions(&fa, rs3);
	int fah = fa.size().height, faw = fa.size().width;
	for (std::vector<std::vector<CvPoint>>::iterator itt = rs3.begin(); itt != rs3.end();)
	{
	mark158:
		if (itt == rs3.end())
			break;
		for (std::vector<CvPoint>::iterator itp = itt->begin(); itp != itt->end();)
		{
			if (itp->x <= 10 ||
				itp->y <= 10 ||
				itp->x >= (faw - 10) ||
				itp->y >= (fah - 10))
			{
				itt = rs3.erase(itt);
				goto mark158;
			}
			itp++;
		}
		itt++;
	}
	fa = ic.vecs2binimg(rs3);

	ic.findoutlines(&fa, m1, 'R', 1);
	//ic.findoutlines(&fa, m0, 'R', 1);
	//cv::imshow("jieguo0", m0);
	//cv::imshow("jieguo1", m1);

	//求rect和区域得分
	std::vector<std::pair<cv::Rect, int>> rectandsc;
	ic.compute_sc(rs3, scmp_, rectandsc);
	for (int i = 0; i < rectandsc.size(); i++)
	{
		//cv::rectangle(m1, rectandsc[i].first, cv::Scalar(0, 0, 255), 1);
		cv::putText(m1, to_string(rectandsc[i].second + 20),
			cv::Point(rectandsc[i].first.x, rectandsc[i].first.y - 4), 
			2, 0.7, cv::Scalar(0, 0, 255));
	}
	cv::imshow("jieguo", m1);

	tim_end = clock();
	cout << "运行时间: " << (double)(tim_end - tim_str) / CLOCKS_PER_SEC << "S" << endl;
	cv::waitKey();
	
	//system("pause");
	return 0;
}
cv::Mat suoxiao(cv::Mat *m)
{
	cv::Mat temp;
	cv::Size s(m->size().width / 2, m->size().height / 2);
	cv::resize(*m, temp, s);
	return temp;
}
