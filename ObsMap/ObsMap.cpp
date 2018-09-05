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

void GenerateRuns(int& numberOfRuns, vector<int>& startRun, vector<int>& endRun, vector<int>& rowRun)
{
	for (int i = 0; i < ROWS; i++)
	{
		vector<int> rowData;
		for (int k = 0; k < COLS; k++)
		{
			rowData.push_back(mapGlobal[i][k]);
		}

		if (rowData[0] == 1)
		{
			numberOfRuns++;
			startRun.push_back(0);
			rowRun.push_back(i);
		}
		for (int j = 1; j < COLS; j++)
		{
			if (rowData[j-1] == 0 && rowData[j] == 1)		// 列开始处
			{
				numberOfRuns++;
				startRun.push_back(j);
				rowRun.push_back(i);
			}
			else if (rowData[j-1] == 1 && rowData[j] == 0)		// 列结束处
			{
				endRun.push_back(j - 1);
			}
		}
		if (rowData[COLS - 1])
		{
			endRun.push_back(COLS - 1);
		}
	}
}

void GenerateLabelAndEquicalence(vector<int>& startRun, vector<int>& endRun, vector<int>& rowRun, int numberOfRuns, vector<int>& runLabels, vector<pair<int, int>>& equivalences)
{
	runLabels.assign(numberOfRuns, 0);
	int idxLabel = 1;
	int curRowIdx = 0;
	int firstRunOnCur = 0;
	int firstRunOnPre = 0;
	int lastRunOnPre = -1;

	for (int i = 0; i < numberOfRuns; i++)		// run的序号
	{
		if (rowRun[i] != curRowIdx)		//如果是该行的第一个run，则更新上一行第一个run和最后一个run的序号
		{
			curRowIdx = rowRun[i];
			firstRunOnPre = firstRunOnCur;
			lastRunOnPre = i;
			firstRunOnCur = i;
		}
		for (int j = firstRunOnPre; j < lastRunOnPre; j++)		//遍历上一行所有run，判断是否当前run有重合的区域
		{
			//区域重合且处于相邻的两行
			if ((startRun[i] <= (endRun[j] + 1)) && (endRun[i] >= (startRun[j] - 1)) && (rowRun[i] == rowRun[j] + 1))		// 8邻域
			{
				if (runLabels[i] == 0)		// 没有被标记过
				{
					runLabels[i] = runLabels[j];		//与上一行重叠，标记上一行标号
				}
				else if (runLabels[i] != runLabels[j])		//已经被标记
				{
					equivalences.push_back(make_pair(runLabels[i], runLabels[j]));		//保存等价对
				}
			}
		}
		if (runLabels[i] == 0)		//没有与前一行任何run重合
		{
			runLabels[i] = idxLabel++;
		}
	}
}

void CombineEquivalences(vector<int>& runLabels, vector<pair<int, int>>& equivalence, int& clusterNum)
{
	int maxLabel = *max_element(runLabels.begin(), runLabels.end());
	vector<vector<bool>> eqTable(maxLabel, vector<bool>(maxLabel, false));
	vector<pair<int, int>>::iterator vecPairIt = equivalence.begin();
	while (equivalence.end() != vecPairIt)		// 生成等价对二维表
	{
		eqTable[vecPairIt->first - 1][vecPairIt->second - 1] = true;
		eqTable[vecPairIt->second - 1][vecPairIt->first - 1] = true;
		vecPairIt++;
	}

	vector<int> labelFlag(maxLabel, 0);
	vector<vector<int>> equaList;
	vector<int> tempList;
	for (int i = 1; i <= maxLabel; i++)
	{
		if (labelFlag[i - 1])		//设置过的序号的返回
		{
			continue;
		}
		labelFlag[i - 1] = equaList.size() + 1;
		tempList.push_back(i);
		for (vector<int>::size_type j = 0; j < tempList.size(); j++)
		{
			for (vector<bool>::size_type k = 0; k < eqTable[tempList[j] - 1].size(); k++)
			{
				if (eqTable[tempList[j] - 1][k] && !labelFlag[k])
				{
					tempList.push_back(k + 1);
					labelFlag[k] = equaList.size() + 1;
				}
			}
		}
		equaList.push_back(tempList);
		tempList.clear();
	}
	for (vector<int>::size_type i = 0; i < runLabels.size(); i++)
	{
		runLabels[i] = labelFlag[runLabels[i] - 1];
	}
	clusterNum = equaList.size();
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

	// 腐蚀膨胀
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

	// 连通域
	int numberOfRuns = 0;
	vector<int> startRun;
	vector<int> endRun;
	vector<int> rowRun;
	vector<int> runLabels;
	vector<pair<int, int>> equivalences;
	int clusterNum = 0;
	int mapResult[ROWS][COLS] = { 0 };

	GenerateRuns(numberOfRuns, startRun, endRun, rowRun);
	GenerateLabelAndEquicalence(startRun, endRun, rowRun, numberOfRuns, runLabels, equivalences);
	CombineEquivalences(runLabels, equivalences, clusterNum);

	// 生成标记过的地图
	for (int i = 0; i < numberOfRuns; i++)
	{
		for (int j = startRun[i]; j <= endRun[i]; j++)
		{
			mapResult[rowRun[i]][j] = runLabels[i];
		}
	}

	// 输出地图信息txt
	ofstream out("mapOut.txt");
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			out << mapResult[i][j] << ',';
		}
		out << endl;
	}

	waitKey(0);
}