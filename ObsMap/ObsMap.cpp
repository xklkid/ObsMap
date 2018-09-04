#include "ObsMap.h"

int mapGlobal[ROWS][COLS] = { 0 };
//int se[WIDTH][WIDTH] = { { -1, -1, 1 }, { -1, 1, -1 }, { 1, -1, -1 } };
int se[WIDTH][WIDTH]{{ -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
{ -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1 },
{ -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1 },
{ -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1 },
{ -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, 
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1 },
{ -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1 },
{ -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1 },
{ -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1 },
{ -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 }, };

void erosion()
{
	int mapErode[ROWS][COLS] = { 0 };
	//memcpy(mapErode, mapGlobal, sizeof(mapGlobal));
	bool bMatch = true;

	for (int i = WIDTHHALF; i < ROWS - WIDTHHALF; i++)
	{
		for (int j = WIDTHHALF; j < COLS - WIDTHHALF; j++)
		{
			bMatch = true;
			if (mapGlobal[i][j] != 1)
			{
				continue;
			}
			for (int m = 0; m < WIDTH; m++)
			{
				for (int n = 0; n < WIDTH; n++)
				{
					if (se[m][n] == -1)
					{
						continue;
					}
					if (se[m][n] == 1)		// 前景
					{
						if (mapGlobal[i - WIDTHHALF + m][j - WIDTHHALF + n] != 1)
						{
							bMatch = false;
							break;
						}
					}
					else
					{
						return;
					}
				}
				if (bMatch == false)
				{
					break;
				}
			}

			if (bMatch)
			{
				mapErode[i][j] = 1;
			}
		}
	}
	memcpy(mapGlobal, mapErode, sizeof(mapErode));
}

void dilation()
{
	int mapDilate[ROWS][COLS] = { 0 };
	memcpy(mapDilate, mapGlobal, sizeof(mapGlobal));

	for (int i = WIDTHHALF; i < ROWS - WIDTHHALF; i++)
	{
		for (int j = WIDTHHALF; j < COLS - WIDTHHALF; j++)
		{
			for (int m = 0; m < WIDTH; m++)
			{
				if (mapDilate[i][j] == 1)
				{
					for (int n = 0; n < WIDTH; n++)
					{
						if (se[m][n] == -1)
						{
							continue;
						}
						if (se[m][n] == 1)		// 前景
						{
							if (mapDilate[i - WIDTHHALF + m][j - WIDTHHALF + n] == 0)
							{
								mapGlobal[i - WIDTHHALF + m][j - WIDTHHALF + n] = 1;
							}
						}
						else
						{
							return;
						}
					}
				}				
			}
		}
	}
}

void main()
{
	// 读取csv数据
	CvMLData mlData;
	int ret = mlData.read_csv("obstest.csv");
	Mat imgOriginal = Mat(mlData.get_values(), true);
	namedWindow("imgOriginal", CV_WINDOW_NORMAL);
	imshow("imgOriginal", imgOriginal);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			mapGlobal[i][j] = imgOriginal.at<int>(i, j);
			if (mapGlobal[i][j] > 1)
			{
				mapGlobal[i][j] = 1;
			}
		}
	}

	//int testMap[5][8] = { { 0, 0, 0, 0, 0, 0, 0, 0 },
	//						{ 0, 1, 1, 1, 1, 1, 1, 0 },
	//						{ 0, 1, 1, 1, 1, 1, 1, 0 },
	//						{ 0, 1, 1, 1, 1, 1, 1, 0 },
	//						{ 0, 0, 0, 0, 0, 0, 0, 0 } };
	//memcpy(mapGlobal, testMap, sizeof(testMap));
	//int testMap2[4][5] = { { 0, 0, 0, 0, 0 },
	//{ 0, 1, 1, 1, 0 },
	//{ 0, 1, 1, 1, 0 },
	//{ 0, 0, 0, 0, 0 } };
	//memcpy(mapGlobal, testMap2, sizeof(testMap2));

	//for (int i = 0; i < WIDTH; i++)
	//{
	//	for (int j = 0; j < WIDTH; j++)
	//	{
	//		se[i][j] = 1;
	//	}
	//}

	dilation();
	erosion();


	Mat imgDenoising(ROWS, COLS, CV_8UC1);
	for (int i = 0; i < ROWS; i++)
	{
		uchar *p = imgDenoising.ptr<uchar>(i);
		for (int j = 0; j < COLS; j++)
		{
			p[j] = mapGlobal[i][j];
		}
	}

	imgDenoising = imgDenoising > 0;
	namedWindow("imgDenoising", CV_WINDOW_NORMAL);
	imshow("imgDenoising", imgDenoising);
	waitKey(0);
}