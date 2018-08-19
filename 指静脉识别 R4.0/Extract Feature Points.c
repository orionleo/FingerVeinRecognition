#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

int* ExtractFeaturePoints(IplImage* src)
{
	int* juzhen2arr(int ImageArray[160 + 2][90 + 2], int i, int j);

	int Height = src->height;
	int Width = src->width;
	int kk[160 + 2][90 + 2] = { 0 };
	int ImageArray[160 + 2][90 + 2] = { 0 };

	for (int i = 0; i < Height; i++)
	{
		uchar* ptr = (uchar*)(src->imageData + i * src->widthStep);
		for (int j = 0; j < Width; j++)
		{
			ImageArray[i + 1][j + 1] = ptr[j];
		}
	}

	int* shunxu = 0;
	int dian = 0;
	int sum = 0;
	for (int i = 1; i < Height + 1; i++)
	{
		for (int j = 1; j < Width + 1; j++)
		{
			shunxu = juzhen2arr(ImageArray, i, j);
			dian = *(shunxu + 8);
			sum = 0;
			for (int k = 0; k <= 7; k++)
			{
				if (k == 7)
				{
					*(shunxu + 8) = *(shunxu);
				}
				sum = sum + abs((*(shunxu + k + 1)) - (*(shunxu + k)));
			}
			if (dian != 0 && ((sum == 2) || (sum >= 6)))
			{
				kk[i][j] = 1;
			}
		}
	}
	
	int FeaturePoints[100][2] = { 0 };
	int PointNums = 0;
	for (int i = 1; i < Height + 1; i++)
	{
		for (int j = 1; j < Width + 1; j++)
		{
			if (kk[i][j] == 1)
			{
				FeaturePoints[PointNums][0] = i - 1;
				FeaturePoints[PointNums][1] = j - 1;
				PointNums++;
			}
		}
	}
	
	return FeaturePoints;
}

int* juzhen2arr(int ImageArray[160 + 2][90 + 2], int i, int j)
{
	int p[9] = { 1 };
	int x = 0;
	for (int m = i - 1; m <= i + 1; m++)
	{
		for (int n = j - 1; n <= j + 1; n++)
		{
			p[x++] = ImageArray[m][n];
		}
	}
	static int biao[9] = { 1 };
	biao[0] = p[0];
	biao[1] = p[1];
	biao[2] = p[2];
	biao[7] = p[3];
	biao[8] = p[4];
	biao[3] = p[5];
	biao[6] = p[6];
	biao[5] = p[7];
	biao[4] = p[8];
	return biao;
}