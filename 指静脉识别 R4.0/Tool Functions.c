#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

/*
在一个窗口内显示、对比多张图像
*/
IplImage* DisplayMultiImages(IplImage* Image1, IplImage* Image2, IplImage* Image3, IplImage* Image4)
{
	int Height = Image1->height;
	int Width = Image1->width;
	int DstHeight = Height * 2;
	int DstWidth = Width * 2;
	IplImage* DstImage = cvCreateImage(cvSize(DstWidth + 4, DstHeight + 4), IPL_DEPTH_8U, 1);

	
	cvSetImageROI(DstImage, cvRect(0, 0, Width, Height));
	cvCopy(Image1, DstImage, 0);
	cvResetImageROI(DstImage);

	cvSetImageROI(DstImage, cvRect(Width + 4, 0, Width, Height));
	cvCopy(Image2, DstImage, 0);
	cvResetImageROI(DstImage);

	cvSetImageROI(DstImage, cvRect(0, Height + 4, Width, Height));
	cvCopy(Image3, DstImage, 0);
	cvResetImageROI(DstImage);

	cvSetImageROI(DstImage, cvRect(Width + 4, Height + 4, Width, Height));
	cvCopy(Image4, DstImage, 0);
	cvResetImageROI(DstImage);

	return DstImage;
}

/*
横向显示三张图像
*/
IplImage* HorizontalDisplayImages(IplImage* Image1, IplImage* Image2, IplImage* Image3)
{
	int ImagesWidth = Image1->width;
	int ImagesHeight = Image1->height;
	IplImage* Images = cvCreateImage(cvSize(3 * ImagesWidth + 10, ImagesHeight), IPL_DEPTH_8U, 1);

	// Image1
	cvSetImageROI(Images, cvRect(0, 0, ImagesWidth, ImagesHeight));
	cvCopy(Image1, Images, 0);
	cvResetImageROI(Images);
	// Image2
	cvSetImageROI(Images, cvRect(ImagesWidth + 5, 0, ImagesWidth, ImagesHeight));
	cvCopy(Image2, Images, 0);
	cvResetImageROI(Images);
	// Image3
	cvSetImageROI(Images, cvRect(ImagesWidth * 2 + 10, 0, ImagesWidth, ImagesHeight));
	cvCopy(Image3, Images, 0);
	cvResetImageROI(Images);

	return Images;
}