#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

#define	ONETWO(L, r, c, col) (L[(r) * (col) + c])

// 填充黑色空洞
IplImage* FillBlackHoles(IplImage* src, int FillArea)
{
	int Height = src->height;
	int Width = src->width;

	for (int y = 0; y < Height; y++)
	{
		uchar* ptr = (uchar*)(src->imageData + y * src->widthStep);
		for (int x = 0; x < Width; x++)
		{
			if (ptr[x] == 0)
			{
				ptr[x] = 1;
			}
			else if (ptr[x] == 1)
			{
				ptr[x] = 0;
			}
		}
	}

	int Points[500][2] = { 0 };
	int nums = 0;
	int ConnectedDomainNums = 0;
	for (int i = 0; i < 3; i++)
	{
		int* labels = (int*)malloc(sizeof(int)* (src->height * src->widthStep));
		ConnectedDomainNums = bwlabel(src, 4, labels);
		for (int k = 1; k <= ConnectedDomainNums; k++)
		{
			memset(Points, 0, sizeof(Points));
			nums = 0;
			for (int y = 0; y < Height; y++)
			{
				for (int x = 0; x < Width; x++)
				{
					if (ONETWO(labels, y, x, Width) == k)
					{
						Points[nums][0] = y;
						Points[nums][1] = x;
						nums++;
					}
				}
				if (nums > 200)
				{
					break;
				}
			}
			if (nums < FillArea)
			{
				for (int m = 0; m < nums; m++)
				{
					CV_IMAGE_ELEM(src, uchar, Points[m][0], Points[m][1]) = 0;
				}
			}
		}
	}

	for (int y = 0; y < Height; y++)
	{
		uchar* ptc = (uchar*)(src->imageData + y * src->widthStep);
		for (int x = 0; x < Width; x++)
		{
			if (ptc[x] == 0)
			{
				ptc[x] = 1;
			}
			else if (ptc[x] == 1)
			{
				ptc[x] = 0;
			}
		}
	}

	return src;
}

// 骨骼化
IplImage* cvThin(IplImage* src, int iterations)
{
	int Height = src->height;
	int Width = src->width;
	cvThreshold(src, src, 0, 1, CV_THRESH_BINARY);
	IplImage* dst = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	cvCopy(src, dst, NULL);
	for (int n = 0; n < iterations; n++)
	{
		IplImage* t_image = cvCloneImage(dst);
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				if (CV_IMAGE_ELEM(t_image, uchar, i, j) != 0)
				{
					int ap = 0;
					int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i - 1, j);
					int p3 = (i == 0 || j == Width - 1) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i - 1, j + 1);
					if (p2 == 0 && p3 != 0)
					{
						ap++;
					}
					int p4 = (j == Width - 1) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i, j + 1);
					if (p3 == 0 && p4 != 0)
					{
						ap++;
					}
					int p5 = (i == Height - 1 || j == Width - 1) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i + 1, j + 1);
					if (p4 == 0 && p5 != 0)
					{
						ap++;
					}
					int p6 = (i == Height - 1) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i + 1, j);
					if (p5 == 0 && p6 != 0)
					{
						ap++;
					}
					int p7 = (i == Height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i + 1, j - 1);
					if (p6 == 0 && p7 != 0)
					{
						ap++;
					}
					int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i, j - 1);
					if (p7 == 0 && p8 != 0)
					{
						ap++;
					}
					int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i - 1, j - 1);
					if (p8 == 0 && p9 != 0)
					{
						ap++;
					}
					if (p9 == 0 && p2 != 0)
					{
						ap++;
					}
					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)>1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)<7)
					{
						if (ap == 1)
						{
							if (!(p2 && p4 && p6))
							{
								if (!(p4 && p6 && p8))
								{
									CV_IMAGE_ELEM(dst, uchar, i, j) = 0;
								}
							}
						}
					}

				}
			}
		}
		cvReleaseImage(&t_image);
		t_image = cvCloneImage(dst);
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				if (CV_IMAGE_ELEM(t_image, uchar, i, j) != 0)
				{
					int ap = 0;
					int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i - 1, j);
					int p3 = (i == 0 || j == Width - 1) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i - 1, j + 1);
					if (p2 == 0 && p3 != 0)
					{
						ap++;
					}
					int p4 = (j == Width - 1) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i, j + 1);
					if (p3 == 0 && p4 != 0)
					{
						ap++;
					}
					int p5 = (i == Height - 1 || j == Width - 1) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i + 1, j + 1);
					if (p4 == 0 && p5 != 0)
					{
						ap++;
					}
					int p6 = (i == Height - 1) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i + 1, j);
					if (p5 == 0 && p6 != 0)
					{
						ap++;
					}
					int p7 = (i == Height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i + 1, j - 1);
					if (p6 == 0 && p7 != 0)
					{
						ap++;
					}
					int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i, j - 1);
					if (p7 == 0 && p8 != 0)
					{
						ap++;
					}
					int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(t_image, uchar, i - 1, j - 1);
					if (p8 == 0 && p9 != 0)
					{
						ap++;
					}
					if (p9 == 0 && p2 != 0)
					{
						ap++;
					}
					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)>1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)<7)
					{
						if (ap == 1)
						{
							if (p2*p4*p8 == 0)
							{
								if (p2*p6*p8 == 0)
								{
									CV_IMAGE_ELEM(dst, uchar, i, j) = 0;
								}
							}
						}
					}
				}
			}
		}
		cvReleaseImage(&t_image);
	}
	return dst;
}

