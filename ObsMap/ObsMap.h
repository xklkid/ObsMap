#include <iostream>
#include <stdio.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

#define ROWS 536
#define COLS 652
#define WIDTH 15
#define WIDTHHALF 7


void erosion();
void dilation();