

#include "ImageContrast.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <F:\\azcaffevs13\\Process_preFrame\\myfile\\zqjtools.hpp>

using namespace std;

int main4896()
{
	vector<int> vec;
	vector<pair<int, int>> vec_pr;
	pair<int, int> pr;
	pr.first = 9;
	pr.second = 1;
	vec_pr.push_back(pr);
	pr.first = 12;
	pr.second = 2;
	vec_pr.push_back(pr);
	pr.first = 6;
	pr.second = 3;
	vec_pr.push_back(pr);
	pr.first = 4;
	pr.second = 4;
	vec_pr.push_back(pr);

	//cout << "size:" << vec.size() << endl << "neirong:" << endl;
	//std::sort(vec.begin(), vec.end());

	sort(vec_pr.begin(), vec_pr.end());

	for (int i = 0; i < vec_pr.size(); i++)
	{
		cout << vec_pr[i].first<<" "<<vec_pr[i].second << endl;
	}
	system("pause");

	return 0;
}

int main()
{
	clock_t tim_str, tim_end;
	tim_str = clock();
	//测试积分图
	//cv::Mat img = cv::imread("d:/hs1.bmp");
	//ImgCon icc;
	//icc.integral_img();
	//int h = img.size().height;
	//int w = img.size().width;
	//int max = icc.intgrlimg[h - 1][w - 1];
	//float rd = 255.0 / (float)max;
	//cv::Mat res(h, w, CV_8U, cv::Scalar(0));
	//for (int i = 0; i < h; i++)
	//{
	//	for (int j = 0; j < w; j++)
	//	{
	//		res.at<uchar>(i, j) = (int)((float)icc.intgrlimg[i][j] * rd);
	//	}
	//}
	//cv::imshow("1234w", res);
	//cv::waitKey();
	cv::Mat m0, m1;
	
	m0 = cv::imread("D:\\毁伤检测0809\\img/leina0.jpg");
	m1 = cv::imread("D:\\毁伤检测0809\\img/leina1.jpg");

	cv::medianBlur(m0, m0, 3);
	cv::medianBlur(m1, m1, 3);
	ImgCon ic(&m0, &m1);

	//ic.integral_img();
	//ic.compute_h_w_bin();
	//cv::Mat res = ic.compare(6);

	cv::Mat fres, fres1, fres2, fres3;

	/* HAAR */
	ic.integral_img();
	ic.compute_h_w_bin(1);
	cv::Mat res =  ic.compare(6, true, 9);
	cv::Mat res1 = ic.compare(6, true, 10);
	cv::Mat res2 = ic.compare(6, true, 11);
	cv::Mat res3 = ic.compare(6, true, 12);
	cv::Mat res4 = ic.compare(6, true, 0);
	//for (int i = 0; i < 25; i++)
	//{
	//	for (int j = 0; j < 25; j++)
	//	{
	//		cout << ic.damagelevel[i][j] << " ";
	//	}
	//	cout << endl;
	//}

	///* LBP */
	//ic.compute_h_w_bin();
	//cv::Mat res = ic.compare(6, false, 1);
	//cv::Mat res1 = ic.compare(6, false, 2);
	//cv::Mat res2 = ic.compare(6, false, 3);
	//cv::Mat res3 = ic.compare(6, false, 4);

	//cv::medianBlur(res, res,   5);
	//cv::medianBlur(res1, res1, 5);
	//cv::medianBlur(res2, res2, 5);
	//cv::medianBlur(res3, res3, 5);

	//cv::Mat fa = ic.feature_and(&res, &res1);
	//fa = ic.feature_and(&fa, &res2);
	//fa = ic.feature_and(&fa, &res3);

	//ic.findconnectedregions(&res);
	//ic.findmainregion(1);
	//fres = ic.vecs2binimg(ic.vecs);
	//ic.findconnectedregions(&res1);
	//ic.findmainregion(1);
	//fres1 = ic.vecs2binimg(ic.vecs);
	//ic.findconnectedregions(&res2);
	//ic.findmainregion(1);
	//fres2 = ic.vecs2binimg(ic.vecs);
	//ic.findconnectedregions(&res3);
	//ic.findmainregion(1);
	//fres3 = ic.vecs2binimg(ic.vecs);

	cv::Mat fa;
	fa = ic.feature_and(&res, &res1);
	fa = ic.feature_and(&fa, &res2);
	fa = ic.feature_and(&fa, &res3);
	fa = ic.feature_and(&fa, &res4);
	cv::medianBlur(fa, fa, 3);

	cv::Mat mk = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));
	cv::imshow("fushi", fa);

	//腐蚀
	cv::erode(fa, fa, mk);
	cv::Mat mj = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10,10));
	//膨胀
	cv::dilate(fa, fa, mj);

	cv::imshow("pengzhang", fa);

	//ic.findconnectedregions(&fa);
	//ic.findmainregion(1);
	//fa = ic.vecs2binimg(ic.vecs);
	ic.findoutlines(&fa, m1, 'R', 1);
	cv::imshow("jieguo", m1);

	//vector<pair<vector<CvPoint>, int>> damagelist;
	//int d = 0;		//用于保存损坏程度的累加值
	//for (int i = 0; i < ic.vecs.size(); i++)
	//{
	//	for (int j = 0; j < ic.vecs[i].size(); j++)
	//	{
	//		d += ic.damagelevel[ic.vecs[i][j].y][ic.vecs[i][j].x];
	//	}
	//	d = (int)(d / ic.vecs[i].size());
	//	pair<vector<CvPoint>, int> damagepair(ic.vecs[i], d);		//pair<损坏区域, 损坏程度>
	//	damagelist.push_back(damagepair);
	//}
	//for (int i = 0; i < damagelist.size(); i++)
	//{
	//	cout << "区域" << i + 1 << ",面积:" << damagelist[i].first.size() << ",损坏程度:" << damagelist[i].second << endl;
	//}
	tim_end = clock();
	cout << "运行时间: " << (double)(tim_end - tim_str) / CLOCKS_PER_SEC << "S" << endl;
	az::showMemoryInfo();
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

