#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

int* PreProcessing2(IplImage* Image1, IplImage* Image2, IplImage* Image3)
{
	// ¼ì²âÍ¼ÏñÆ«ÒÆÇé¿ö
	int* ptr1 = MyLocation(Image1, Image2);
	int p1 = *ptr1, q1 = *(ptr1 + 1);
	int* ptr2 = MyLocation(Image1, Image3);
	int p2 = *ptr2, q2 = *(ptr2 + 1);

	//printf("p1: %d  q1:  %d  \np2: %d  q2: %d\n", p1, q1, p2, q2);

	int* tailor1 = NULL;
	int* tailor2 = NULL;
	static int tailor[12] = { 0 };
	tailor1 = tailor_fusion(p1, p2);
	for (int i = 0; i < 6; i++)
	{
		tailor[i] = *(tailor1 + i);
	}
	tailor2 = tailor_fusion(q1, q2);
	for (int i = 6; i < 12; i++)
	{
		tailor[i] = *(tailor2 + i - 6);
	}

	return tailor;
}