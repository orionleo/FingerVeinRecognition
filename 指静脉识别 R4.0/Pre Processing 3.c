#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

#define Height 160
#define Width 90

IplImage* PreProcessing3(IplImage* img, int* ptr, int flag)
{
	int tail[12] = {0};
	for (int i = 0; i < 12; i++){
		tail[i] = *(ptr + i);
	}
	int height = img->height;
	int width = img->width;
	if (flag == 1){
		IplImage * dst = cvCreateImage(cvSize(width-tail[7],height-tail[1]),8,1);
		cvSetImageROI(img, cvRect(tail[6], tail[0], width - tail[7], height - tail[1]));
		cvCopy(img, dst, 0);
		IplImage * dst_norm = Normalize_Size(dst, Height, Width);
		return dst_norm;
	}
	else if (flag == 2){
		IplImage * dst = cvCreateImage(cvSize(width - tail[9], height - tail[3]), 8, 1);
		cvSetImageROI(img, cvRect(tail[8], tail[2], width - tail[9], height - tail[3]));
		cvCopy(img, dst, 0);
		IplImage * dst_norm = Normalize_Size(dst, Height, Width);
		return dst_norm;
	}
	else 
	{
		IplImage * dst = cvCreateImage(cvSize(width - tail[11], height - tail[5]), 8, 1);
		cvSetImageROI(img, cvRect(tail[10], tail[4], width - tail[11], height - tail[5]));
		cvCopy(img, dst, 0);
		IplImage * dst_norm = Normalize_Size(dst, Height, Width);
		return dst_norm;
	}
}