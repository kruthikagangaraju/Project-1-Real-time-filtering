#ifndef FILTERS_TEST_HPP
#define FILTERS_TEST_HPP
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat grey_scale(Mat &src, Mat &dst);
int alt_greyscale(Mat &src, Mat &dst);
int blur(Mat &src, Mat &dst);
int sobel_x(Mat &src, Mat &dst);
int sobel_y(Mat& src, Mat& dst);
int gradMagnitude(Mat &srcx, Mat &srcy, Mat &dst);
int blurQuantize(Mat &src, Mat &dst, int levels);
int cartoon(Mat &src, Mat &dst, int levels, int threshold);
int bright(Mat &src, Mat &dst, int brightval);
int contrast(Mat &src, Mat &dst, int conval1, int conval2, int conval3);
int negative(Mat &src, Mat &dst);
#endif // !FILTERS_TEST_HPP

