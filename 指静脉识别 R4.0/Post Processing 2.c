#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

IplImage* PostProcessing2(IplImage* src)
{
	// ����ɫ�ն�
	// k_register = tianchong(k_register,30);
	IplImage* FilledImage = FillBlackHoles(src, 100);

	// ������
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

	// ������
	// k_register = bwmorph(k_register,'thin',Inf);
	IplImage* ThinnedImage = cvThin(FilledImage, 10);

	// ȥ�� T �������
	IplImage* AdvancedThinnedImage = RemoveTRedundantPoints(ThinnedImage);
	
	// �ü���֧
	IplImage* CuttedImage = PruneBranches(AdvancedThinnedImage, 50);

	// �ü�����
	IplImage* RepairedImage = xiufu(AdvancedThinnedImage, CuttedImage, 8);

	return RepairedImage;
}