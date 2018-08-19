#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "MyFunction.h"

void register_mode()
{
	// �������ž���ͼ��
	IplImage* Image1 = cvLoadImage("C:\\Users\\orion\\Desktop\\ָ����ʶ����Ŀ\\ָ������ͼ20171228\\601\\601_1.jpg", CV_LOAD_IMAGE_UNCHANGED);
	IplImage* Image2 = cvLoadImage("C:\\Users\\orion\\Desktop\\ָ����ʶ����Ŀ\\ָ������ͼ20171228\\601\\601_2.jpg", CV_LOAD_IMAGE_UNCHANGED);
	IplImage* Image3 = cvLoadImage("C:\\Users\\orion\\Desktop\\ָ����ʶ����Ŀ\\ָ������ͼ20171228\\601\\601_3.jpg", CV_LOAD_IMAGE_UNCHANGED);

	// Ԥ����1 ��170 * 300��
	IplImage* norm1 = PreProcessing1(Image1);
	IplImage* norm2 = PreProcessing1(Image2);
    IplImage* norm3 = PreProcessing1(Image3);

	// Ԥ����2����ֱУ����׼����
	int* tailor = PreProcessing2(norm1, norm2, norm3);

	// Ԥ����3��������׼�����ü�ͼ�񣬲���һ���ߴ絽 ��90 * 160��
	IplImage* k1 = PreProcessing3(norm1, tailor, 1);
	IplImage* k2 = PreProcessing3(norm2, tailor, 2);
	IplImage* k3 = PreProcessing3(norm3, tailor, 3);

	// ����1��������ηָ�
	IplImage* k11 = FangXiangFenGe(k1, 0.7, 0, 0);
	IplImage* k21 = FangXiangFenGe(k2, 0.7, 0, 0);
	IplImage* k31 = FangXiangFenGe(k3, 0.7, 0, 0);

	// �����ž�������ָ���ͼ���ںϳ�һ��ע��ͼ��
	IplImage* RegisterImage = Fusion(k11, k21, k31);

	// ����2��ͼ���޸�
	IplImage* FinalImage = PostProcessing2(RegisterImage);

	IplImage* Display1 = HorizontalDisplayImages(k1, k2, k3);
	cvShowImage("  k1  k2  k3  ", Display1);

	cvThreshold(k11, k11, 0, 255, CV_THRESH_BINARY);
	cvThreshold(k21, k21, 0, 255, CV_THRESH_BINARY);
	cvThreshold(k31, k31, 0, 255, CV_THRESH_BINARY);
	cvThreshold(RegisterImage, RegisterImage, 0, 255, CV_THRESH_BINARY);
	cvThreshold(FinalImage, FinalImage, 0, 255, CV_THRESH_BINARY);
	IplImage* Display2 = HorizontalDisplayImages(k11, k21, k31);
	cvShowImage("  k11  k21  k31  ", Display2);
	cvShowImage(" RegisterImage ", RegisterImage);
	cvShowImage(" FinalImage ", FinalImage);
}