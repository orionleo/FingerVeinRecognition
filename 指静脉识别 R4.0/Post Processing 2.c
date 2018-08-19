#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

IplImage* PostProcessing2(IplImage* src)
{
	// Ìî³äºÚÉ«¿Õ¶´
	// k_register = tianchong(k_register,30);
	IplImage* FilledImage = FillBlackHoles(src, 100);

	// ±ÕÔËËã
	// se2 = strel('disk',2);
	// k_register = imclose(k_register, se2);
	int Disk[5][5] = { { 0, 0, 1, 0, 0 }, 
					   { 0, 1, 1, 1, 0 }, 
					   { 1, 1, 1, 1, 1 }, 
					   { 0, 1, 1, 1, 0 }, 
					   { 0, 0, 1, 0, 0 } };
	IplConvKernel* Kernel_4 = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_CUSTOM, Disk);
	IplImage* ClosedImage = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvMorphologyEx(FilledImage, ClosedImage, NULL, Kernel_4, CV_MOP_CLOSE, 1);

	// ¹Ç÷À»¯
	// k_register = bwmorph(k_register,'thin',Inf);
	IplImage* ThinnedImage = cvThin(FilledImage, 10);

	// È¥³ý T ÐÍÈßÓàµã
	IplImage* AdvancedThinnedImage = RemoveTRedundantPoints(ThinnedImage);
	
	// ²Ã¼ô·ÖÖ§
	IplImage* CuttedImage = PruneBranches(AdvancedThinnedImage, 50);

	// ²Ã¼ô²¹³¥
	IplImage* RepairedImage = xiufu(AdvancedThinnedImage, CuttedImage, 8);

	return RepairedImage;
}