#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

IplImage* PreProcessing1(IplImage* src)
{	
	// 旋转90度
	IplImage* RotatedImage = RandomRotateImage(src, 90);

	// 高斯滤波
	IplImage* GaussianImage = GaussianFiltering(RotatedImage);

	// CLAHE
	IplImage* CLAHEImage = CLAHEMain(GaussianImage);

	// 高、低帽变换
	IplImage* TopBotHatImage = TopLowTrans(CLAHEImage);
	
	// 中值滤波
	IplImage* MedianImage = MedianFiltering(TopBotHatImage);

	// Sobel 边缘检测
	IplImage* SobelImage = SobelDetect(CLAHEImage);
	cvThreshold(SobelImage, SobelImage, 254, 255, CV_THRESH_BINARY);

	// lvbo
	IplImage* lvboImage = MyLvbo(SobelImage, 10, 30);
	
	// 闭运算
	IplImage* ClosedImage = ClosedOperation(lvboImage);

	// 骨骼化
	IplImage* SkeletonizedImage = cvThin(ClosedImage, 10);
	cvThreshold(SkeletonizedImage, SkeletonizedImage, 0, 255, CV_THRESH_BINARY);
	
	// lvboo
	IplImage* lvbooImage = MyLvboo(SkeletonizedImage, 30);

	// 水平校正
	double angle = FitMidLine(lvbooImage);
	IplImage* HorizontalGrayImage = HorizontalCalibration(MedianImage, -angle);
	IplImage* HorizontalBinaryImage = HorizontalCalibration(lvbooImage, -angle);
	cvThreshold(lvbooImage, lvbooImage, 0, 255, CV_THRESH_BINARY);
	cvThreshold(HorizontalBinaryImage, HorizontalBinaryImage, 0, 255, CV_THRESH_BINARY);
	
	IplImage* test = DisplayMultiImages(MedianImage, lvbooImage, HorizontalGrayImage, HorizontalBinaryImage);
	cvShowImage("test", test);

	// 内切边裁剪
	IplImage* ClippedImage = ClipInnerContent(HorizontalGrayImage, HorizontalBinaryImage);
	
	// 水平校正后，裁剪上下黑边
	IplImage* SecondClippedImage = ClipBlackPart(ClippedImage, 15);

	// 将三幅输入图像的大小归一化，用于后面的配对（因为手指偏移角度不一样，裁剪的黑边的大小不一样）
	IplImage* Image = cvCreateImage(cvSize(170, 300), IPL_DEPTH_8U, 1);
	cvResize(SecondClippedImage, Image, CV_INTER_LINEAR);

	return Image;
}