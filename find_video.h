#ifndef FIND_VIDEO_H
#define FIND_VIDEO_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;

void getVariance(Mat, int, double, double, double &, double &);

void getAverage(Mat, int, double &, double &);

Mat getContour(Mat, int &);

void analyzeData(void);

void storeToDB(void);

#endif // FIND_VIDEO_H