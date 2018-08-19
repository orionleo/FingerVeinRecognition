#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

#define Height 300
#define Width 170
#define resize_updown 20
#define resize_leftright 10
#define matchCut_updown 15
#define matchCut_leftright 10

int* MyLocation(IplImage* Image1, IplImage* Image2)
{
	// 声明子函数
	// void InitializeArray(int Image1TempArray[Height][Width], int Image2TempArray[Height][Width]);
	double SubArray_CalculateVariance(int Image1TempArray[Height][Width], int Image2TempArray[Height][Width], int tempArray[Height][Width]);

	// 将图像转换成对应的二维数组
	int Image1Array[Height][Width] = { 0 };
	int Image2Array[Height][Width] = { 0 };
	for (int i = 0; i < Height; i++)
	{
		uchar* ptr1 = (uchar*)(Image1->imageData + i * Image1->widthStep);
		uchar* ptr2 = (uchar*)(Image2->imageData + i * Image2->widthStep);
		for (int j = 0; j < Width; j++)
		{
			Image1Array[i][j] = ptr1[j];
			Image2Array[i][j] = ptr2[j];
		}
	}

	// 变量和数组
	int Image1TempArray[Height][Width] = { 0 };
	int Image2TempArray[Height][Width] = { 0 };
	int tempArray[Height][Width] = { 0 };
	double differences[2 * resize_updown + 1][2 * resize_leftright + 1] = { 0 };
	double variance = 0;

	// 图像 2 向上平移
	for (int i = -1 * resize_updown; i <= -1; i++)
	{
		// <左>
		for (int j = -1 * resize_leftright; j <= -1; j++)
		{
			// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
			memset(Image1TempArray, 0, sizeof(Image1TempArray));
			memset(Image2TempArray, 0, sizeof(Image2TempArray));
			memset(tempArray, 0, sizeof(tempArray));
			for (int m = matchCut_updown; m < Height + i - matchCut_updown; m++)
			{
				for (int n = matchCut_leftright; n < Width + j - matchCut_leftright; n++)
				{
					Image1TempArray[m - matchCut_updown][n - matchCut_leftright] = Image1Array[m][n];
				}
			}
			for (int m = -i + matchCut_updown; m < Height - matchCut_updown; m++)
			{
				for (int n = -j + matchCut_leftright; n < Width - matchCut_leftright; n++)
				{
					Image2TempArray[m - (-i + matchCut_updown)][n - (-j + matchCut_leftright)] = Image2Array[m][n];
				}
			}
			variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
			differences[resize_updown + i][resize_leftright + j] = variance;
		}
		// <中>
		// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
		memset(Image1TempArray, 0, sizeof(Image1TempArray));
		memset(Image2TempArray, 0, sizeof(Image2TempArray));
		memset(tempArray, 0, sizeof(tempArray));
		for (int m = matchCut_updown; m < Height + i - matchCut_updown; m++)
		{
			for (int n = matchCut_leftright; n < Width - matchCut_leftright; n++)
			{
				Image1TempArray[m - matchCut_updown][n - matchCut_leftright] = Image1Array[m][n];
			}
		}
		for (int m = -i + matchCut_updown; m < Height - matchCut_updown; m++)
		{
			for (int n = matchCut_leftright; n < Width - matchCut_leftright; n++)
			{
				Image2TempArray[m - (-i + matchCut_updown)][n - matchCut_leftright] = Image2Array[m][n];
			}
		}
		variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
		differences[resize_updown + i][resize_leftright] = variance;
		// <右>
		for (int j = 1; j <= resize_leftright; j++)
		{
			// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
			memset(Image1TempArray, 0, sizeof(Image1TempArray));
			memset(Image2TempArray, 0, sizeof(Image2TempArray));
			memset(tempArray, 0, sizeof(tempArray));
			for (int m = matchCut_updown; m < Height + i - matchCut_updown; m++)
			{
				for (int n = j + matchCut_leftright; n < Width - matchCut_leftright; n++)
				{
					Image1TempArray[m - matchCut_updown][n - (j + matchCut_leftright)] = Image1Array[m][n];
				}
			}
			for (int m = -i + matchCut_updown; m < Height - matchCut_updown; m++)
			{
				for (int n = matchCut_leftright; n < Width - j - matchCut_leftright; n++)
				{
					Image2TempArray[m - (-i + matchCut_updown)][n - matchCut_leftright] = Image2Array[m][n];
				}
			}
			variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
			differences[resize_updown + i][resize_leftright + j] = variance;
		}
	}

	// 图像 2 不向上、下平移
	// <左>
	for (int j = -1 * resize_leftright; j <= -1; j++)
	{
		// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
		memset(Image1TempArray, 0, sizeof(Image1TempArray));
		memset(Image2TempArray, 0, sizeof(Image2TempArray));
		memset(tempArray, 0, sizeof(tempArray));
		for (int m = matchCut_updown; m < Height - matchCut_updown; m++)
		{
			for (int n = matchCut_leftright; n < Width + j - matchCut_leftright; n++)
			{
				Image1TempArray[m - matchCut_updown][n - matchCut_leftright] = Image1Array[m][n];
			}
		}
		for (int m = matchCut_updown; m < Height - matchCut_updown; m++)
		{
			for (int n = -j + matchCut_leftright; n < Width - matchCut_leftright; n++)
			{
				Image2TempArray[m - matchCut_updown][n - (-j + matchCut_leftright)] = Image2Array[m][n];
			}
		}
		variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
		differences[resize_updown][resize_leftright + j] = variance;
	}
	// <中>
	// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
	memset(Image1TempArray, 0, sizeof(Image1TempArray));
	memset(Image2TempArray, 0, sizeof(Image2TempArray));
	memset(tempArray, 0, sizeof(tempArray));
	for (int m = matchCut_updown; m < Height - matchCut_updown; m++)
	{
		for (int n = matchCut_leftright; n < Width - matchCut_leftright; n++)
		{
			Image1TempArray[m - matchCut_updown][n - matchCut_leftright] = Image1Array[m][n];
			Image2TempArray[m - matchCut_updown][n - matchCut_leftright] = Image2Array[m][n];
		}
	}
	variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
	differences[resize_updown][resize_leftright] = variance;
	// <右>
	for (int j = 1; j <= resize_leftright; j++)
	{
		// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
		memset(Image1TempArray, 0, sizeof(Image1TempArray));
		memset(Image2TempArray, 0, sizeof(Image2TempArray));
		memset(tempArray, 0, sizeof(tempArray));
		for (int m = matchCut_updown; m < Height - matchCut_updown; m++)
		{
			for (int n = j + matchCut_leftright; n < Width - matchCut_leftright; n++)
			{
				Image1TempArray[m - matchCut_updown][n - (j + matchCut_leftright)] = Image1Array[m][n];
			}
		}
		for (int m = matchCut_updown; m < Height - matchCut_updown; m++)
		{
			for (int n = matchCut_leftright; n < Width - j - matchCut_leftright; n++)
			{
				Image2TempArray[m - matchCut_updown][n - matchCut_leftright] = Image2Array[m][n];
			}
		}
		variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
		differences[resize_updown][resize_leftright + j] = variance;
	}

	// 图像 2 向下平移
	for (int i = 1; i <= resize_updown; i++)
	{
		// <左>
		for (int j = -1 * resize_leftright; j <= -1; j++)
		{
			// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
			memset(Image1TempArray, 0, sizeof(Image1TempArray));
			memset(Image2TempArray, 0, sizeof(Image2TempArray));
			memset(tempArray, 0, sizeof(tempArray));
			for (int m = i + matchCut_updown; m < Height - matchCut_updown; m++)
			{
				for (int n = matchCut_leftright; n < Width + j - matchCut_leftright; n++)
				{
					Image1TempArray[m - (i + matchCut_updown)][n - matchCut_leftright] = Image1Array[m][n];
				}
			}
			for (int m = matchCut_updown; m < Height - i - matchCut_updown; m++)
			{
				for (int n = -j + matchCut_leftright; n < Width - matchCut_leftright; n++)
				{
					Image2TempArray[m - matchCut_updown][n - (-j + matchCut_leftright)] = Image2Array[m][n];
				}
			}
			variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
			differences[resize_updown + i][resize_leftright + j] = variance;
		}
		// <中>
		// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
		memset(Image1TempArray, 0, sizeof(Image1TempArray));
		memset(Image2TempArray, 0, sizeof(Image2TempArray));
		memset(tempArray, 0, sizeof(tempArray));
		for (int m = i + matchCut_updown; m < Height - matchCut_updown; m++)
		{
			for (int n = matchCut_leftright; n < Width - matchCut_leftright; n++)
			{
				Image1TempArray[m - (i + matchCut_updown)][n - matchCut_leftright] = Image1Array[m][n];
			}
		}
		for (int m = matchCut_updown; m < Height - i - matchCut_updown; m++)
		{
			for (int n = matchCut_leftright; n < Width - matchCut_leftright; n++)
			{
				Image2TempArray[m - matchCut_updown][n - matchCut_leftright] = Image2Array[m][n];
			}
		}
		variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
		differences[resize_updown + i][resize_leftright] = variance;
		// <右>
		for (int j = 1; j <= resize_leftright; j++)
		{
			// InitializeArray(Image1TempArray, Image2TempArray, tempArray);
			memset(Image1TempArray, 0, sizeof(Image1TempArray));
			memset(Image2TempArray, 0, sizeof(Image2TempArray));
			memset(tempArray, 0, sizeof(tempArray));
			for (int m = i + matchCut_updown; m < Height - matchCut_updown; m++)
			{
				for (int n = j + matchCut_leftright; n < Width - matchCut_leftright; n++)
				{
					Image1TempArray[m - (i + matchCut_updown)][n - (j + matchCut_leftright)] = Image1Array[m][n];
				}
			}
			for (int m = matchCut_updown; m < Height - i - matchCut_updown; m++)
			{
				for (int n = matchCut_leftright; n < Width - j - matchCut_leftright; n++)
				{
					Image2TempArray[m - matchCut_updown][n - matchCut_leftright] = Image2Array[m][n];
				}
			}
			variance = SubArray_CalculateVariance(Image1TempArray, Image2TempArray, tempArray);
			differences[resize_updown + i][resize_leftright + j] = variance;
		}
	}

	double minValue = differences[0][0];
	int minX = 0;
	int minY = 0;
	for (int i = 0; i < 2 * resize_updown + 1; i++)
	{
		for (int j = 0; j < 2 * resize_leftright + 1; j++)
		{
			if (differences[i][j] < minValue)
			{
				minValue = differences[i][j];
				minX = i;
				minY = j;
			}
		}
	}
	int loc[2] = { 0 };
	loc[0] = minX - 20;
	loc[1] = minY - 10;

	return loc;
}

// 子函数：计算方差
double SubArray_CalculateVariance(int Image1TempArray[Height][Width], int Image2TempArray[Height][Width], int tempArray[Height][Width])
{
	double tempSum = 0;
	double tempMean = 0;
	double tempSquare = 0;
	double tempVariance = 0;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			tempArray[i][j] = Image1TempArray[i][j] - Image2TempArray[i][j];
			tempSum = tempSum + tempArray[i][j];
		}
	}
	tempMean = tempSum / (Height * Width);
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			tempSquare = tempSquare + pow((tempArray[i][j] - tempMean), 2);
		}
	}
	tempVariance = tempSquare / (Height * Width);
	return tempVariance;
}