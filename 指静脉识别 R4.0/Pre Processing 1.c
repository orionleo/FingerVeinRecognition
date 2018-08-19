#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

IplImage* PreProcessing1(IplImage* src)
{	
	// ��ת90��
	IplImage* RotatedImage = RandomRotateImage(src, 90);

	// ��˹�˲�
	IplImage* GaussianImage = GaussianFiltering(RotatedImage);

	// CLAHE
	IplImage* CLAHEImage = CLAHEMain(GaussianImage);

	// �ߡ���ñ�任
	IplImage* TopBotHatImage = TopLowTrans(CLAHEImage);
	
	// ��ֵ�˲�
	IplImage* MedianImage = MedianFiltering(TopBotHatImage);

	// Sobel ��Ե���
	IplImage* SobelImage = SobelDetect(CLAHEImage);
	cvThreshold(SobelImage, SobelImage, 254, 255, CV_THRESH_BINARY);

	// lvbo
	IplImage* lvboImage = MyLvbo(SobelImage, 10, 30);
	
	// ������
	IplImage* ClosedImage = ClosedOperation(lvboImage);

	// ������
	IplImage* SkeletonizedImage = cvThin(ClosedImage, 10);
	cvThreshold(SkeletonizedImage, SkeletonizedImage, 0, 255, CV_THRESH_BINARY);
	
	// lvboo
	IplImage* lvbooImage = MyLvboo(SkeletonizedImage, 30);

	// ˮƽУ��
	double angle = FitMidLine(lvbooImage);
	IplImage* HorizontalGrayImage = HorizontalCalibration(MedianImage, -angle);
	IplImage* HorizontalBinaryImage = HorizontalCalibration(lvbooImage, -angle);
	cvThreshold(lvbooImage, lvbooImage, 0, 255, CV_THRESH_BINARY);
	cvThreshold(HorizontalBinaryImage, HorizontalBinaryImage, 0, 255, CV_THRESH_BINARY);
	
	IplImage* test = DisplayMultiImages(MedianImage, lvbooImage, HorizontalGrayImage, HorizontalBinaryImage);
	cvShowImage("test", test);

	// ���б߲ü�
	IplImage* ClippedImage = ClipInnerContent(HorizontalGrayImage, HorizontalBinaryImage);
	
	// ˮƽУ���󣬲ü����ºڱ�
	IplImage* SecondClippedImage = ClipBlackPart(ClippedImage, 15);

	// ����������ͼ��Ĵ�С��һ�������ں������ԣ���Ϊ��ָƫ�ƽǶȲ�һ�����ü��ĺڱߵĴ�С��һ����
	IplImage* Image = cvCreateImage(cvSize(170, 300), IPL_DEPTH_8U, 1);
	cvResize(SecondClippedImage, Image, CV_INTER_LINEAR);

	return Image;
}