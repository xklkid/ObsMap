#include <iostream>
#include <stdio.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <fstream>

using namespace cv;
using namespace std;

#define ROWS 536
#define COLS 652
#define WIDTH 15
#define WIDTHHALF 7


void erosion();
void dilation();
void GenerateRuns(int& numberOfRuns, vector<int>& startRun, vector<int>& endRun, vector<int>& rowRun);
void GenerateLabelAndEquicalence(vector<int>& startRun, vector<int>& endRun, vector<int>& rowRun, int numberOfRuns, vector<int>& runLabels, vector<pair<int, int>>& equivalences);
void CombineEquivalences(vector<int>& runLabels, vector<pair<int, int>>& equivalence, int& clusterNum);