// 去除 T 型冗余点
IplImage* RemoveTRedundantPoints(IplImage* src)
{
	int RTRP_SubFunction(int ExtendedArray[160 + 2][90 + 2], int moban[3][3], int i, int j);

	int Height = src->height;
	int Width = src->width;
	IplImage* RemovedImage = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	cvCopy(src, RemovedImage, 0);
	int ExtendedArray[160 + 2][90 + 2] = { 0 };

	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			ExtendedArray[i + 1][j + 1] = CV_IMAGE_ELEM(RemovedImage, uchar, i, j);
		}
	}

	int moban111[3][3] = { { 0, 1, 0 },
						   { 1, 1, 0 },
	                       { 0, 1, 0 } };

	int moban222[3][3] = { { 0, 1, 0 },
						   { 0, 1, 1 },
						   { 0, 1, 0 } };

	int moban333[3][3] = { { 0, 1, 0 },
						   { 1, 1, 1 },
						   { 0, 0, 0 } };

	int moban444[3][3] = { { 0, 0, 0 },
						   { 1, 1, 1 },
						   { 0, 1, 0 } };

	for (int i = 1; i < Height + 1; i++)
	{
		for (int j = 1; j < Width + 1; j++)
		{
			if (ExtendedArray[i][j] != 0 && 
				RTRP_SubFunction(ExtendedArray, moban111, i, j) == 4 ||
				RTRP_SubFunction(ExtendedArray, moban222, i, j) == 4 ||
				RTRP_SubFunction(ExtendedArray, moban333, i, j) == 4 ||
				RTRP_SubFunction(ExtendedArray, moban444, i, j) == 4)
			{
				CV_IMAGE_ELEM(RemovedImage, uchar, i - 1, j - 1) = 0;
			}
		}
	}
	return RemovedImage;
}
// 去除 T 型冗余点子函数
int RTRP_SubFunction(int ExtendedArray[160 + 2][90 + 2], int moban[3][3], int i, int j)
{
	int nums = 0;
	int TempArray[3][3] = { 0 };
	for (int m = i - 1; m <= i + 1; m++)
	{
		for (int n = j - 1; n <= j + 1; n++)
		{
			if (ExtendedArray[m][n] * moban[m - (i - 1)][n - (j - 1)] == 1)
			{
				nums++;
			}
		}
	}
	return nums;
}

// 裁剪分支
IplImage* PruneBranches(IplImage* src, int BranchLength)
{
	int PB_SubFunction(int Array[160 + 2][90 + 2], int i, int j);

	int Height = src->height;
	int Width = src->width;
	IplImage* PrunedImage = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, 1);
	cvCopy(src, PrunedImage, 0);
	int hh[160 + 2][90 + 2] = { 0 };
	int canzhao[160 + 2][90 + 2] = { 0 };

	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			hh[i + 1][j + 1] = CV_IMAGE_ELEM(PrunedImage, uchar, i, j);
			canzhao[i + 1][j + 1] = CV_IMAGE_ELEM(PrunedImage, uchar, i, j);
		}
	}

	for (int k = 1; k <= BranchLength; k++)
	{
		for (int i = 1; i < Height + 1; i++)
		{
			for (int j = 1; j < Width + 1; j++)
			{
				if (hh[i][j] != 0 && PB_SubFunction(hh, i, j) <= 2 && PB_SubFunction(canzhao, i, j) < 4)
				{
					CV_IMAGE_ELEM(PrunedImage, uchar, i - 1, j - 1) = 0;
				}
			}
		}
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				hh[i + 1][j + 1] = CV_IMAGE_ELEM(PrunedImage, uchar, i, j);
			}
		}
	}
	return PrunedImage;
}
// 裁剪分支子函数
int PB_SubFunction(int Array[160 + 2][90 + 2], int i, int j)
{
	int nums = 0;
	for (int m = i - 1; m <= i + 1; m++)
	{
		for (int n = j - 1; n <= j + 1; n++)
		{
			if (Array[m][n] == 1)
			{
				nums++;
			}
		}
	}
	return nums;
}

// 裁剪修复函数
IplImage* xiufu(IplImage* y, IplImage * k, int caijianshu)
{
	//对裁剪后的图像(过裁剪现象)进行修复，y 去除冗余点后的图像， k为过裁剪后的图像,caijianshu ：修复阈值
	//返回修复后的图像
	IplImage * dst = cvCreateImage(cvSize(90, 160), 8, 1);
	int nr = dst->height;
	int nc = dst->width;
	cvSub(y, k, dst, NULL);				//dst：y-k
	int * label = (int *)malloc(sizeof(int)*(dst->height*dst->widthStep));
	int num = bwlabel(dst, 8, label);
	int i, j, r, c;
	for (j = 1; j <= 8; j++){
		for (i = 1; i <= num; i++){
			int point[100][2] = { 0 };		//存放每个连通域上的点
			int x = 0;							//记录每个连通域上点的个数
			for (r = 0; r < nr; r++){
				for (c = 0; c < nc; c++){
					if (ONETWO(label, r, c, nc) == i) {
						point[x][0] = r;
						point[x][1] = c;
						x++;
					}
				}
			}
			if (x < caijianshu){
				int m;
				for (m = 0; m < x; m++)
				{
					CV_IMAGE_ELEM(dst, uchar, point[m][0], point[m][1]) = 0;
				}
			}
		}
	}
	IplImage* dst1 = cvCreateImage(cvSize(90, 160), 8, 1);
	cvAdd(k, dst, dst1, NULL);
	return dst1;
}