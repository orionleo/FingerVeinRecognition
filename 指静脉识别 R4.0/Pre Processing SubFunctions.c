#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

#define ONETWO(L, r, c, col) (L[(r) * (col) + c])

/*
PreProcessing 1
*/
// ��תͼ��
IplImage* RandomRotateImage(IplImage* src, double degree)
{
	int Height = src->height;
	int Width = src->width;
	double angle = degree * CV_PI / 180;
	double a = sin(angle);
	double b = cos(angle);
	int RotatedHeight = (int)(Width * fabs(a) + Height * fabs(b));
	int RotatedWidth = (int)(Height * fabs(a) + Width * fabs(b));
	float map[6];
	CvMat map_matrix = cvMat(2, 3, CV_32F, map);
	CvPoint2D32f center = cvPoint2D32f(Width / 2, Height / 2);
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix);
	map[2] += (RotatedWidth - Width) / 2;
	map[5] += (RotatedHeight - Height) / 2;
	IplImage* RotatedImage = cvCreateImage(cvSize(RotatedWidth, RotatedHeight), IPL_DEPTH_8U, 1);
	cvWarpAffine(src, RotatedImage, &map_matrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
	return RotatedImage;
}

// ��˹�˲�
IplImage* GaussianFiltering(IplImage* src)
{
	IplImage* GaussianFilteredImage = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvSmooth(src, GaussianFilteredImage, CV_GAUSSIAN, 5, 5, 1, 0);
	return GaussianFilteredImage;
}

// �ߡ���ñ�任
IplImage* TopLowTrans(IplImage* src)
{
	int Height = src->height;
	int Width = src->width;

	// �Զ�����̬��
	int MyDisk[29][29];
	for (int i = 0; i < 29; i++)
	{
		for (int j = 0; j < 29; j++)
		{
			MyDisk[i][j] = 1;
		}
	}
	int LowNums = 8;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < LowNums; j++)
		{
			MyDisk[i][j] = 0;
		}
		for (int j = 28; j > 28 - LowNums; j--)
		{
			MyDisk[i][j] = 0;
		}
		LowNums--;
	}
	int UpNums = 1;
	for (int i = 21; i < 29; i++)
	{
		for (int j = 0; j < UpNums; j++)
		{
			MyDisk[i][j] = 0;
		}
		for (int j = 28; j > 28 - UpNums; j--)
		{
			MyDisk[i][j] = 0;
		}
		UpNums++;
	}
	IplConvKernel* Kernel_1 = cvCreateStructuringElementEx(29, 29, 14, 14, CV_SHAPE_CUSTOM, MyDisk);

	// ��ñ
	IplImage* TopHat = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	cvMorphologyEx(src, TopHat, NULL, Kernel_1, CV_MOP_TOPHAT, 2);
	// ��ñ
	IplImage* BotHat = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	cvMorphologyEx(src, BotHat, NULL, Kernel_1, CV_MOP_BLACKHAT, 2);
	// ��ñ + CLAHEImage
	IplImage* AddedImage = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	cvAdd(TopHat, src, AddedImage, 0);
	// addImage - ��ñ
	IplImage* DstImage = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	cvSub(AddedImage, BotHat, DstImage, 0);

	return DstImage;
}

// ��ֵ�˲�
IplImage* MedianFiltering(IplImage* src)
{
	IplImage* MedianFilteredImage = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvSmooth(src, MedianFilteredImage, CV_MEDIAN, 3, 3, 0, 0);
	return MedianFilteredImage;
}

// Sobel ��Ե���
IplImage* SobelDetect(IplImage* src)
{
	IplImage* TempImage = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_16S, 1);
	IplImage* DetectedImage = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvSobel(src, TempImage, 2, 0, 5);
	cvConvert(TempImage, DetectedImage);
	return DetectedImage;
}

