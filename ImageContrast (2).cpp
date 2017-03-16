

#include "ImageContrast.hpp"

ImgCon::~ImgCon()
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			delete[] h_w_bin0[i][j];
		}
		delete[] h_w_bin0[i];
	}
	delete[] h_w_bin0;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			delete[] h_w_bin1[i][j];
		}
		delete[] h_w_bin1[i];
	}
	delete[] h_w_bin1;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			delete[] haar_val0[i][j];
		}
		delete[] haar_val0[i];
	}
	delete[] haar_val0;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			delete[] haar_val1[i][j];
		}
		delete[] haar_val1[i];
	}
	delete[] haar_val1;

	for (int i = 0; i < h; i++)
	{
		delete[] intgrlimg0[i];
	}
	delete[] intgrlimg0;

	for (int i = 0; i < h; i++)
	{
		delete[] intgrlimg1[i];
	}
	delete[] intgrlimg1;

	//for (int i = 0; i < h; i++)
	//{
	//	delete[] damagelevel[i];
	//}
	//delete[] damagelevel;
}
ImgCon::ImgCon(cv::Mat* img0, cv::Mat* img1)
{
	this->channels = img0->channels();
	this->w = img0->size().width;
	this->h = img0->size().height;
	img0->copyTo(m0);
	img1->copyTo(m1);

	h_w_bin0 = new bool **[h];
	for (int i = 0; i < h; i++)
	{
		h_w_bin0[i] = new bool *[w];
		for (int j = 0; j < w; j++)
		{
			h_w_bin0[i][j] = new bool[24];
		}
	}

	h_w_bin1 = new bool **[h];
	for (int i = 0; i < h; i++)
	{
		h_w_bin1[i] = new bool *[w];
		for (int j = 0; j < w; j++)
		{
			h_w_bin1[i][j] = new bool[24];
		}
	}

	haar_val0 = new int **[h];
	for (int i = 0; i < h; i++)
	{
		haar_val0[i] = new int *[w];
		for (int j = 0; j < w; j++)
		{
			haar_val0[i][j] = new int[12];
		}
	}

	haar_val1 = new int **[h];
	for (int i = 0; i < h; i++)
	{
		haar_val1[i] = new int *[w];
		for (int j = 0; j < w; j++)
		{
			haar_val1[i][j] = new int[12];
		}
	}

	intgrlimg0 = new int *[h];
	for (int i = 0; i < h; i++)
	{
		intgrlimg0[i] = new int[w];
	}

	intgrlimg1 = new int *[h];
	for (int i = 0; i < h; i++)
	{
		intgrlimg1[i] = new int[w];
	}

	//damagelevel = new int *[h];
	//for (int i = 0; i < h; i++)
	//{
	//	damagelevel[i] = new int[w];
	//}
}

void ImgCon::setm0(cv::Mat* m)
{
	this->channels = m->channels();
	this->w = m->size().width;
	this->h = m->size().height;
	m->copyTo(m0);
}

void ImgCon::setm1(cv::Mat* m)
{
	this->channels = m->channels();
	this->w = m->size().width;
	this->h = m->size().height;
	m->copyTo(m1);
}

bool ImgCon::bin(int x, int y)
{
	if (x >= y)	return true;
	else return false;
}

