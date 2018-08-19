#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

IplImage* FangXiangFenGe(IplImage* src, double alpha, double FusionPara1, double FusionPara2)
{
	int Height = src->height;
	int Width = src->width;

	int tt = 4;
	int kuozhantu[160 + 2 * 4][90 + 2 * 4] = { 0 };
	int kongjuzhen1[160][90] = { 0 };
	int kongjuzhen11[160][90] = { 0 };
	int kongjuzhen2[160][90] = { 0 };
	int kongjuzhen3[160][90] = { 0 };
	int average_pixel_block[8] = { 0 };
	double sum_pixel = 0;
	double average_pixel = 0;
	for (int y = 0; y < Height; y++)
	{
		uchar* ptr = (uchar*)(src->imageData + y * src->widthStep);
		for (int x = 0; x < Width; x++)
		{
			kuozhantu[y + tt][x + tt] = ptr[x];
			sum_pixel += ptr[x];
		}
	}
	average_pixel = sum_pixel / (Height * Width);
	for (int k = 0; k < 8; k++)
	{
		sum_pixel = 0;
		for (int y = 20 * k; y < 20 * (k + 1); y++)
		{
			for (int x = 0; x < Width; x++)
			{
				sum_pixel += CV_IMAGE_ELEM(src, uchar, y, x);
			}
		}
		average_pixel_block[k] = sum_pixel / (Height * Width / 8);
	}

	double weight_direction[8] = { 1, 0.8, 0.6, 0.4, 0.2, 0.4, 0.6, 0.8 };
	int block = 0;
	int temp_matrix[9][9] = { 0 };
	int temp_templet[8][5] = { 0 };
	double temp_result[8] = { 0 };
	double maxx = 0;
	int temp_d = 0;
	for (int y = tt; y < Height + tt; y++)
	{
		block = cvFloor((y - tt + 20) / 20) - 1;
		for (int x = tt; x < Width + tt; x++)
		{
			// 遍历整张图像，对每个像素点取其 9*9 的邻域内的所有点
			for (int m = y - tt; m <= y + tt; m++)
			{
				for (int n = x - tt; n <= x + tt; n++)
				{
					temp_matrix[m - (y - tt)][n - (x - tt)] = kuozhantu[m][n];
				}
			}
			// 第一个模板
			temp_templet[0][0] = temp_matrix[4][0];
			temp_templet[0][1] = temp_matrix[4][2];
			temp_templet[0][2] = temp_matrix[4][4];
			temp_templet[0][3] = temp_matrix[4][6];
			temp_templet[0][4] = temp_matrix[4][8];
			// 第二个模板
			temp_templet[1][0] = temp_matrix[2][0];
			temp_templet[1][1] = temp_matrix[3][2];
			temp_templet[1][2] = temp_matrix[4][4];
			temp_templet[1][3] = temp_matrix[5][6];
			temp_templet[1][4] = temp_matrix[6][8];
			// 第三个模板
			temp_templet[2][0] = temp_matrix[0][0];
			temp_templet[2][1] = temp_matrix[2][2];
			temp_templet[2][2] = temp_matrix[4][4];
			temp_templet[2][3] = temp_matrix[6][6];
			temp_templet[2][4] = temp_matrix[8][8];
			// 第四个模板
			temp_templet[3][0] = temp_matrix[0][2];
			temp_templet[3][1] = temp_matrix[2][3];
			temp_templet[3][2] = temp_matrix[4][4];
			temp_templet[3][3] = temp_matrix[6][5];
			temp_templet[3][4] = temp_matrix[8][6];
			// 第五个模板
			temp_templet[4][0] = temp_matrix[0][4];
			temp_templet[4][1] = temp_matrix[2][4];
			temp_templet[4][2] = temp_matrix[4][4];
			temp_templet[4][3] = temp_matrix[6][4];
			temp_templet[4][4] = temp_matrix[8][4];
			// 第六个模板
			temp_templet[5][0] = temp_matrix[0][6];
			temp_templet[5][1] = temp_matrix[2][5];
			temp_templet[5][2] = temp_matrix[4][4];
			temp_templet[5][3] = temp_matrix[6][3];
			temp_templet[5][4] = temp_matrix[8][2];
			// 第七个模板
			temp_templet[6][0] = temp_matrix[0][8];
			temp_templet[6][1] = temp_matrix[2][6];
			temp_templet[6][2] = temp_matrix[4][4];
			temp_templet[6][3] = temp_matrix[6][2];
			temp_templet[6][4] = temp_matrix[8][0];
			// 第八个模板
			temp_templet[7][0] = temp_matrix[2][8];
			temp_templet[7][1] = temp_matrix[3][6];
			temp_templet[7][2] = temp_matrix[4][4];
			temp_templet[7][3] = temp_matrix[5][2];
			temp_templet[7][4] = temp_matrix[6][0];
			// 对数组 temp_templet[8][5] 每一行的各个元素乘上对应的系数存入新数组 temp_result 中
			for (int k = 0; k < 8; k++)
			{
				temp_result[k] = weight_direction[k] * ((3 * temp_templet[k][0] + (-1) * temp_templet[k][1] + (-4) * temp_templet[k][2] + (-1) * temp_templet[k][3] + 3 * temp_templet[k][4]) / 7);
			}
			while (1)
			{
				maxx = temp_result[0];
				temp_d = 0;
				for (int k = 1; k < 8; k++)
				{
					if (temp_result[k] > maxx)
					{
						maxx = temp_result[k];
						temp_d = k;
					}
				}
				temp_result[temp_d] = 0;
				if ((maxx > 0) && (temp_templet[temp_d][0] > temp_templet[temp_d][2]) && (temp_templet[temp_d][4] > temp_templet[temp_d][2]) && (kuozhantu[y][x] < average_pixel_block[block] * 1))
				{
					kongjuzhen1[y - tt][x - tt] = maxx;
					break;
				}
				if (maxx <= 0)
				{
					break;
				}
			}
		}
	}

	/*
	全局像素均值和全局阈值
	*/
	double GlobalSum = 0;
	double GlobalAverage = 0;
	int GlobalLength = 0;
	double GlobalThreshold = 0;
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			GlobalSum += kongjuzhen1[y][x];
			if (kongjuzhen1[y][x] > 0)
			{
				GlobalLength++;
			}
		}
	}
	GlobalAverage = GlobalSum / GlobalLength;
	GlobalThreshold = GlobalAverage * alpha;

	/*
	局部像素均值和局部阈值
	*/
	double SubSum = 0;
	double SubAverage[8] = { 0 };
	int SubLength = 0;
	double SubThreshold[8] = { 0 };
	for (int k = 0; k < 8; k++)
	{
		SubSum = 0;
		SubLength = 0;
		for (int y = 20 * k; y < 20 * (k + 1); y++)
		{
			for (int x = 0; x < Width; x++)
			{
				SubSum += kongjuzhen1[y][x];
				if (kongjuzhen1[y][x] > 0)
				{
					SubLength++;
				}
			}
		}
		SubAverage[k] = SubSum / SubLength;
		SubThreshold[k] = GlobalThreshold * (SubAverage[k] / GlobalAverage);
	}
	
	/*
	阈值分割
	*/
	int count = 0;
	double Threshold = 0;
	for (int y = 0; y < Height; y++)
	{
		count = cvFloor((y + 20) / 20) - 1;
		Threshold = SubThreshold[count] * 0.8;
		for (int x = 0; x < Width; x++)
		{
			if (kongjuzhen1[y][x] > Threshold)
			{
				kongjuzhen11[y][x] = 1;
			}
			if (kongjuzhen1[y][x] > Threshold * 2)
			{
				kongjuzhen2[y][x] = 1;
			}
			if (kongjuzhen1[y][x] > Threshold * 3)
			{
				kongjuzhen3[y][x] = 1;
			}
		}
	}

	/*
	将二维数组 kongjuzhen11 转换成图像，并进行闭运算、中值滤波、小噪声去除和填充处理 
	*/
	IplImage* kongjuzhen11Image = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			CV_IMAGE_ELEM(kongjuzhen11Image, uchar, y, x) = kongjuzhen11[y][x];
		}
	}

	// 闭运算 Step1
	// strel('disk',3)
	IplImage* Step1Image = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	IplConvKernel* Kernel_3 = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT, NULL);
	cvMorphologyEx(kongjuzhen11Image, Step1Image, NULL, Kernel_3, CV_MOP_CLOSE, 1);
	// 中值滤波 Step2
	IplImage* Step2Image = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	cvSmooth(Step1Image, Step2Image, CV_MEDIAN, 3, 3, 0, 0);	
	// 小噪声去除 Step3
	IplImage* Step3Image = MyLvbo(Step2Image, 30, 10);
	// 填充黑色空洞 Step4
	IplImage* Step4Image = FillBlackHoles(Step3Image, 100);

	// Step4Image + kongjuzhen2[160][90] + kongjuzhen3[160][90]
	IplImage* DstImage = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			CV_IMAGE_ELEM(DstImage, uchar, y, x) = CV_IMAGE_ELEM(Step4Image, uchar, y, x) + FusionPara1 * kongjuzhen2[y][x] + FusionPara2 * kongjuzhen3[y][x];
		}
	}

	return DstImage;
}