// ������
IplImage* ClosedOperation(IplImage* src)
{
	IplConvKernel* Kernel_2 = cvCreateStructuringElementEx(1, 11, 0, 5, CV_SHAPE_RECT, NULL);
	IplImage* OperatedImage = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvMorphologyEx(src, OperatedImage, NULL, Kernel_2, CV_MOP_CLOSE, 1);
	return OperatedImage;
}

// ȥ��С��������Ӧ MALTAB �еĺ��� lvbo(h1, d1, d2)��
IplImage* MyLvbo(IplImage* src, int d1, int d2)
{
	int Height = src->height;
	int Width = src->width;
	int points[4000][2] = { 0 };
	int pointnums = 0;
	int maxr = NULL;
	int minr = NULL;

	for (int i = 0; i < 5; i++)
	{
		int* labels = (int*)malloc(sizeof(int)* (src->height * src->widthStep));
		int ConnectedDomainNums = bwlabel(src, 4, labels);
		for (int j = 1; j <= ConnectedDomainNums; j++)
		{
			memset(points, 0, sizeof(points));
			pointnums = 0;
			for (int x = 0; x < Height; x++)
			{
				for (int y = 0; y < Width; y++)
				{
					if (ONETWO(labels, x, y, Width) == j)
					{
						points[pointnums][0] = x;
						points[pointnums][1] = y;
						pointnums++;
					}
				}
			}
			maxr = points[0][0];
			minr = points[0][0];
			for (int x = 1; x < pointnums; x++)
			{
				if (points[x][0] > maxr)
				{
					maxr = points[x][0];
				}
				if (points[x][0] < minr && points[x][0] != 0)
				{
					minr = points[x][0];
				}
			}
			if (pointnums < d1 || (maxr - minr) < d2)
			{
				for (int x = 0; x < pointnums; x++)
				{
					CV_IMAGE_ELEM(src, uchar, points[x][0], points[x][1]) = 0;
				}
			}
		}
	}
	return src;
}

// ȥ������������Ӧ MALTAB �еĺ��� lvboo(h1, d)��
IplImage* MyLvboo(IplImage* src, int d)
{
	int Height = src->height;
	int Width = src->width;
	int	ConnectedDomainNums = 0;
	int points[2000][2] = { 0 };
	int pointnums = 0;
	int maxr = NULL;
	int minr = NULL;

	for (int i = 0; i < 15; i++)
	{
		int* labels = (int*)malloc(sizeof(int)* (src->height * src->widthStep));
		ConnectedDomainNums = bwlabel(src, 8, labels);
		for (int j = 1; j <= ConnectedDomainNums; j++)
		{
			memset(points, 0, sizeof(points));
			pointnums = 0;
			for (int x = 0; x < Height; x++)
			{
				for (int y = 0; y < Width; y++)
				{
					if (ONETWO(labels, x, y, Width) == j)
					{
						points[pointnums][0] = x;
						points[pointnums][1] = y;
						pointnums++;
					}
				}
			}
			maxr = points[0][0];
			minr = points[0][0];
			for (int x = 1; x < pointnums; x++)
			{
				if (points[x][0] > maxr)
				{
					maxr = points[x][0];
				}
				if (points[x][0] < minr && points[x][0] != 0)
				{
					minr = points[x][0];
				}
			}
			if ((maxr - minr) < d)
			{
				for (int x = 0; x < pointnums; x++)
				{
					CV_IMAGE_ELEM(src, uchar, points[x][0], points[x][1]) = 0;
				}
			}
		}
	}
	return src;
}