int main13(int argc,char* argv[])
{
	////命令行原版
	////读取配置文件
	//if (argc != 4)
	//{
	//	std::cout << "命令行参数设置错误！" << std::endl;
	//	return -1;
	//}
	//std::ifstream filein;
	//filein.open(argv[1]);
	//std::string stemp;
	//std::vector<std::string> pzf;
	//while (!filein.eof())
	//{
	//	filein >> stemp;
	//	if (stemp[0] != '-'&&stemp[0] != '[')
	//		pzf.push_back(stemp);
	//}
	//filein.close();

	///*for (int i = 0; i < pzf.size(); i++)*/
	//{
	//	std::cout << "[conf.ini]" << std::endl;
	//	std::cout << "srcimg:" << pzf[0] << std::endl;
	//	std::cout << "denoiseingimg:" << pzf[1] << std::endl;
	//	std::cout << "resultimg:" << pzf[2] << std::endl;
	//	std::cout << "contrastparam:" << pzf[3] << std::endl;
	//	std::cout << "denoiseingparam:" << pzf[4] << std::endl;
	//	std::cout << "seconddenoise:" << pzf[5] << std::endl;
	//	std::cout << "seconddenoiseingparam:" << pzf[6] << std::endl;
	//	std::cout << "mainregion:" << pzf[7] << std::endl;
	//	std::cout << "mainregionmode:" << pzf[8] << std::endl;
	//	std::cout << "contourcolor:" << pzf[9] << std::endl;
	//	std::cout << "contourthickness:" << pzf[10] << std::endl;
	//	std::cout << "原始图像1:" << pzf[11] << std::endl;
	//	std::cout << "原始图像2:" << pzf[12] << std::endl;
	//}

	//cv::Mat m0 = cv::imread(argv[2]);
	//cv::Mat m1 = cv::imread(argv[3]);
	//if (pzf[11] == "true")
	//	cv::imshow("原始图像1", m0);
	//if (pzf[12] == "true")
	//	cv::imshow("原始图像2", m1);
	//ImgCon ic(&m0, &m1);
	//ic.compute_h_w_bin();
	//cv::Mat m = ic.compare(std::atoi(pzf[3].c_str()));
	//if (pzf[0]=="true")
	//	cv::imshow("特征图1", suoxiao(&m));
	//cv::medianBlur(m, m, std::atoi(pzf[4].c_str()));
	//if (pzf[5] == "true")	
	//	cv::medianBlur(m, m, std::atoi(pzf[6].c_str()));
	//if (pzf[1] == "true")	
	//	cv::imshow("特征图2", suoxiao(&m));
	//ic.findoutlines(&m, m1, pzf[9][0], std::atoi(pzf[10].c_str()));
	//if (pzf[2] == "true")	
	//	cv::imshow("result", m1);
	//cv::waitKey();


	//debug版
	//读取配置文件
	std::ifstream filein;
	filein.open("d:/conf.ini");
	std::string stemp;
	std::vector<std::string> pzf;
	while (!filein.eof())
	{
		filein >> stemp;
		if (stemp[0] != '-'&&stemp[0] != '[')
			pzf.push_back(stemp);
	}
	filein.close();

	/*for (int i = 0; i < pzf.size(); i++)*/
	{
		std::cout << "[conf.ini]" << std::endl;
		std::cout << "srcimg:" << pzf[0] << std::endl;
		std::cout << "denoiseingimg:" << pzf[1] << std::endl;
		std::cout << "resultimg:" << pzf[2] << std::endl;
		std::cout << "contrastparam:" << pzf[3] << std::endl;
		std::cout << "denoiseingparam:" << pzf[4] << std::endl;
		std::cout << "seconddenoise:" << pzf[5] << std::endl;
		std::cout << "seconddenoiseingparam:" << pzf[6] << std::endl;
		std::cout << "mainregion:" << pzf[7] << std::endl;
		std::cout << "mainregionmode:" << pzf[8] << std::endl;
		std::cout << "contourcolor:" << pzf[9] << std::endl;
		std::cout << "contourthickness:" << pzf[10] << std::endl;
		std::cout << "原始图像1:" << pzf[11] << std::endl;
		std::cout << "原始图像2:" << pzf[12] << std::endl;
	}

	int cmp_parm = 6;
	int gas_parm = 3;
	bool cr = false;
	cv::Mat m0 = cv::imread("d:/bc0.bmp");
	cv::Mat m1 = cv::imread("d:/bc2.bmp");
	//cv::medianBlur(m0, m0, 3);
	//cv::medianBlur(m1, m1, 3);
	if (pzf[11] == "true")
		cv::imshow("原始图像1", m0);
	if (pzf[12] == "true")
		cv::imshow("原始图像2", m1);
	ImgCon ic(&m0, &m1);
	ic.compute_h_w_bin();

//mark:
re_cmp:
	cv::Mat m   = ic.compare(cmp_parm, 0);
	cv::Mat m01 = ic.compare(cmp_parm, 1);
	cv::Mat m02 = ic.compare(cmp_parm, 2);
	cv::Mat m03 = ic.compare(cmp_parm, 3);
	cv::Mat m04 = ic.compare(cmp_parm, 4);
	cv::Mat m05 = ic.compare(cmp_parm, 5);
	cv::Mat m06 = ic.compare(cmp_parm, 6);
	cv::Mat m07 = ic.compare(cmp_parm, 7);
	cv::Mat m08 = ic.compare(cmp_parm, 8);
	cv::Mat m09 = ic.compare(cmp_parm, 9);
	cv::Mat m010 = ic.compare(cmp_parm, 10);
	//cv::Mat m011 = ic.compare(cmp_parm, 11);
	//cv::Mat m012 = ic.compare(cmp_parm, 12);

	cv::Mat mres = ic.feature_and(&m01, &m02);
	mres = ic.feature_and(&mres, &m03);
	mres = ic.feature_and(&mres, &m04);
	mres = ic.feature_and(&mres, &m05);
	mres = ic.feature_and(&mres, &m06);
	mres = ic.feature_and(&mres, &m07);
	mres = ic.feature_and(&mres, &m08);
	mres = ic.feature_and(&mres, &m09);
	mres = ic.feature_and(&mres, &m010);
	//mres = ic.feature_and(&mres, &m011);
	//mres = ic.feature_and(&mres, &m012);
	cv::medianBlur(mres, mres, 5);
	cv::medianBlur(mres, mres, 5);
	cv::medianBlur(mres, mres, gas_parm);
	//cv::medianBlur(mres, mres, 3);
	//cv::medianBlur(mres, mres, 3);
	//cv::medianBlur(mres, mres, 9);
	if (cr)
	{
		ic.findconnectedregions(&mres);
		ic.findmainregion(0);
		mres = ic.vecs2binimg(ic.vecs);
		cr = false;
	}
	cv::imshow("innosf", mres);
	char c = cv::waitKey();
	if (c == 'q' ||
		c == 'a' ||
		c == 'w' ||
		c == 's' ||
		c == 'c')
	{
		if (c == 'q')	cmp_parm++;
		if (c == 'a')	cmp_parm--;
		if (c == 'w')	gas_parm += 2;
		if (c == 's')	gas_parm -= 2;
		if (c == 'c')	cr = true;
		if (cmp_parm == -1)	cmp_parm = 0;
		if (cmp_parm == 9)	cmp_parm = 8;
		if (gas_parm == -1)	gas_parm = 1;
		goto re_cmp;
	}
	else
		;



	////ic.feature_and(&m, &m11);
	//if (pzf[0] == "true")
	//	cv::imshow("特征图1", suoxiao(&m));

	////cv::medianBlur(m, m, std::atoi(pzf[4].c_str()));
	////if (pzf[5] == "true")
	////	cv::medianBlur(m, m, std::atoi(pzf[6].c_str()));

	//cv::imshow("特征图2", m);
	//ic.findoutlines(&m, m1, pzf[9][0], std::atoi(pzf[10].c_str()));
	//if (pzf[2] == "true")
	//	cv::imshow("result", m1);
	//cv::waitKey();
	
	return 0;
}