void ImgCon::compute_h_w_bin(int mode)
{
	//int u = 0, v = 0;

#define ImgCon_compute_h_w_bin_V2

#ifdef ImgCon_compute_h_w_bin_V1
	if (channels == 1)		//灰度图，只存前8个数组里面
	{
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				if (i>0 && i < (w - 1) &&
					j>0 && j < (h - 1))		//中间区域
				{
					h_w_bin0[i][j][0] = bin((int)m0.at<uchar>(i, j + 1), (int)m0.at<uchar>(i, j));
					h_w_bin0[i][j][1] = bin((int)m0.at<uchar>(i - 1, j + 1), (int)m0.at<uchar>(i, j));
					h_w_bin0[i][j][2] = bin((int)m0.at<uchar>(i - 1, j), (int)m0.at<uchar>(i, j));
					h_w_bin0[i][j][3] = bin((int)m0.at<uchar>(i - 1, j - 1), (int)m0.at<uchar>(i, j));
					h_w_bin0[i][j][4] = bin((int)m0.at<uchar>(i, j - 1), (int)m0.at<uchar>(i, j));
					h_w_bin0[i][j][5] = bin((int)m0.at<uchar>(i + 1, j - 1), (int)m0.at<uchar>(i, j));
					h_w_bin0[i][j][6] = bin((int)m0.at<uchar>(i + 1, j), (int)m0.at<uchar>(i, j));
					h_w_bin0[i][j][7] = bin((int)m0.at<uchar>(i + 1, j + 1), (int)m0.at<uchar>(i, j));

					h_w_bin1[i][j][0] = bin((int)m1.at<uchar>(i, j + 1), (int)m1.at<uchar>(i, j));
					h_w_bin1[i][j][1] = bin((int)m1.at<uchar>(i - 1, j + 1), (int)m1.at<uchar>(i, j));
					h_w_bin1[i][j][2] = bin((int)m1.at<uchar>(i - 1, j), (int)m1.at<uchar>(i, j));
					h_w_bin1[i][j][3] = bin((int)m1.at<uchar>(i - 1, j - 1), (int)m1.at<uchar>(i, j));
					h_w_bin1[i][j][4] = bin((int)m1.at<uchar>(i, j - 1), (int)m1.at<uchar>(i, j));
					h_w_bin1[i][j][5] = bin((int)m1.at<uchar>(i + 1, j - 1), (int)m1.at<uchar>(i, j));
					h_w_bin1[i][j][6] = bin((int)m1.at<uchar>(i + 1, j), (int)m1.at<uchar>(i, j));
					h_w_bin1[i][j][7] = bin((int)m1.at<uchar>(i + 1, j + 1), (int)m1.at<uchar>(i, j));
				}
				else		//边缘像素不做判断，都设为0
				{
					h_w_bin0[i][j][0] = 0;
					h_w_bin0[i][j][1] = 0;
					h_w_bin0[i][j][2] = 0;
					h_w_bin0[i][j][3] = 0;
					h_w_bin0[i][j][4] = 0;
					h_w_bin0[i][j][5] = 0;
					h_w_bin0[i][j][6] = 0;
					h_w_bin0[i][j][7] = 0;

					h_w_bin1[i][j][0] = 0;
					h_w_bin1[i][j][1] = 0;
					h_w_bin1[i][j][2] = 0;
					h_w_bin1[i][j][3] = 0;
					h_w_bin1[i][j][4] = 0;
					h_w_bin1[i][j][5] = 0;
					h_w_bin1[i][j][6] = 0;
					h_w_bin1[i][j][7] = 0;
				}
				if (i == 300 && j == 456)
				{
					//for (int b = 0; b < 24; b++)
					{
						//std::cout << h_w_bin0[i][j][b] << "," << h_w_bin1[i][j][0] << "  ";
						std::cout << (int)m0.at<uchar>(i, j) << " ";
						std::cout << (int)m0.at<uchar>(i, j + 1) << " ";
						int l = (int)m0.at<uchar>(i, j - 1);
						int l1 = (int)m0.at<uchar>(i, j);
						h_w_bin0[i][j][0] = bin(l, l1);
						h_w_bin0[i][j][1] = bin((int)m0.at<uchar>(i - 1, j + 1), (int)m0.at<uchar>(i, j));
						h_w_bin0[i][j][2] = bin((int)m0.at<uchar>(i - 1, j), (int)m0.at<uchar>(i, j));
					}
					std::cout << std::endl;
				}
			}
		}
	}
	else				//多通道彩色图，存满数组
	{
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				if (i>0 && i < (w - 1) &&
					j>0 && j < (h - 1))		//中间区域
				{
					h_w_bin0[i][j][0] = bin(m0.at<cv::Vec3b>(i, j + 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
					h_w_bin0[i][j][1] = bin(m0.at<cv::Vec3b>(i, j + 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
					h_w_bin0[i][j][2] = bin(m0.at<cv::Vec3b>(i, j + 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
					h_w_bin0[i][j][3] = bin(m0.at<cv::Vec3b>(i - 1, j + 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
					h_w_bin0[i][j][4] = bin(m0.at<cv::Vec3b>(i - 1, j + 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
					h_w_bin0[i][j][5] = bin(m0.at<cv::Vec3b>(i - 1, j + 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
					h_w_bin0[i][j][6] = bin(m0.at<cv::Vec3b>(i - 1, j)[0], m0.at<cv::Vec3b>(i, j)[0]);
					h_w_bin0[i][j][7] = bin(m0.at<cv::Vec3b>(i - 1, j)[1], m0.at<cv::Vec3b>(i, j)[1]);
					h_w_bin0[i][j][8] = bin(m0.at<cv::Vec3b>(i - 1, j)[2], m0.at<cv::Vec3b>(i, j)[2]);
					h_w_bin0[i][j][9] = bin(m0.at<cv::Vec3b>(i - 1, j - 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
					h_w_bin0[i][j][10] = bin(m0.at<cv::Vec3b>(i - 1, j - 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
					h_w_bin0[i][j][11] = bin(m0.at<cv::Vec3b>(i - 1, j - 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
					h_w_bin0[i][j][12] = bin(m0.at<cv::Vec3b>(i, j - 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
					h_w_bin0[i][j][13] = bin(m0.at<cv::Vec3b>(i, j - 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
					h_w_bin0[i][j][14] = bin(m0.at<cv::Vec3b>(i, j - 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
					h_w_bin0[i][j][15] = bin(m0.at<cv::Vec3b>(i + 1, j - 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
					h_w_bin0[i][j][16] = bin(m0.at<cv::Vec3b>(i + 1, j - 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
					h_w_bin0[i][j][17] = bin(m0.at<cv::Vec3b>(i + 1, j - 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
					h_w_bin0[i][j][18] = bin(m0.at<cv::Vec3b>(i + 1, j)[0], m0.at<cv::Vec3b>(i, j)[0]);
					h_w_bin0[i][j][19] = bin(m0.at<cv::Vec3b>(i + 1, j)[1], m0.at<cv::Vec3b>(i, j)[1]);
					h_w_bin0[i][j][20] = bin(m0.at<cv::Vec3b>(i + 1, j)[2], m0.at<cv::Vec3b>(i, j)[2]);
					h_w_bin0[i][j][21] = bin(m0.at<cv::Vec3b>(i + 1, j + 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
					h_w_bin0[i][j][22] = bin(m0.at<cv::Vec3b>(i + 1, j + 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
					h_w_bin0[i][j][23] = bin(m0.at<cv::Vec3b>(i + 1, j + 1)[2], m0.at<cv::Vec3b>(i, j)[2]);

					h_w_bin1[i][j][0] = bin(m1.at<cv::Vec3b>(i, j + 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
					h_w_bin1[i][j][1] = bin(m1.at<cv::Vec3b>(i, j + 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
					h_w_bin1[i][j][2] = bin(m1.at<cv::Vec3b>(i, j + 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
					h_w_bin1[i][j][3] = bin(m1.at<cv::Vec3b>(i - 1, j + 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
					h_w_bin1[i][j][4] = bin(m1.at<cv::Vec3b>(i - 1, j + 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
					h_w_bin1[i][j][5] = bin(m1.at<cv::Vec3b>(i - 1, j + 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
					h_w_bin1[i][j][6] = bin(m1.at<cv::Vec3b>(i - 1, j)[0], m1.at<cv::Vec3b>(i, j)[0]);
					h_w_bin1[i][j][7] = bin(m1.at<cv::Vec3b>(i - 1, j)[1], m1.at<cv::Vec3b>(i, j)[1]);
					h_w_bin1[i][j][8] = bin(m1.at<cv::Vec3b>(i - 1, j)[2], m1.at<cv::Vec3b>(i, j)[2]);
					h_w_bin1[i][j][9] = bin(m1.at<cv::Vec3b>(i - 1, j - 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
					h_w_bin1[i][j][10] = bin(m1.at<cv::Vec3b>(i - 1, j - 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
					h_w_bin1[i][j][11] = bin(m1.at<cv::Vec3b>(i - 1, j - 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
					h_w_bin1[i][j][12] = bin(m1.at<cv::Vec3b>(i, j - 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
					h_w_bin1[i][j][13] = bin(m1.at<cv::Vec3b>(i, j - 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
					h_w_bin1[i][j][14] = bin(m1.at<cv::Vec3b>(i, j - 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
					h_w_bin1[i][j][15] = bin(m1.at<cv::Vec3b>(i + 1, j - 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
					h_w_bin1[i][j][16] = bin(m1.at<cv::Vec3b>(i + 1, j - 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
					h_w_bin1[i][j][17] = bin(m1.at<cv::Vec3b>(i + 1, j - 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
					h_w_bin1[i][j][18] = bin(m1.at<cv::Vec3b>(i + 1, j)[0], m1.at<cv::Vec3b>(i, j)[0]);
					h_w_bin1[i][j][19] = bin(m1.at<cv::Vec3b>(i + 1, j)[1], m1.at<cv::Vec3b>(i, j)[1]);
					h_w_bin1[i][j][20] = bin(m1.at<cv::Vec3b>(i + 1, j)[2], m1.at<cv::Vec3b>(i, j)[2]);
					h_w_bin1[i][j][21] = bin(m1.at<cv::Vec3b>(i + 1, j + 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
					h_w_bin1[i][j][22] = bin(m1.at<cv::Vec3b>(i + 1, j + 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
					h_w_bin1[i][j][23] = bin(m1.at<cv::Vec3b>(i + 1, j + 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
				}
				else		//边缘像素不做判断，都设为0
				{
					h_w_bin0[i][j][0] = 0;
					h_w_bin0[i][j][1] = 0;
					h_w_bin0[i][j][2] = 0;
					h_w_bin0[i][j][3] = 0;
					h_w_bin0[i][j][4] = 0;
					h_w_bin0[i][j][5] = 0;
					h_w_bin0[i][j][6] = 0;
					h_w_bin0[i][j][7] = 0;
					h_w_bin0[i][j][8] = 0;
					h_w_bin0[i][j][9] = 0;
					h_w_bin0[i][j][10] = 0;
					h_w_bin0[i][j][11] = 0;
					h_w_bin0[i][j][12] = 0;
					h_w_bin0[i][j][13] = 0;
					h_w_bin0[i][j][14] = 0;
					h_w_bin0[i][j][15] = 0;
					h_w_bin0[i][j][16] = 0;
					h_w_bin0[i][j][17] = 0;
					h_w_bin0[i][j][18] = 0;
					h_w_bin0[i][j][19] = 0;
					h_w_bin0[i][j][20] = 0;
					h_w_bin0[i][j][21] = 0;
					h_w_bin0[i][j][22] = 0;
					h_w_bin0[i][j][23] = 0;

					h_w_bin1[i][j][0] = 0;
					h_w_bin1[i][j][1] = 0;
					h_w_bin1[i][j][2] = 0;
					h_w_bin1[i][j][3] = 0;
					h_w_bin1[i][j][4] = 0;
					h_w_bin1[i][j][5] = 0;
					h_w_bin1[i][j][6] = 0;
					h_w_bin1[i][j][7] = 0;
					h_w_bin1[i][j][8] = 0;
					h_w_bin1[i][j][9] = 0;
					h_w_bin1[i][j][10] = 0;
					h_w_bin1[i][j][11] = 0;
					h_w_bin1[i][j][12] = 0;
					h_w_bin1[i][j][13] = 0;
					h_w_bin1[i][j][14] = 0;
					h_w_bin1[i][j][15] = 0;
					h_w_bin1[i][j][16] = 0;
					h_w_bin1[i][j][17] = 0;
					h_w_bin1[i][j][18] = 0;
					h_w_bin1[i][j][19] = 0;
					h_w_bin1[i][j][20] = 0;
					h_w_bin1[i][j][21] = 0;
					h_w_bin1[i][j][22] = 0;
					h_w_bin1[i][j][23] = 0;
				}
				if (i == 50 && j == 100)
				{
					//for (int b = 0; b < 24; b++)
					{
						//std::cout << h_w_bin0[i][j][b] << "," << h_w_bin1[i][j][0] << "  ";
						std::cout << m0.at<cv::Vec3b>(i, j)[0] << " ";
						std::cout << m0.at<cv::Vec3b>(i, j)[1] << " ";
						std::cout << m0.at<cv::Vec3b>(i, j)[2] << " ";
					}
					std::cout << std::endl;
				}
			}
		}
	}
#endif
#ifdef ImgCon_compute_h_w_bin_V2
	if (mode == 0)	//LBP
	{
		int v1, v0;
		if (channels == 1)		//灰度图，只存前8个数组里面
		{
			for (int i = 0; i < h; i++)
			{
				for (int j = 0; j < w; j++)
				{
					if (i>0 && i < (h - 1) &&
						j>0 && j < (w - 1))		//中间区域
					{
						v0 = (int)m0.at<uchar>(i, j);
						v1 = (int)m0.at<uchar>(i, j + 1);		h_w_bin0[i][j][0] = bin(v1, v0);
						v1 = (int)m0.at<uchar>(i - 1, j + 1);	h_w_bin0[i][j][1] = bin(v1, v0);
						v1 = (int)m0.at<uchar>(i - 1, j);		h_w_bin0[i][j][2] = bin(v1, v0);
						v1 = (int)m0.at<uchar>(i - 1, j - 1);	h_w_bin0[i][j][3] = bin(v1, v0);
						v1 = (int)m0.at<uchar>(i, j - 1);		h_w_bin0[i][j][4] = bin(v1, v0);
						v1 = (int)m0.at<uchar>(i + 1, j - 1);	h_w_bin0[i][j][5] = bin(v1, v0);
						v1 = (int)m0.at<uchar>(i + 1, j);		h_w_bin0[i][j][6] = bin(v1, v0);
						v1 = (int)m0.at<uchar>(i + 1, j + 1);	h_w_bin0[i][j][7] = bin(v1, v0);

						v0 = (int)m1.at<uchar>(i, j);
						v1 = (int)m1.at<uchar>(i, j + 1);		h_w_bin1[i][j][0] = bin(v1, v0);
						v1 = (int)m1.at<uchar>(i - 1, j + 1);	h_w_bin1[i][j][1] = bin(v1, v0);
						v1 = (int)m1.at<uchar>(i - 1, j);		h_w_bin1[i][j][2] = bin(v1, v0);
						v1 = (int)m1.at<uchar>(i - 1, j - 1);	h_w_bin1[i][j][3] = bin(v1, v0);
						v1 = (int)m1.at<uchar>(i, j - 1);		h_w_bin1[i][j][4] = bin(v1, v0);
						v1 = (int)m1.at<uchar>(i + 1, j - 1);	h_w_bin1[i][j][5] = bin(v1, v0);
						v1 = (int)m1.at<uchar>(i + 1, j);		h_w_bin1[i][j][6] = bin(v1, v0);
						v1 = (int)m1.at<uchar>(i + 1, j + 1);	h_w_bin1[i][j][7] = bin(v1, v0);
					}
					else		//边缘像素不做判断，都设为0
					{
						h_w_bin0[i][j][0] = 0;
						h_w_bin0[i][j][1] = 0;
						h_w_bin0[i][j][2] = 0;
						h_w_bin0[i][j][3] = 0;
						h_w_bin0[i][j][4] = 0;
						h_w_bin0[i][j][5] = 0;
						h_w_bin0[i][j][6] = 0;
						h_w_bin0[i][j][7] = 0;

						h_w_bin1[i][j][0] = 0;
						h_w_bin1[i][j][1] = 0;
						h_w_bin1[i][j][2] = 0;
						h_w_bin1[i][j][3] = 0;
						h_w_bin1[i][j][4] = 0;
						h_w_bin1[i][j][5] = 0;
						h_w_bin1[i][j][6] = 0;
						h_w_bin1[i][j][7] = 0;
					}
				}
			}
		}
		else				//多通道彩色图，存满数组
		{
			for (int i = 0; i < h; i++)
			{
				for (int j = 0; j < w; j++)
				{
					if (i>0 && i < (h - 1) &&
						j>0 && j < (w - 1))		//中间区域
					{
						h_w_bin0[i][j][0] = bin(m0.at<cv::Vec3b>(i, j + 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
						h_w_bin0[i][j][1] = bin(m0.at<cv::Vec3b>(i, j + 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
						h_w_bin0[i][j][2] = bin(m0.at<cv::Vec3b>(i, j + 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
						h_w_bin0[i][j][3] = bin(m0.at<cv::Vec3b>(i - 1, j + 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
						h_w_bin0[i][j][4] = bin(m0.at<cv::Vec3b>(i - 1, j + 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
						h_w_bin0[i][j][5] = bin(m0.at<cv::Vec3b>(i - 1, j + 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
						h_w_bin0[i][j][6] = bin(m0.at<cv::Vec3b>(i - 1, j)[0], m0.at<cv::Vec3b>(i, j)[0]);
						h_w_bin0[i][j][7] = bin(m0.at<cv::Vec3b>(i - 1, j)[1], m0.at<cv::Vec3b>(i, j)[1]);
						h_w_bin0[i][j][8] = bin(m0.at<cv::Vec3b>(i - 1, j)[2], m0.at<cv::Vec3b>(i, j)[2]);
						h_w_bin0[i][j][9] = bin(m0.at<cv::Vec3b>(i - 1, j - 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
						h_w_bin0[i][j][10] = bin(m0.at<cv::Vec3b>(i - 1, j - 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
						h_w_bin0[i][j][11] = bin(m0.at<cv::Vec3b>(i - 1, j - 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
						h_w_bin0[i][j][12] = bin(m0.at<cv::Vec3b>(i, j - 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
						h_w_bin0[i][j][13] = bin(m0.at<cv::Vec3b>(i, j - 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
						h_w_bin0[i][j][14] = bin(m0.at<cv::Vec3b>(i, j - 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
						h_w_bin0[i][j][15] = bin(m0.at<cv::Vec3b>(i + 1, j - 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
						h_w_bin0[i][j][16] = bin(m0.at<cv::Vec3b>(i + 1, j - 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
						h_w_bin0[i][j][17] = bin(m0.at<cv::Vec3b>(i + 1, j - 1)[2], m0.at<cv::Vec3b>(i, j)[2]);
						h_w_bin0[i][j][18] = bin(m0.at<cv::Vec3b>(i + 1, j)[0], m0.at<cv::Vec3b>(i, j)[0]);
						h_w_bin0[i][j][19] = bin(m0.at<cv::Vec3b>(i + 1, j)[1], m0.at<cv::Vec3b>(i, j)[1]);
						h_w_bin0[i][j][20] = bin(m0.at<cv::Vec3b>(i + 1, j)[2], m0.at<cv::Vec3b>(i, j)[2]);
						h_w_bin0[i][j][21] = bin(m0.at<cv::Vec3b>(i + 1, j + 1)[0], m0.at<cv::Vec3b>(i, j)[0]);
						h_w_bin0[i][j][22] = bin(m0.at<cv::Vec3b>(i + 1, j + 1)[1], m0.at<cv::Vec3b>(i, j)[1]);
						h_w_bin0[i][j][23] = bin(m0.at<cv::Vec3b>(i + 1, j + 1)[2], m0.at<cv::Vec3b>(i, j)[2]);

						h_w_bin1[i][j][0] = bin(m1.at<cv::Vec3b>(i, j + 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
						h_w_bin1[i][j][1] = bin(m1.at<cv::Vec3b>(i, j + 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
						h_w_bin1[i][j][2] = bin(m1.at<cv::Vec3b>(i, j + 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
						h_w_bin1[i][j][3] = bin(m1.at<cv::Vec3b>(i - 1, j + 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
						h_w_bin1[i][j][4] = bin(m1.at<cv::Vec3b>(i - 1, j + 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
						h_w_bin1[i][j][5] = bin(m1.at<cv::Vec3b>(i - 1, j + 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
						h_w_bin1[i][j][6] = bin(m1.at<cv::Vec3b>(i - 1, j)[0], m1.at<cv::Vec3b>(i, j)[0]);
						h_w_bin1[i][j][7] = bin(m1.at<cv::Vec3b>(i - 1, j)[1], m1.at<cv::Vec3b>(i, j)[1]);
						h_w_bin1[i][j][8] = bin(m1.at<cv::Vec3b>(i - 1, j)[2], m1.at<cv::Vec3b>(i, j)[2]);
						h_w_bin1[i][j][9] = bin(m1.at<cv::Vec3b>(i - 1, j - 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
						h_w_bin1[i][j][10] = bin(m1.at<cv::Vec3b>(i - 1, j - 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
						h_w_bin1[i][j][11] = bin(m1.at<cv::Vec3b>(i - 1, j - 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
						h_w_bin1[i][j][12] = bin(m1.at<cv::Vec3b>(i, j - 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
						h_w_bin1[i][j][13] = bin(m1.at<cv::Vec3b>(i, j - 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
						h_w_bin1[i][j][14] = bin(m1.at<cv::Vec3b>(i, j - 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
						h_w_bin1[i][j][15] = bin(m1.at<cv::Vec3b>(i + 1, j - 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
						h_w_bin1[i][j][16] = bin(m1.at<cv::Vec3b>(i + 1, j - 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
						h_w_bin1[i][j][17] = bin(m1.at<cv::Vec3b>(i + 1, j - 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
						h_w_bin1[i][j][18] = bin(m1.at<cv::Vec3b>(i + 1, j)[0], m1.at<cv::Vec3b>(i, j)[0]);
						h_w_bin1[i][j][19] = bin(m1.at<cv::Vec3b>(i + 1, j)[1], m1.at<cv::Vec3b>(i, j)[1]);
						h_w_bin1[i][j][20] = bin(m1.at<cv::Vec3b>(i + 1, j)[2], m1.at<cv::Vec3b>(i, j)[2]);
						h_w_bin1[i][j][21] = bin(m1.at<cv::Vec3b>(i + 1, j + 1)[0], m1.at<cv::Vec3b>(i, j)[0]);
						h_w_bin1[i][j][22] = bin(m1.at<cv::Vec3b>(i + 1, j + 1)[1], m1.at<cv::Vec3b>(i, j)[1]);
						h_w_bin1[i][j][23] = bin(m1.at<cv::Vec3b>(i + 1, j + 1)[2], m1.at<cv::Vec3b>(i, j)[2]);
					}
					else		//边缘像素不做判断，都设为0
					{
						h_w_bin0[i][j][0] = 0;
						h_w_bin0[i][j][1] = 0;
						h_w_bin0[i][j][2] = 0;
						h_w_bin0[i][j][3] = 0;
						h_w_bin0[i][j][4] = 0;
						h_w_bin0[i][j][5] = 0;
						h_w_bin0[i][j][6] = 0;
						h_w_bin0[i][j][7] = 0;
						h_w_bin0[i][j][8] = 0;
						h_w_bin0[i][j][9] = 0;
						h_w_bin0[i][j][10] = 0;
						h_w_bin0[i][j][11] = 0;
						h_w_bin0[i][j][12] = 0;
						h_w_bin0[i][j][13] = 0;
						h_w_bin0[i][j][14] = 0;
						h_w_bin0[i][j][15] = 0;
						h_w_bin0[i][j][16] = 0;
						h_w_bin0[i][j][17] = 0;
						h_w_bin0[i][j][18] = 0;
						h_w_bin0[i][j][19] = 0;
						h_w_bin0[i][j][20] = 0;
						h_w_bin0[i][j][21] = 0;
						h_w_bin0[i][j][22] = 0;
						h_w_bin0[i][j][23] = 0;

						h_w_bin1[i][j][0] = 0;
						h_w_bin1[i][j][1] = 0;
						h_w_bin1[i][j][2] = 0;
						h_w_bin1[i][j][3] = 0;
						h_w_bin1[i][j][4] = 0;
						h_w_bin1[i][j][5] = 0;
						h_w_bin1[i][j][6] = 0;
						h_w_bin1[i][j][7] = 0;
						h_w_bin1[i][j][8] = 0;
						h_w_bin1[i][j][9] = 0;
						h_w_bin1[i][j][10] = 0;
						h_w_bin1[i][j][11] = 0;
						h_w_bin1[i][j][12] = 0;
						h_w_bin1[i][j][13] = 0;
						h_w_bin1[i][j][14] = 0;
						h_w_bin1[i][j][15] = 0;
						h_w_bin1[i][j][16] = 0;
						h_w_bin1[i][j][17] = 0;
						h_w_bin1[i][j][18] = 0;
						h_w_bin1[i][j][19] = 0;
						h_w_bin1[i][j][20] = 0;
						h_w_bin1[i][j][21] = 0;
						h_w_bin1[i][j][22] = 0;
						h_w_bin1[i][j][23] = 0;
					}
				}
			}
		}
	}
	else		//haar mode
	{
		int v0, v1;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				if (i>=5 && i <= (h - 6) &&
					j>=5 && j <= (w - 6))		//中间区域
				{
					h_w_bin0[i][j][0] = haar0b(cvPoint(j, i), 5, 1);
					h_w_bin0[i][j][1] = haar0b(cvPoint(j, i), 5, 2);
					h_w_bin0[i][j][2] = haar0b(cvPoint(j, i), 5, 3);
					h_w_bin0[i][j][3] = haar0b(cvPoint(j, i), 7, 1);
					h_w_bin0[i][j][4] = haar0b(cvPoint(j, i), 7, 2);
					h_w_bin0[i][j][5] = haar0b(cvPoint(j, i), 7, 3);
					h_w_bin0[i][j][6] = haar0b(cvPoint(j, i), 9, 1);
					h_w_bin0[i][j][7] = haar0b(cvPoint(j, i), 9, 2);
					h_w_bin0[i][j][8] = haar0b(cvPoint(j, i), 9, 3);
					h_w_bin0[i][j][9] = haar0b(cvPoint(j, i), 11, 1);
					h_w_bin0[i][j][10] = haar0b(cvPoint(j, i), 11, 2);
					h_w_bin0[i][j][11] = haar0b(cvPoint(j, i), 11, 3);

					h_w_bin1[i][j][0] = haar1b(cvPoint(j, i), 5, 1);
					h_w_bin1[i][j][1] = haar1b(cvPoint(j, i), 5, 2);
					h_w_bin1[i][j][2] = haar1b(cvPoint(j, i), 5, 3);
					h_w_bin1[i][j][3] = haar1b(cvPoint(j, i), 7, 1);
					h_w_bin1[i][j][4] = haar1b(cvPoint(j, i), 7, 2);
					h_w_bin1[i][j][5] = haar1b(cvPoint(j, i), 7, 3);
					h_w_bin1[i][j][6] = haar1b(cvPoint(j, i), 9, 1);
					h_w_bin1[i][j][7] = haar1b(cvPoint(j, i), 9, 2);
					h_w_bin1[i][j][8] = haar1b(cvPoint(j, i), 9, 3);
					h_w_bin1[i][j][9] = haar1b(cvPoint(j, i), 11, 1);
					h_w_bin1[i][j][10] = haar1b(cvPoint(j, i), 11, 2);
					h_w_bin1[i][j][11] = haar1b(cvPoint(j, i), 11, 3);
				}
				else		//边缘像素不做判断，都设为0
				{
					h_w_bin0[i][j][0] = 0;
					h_w_bin0[i][j][1] = 0;
					h_w_bin0[i][j][2] = 0;
					h_w_bin0[i][j][3] = 0;
					h_w_bin0[i][j][4] = 0;
					h_w_bin0[i][j][5] = 0;
					h_w_bin0[i][j][6] = 0;
					h_w_bin0[i][j][7] = 0;
					h_w_bin0[i][j][8] = 0;
					h_w_bin0[i][j][9] = 0;
					h_w_bin0[i][j][10] =0;
					h_w_bin0[i][j][11] =0;

					h_w_bin1[i][j][0] = 0;
					h_w_bin1[i][j][1] = 0;
					h_w_bin1[i][j][2] = 0;
					h_w_bin1[i][j][3] = 0;
					h_w_bin1[i][j][4] = 0;
					h_w_bin1[i][j][5] = 0;
					h_w_bin1[i][j][6] = 0;
					h_w_bin1[i][j][7] = 0;
					h_w_bin1[i][j][8] = 0;
					h_w_bin1[i][j][9] = 0;
					h_w_bin1[i][j][10] =0;
					h_w_bin1[i][j][11] =0;
				}
			}
		}
	}


#endif
}

//0 <= threshold <= 8,计算3通道时*3，越高比较规格越严格,返回一个二值特征图。flag:偏移比较，0-12
cv::Mat ImgCon::compare(int threshold, int** scoreMap, bool haar, int flag)
{
	int u, v;
	//cv::Mat scoreMap(h, w, CV_8U, cv::Scalar(0));	//得分图，越高毁伤程度越高
	cv::Mat res(h, w, CV_8U, cv::Scalar(0));		//The result of compared
	int count = 0;
	float rad = 100.0 / 12.0;
	if (haar)
	{
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				if (i >= 2 && i < h - 2 &&
					j >= 2 && j < w - 2)
				{
					if (flag == 0)	//cc
					{
						u = i;	v = j;
					}
					if (flag == 1)	//lu
					{
						u = i - 1;	v = j - 1;
					}
					if (flag == 2)	//ru
					{
						u = i - 1;	v = j + 1;
					}
					if (flag == 3)	//ld
					{
						u = i + 1;	v = j - 1;
					}
					if (flag == 4)	//rd
					{
						u = i + 1;	v = j + 1;
					}
					if (flag == 5)	//u
					{
						u = i - 1;	v = j;
					}
					if (flag == 6)	//r
					{
						u = i;	v = j + 1;
					}
					if (flag == 7)	//d
					{
						u = i + 1;	v = j;
					}
					if (flag == 8)	//l
					{
						u = i;	v = j - 1;
					}
					if (flag == 9)	//lu2
					{
						u = i - 2;	v = j - 2;
					}
					if (flag == 10)	//ru2
					{
						u = i - 2;	v = j + 2;
					}
					if (flag == 11)	//rd2
					{
						u = i + 2;	v = j + 2;
					}
					if (flag == 12)	//ld2
					{
						u = i + 2;	v = j - 2;
					}
					if (flag == 13)	//u2
					{
						u = i - 2;	v = j;
					}
					if (flag == 14)	//d2
					{
						u = i + 2;	v = j;
					}
					for (int k = 0; k < 12; k++)
					{
						if (h_w_bin0[i][j][k] == h_w_bin1[u][v][k])
							count++;	//一样的个数
					}
					if (count <= threshold)
						res.at<uchar>(i, j) = 255;
					//if (flag == 0)
					scoreMap[i][j] = (int)(rad*(12 - count));
					count = 0;
				}
				else
				{
					scoreMap[i][j] = 0;
				}
			}
		}
		////把边界得分赋为0；
		//for (int i = 0; i == 0 || i == 1 || i == h - 2 || i == h - 1; i++)
		//{
		//	for (int j = 0; j == 0 || j == 1 || j == w - 2 || j == w - 1; j++)
		//	{
		//		scoreMap[i][j] = 0;
		//	}
		//}
	}
	else
	{
		if (channels == 1)
		{
			for (int i = 2; i < h - 2; i++)
			{
				for (int j = 2; j < w - 2; j++)
				{
					if (flag == 0)	//cc
					{
						u = i;	v = j;
					}
					if (flag == 1)	//lu
					{
						u = i - 1;	v = j - 1;
					}
					if (flag == 2)	//ru
					{
						u = i - 1;	v = j + 1;
					}
					if (flag == 3)	//ld
					{
						u = i + 1;	v = j - 1;
					}
					if (flag == 4)	//rd
					{
						u = i + 1;	v = j + 1;
					}
					if (flag == 5)	//u
					{
						u = i - 1;	v = j;
					}
					if (flag == 6)	//r
					{
						u = i;	v = j + 1;
					}
					if (flag == 7)	//d
					{
						u = i + 1;	v = j;
					}
					if (flag == 8)	//l
					{
						u = i;	v = j - 1;
					}
					if (flag == 9)	//uu
					{
						u = i - 2;	v = j;
					}
					if (flag == 10)	//rr
					{
						u = i;	v = j + 2;
					}
					if (flag == 11)	//dd
					{
						u = i + 2;	v = j;
					}
					if (flag == 12)	//ll
					{
						u = i;	v = j - 2;
					}
					for (int k = 0; k < 8; k++)
					{
						if (h_w_bin0[i][j][k] == h_w_bin1[u][v][k])
							count++;
					}
					if (count <= threshold)
						res.at<uchar>(i, j) = 255;
					count = 0;
				}
			}
		}
		else
		{
			for (int i = 2; i < h - 2; i++)
			{
				for (int j = 2; j < w - 2; j++)
				{
					if (flag == 0)
					{
						u = i;	v = j;
					}
					if (flag == 1)
					{
						u = i - 1;	v = j - 1;
					}
					if (flag == 2)
					{
						u = i - 1;	v = j + 1;
					}
					if (flag == 3)
					{
						u = i + 1;	v = j - 1;
					}
					if (flag == 4)
					{
						u = i + 1;	v = j + 1;
					}
					for (int k = 0; k < 24; k++)
					{
						if (h_w_bin0[i][j][k] == h_w_bin1[u][v][k])
							count++;
					}
					if (count <= 3 * threshold)
						res.at<uchar>(i, j) = 255;
					count = 0;
				}
			}
		}
	}
	return res;
}

bool ImgCon::p_isinimg(CvPoint p, cv::Mat* m)
{
	if (p.x >= 0 && p.x < m->size().width &&
		p.y >= 0 && p.y < m->size().height)
		return true;
	else
		return false;
}

bool ImgCon::notinvec(std::vector<CvPoint> vec, CvPoint p)
{
	bool bo = true;
	for (int i = 0; i < vec.size(); i++)
	{
		if ((vec[i].x == p.x) && 
			(vec[i].y == p.y))	bo = false;
	}
	return bo;
}

std::vector<CvPoint> ImgCon::connectedregion(cv::Mat& m, CvPoint p)
{
#define ImgCon_connectedregion_V2

	std::vector<CvPoint> ps;

#ifdef ImgCon_connectedregion_V1
	CvPoint p1;
	int size1 = 0, size2 = 0;

	ps.push_back(p);
	while (1)
	{
		size1 = ps.size();
		for (int i = 0; i < size1; i++)
		{
			p = ps[i];
			p1 = cvPoint(p.x, p.y + 1);
			if (p_isinimg(p1, m))	if (m->at<uchar>(p1) == m->at<uchar>(p))	if (notinvec(ps, p1))	ps.push_back(p1);
			p1 = cvPoint(p.x - 1, p.y + 1);
			if (p_isinimg(p1, m))	if (m->at<uchar>(p1) == m->at<uchar>(p))	if (notinvec(ps, p1))	ps.push_back(p1);
			p1 = cvPoint(p.x - 1, p.y);
			if (p_isinimg(p1, m))	if (m->at<uchar>(p1) == m->at<uchar>(p))	if (notinvec(ps, p1))	ps.push_back(p1);
			p1 = cvPoint(p.x - 1, p.y - 1);
			if (p_isinimg(p1, m))	if (m->at<uchar>(p1) == m->at<uchar>(p))	if (notinvec(ps, p1))	ps.push_back(p1);
			p1 = cvPoint(p.x, p.y - 1);
			if (p_isinimg(p1, m))	if (m->at<uchar>(p1) == m->at<uchar>(p))	if (notinvec(ps, p1))	ps.push_back(p1);
			p1 = cvPoint(p.x + 1, p.y - 1);
			if (p_isinimg(p1, m))	if (m->at<uchar>(p1) == m->at<uchar>(p))	if (notinvec(ps, p1))	ps.push_back(p1);
			p1 = cvPoint(p.x + 1, p.y);
			if (p_isinimg(p1, m))	if (m->at<uchar>(p1) == m->at<uchar>(p))	if (notinvec(ps, p1))	ps.push_back(p1);
			p1 = cvPoint(p.x + 1, p.y + 1);
			if (p_isinimg(p1, m))	if (m->at<uchar>(p1) == m->at<uchar>(p))	if (notinvec(ps, p1))	ps.push_back(p1);
		}
		size2 = ps.size();
		if (size2 - size1 == 0)	break;
	}
#endif
#ifdef ImgCon_connectedregion_V2
	//CvPoint pss[500000];	//存连通区域像素
	CvPoint *pss = new CvPoint[500000];
	int p_ = 1;		//记录指针位置
	//int lon = 1;	//轮廓长度
	int numb = 1;		//新加入的像素的个数

	pss[0] = p;
	while (1)
	{
		//清除和复位
		//for (int i = p_ - 1; i >= p_ - numb + 1; i--)
		//{
		//	m->at<uchar>(pss[i]) = 0;
		//}
		int k = numb;	//轮廓长度
		numb = 0;	
		for (int i = p_ - 1; i >= p_ - k; i--)	//求新加入的点
		{
			p = pss[i];
			CvPoint p1;
			p1 = CvPoint(p.x, p.y + 1);
			if (p_isinimg(p1, &m))
			{
				//std::cout << p1.x << " " << p1.y << std::endl;
				if (m.at<uchar>(p1) >= 200)
				{
					pss[p_ + numb] = p1;
					numb++;
					m.at<uchar>(p1) = 0;
				}
			}
			p1 = CvPoint(p.x, p.y - 1);
			if (p_isinimg(p1, &m))	if (m.at<uchar>(p1) >= 200)	{ pss[p_ + numb] = p1;	numb++;	m.at<uchar>(p1) = 0; }
			p1 = CvPoint(p.x - 1, p.y + 1);
			if (p_isinimg(p1, &m))	if (m.at<uchar>(p1) >= 200)	{ pss[p_ + numb] = p1;	numb++;	m.at<uchar>(p1) = 0; }
			p1 = CvPoint(p.x - 1, p.y);
			if (p_isinimg(p1, &m))	if (m.at<uchar>(p1) >= 200)	{ pss[p_ + numb] = p1;	numb++;	m.at<uchar>(p1) = 0; }
			p1 = CvPoint(p.x - 1, p.y - 1);
			if (p_isinimg(p1, &m))	if (m.at<uchar>(p1) >= 200)	{ pss[p_ + numb] = p1;	numb++;	m.at<uchar>(p1) = 0; }
			p1 = CvPoint(p.x + 1, p.y + 1);
			if (p_isinimg(p1, &m))	if (m.at<uchar>(p1) >= 200)	{ pss[p_ + numb] = p1;	numb++;	m.at<uchar>(p1) = 0; }
			p1 = CvPoint(p.x + 1, p.y - 1);
			if (p_isinimg(p1, &m))	if (m.at<uchar>(p1) >= 200)	{ pss[p_ + numb] = p1;	numb++;	m.at<uchar>(p1) = 0; }
			p1 = CvPoint(p.x + 1, p.y);
			if (p_isinimg(p1, &m))	if (m.at<uchar>(p1) >= 200)	{ pss[p_ + numb] = p1;	numb++;	m.at<uchar>(p1) = 0; }
		}

		p_ = p_ + numb;		//指针往后移
		if (numb == 0)	break;		//没有新加入的点了跳出
	}

	for (int i = 0; i < p_; i++)
	{
		ps.push_back(pss[i]);
	}
	delete[] pss;

#endif

	return ps;
}

void ImgCon::revalregion(cv::Mat& m, std::vector<CvPoint> ps, int val)
{
	for (int i = 0; i < ps.size(); i++)
	{
		m.at<uchar>(ps[i]) = val;
	}
}

//m应为二值图像
void ImgCon::findconnectedregions(cv::Mat* m)
{
	//cv::Mat m;
	//img->copyTo(m);
	int h = m->size().height;
	int w = m->size().width;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m->at<uchar>(i, j) >= 200)
			{
				vecs.push_back(connectedregion(*m, CvPoint(j, i)));
			}
		}
	}
}
//m应为二值图像
void ImgCon::findconnectedregions(cv::Mat* m, std::vector<std::vector<CvPoint>> &vs)
{
	//cv::Mat m;
	//img->copyTo(m);
	int h = m->size().height;
	int w = m->size().width;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m->at<uchar>(i, j) >= 200)
			{
				vs.push_back(connectedregion(*m, CvPoint(j, i)));
			}
		}
	}
}

void ImgCon::revalregion_s(cv::Mat& m, int val, int flag)
{
	if (flag == 0)		//对连通区域赋值
	{
		if (m.channels() == 1)
		{
			for (int i = 0; i < vecs.size(); i++)
			{
				for (int j = 0; j < vecs[i].size(); j++)
				{
					m.at<uchar>(vecs[i][j]) = val;
				}
			}
		}
		else
		{
			for (int i = 0; i < vecs.size(); i++)
			{
				for (int j = 0; j < vecs[i].size(); j++)
				{
					m.at<cv::Vec3b>(vecs[i][j]) = cv::Vec3b(0, 0, 255);		//标记红色
				}
			}
		}
	}
	else		//对轮廓赋值
	{
		if (m.channels() == 1)
		{
			for (int i = 0; i < outlines.size(); i++)
			{
				for (int j = 0; j < outlines[i].size(); j++)
				{
					m.at<uchar>(outlines[i][j]) = val;
				}
			}
		}
		else
		{
			for (int i = 0; i < outlines.size(); i++)
			{
				for (int j = 0; j < outlines[i].size(); j++)
				{
					m.at<cv::Vec3b>(outlines[i][j]) = cv::Vec3b(0, 0, 255);		//标记红色
				}
			}
		}
	}
}

void ImgCon::findmainregion(int mode)
{
	std::vector<std::vector<CvPoint>> mainvecs;

	if (mode == 0)
	{
		int temp = 0;
		for (int i = 0; i < vecs.size(); i++)
		{
			temp = temp + vecs[i].size();
		}
		temp = temp / vecs.size();		//平均面积
		for (int i = 0; i < vecs.size(); i++)
		{
			if (vecs[i].size() >= temp)
				mainvecs.push_back(vecs[i]);
		}
	}
	else if (mode == 1)
	{
		//最小面积阈值
		int minarea = 25;
		for (int i = 0; i < vecs.size(); i++)
		{
			if (vecs[i].size() >= 9)
				mainvecs.push_back(vecs[i]);
		}
	}

	vecs = mainvecs;
}

void ImgCon::findoutlines(cv::Mat* img, cv::Mat &m, char color, int thickness)
{
#define ImgConfindoutlines_V2

#ifdef ImgConfindoutlines_V1
	for (int i = 0; i < vecs.size(); i++)
	{
		std::vector<CvPoint> outl;
		for (int j = 0; j < vecs[i].size(); j++)
		{
			CvPoint cp = vecs[i][j];
			//if (cp.x>0 && cp.x < img->size().width - 1 &&
			//	cp.y>0 && cp.y < img->size().height - 1)
			if (img->at<uchar>(cp.y, cp.x + 1) == 0 ||
				img->at<uchar>(cp.y, cp.x - 1) == 0 ||
				img->at<uchar>(cp.y + 1, cp.x + 1) == 0 ||
				img->at<uchar>(cp.y + 1, cp.x - 1) == 0 ||
				img->at<uchar>(cp.y + 1, cp.x) == 0 ||
				img->at<uchar>(cp.y - 1, cp.x + 1) == 0 ||
				img->at<uchar>(cp.y - 1, cp.x - 1) == 0 ||
				img->at<uchar>(cp.y - 1, cp.x) == 0
				)
				outl.push_back(vecs[i][j]);
		}
		outlines.push_back(outl);
		outl.clear();
	}
#endif

	//找轮廓从img
#ifdef ImgConfindoutlines_V2
	//cv::imshow("90",*img);
	//cv::waitKey();
	std::vector<CvPoint> ps;	//cunlunkuodian
	int p0, p1, p2, p3, p4, p5, p6, p7, p8;
	for (int i = 1; i < (img->size().height - 1); i++)
	{
		for (int j = 1; j < (img->size().width - 1); j++)
		{
			p0 = img->at<uchar>(i, j);
			p1 = img->at<uchar>(i, j - 1);
			p2 = img->at<uchar>(i, j + 1);
			p3 = img->at<uchar>(i + 1, j);
			p4 = img->at<uchar>(i + 1, j - 1);
			p5 = img->at<uchar>(i + 1, j + 1);
			p6 = img->at<uchar>(i - 1, j);
			p7 = img->at<uchar>(i - 1, j - 1);
			p8 = img->at<uchar>(i - 1, j + 1);

			if ((p0 != p1) ||
				(p0 != p2) ||
				(p0 != p3) ||
				(p0 != p4) ||
				(p0 != p5) ||
				(p0 != p6) ||
				(p0 != p7) ||
				(p0 != p8))
			{
				if (thickness == 1)
				{
					ps.push_back(cvPoint(j, i));
				}
				if (thickness == 2)
				{
					ps.push_back(cvPoint(j, i));
					ps.push_back(cvPoint(j, i - 1));
					ps.push_back(cvPoint(j, i + 1));
					ps.push_back(cvPoint(j + 1, i));
					ps.push_back(cvPoint(j - 1, i));
				}
				if (thickness == 3)
				{
					ps.push_back(cvPoint(j, i));
					ps.push_back(cvPoint(j, i - 1));
					ps.push_back(cvPoint(j, i + 1));
					ps.push_back(cvPoint(j + 1, i));
					ps.push_back(cvPoint(j - 1, i));
					ps.push_back(cvPoint(j + 1, i - 1));
					ps.push_back(cvPoint(j + 1, i + 1));
					ps.push_back(cvPoint(j - 1, i - 1));
					ps.push_back(cvPoint(j - 1, i + 1));
				}
			}
		}
	}
	//画轮廓到m
	if (m.channels() == 1)
	{
		for (int i = 0; i < ps.size(); i++)
		{
			m.at<uchar>(ps[i]) = 150;
		}
	}
	else
	{
		for (int i = 0; i < ps.size(); i++)
		{
			if (color == 'R')
				m.at<cv::Vec3b>(ps[i]) = cv::Vec3b(0, 0, 255);
			if (color == 'G')
				m.at<cv::Vec3b>(ps[i]) = cv::Vec3b(0, 255, 0);
			if (color == 'B')
				m.at<cv::Vec3b>(ps[i]) = cv::Vec3b(255, 0, 0);
		}
	}



#endif
}

void ImgCon::debuginfo()
{
	std::cout << vecs[0].size() << std::endl;
	std::cout << outlines[0].size() << std::endl;
}

cv::Mat ImgCon::feature_and(cv::Mat* m1, cv::Mat* m2)	//
{
	int h = m1->size().height;
	int w = m1->size().width;
	cv::Mat res(h, w, CV_8U, cv::Scalar(0));		//The result of compared

	if (h != m2->size().height ||
		w != m2->size().width)
		return res;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m1->at<uchar>(i, j) >= 250 &&
				m2->at<uchar>(i, j) >= 250)
				res.at<uchar>(i, j) = 255;
		}
	}

	return res;
}
cv::Mat ImgCon::feature_or(cv::Mat* m1, cv::Mat* m2)	//
{
	int h = m1->size().height;
	int w = m1->size().width;
	cv::Mat res(h, w, CV_8U, cv::Scalar(0));		//The result of compared

	if (h != m2->size().height ||
		w != m2->size().width)
		return res;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m1->at<uchar>(i, j) >= 250 ||
				m2->at<uchar>(i, j) >= 250)
				res.at<uchar>(i, j) = 255;
		}
	}

	return res;
}

cv::Mat ImgCon::vecs2binimg(std::vector<std::vector<CvPoint>> vs)
{
	cv::Mat res(h, w, CV_8U, cv::Scalar(0));

	for (int i = 0; i < vs.size(); i++)
	{
		for (int j = 0; j < vs[i].size(); j++)
		{
			res.at<uchar>(vs[i][j]) = 255;
		}
	}

	return res;
}

void ImgCon::integral_img()
{
	cv::Mat img;

	m0.copyTo(img);
	if (img.channels()>1)
		cv::cvtColor(img, img, CV_BGR2GRAY);

	int h = m0.size().height;
	int w = m0.size().width;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int temp1, temp2, temp3;
			if (j == 0)	
				temp1 = 0;
			else
				temp1 = intgrlimg0[i][j - 1];
			if (i == 0 || j == 0)
				temp2 = 0;
			else
				temp2 = intgrlimg0[i - 1][j - 1];
			if (i == 0)
				temp3 = 0;
			else
				temp3 = intgrlimg0[i - 1][j];
			intgrlimg0[i][j] = temp1 + temp3 - temp2 + img.at<uchar>(i, j);
		}
	}

	m1.copyTo(img);
	if (img.channels()>1)
		cv::cvtColor(img, img, CV_BGR2GRAY);

	h = m1.size().height;
	w = m1.size().width;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int temp1, temp2, temp3;
			if (j == 0)
				temp1 = 0;
			else
				temp1 = intgrlimg1[i][j - 1];
			if (i == 0 || j == 0)
				temp2 = 0;
			else
				temp2 = intgrlimg1[i - 1][j - 1];
			if (i == 0)
				temp3 = 0;
			else
				temp3 = intgrlimg1[i - 1][j];
			intgrlimg1[i][j] = temp1 + temp3 - temp2 + img.at<uchar>(i, j);
		}
	}
}

int ImgCon::conv(cv::Mat *m, int mode)
{
	int h = m->size().height;
	int w = m->size().width;

	//用9个点表示田字结构
	cv::Point lu(0, 0);
	cv::Point l(0, (int)(h / 2));
	cv::Point ld(0, h - 1);
	cv::Point ru(w - 1, 0);
	cv::Point r(w - 1, (int)(h / 2));
	cv::Point rd(w - 1, h - 1);
	cv::Point u((int)(w / 2), 0);
	cv::Point c((int)(w / 2), (int)(h / 2));
	cv::Point d((int)(h / 2), h - 1);

	int v1, v2, v3, v4;
	int x1 = intgrlimg0[r.y][r.x];
	int x2 = intgrlimg0[lu.y][lu.x];
	int x3 = intgrlimg0[ru.y][ru.x];
	int x4 = intgrlimg0[l.y][l.x];
	switch (mode)
	{
		//日
	case 1:												//	--------------------> w
		// v1, v2;	"日"的上下两个区域的值				//
		v1 = intgrlimg0[r.y][r.x] +						//	lu		u		ru
			intgrlimg0[lu.y][lu.x] -						//	
			intgrlimg0[ru.y][ru.x] -						//	l		c		r
			intgrlimg0[l.y][l.x];						//
		v2 = intgrlimg0[rd.y][rd.x] +					//	ld		d		rd
			intgrlimg0[l.y][l.x] -						//
			intgrlimg0[r.y][r.x] -						// h
			intgrlimg0[ld.y][ld.x];
		return v1 - v2;
		//躺日
	case 2:
		v1 = intgrlimg0[d.y][d.x] +
			intgrlimg0[lu.y][lu.x] -
			intgrlimg0[u.y][u.x] -
			intgrlimg0[ld.y][ld.x];
		v2 = intgrlimg0[ld.y][ld.x] +
			intgrlimg0[u.y][u.x] -
			intgrlimg0[lu.y][lu.x] -
			intgrlimg0[d.y][d.x];
		return v1 - v2;
		//田
	case 3:
		v1 = intgrlimg0[c.y][c.x] +
			intgrlimg0[lu.y][lu.x] -
			intgrlimg0[u.y][u.x] -
			intgrlimg0[l.y][l.x];
		v2 = intgrlimg0[r.y][r.x] +
			intgrlimg0[u.y][u.x] -
			intgrlimg0[ru.y][ru.x] -
			intgrlimg0[c.y][c.x];
		v3 = intgrlimg0[d.y][d.x] +
			intgrlimg0[l.y][l.x] -
			intgrlimg0[c.y][c.x] -
			intgrlimg0[ld.y][ld.x];
		v4 = intgrlimg0[rd.y][rd.x] +
			intgrlimg0[c.y][c.x] -
			intgrlimg0[r.y][r.x] -
			intgrlimg0[d.y][d.x];
		return v1 + v4 - v2 - v3;
	default:
		return 0;
	}
}

//p>=5, kernel:3,5,7,9,11, mode:1,2,3
int ImgCon::haar0(CvPoint p, int kernel, int mode)
{
	int w = p.x;
	int h = p.y;
	int s = (kernel - 1) / 2;

	//用9个点表示田字结构								//	--------------------> w
	cv::Point lu(w - s, h - s);							//
	cv::Point l(w - s, h);								//	lu(w-s,h-s)	u(w,h-s)	ru(w+s,h-s)
	cv::Point ld(w - s, h + s);							//	
	cv::Point ru(w + s, h - s);							//	l(w-s,h)	c(w,h)		r(w+s,h)
	cv::Point r(w + s, h);								//
	cv::Point rd(w + s, h + s);							//	ld(w-s,h+s)	d(w,h+s)	rd(w+s,h+s)
	cv::Point u(w, h - s);								//
	cv::Point c(w, h);									// h
	cv::Point d(w, h + s);

	//4个区域
	int v1, v2, v3, v4;
	int i1, i2, i3, i4;
	int hh, ww;
	switch (mode)
	{
		//日
	case 1:											
		// v1, v2;	"日"的上下两个区域的值			
		hh = r.y;
		ww = r.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = lu.y - 1;
		ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = ru.y - 1;
		ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = l.y;
		ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = rd.y;
		ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = l.y - 1;
		ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = r.y - 1;
		ww = r.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = ld.y;
		ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		return v1 - v2;
		//躺日
	case 2:
		hh = d.y;
		ww = d.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = lu.y - 1;
		ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = u.y - 1;
		ww = u.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = ld.y;
		ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = rd.y;
		ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = u.y - 1;
		ww = u.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = ru.y - 1;
		ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = d.y;
		ww = d.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		return v1 - v2;
		//田
	case 3:
		hh = c.y;	ww = c.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = lu.y - 1;	ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = u.y - 1;	ww = u.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = l.y;	ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = r.y;	ww = r.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = u.y - 1;	ww = u.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = ru.y - 1;	ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = c.y;	ww = c.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		hh = d.y;	ww = d.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = l.y - 1;	ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = c.y - 1;	ww = c.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = ld.y;	ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v3 = i1 + i2 - i3 - i4;

		hh = rd.y;	ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = c.y - 1;	ww = c.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = r.y - 1;	ww = r.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = d.y;	ww = d.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v4 = i1 + i2 - i3 - i4;

		return v1 + v4 - v2 - v3;
	default:
		return 0;
	}
}

int ImgCon::haar1(CvPoint p, int kernel, int mode)
{
	int w = p.x;
	int h = p.y;
	int s = (kernel - 1) / 2;

	//用9个点表示田字结构								//	--------------------> w
	cv::Point lu(w - s, h - s);							//
	cv::Point l(w - s, h);								//	lu(w-s,h-s)	u(w,h-s)	ru(w+s,h-s)
	cv::Point ld(w - s, h + s);							//	
	cv::Point ru(w + s, h - s);							//	l(w-s,h)	c(w,h)		r(w+s,h)
	cv::Point r(w + s, h);								//
	cv::Point rd(w + s, h + s);							//	ld(w-s,h+s)	d(w,h+s)	rd(w+s,h+s)
	cv::Point u(w, h - s);								//
	cv::Point c(w, h);									// h
	cv::Point d(w, h + s);

	//4个区域
	int v1, v2, v3, v4;
	int i1, i2, i3, i4;
	int hh, ww;
	switch (mode)
	{
		//日
	case 1:
		// v1, v2;	"日"的上下两个区域的值			
		hh = r.y;
		ww = r.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = lu.y - 1;
		ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = ru.y - 1;
		ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = l.y;
		ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = rd.y;
		ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = l.y - 1;
		ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = r.y - 1;
		ww = r.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = ld.y;
		ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		return v1 - v2;
		//躺日
	case 2:
		hh = d.y;
		ww = d.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = lu.y - 1;
		ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = u.y - 1;
		ww = u.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = ld.y;
		ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = rd.y;
		ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = u.y - 1;
		ww = u.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = ru.y - 1;
		ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = d.y;
		ww = d.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		return v1 - v2;
		//田
	case 3:
		hh = c.y;	ww = c.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = lu.y - 1;	ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = u.y - 1;	ww = u.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = l.y;	ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = r.y;	ww = r.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = u.y - 1;	ww = u.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = ru.y - 1;	ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = c.y;	ww = c.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		hh = d.y;	ww = d.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = l.y - 1;	ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = c.y - 1;	ww = c.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = ld.y;	ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v3 = i1 + i2 - i3 - i4;

		hh = rd.y;	ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = c.y - 1;	ww = c.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = r.y - 1;	ww = r.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = d.y;	ww = d.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v4 = i1 + i2 - i3 - i4;

		return v1 + v4 - v2 - v3;
	default:
		return 0;
	}
}

bool ImgCon::haar0b(CvPoint p, int kernel, int mode)
{
	int w = p.x;
	int h = p.y;
	int s = (kernel - 1) / 2;

	//用9个点表示田字结构								//	--------------------> w
	cv::Point lu(w - s, h - s);							//
	cv::Point l(w - s, h);								//	lu(w-s,h-s)	u(w,h-s)	ru(w+s,h-s)
	cv::Point ld(w - s, h + s);							//	
	cv::Point ru(w + s, h - s);							//	l(w-s,h)	c(w,h)		r(w+s,h)
	cv::Point r(w + s, h);								//
	cv::Point rd(w + s, h + s);							//	ld(w-s,h+s)	d(w,h+s)	rd(w+s,h+s)
	cv::Point u(w, h - s);								//
	cv::Point c(w, h);									// h
	cv::Point d(w, h + s);

	//4个区域
	int v1, v2, v3, v4;
	int i1, i2, i3, i4;
	int hh, ww;
	switch (mode)
	{
		//日
	case 1:
		// v1, v2;	"日"的上下两个区域的值			
		hh = r.y;
		ww = r.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = lu.y - 1;
		ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = ru.y - 1;
		ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = l.y;
		ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = rd.y;
		ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = l.y - 1;
		ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = r.y - 1;
		ww = r.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = ld.y;
		ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		if (v1 >= v2)	return true;
		else	return false;
		//躺日
	case 2:
		hh = d.y;
		ww = d.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = lu.y - 1;
		ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = u.y - 1;
		ww = u.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = ld.y;
		ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = rd.y;
		ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = u.y - 1;
		ww = u.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = ru.y - 1;
		ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = d.y;
		ww = d.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		if (v1 >= v2)	return true;
		else	return false;
		//田
	case 3:
		hh = c.y;	ww = c.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = lu.y - 1;	ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = u.y - 1;	ww = u.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = l.y;	ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = r.y;	ww = r.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = u.y - 1;	ww = u.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = ru.y - 1;	ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = c.y;	ww = c.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		hh = d.y;	ww = d.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = l.y - 1;	ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = c.y - 1;	ww = c.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = ld.y;	ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v3 = i1 + i2 - i3 - i4;

		hh = rd.y;	ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg0[hh][ww];

		hh = c.y - 1;	ww = c.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg0[hh][ww];

		hh = r.y - 1;	ww = r.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg0[hh][ww];

		hh = d.y;	ww = d.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg0[hh][ww];
		v4 = i1 + i2 - i3 - i4;

		if ((v1 + v4) >= (v2 + v3))	return true;
		else	return false;
	default:
		return 0;
	}
}

bool ImgCon::haar1b(CvPoint p, int kernel, int mode)
{
	int w = p.x;
	int h = p.y;
	int s = (kernel - 1) / 2;

	//用9个点表示田字结构								//	--------------------> w
	cv::Point lu(w - s, h - s);							//
	cv::Point l(w - s, h);								//	lu(w-s,h-s)	u(w,h-s)	ru(w+s,h-s)
	cv::Point ld(w - s, h + s);							//	
	cv::Point ru(w + s, h - s);							//	l(w-s,h)	c(w,h)		r(w+s,h)
	cv::Point r(w + s, h);								//
	cv::Point rd(w + s, h + s);							//	ld(w-s,h+s)	d(w,h+s)	rd(w+s,h+s)
	cv::Point u(w, h - s);								//
	cv::Point c(w, h);									// h
	cv::Point d(w, h + s);

	//4个区域
	int v1, v2, v3, v4;
	int i1, i2, i3, i4;
	int hh, ww;
	switch (mode)
	{
		//日
	case 1:
		// v1, v2;	"日"的上下两个区域的值			
		hh = r.y;
		ww = r.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = lu.y - 1;
		ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = ru.y - 1;
		ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = l.y;
		ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = rd.y;
		ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = l.y - 1;
		ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = r.y - 1;
		ww = r.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = ld.y;
		ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		if (v1 >= v2)	return true;
		else	return false;
		//躺日
	case 2:
		hh = d.y;
		ww = d.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = lu.y - 1;
		ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = u.y - 1;
		ww = u.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = ld.y;
		ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = rd.y;
		ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = u.y - 1;
		ww = u.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = ru.y - 1;
		ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = d.y;
		ww = d.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		if (v1 >= v2)	return true;
		else	return false;
		//田
	case 3:
		hh = c.y;	ww = c.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = lu.y - 1;	ww = lu.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = u.y - 1;	ww = u.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = l.y;	ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v1 = i1 + i2 - i3 - i4;


		hh = r.y;	ww = r.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = u.y - 1;	ww = u.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = ru.y - 1;	ww = ru.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = c.y;	ww = c.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v2 = i1 + i2 - i3 - i4;

		hh = d.y;	ww = d.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = l.y - 1;	ww = l.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = c.y - 1;	ww = c.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = ld.y;	ww = ld.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v3 = i1 + i2 - i3 - i4;

		hh = rd.y;	ww = rd.x;
		if (hh < 0 || ww < 0)
			i1 = 0;
		else
			i1 = intgrlimg1[hh][ww];

		hh = c.y - 1;	ww = c.x - 1;
		if (hh < 0 || ww < 0)
			i2 = 0;
		else
			i2 = intgrlimg1[hh][ww];

		hh = r.y - 1;	ww = r.x;
		if (hh < 0 || ww < 0)
			i3 = 0;
		else
			i3 = intgrlimg1[hh][ww];

		hh = d.y;	ww = d.x - 1;
		if (hh < 0 || ww < 0)
			i4 = 0;
		else
			i4 = intgrlimg1[hh][ww];
		v4 = i1 + i2 - i3 - i4;

		if ((v1 + v4) >= (v2 + v3))	return true;
		else	return false;

	default:
		return false;
	}
}

void ImgCon::compute_haar_val()
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (i >= 5 &&
				j >= 5 &&
				i <= h - 6 &&
				j <= w - 6)
			{
				haar_val0[i][j][0] = haar0(cvPoint(j, i), 5, 1);
				haar_val0[i][j][1] = haar0(cvPoint(j, i), 5, 2);
				haar_val0[i][j][2] = haar0(cvPoint(j, i), 5, 3);
				haar_val0[i][j][3] = haar0(cvPoint(j, i), 7, 1);
				haar_val0[i][j][4] = haar0(cvPoint(j, i), 7, 2);
				haar_val0[i][j][5] = haar0(cvPoint(j, i), 7, 3);
				haar_val0[i][j][6] = haar0(cvPoint(j, i), 9, 1);
				haar_val0[i][j][7] = haar0(cvPoint(j, i), 9, 2);
				haar_val0[i][j][8] = haar0(cvPoint(j, i), 9, 3);
				haar_val0[i][j][9] = haar0(cvPoint(j, i), 11, 1);
				haar_val0[i][j][10] = haar0(cvPoint(j, i), 11, 2);
				haar_val0[i][j][11] = haar0(cvPoint(j, i), 11, 3);

				haar_val1[i][j][0] = haar1(cvPoint(j, i), 5, 1);
				haar_val1[i][j][1] = haar1(cvPoint(j, i), 5, 2);
				haar_val1[i][j][2] = haar1(cvPoint(j, i), 5, 3);
				haar_val1[i][j][3] = haar1(cvPoint(j, i), 7, 1);
				haar_val1[i][j][4] = haar1(cvPoint(j, i), 7, 2);
				haar_val1[i][j][5] = haar1(cvPoint(j, i), 7, 3);
				haar_val1[i][j][6] = haar1(cvPoint(j, i), 9, 1);
				haar_val1[i][j][7] = haar1(cvPoint(j, i), 9, 2);
				haar_val1[i][j][8] = haar1(cvPoint(j, i), 9, 3);
				haar_val1[i][j][9] = haar1(cvPoint(j, i), 11, 1);
				haar_val1[i][j][10] = haar1(cvPoint(j, i), 11, 2);
				haar_val1[i][j][11] = haar1(cvPoint(j, i), 11, 3);
			}
			else
			{
				haar_val0[i][j][0] = 0;
				haar_val0[i][j][1] = 0;
				haar_val0[i][j][2] = 0;
				haar_val0[i][j][3] = 0;
				haar_val0[i][j][4] = 0;
				haar_val0[i][j][5] = 0;
				haar_val0[i][j][6] = 0;
				haar_val0[i][j][7] = 0;
				haar_val0[i][j][8] = 0;
				haar_val0[i][j][9] = 0;
				haar_val0[i][j][10] =0;
				haar_val0[i][j][11] =0;

				haar_val1[i][j][0] = 0;
				haar_val1[i][j][1] = 0;
				haar_val1[i][j][2] = 0;
				haar_val1[i][j][3] = 0;
				haar_val1[i][j][4] = 0;
				haar_val1[i][j][5] = 0;
				haar_val1[i][j][6] = 0;
				haar_val1[i][j][7] = 0;
				haar_val1[i][j][8] = 0;
				haar_val1[i][j][9] = 0;
				haar_val1[i][j][10] =0;
				haar_val1[i][j][11] =0;
			}
		}
	}
}

cv::Mat ImgCon::compare_haar(int threshold, int threshold1)
{
	cv::Mat res(h, w, CV_8U, cv::Scalar(0));

	//匹配成功的特征的个数
	int count = 0;

	//对四个核设不同的阈值
	int t5, t7, t9, t11;
	t5 = threshold;
	t7 = (int)(49 * threshold / 25);
	t9 = (int)(81 * threshold / 25);
	t11 = (int)(121 * threshold / 25);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (abs(haar_val0[i][j][0] - haar_val1[i][j][0]) <= t5)		count++;
			if (abs(haar_val0[i][j][1] - haar_val1[i][j][1]) <= t5)		count++;
			if (abs(haar_val0[i][j][2] - haar_val1[i][j][2]) <= t5)		count++;

			if (abs(haar_val0[i][j][3] - haar_val1[i][j][3]) <= t7)		count++;
			if (abs(haar_val0[i][j][4] - haar_val1[i][j][4]) <= t7)		count++;
			if (abs(haar_val0[i][j][5] - haar_val1[i][j][5]) <= t7)		count++;

			if (abs(haar_val0[i][j][6] - haar_val1[i][j][6]) <= t9)		count++;
			if (abs(haar_val0[i][j][7] - haar_val1[i][j][7]) <= t9)		count++;
			if (abs(haar_val0[i][j][8] - haar_val1[i][j][8]) <= t9)		count++;

			if (abs(haar_val0[i][j][9] - haar_val1[i][j][9]) <= t11)		count++;
			if (abs(haar_val0[i][j][10] - haar_val1[i][j][10]) <= t11)		count++;
			if (abs(haar_val0[i][j][11] - haar_val1[i][j][11]) <= t11)		count++;

			if (count >= threshold1)
				res.at<uchar>(i, j) = 255;

			count = 0;
		}
	}

	return res;
}


//去除小点，核越大能去除的小点越大,核只能去基数
void clear_small_point(cv::Mat &m, int kernel)
{
	int h = m.size().height;
	int w = m.size().width;
	int s = (kernel - 1) / 2;
	for (int i = s; i <= h - s - 1; i++)
	{
		for (int j = s; j <= w - s - 1; j++)
		{
			; 
		}
	}

}

void ImgCon::arrange_cr(int threshold)
{
	std::vector<std::vector<CvPoint>> vs;

#define ImgCon_arrange_cr_V2

#ifdef ImgCon_arrange_cr_V1
	int group = 0;
	std::vector<std::pair<std::vector<CvPoint>, int>> vecs_pair;
	std::pair<std::vector<CvPoint>, int> pr;

	//先把所有区域组别设为0
	for (int i = 0; i < vecs.size(); i++)
	{
		pr.first = vecs[i];
		pr.second = group;
		vecs_pair.push_back(pr);
	}

	//开始分组
	group = 1;	//组别从1开始
	for (int i = 0; i < vecs_pair.size(); i++)
	{
		int dis = dist(cr_centre(vecs_pair[i].first), cr_centre(vecs_pair[i].first));

		if (vecs_pair[i].second)	break;	//已经被分过组的区域，跳出

		if (i != vecs_pair.size()-1)		//不是最后一个
		for (int j = i + 1; j < vecs_pair.size(); j++)
		{

		}
	}
#endif

#ifdef ImgCon_arrange_cr_V2

	//pair分别对应x/y坐标点以及标号
	std::vector<std::pair<int, int>> pair_x;
	std::vector<std::pair<int, int>> pair_y;
	std::pair<int, int> prx, pry;
	for (int i = 0; i < vecs.size(); i++)
	{
		prx.first = cr_centre(vecs[i]).x;
		pry.first = cr_centre(vecs[i]).y;
		prx.second = i;
		pry.second = i;
		pair_x.push_back(prx);
		pair_y.push_back(pry);
	}

	//按照x坐标y坐标排序
	std::sort(pair_x.begin(), pair_x.end());
	std::sort(pair_y.begin(), pair_y.end());

	//x，y重新分类，离得近的点保存的一类，分别存到r_x, r_y里面
	std::vector<std::vector<int>> r_x, r_y;
	for (int i = 0; i < pair_x.size(); i++)
	{

		if (dist(pair_x[i].first, pair_x[i + 1].first) <= threshold)
		{

		}
	}


	
#endif
}


CvPoint ImgCon::cr_centre(std::vector<CvPoint> ps)
{
	int size = ps.size();
	int p = size / 2;
	return ps[p];
}

int ImgCon::dist(CvPoint p1, CvPoint p2)
{
	int x_ = abs(p1.x - p2.x);
	int y_ = abs(p1.y - p2.y);
	return (int)(sqrt(pow(x_, 2) + pow(y_, 2)));
}

cv::Rect ImgCon::region2R(std::vector<CvPoint> ps)
{
	int minx = ps[0].x, miny = ps[0].y, maxx = ps[0].x, maxy = ps[0].y;
	for (int i = 0; i < ps.size(); i++)
	{
		if (ps[i].x < minx)
			minx = ps[i].x;
		if (ps[i].x > maxx)
			maxx = ps[i].x;
		if (ps[i].y < miny)
			miny = ps[i].y;
		if (ps[i].y > maxy)
			maxy = ps[i].y;
	}
	return cv::Rect(minx, miny, maxx - minx, maxy - miny);
}


//根据多个连通区域和得分图求rect和得分
void ImgCon::compute_sc(
	std::vector<std::vector<CvPoint>> rs,
	int **scmp_,
	std::vector<std::pair<cv::Rect, int>> &res)
{
	for (int i = 0; i < rs.size(); i++)
	{
		std::pair<cv::Rect, int> oneres;
		std::vector<CvPoint> quyu = rs[i];
		int sc = 0;
		for (int j = 0; j < quyu.size(); j++)
		{
			sc += scmp_[quyu[j].y][quyu[j].x];
		}
		sc = sc / quyu.size();	//得分求出了

		oneres.first = region2R(quyu);
		oneres.second = sc;
		res.push_back(oneres);
	}
}