// �������
double FitMidLine(IplImage* src)
{
	int Height = src->height;
	int Width = src->width;
	cvThreshold(src, src, 0, 1, CV_THRESH_BINARY);

	int t[2000] = { 0 };
	int count = 0;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (CV_IMAGE_ELEM(src, uchar, i, j) == 1)
			{
				t[count++] = j;
			}
		}
	}
	int max = t[0];
	for (int k = 1; k < count; k++)
	{
		if (t[k] > max)
		{
			max = t[k];
		}
	}
	int min = t[0];
	for (int k = 1; k < count; k++)
	{
		if (t[k] < min)
		{
			min = t[k];
		}
	}
	if (max < 121 || min > 120)
	{
		if (max < 121)
		{
			for (int i = 0; i < Height; i++)
			{
				CV_IMAGE_ELEM(src, uchar, i, Width - 1) == 1;
			}
		}
		if (min > 120)
		{
			for (int i = 0; i < Height; i++)
			{
				CV_IMAGE_ELEM(src, uchar, i, 0) == 1;
			}
		}
	}

	int leftLine[320] = { 0 };
	int rightLine[320] = { 0 };
	double centerLine[320] = { 0 };

	int num = 0;
	int num1 = 0;
	int num2 = 0;
	int t1 = 0;
	int t2 = 0;
	int temporary[240] = { 0 };
	int c[120] = { 0 };
	int d[120] = { 0 };
	int t_count = 0;
	int c_count = 0;
	int d_count = 0;

	for (int i = 70; i < Height; i++)
	{
		t1 = 0;
		t2 = 0;
		memset(temporary, 0, sizeof(temporary));
		memset(c, 0, sizeof(c));
		memset(d, 0, sizeof(d));
		t_count = 0;
		c_count = 0;
		d_count = 0;
		for (int j = 0; j < Width; j++)
		{
			temporary[t_count++] = CV_IMAGE_ELEM(src, uchar, i, j);
		}
		for (int k = 0; k < Width / 2; k++)
		{
			if (temporary[k] == 1)
			{
				c[c_count++] = k;
			}
		}
		for (int k = Width / 2; k < Width; k++)
		{
			if (temporary[k] == 1)
			{
				d[d_count++] = k;
			}
		}

		int c_max = c[0];
		int c_loc = 0;
		while (c_count > 0)
		{
			c_max = c[0];
			c_loc = 0;
			for (int k = 1; k < 120; k++)
			{
				if (c[k] > c_max)
				{
					c_max = c[k];
					c_loc = k;
				}
			}
			c[c_loc] = 0;
			c_count--;
			if (num1 == 0)
			{
				leftLine[num1++] = c_max;
				t1 = c_max;
				break;
			}
			else if (c_max - leftLine[num1] < 6 || c_max - leftLine[num1] > -6)
			{
				leftLine[num1++] = c_max;
				t1 = c_max;
				break;
			}
		}

		int d_min = d[0];
		int d_loc = 0;
		while (d_count > 0)
		{
			d_min = d[0];
			d_loc = 0;
			for (int k = 1; k < 120; k++)
			{
				if (d[k] < d_min && d[k] != 0)
				{
					d_min = d[k];
					d_loc = k;
				}
			}
			d[d_loc] = 0;
			d_count--;
			if (num2 == 0)
			{
				rightLine[num2++] = d_min;
				t2 = d_min;
				break;
			}
			else if (d_min - rightLine[num2] < 6 || d_min - rightLine[num2] > -6)
			{
				rightLine[num2++] = d_min;
				t2 = d_min;
				break;
			}
		}

		if (t1 != 0 && t2 != 0)
		{
			centerLine[num++] = (t1 + t2) / 2;
		}
	}

	CvPoint* points = (CvPoint*)malloc(num * sizeof(points[0]));
	CvMat pointMat = cvMat(1, num, CV_32SC2, points);
	float line[4];
	int p_count = 0;
	for (int i = 0; i < num; i++)
	{
		if (centerLine[i] != 0){
			points[p_count].x = i;
			points[p_count].y = centerLine[i];
			p_count++;
		}
	}
	cvFitLine(&pointMat, CV_DIST_L1, 1, 0.001, 0.001, line);
	float k1 = line[1] / line[0];
	float alpha = atan(k1) * (180 / CV_PI);
	return alpha;
}

