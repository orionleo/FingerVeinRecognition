#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

#ifndef _MYFUNCTION_H_
#define _MYFUNCTION_H_

void register_mode();
// PreProcessing 1
IplImage* RandomRotateImage(IplImage* src, double degree);
IplImage* GaussianFiltering(IplImage* src);
int CLAHE(uchar* pImage, unsigned int uiXRes, unsigned int uiYRes, uchar Min,
	uchar Max, unsigned int uiNrX, unsigned int uiNrY,
	unsigned int uiNrBins, float fCliplimit);
IplImage* TopLowTrans(IplImage* src);
IplImage* MedianFiltering(IplImage* src);
IplImage* SobelDetect(IplImage* src);
IplImage* MyLvbo(IplImage* src, int d1, int d2);
IplImage* ClosedOperation(IplImage* src);
IplImage* MyLvboo(IplImage* src, int d);
double FitMidLine(IplImage* src);
IplImage* HorizontalCalibration(IplImage* src, int degree);
IplImage* ClipInnerContent(IplImage* GrayImage, IplImage* BinaryImage);
IplImage* ClipBlackPart(IplImage* src, int ClipLength);

// PreProcessing 2
int* PreProcessing2(IplImage* img1, IplImage* img2, IplImage* img3);
int* tailor_fusion(int p1, int p2);
IplImage* Normalize_GrayScale(IplImage* src);
IplImage* Normalize_Size(IplImage* src, int height, int width);

// PreProcessing 3
IplImage* PreProcessing3(IplImage*img, int *ptr, int flag);

// PostProcessing 1
IplImage* FangXiangFenGe(IplImage* src, double alpha, double FusionPara1, double FusionPara2);

// Fusion
IplImage* Fusion(IplImage* src1, IplImage* src2, IplImage* src3);

// PostProcessing 2
IplImage* PostProcessing2(IplImage* src);
IplImage* FillBlackHoles(IplImage* src, int FillArea);
IplImage* cvThin(IplImage* src, int iterations);
IplImage* RemoveTRedundantPoints(IplImage* src);
IplImage* PruneBranches(IplImage* src, int BranchLength);
IplImage* xiufu(IplImage* y, IplImage* k, int caijianshu);

// ¹¤¾ßº¯Êý
IplImage* DisplayMultiImages(IplImage* Image1, IplImage* Image2, IplImage* Image3, IplImage* Image4);
IplImage* HorizontalDisplayImages(IplImage* Image1, IplImage* Image2, IplImage* Image3);
#endif