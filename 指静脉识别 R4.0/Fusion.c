#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

#define Height 160
#define Width 90

IplImage* Fusion(IplImage* src1, IplImage* src2, IplImage* src3)
{
	IplImage* FusionImage = cvCreateImage(cvSize(src1->width, src1->height), IPL_DEPTH_8U, 1);
	int k_register[Height][Width] = { 0 };
	int src1Array[Height][Width] = { 0 };
	int src2Array[Height][Width] = { 0 };
	int src3Array[Height][Width] = { 0 };
	for (int i = 0; i < Height; i++)
	{
		uchar* ptr1 = (uchar*)(src1->imageData + i * src1->widthStep);
		uchar* ptr2 = (uchar*)(src2->imageData + i * src2->widthStep);
		uchar* ptr3 = (uchar*)(src3->imageData + i * src3->widthStep);
		for (int j = 0; j < Width; j++)
		{
			src1Array[i][j] = ptr1[j];
			src2Array[i][j] = ptr2[j];
			src3Array[i][j] = ptr3[j];
		}
	}
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	int totalflag = 0;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			flag1 = 0;
			flag2 = 0;
			flag3 = 0;
			totalflag = 0;
			if (src1Array[i][j] > 0)
			{
				flag1 = 1;
			}
			if (src2Array[i][j] > 0)
			{
				flag2 = 1;
			}
			if (src3Array[i][j] > 0)
			{
				flag3 = 1;
			}
			totalflag = flag1 + flag2 + flag3;
			// if ((totalflag > 1) && (src1Array[i][j] + src2Array[i][j] + src3Array[i][j]) > 2)
			if (totalflag > 1)
			{
				k_register[i][j] = src1Array[i][j] + src2Array[i][j] + src3Array[i][j];
			}
		}
	}
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			CV_IMAGE_ELEM(FusionImage, uchar, i, j) = k_register[i][j];
		}
	}
	return FusionImage;
}