// ˮƽУ��
IplImage* HorizontalCalibration(IplImage* src, int degree)
{
	int Height = src->height;
	int Width = src->width;
	IplImage* RotatedImage = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	CvPoint2D32f center;
	center.x = (float)(Width / 2.0 + 0.5);
	center.y = (float)(Height / 2.0 + 0.5);
	float m[6];
	CvMat M = cvMat(2, 3, CV_32F, m);
	cv2DRotationMatrix(center, degree, 1, &M);
	cvWarpAffine(src, RotatedImage, &M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
	return RotatedImage;
}

// ���б߲ü�
IplImage* ClipInnerContent(IplImage* GrayImage, IplImage* BinaryImage)
{
	int Height = BinaryImage->height;
	int Width = BinaryImage->width;

	/*
	�˹��ܿ����ڼ����ָ���ұ߽��Ƿ���ͼ���Ե�غ�
	*/
	int* labels = (int*)malloc(sizeof(int)* (BinaryImage->height * BinaryImage->widthStep));
	int ConnectedDomainNums = bwlabel(BinaryImage, 8, labels);

	int maxx = 239;
	int minn = 0;
	int tempArray[2000] = { 0 };
	int count = 0;
	for (int y = 10; y < Height - 10; y++)
	{
		uchar* imagePointer = (uchar*)(BinaryImage->imageData + y * BinaryImage->widthStep);
		for (int x = 0; x < Width; x++)
		{
			if (imagePointer[x] == 255)
			{
				tempArray[count++] = x;
			}
		}
	}
	int temp_MAX = tempArray[0];
	int temp_MIN = tempArray[0];
	for (int k = 0; k < count; k++)
	{
		if (tempArray[k] > temp_MAX)
		{
			temp_MAX = tempArray[k];
		}
		if (tempArray[k] < temp_MIN)
		{
			temp_MIN = tempArray[k];
		}
	}

	/*
	�˹��ܿ��Ƕ����������������������Ӧ����벿�� MAX �� �Ұ벿�� MIN
	*/
	if (temp_MAX > 121 && temp_MIN < 120) // ��ָ���ұ߽�����ڵ����
	{
		int Points[1000][2] = { 0 };
		int points_count = 0;
		int row_MAX = Points[0][0];
		int row_MIN = Points[0][0];
		int col_MAX = Points[0][1];
		int col_MIN = Points[0][1];
		for (int i = 1; i <= ConnectedDomainNums; i++)
		{
			memset(Points, 0, sizeof(Points));
			points_count = 0;
			for (int y = 10; y < Height - 10; y++)
			{
				for (int x = 0; x < Width; x++)
				{
					if (ONETWO(labels, y, x, Width) == i)
					{
						Points[points_count][0] = y;
						Points[points_count][1] = x;
						points_count++;
					}
				}
			}
			row_MAX = Points[0][0];
			row_MIN = Points[0][0];
			col_MAX = Points[0][1];
			col_MIN = Points[0][1];
			for (int k = 1; k < points_count; k++)
			{
				if (Points[k][0] > row_MAX)
				{
					row_MAX = Points[k][0];
				}
				if (Points[k][0] < row_MIN)
				{
					row_MIN = Points[k][0];
				}
				if (Points[k][1] > col_MAX)
				{
					col_MAX = Points[k][1];
				}
				if (Points[k][1] < col_MIN)
				{
					col_MIN = Points[k][1];
				}
			}
			if (row_MAX - row_MIN > 20) // ȥ��������ͨ�򣬱�������������ָ�߽�
			{
				if (col_MAX < 91) // ����� col_MAX С�� 91�����ж���ǰ����ͨ��Ϊ��ָ��߽�
				{
					if (minn < col_MAX)
					{
						minn = col_MAX;
					}
				}
				if (col_MAX > 151) // ����� col_MAX ���� 151�����ж���ǰ����ͨ��Ϊ��ָ�ұ߽�
				{
					if (maxx > col_MIN)
					{
						maxx = col_MIN;
					}
				}
			}
		}
	}
	else if (temp_MAX < 121 || temp_MIN > 120)
	{
		if (temp_MAX < 121)
		{
			maxx = 239;
			int LeftPoints[300][2] = { 0 };
			int left_count = 0;
			int left_row_MAX = LeftPoints[0][0];
			int left_row_MIN = LeftPoints[0][0];
			int left_col_MAX = LeftPoints[0][1];
			int left_col_MIN = LeftPoints[0][1];
			for (int i = 1; i <= ConnectedDomainNums; i++)
			{
				memset(LeftPoints, 0, sizeof(LeftPoints));
				left_count = 0;
				for (int y = 10; y < Height - 10; y++)
				{
					for (int x = 0; x < Width / 2; x++)
					{
						if (ONETWO(labels, y, x, Width) == i)
						{
							LeftPoints[left_count][0] = y;
							LeftPoints[left_count][1] = x;
							left_count++;
						}
					}
				}
				left_row_MAX = LeftPoints[0][0];
				left_row_MIN = LeftPoints[0][0];
				left_col_MAX = LeftPoints[0][1];
				left_col_MIN = LeftPoints[0][1];
				for (int k = 1; k < left_count; k++)
				{
					if (LeftPoints[k][0] > left_row_MAX)
					{
						left_row_MAX = LeftPoints[k][0];
					}
					if (LeftPoints[k][0] < left_row_MIN)
					{
						left_row_MIN = LeftPoints[k][0];
					}
					if (LeftPoints[k][1] > left_col_MAX)
					{
						left_col_MAX = LeftPoints[k][1];
					}
					if (LeftPoints[k][1] < left_col_MIN)
					{
						left_col_MIN = LeftPoints[k][1];
					}
				}
				if (left_row_MAX - left_row_MIN > 20)
				{
					if (left_col_MAX > minn)
					{
						minn = left_col_MAX;
					}
				}
			}
		}
		else if (temp_MIN > 120)
		{
			minn = 0;
			int RightPoints[300][2] = { 0 };
			int right_count = 0;
			int right_row_MAX = RightPoints[0][0];
			int right_row_MIN = RightPoints[0][0];
			int right_col_MAX = RightPoints[0][1];
			int right_col_MIN = RightPoints[0][1];
			for (int i = 1; i <= ConnectedDomainNums; i++)
			{
				memset(RightPoints, 0, sizeof(RightPoints));
				right_count = 0;
				for (int y = 10; y < Height - 10; y++)
				{
					for (int x = 0; x < Width / 2; x++)
					{
						if (ONETWO(labels, y, x, Width) == i)
						{
							RightPoints[right_count][0] = y;
							RightPoints[right_count][1] = x;
							right_count++;
						}
					}
				}
				right_row_MAX = RightPoints[0][0];
				right_row_MIN = RightPoints[0][0];
				right_col_MAX = RightPoints[0][1];
				right_col_MIN = RightPoints[0][1];
				for (int k = 1; k < right_count; k++)
				{
					if (RightPoints[k][0] > right_row_MAX)
					{
						right_row_MAX = RightPoints[k][0];
					}
					if (RightPoints[k][0] < right_row_MIN)
					{
						right_row_MIN = RightPoints[k][0];
					}
					if (RightPoints[k][1] > right_col_MAX)
					{
						right_col_MAX = RightPoints[k][1];
					}
					if (RightPoints[k][1] < right_col_MIN)
					{
						right_col_MIN = RightPoints[k][1];
					}
				}
				if (right_row_MAX - right_row_MIN > 20)
				{
					if (right_col_MIN < maxx)
					{
						maxx = right_col_MIN;
					}
				}
			}
		}
	}

	cvSetImageROI(GrayImage, cvRect(minn, 0, maxx - minn, Height));
	IplImage* dstImage = cvCreateImage(cvSize(maxx - minn, Height), IPL_DEPTH_8U, 1);
	cvCopy(GrayImage, dstImage, 0);
	return dstImage;
}

// ���б߲ü��󣬲ü����ºڱ�
IplImage* ClipBlackPart(IplImage* src, int ClipLength)
{
	int Height = src->height;
	int Width = src->width;
	
	int DstHeight = Height - 2 * ClipLength;
	IplImage* DstImage = cvCreateImage(cvSize(Width, DstHeight), IPL_DEPTH_8U, 1);
	cvSetImageROI(src, cvRect(0, ClipLength, Width, DstHeight));
	cvCopy(src, DstImage, 0);
	return DstImage;
}



/*
PreProcessing 2
*/
// �ü��ںϣ�����У��
int* tailor_fusion(int p1, int p2)
{
	//p1,p2�ֱ�Ϊimg2��img3�����img1���µ�ƫ��λ�ã�
	//��������img1,img2,img3����ʼ���������ü����ĳ��ȡ�
	int tailor[6] = { 0 };
	//��һ�������img2��img3������ƫ��
	if (p1 < 0 && p2 < 0){
		//img1����ʼ�������ü�����
		tailor[0] = 0;
		tailor[1] = max(abs(p1), abs(p2));
		//img2����ʼ�������ü�����
		if (p1 < p2){
			tailor[2] = abs(p1);
			tailor[3] = tailor[1];
		}
		else
		{
			tailor[2] = abs(p1);
			tailor[3] = tailor[1];
		}
		//img3����ʼ�������ü�����
		if (p1 < p2){
			tailor[4] = abs(p2);
			tailor[5] = tailor[1];
		}
		else
		{
			tailor[4] = abs(p2);
			tailor[5] = tailor[1];
		}
	}
	//�ڶ��������img2,img3������ƫ��
	else if (p1 > 0 && p2 > 0){
		//img1����ʼ�������ü��߶�
		tailor[0] = max(p1, p2);
		tailor[1] = tailor[0];
		//img2����ʼ�������ü��߶�
		if (p1 > p2){
			tailor[2] = 0;
			tailor[3] = tailor[0];
		}
		else
		{
			tailor[2] = p2 - p1;
			tailor[3] = tailor[0];
		}
		//img3����ʼ�������ü��߶�
		if (p1 > p2){
			tailor[4] = p1 - p2;
			tailor[5] = tailor[0];

		}
		else
		{
			tailor[4] = 0;
			tailor[5] = tailor[0];
		}
	}
	//�����������img2����ƫ�ƣ�img3����ƫ��
	else if (p1 <= 0 && p2 >= 0){
		tailor[0] = p2;
		tailor[1] = abs(p1) + p2;
		tailor[2] = abs(p1) + p2;
		tailor[3] = abs(p1) + p2;
		tailor[4] = 0;
		tailor[5] = abs(p1) + p2;
	}
	//�����������img2�����ƣ�img3������
	else if (p1 >= 0 && p2 <= 0){
		tailor[0] = p1;
		tailor[1] = abs(p2) + p1;
		tailor[2] = 0;
		tailor[3] = abs(p2) + p1;
		tailor[4] = abs(p2) + p1;
		tailor[5] = abs(p2) + p1;
	}
	return tailor;
}

// �Ҷȹ�һ��
IplImage* Normalize_GrayScale(IplImage* src)
{
	IplImage* Normalized_GrayScale_dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	double min_Val, Max_Val;
	double scale;
	double shift;
	cvMinMaxLoc(src, &min_Val, &Max_Val, NULL, NULL, NULL);
	scale = 255 / (Max_Val - min_Val);
	shift = -min_Val * scale;
	cvConvertScale(src, Normalized_GrayScale_dst, scale, shift);
	return Normalized_GrayScale_dst;
}

// �ߴ��һ��
IplImage* Normalize_Size(IplImage* src, int height, int width)
{
	int Standard_Height = height;
	int Standard_Width = width;
	IplImage* Normalized_Size_dst = cvCreateImage(cvSize(Standard_Width, Standard_Height), IPL_DEPTH_8U, 1);
	cvResize(src, Normalized_Size_dst, CV_INTER_LINEAR);
	return Normalized_Size_dst;
}