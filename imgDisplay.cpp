#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "filters.hpp"
#include <opencv2/objdetect.hpp>
#include <cstdio>
#include<cstring>

using namespace cv;
using namespace std;

void main()
{
	int flag = 0;
	while (1)
	{
		string path = "Nature.jpg";
		Mat img = imread(path);
		
		//char key = waitKey(10);
		Mat result;
		if (flag == 0)
		{
			result = img;
		}
		if (flag == 1)
		{
			//greyVid(frame.size(),CV_8UC1);
			Mat greyVid = img.clone();
			cvtColor(img, greyVid, COLOR_BGR2GRAY);
			result = greyVid;
		}
		if (flag == 2)
		{
			Mat greyV(img.size(), CV_8UC1);
			alt_greyscale(img, greyV);
			result = greyV;
		}
		if (flag == 3)
		{
			Mat blurV(img.size(), CV_8UC3);
			blur(img, blurV);
			convertScaleAbs(blurV, blurV);
			result = blurV;
		}
		if (flag == 4)
		{
			Mat sobelVx(img.size(), CV_16SC3);
			sobel_x(img, sobelVx);
			convertScaleAbs(sobelVx, sobelVx);
			result = sobelVx;
		}
		if (flag == 5)
		{
			Mat sobelVy(img.size(), CV_16SC3);
			sobel_y(img, sobelVy);
			convertScaleAbs(sobelVy, sobelVy);
			result = sobelVy;
		}
		if (flag == 6)
		{
			Mat gradmagV(img.size(), CV_16SC3);
			Mat sVx(img.size(), CV_16SC3);
			Mat sVy(img.size(), CV_16SC3);
			sobel_x(img, sVx);
			sobel_y(img, sVy);
			gradMagnitude(sVx, sVy, gradmagV);
			convertScaleAbs(gradmagV, gradmagV);
			result = gradmagV;
		}
		if (flag == 7)
		{
			Mat blurqV(img.size(), CV_8UC3);
			blurQuantize(img, blurqV, 15);
			convertScaleAbs(blurqV, blurqV);
			result = blurqV;
		}
		if (flag == 8)
		{
			Mat cartV(img.size(), CV_8UC3);
			cartoon(img, cartV, 18, 25);
			convertScaleAbs(cartV, cartV);
			result = cartV;
		}
		if (flag == 9)
		{
			Mat negV(img.size(), CV_8UC3);
			negative(img, negV);
			convertScaleAbs(negV, negV);
			result = negV;
		}
		if (flag == 10)
		{
			Mat brghtV(img.size(), CV_8UC3);
			bright(img, brghtV, 100);
			result = brghtV;
		}
		if (flag == 11)
		{
			Mat cnstrV(img.size(), CV_8UC3);
			contrast(img, cnstrV, -1, 2, 0);
			result = cnstrV;
		}
		char key = waitKey(10);
		imshow("Webcam", img);
		imshow("Result", result);
		switch (key)
		{
		case 'g':
		{
			flag = 1;
			continue;
		}
		case 'h':
		{
			flag = 2;
			continue;
		}
		case 'b':
		{
			flag = 3;
			continue;
		}
		case 'x':
		{
			flag = 4;
			continue;
		}
		case 'y':
		{
			flag = 5;
			continue;
		}
		case 'm':
		{
			flag = 6;
			continue;
		}
		case 'l':
		{
			flag = 7;
			continue;
		}
		case 'c':
		{
			flag = 8;
			continue;
		}
		case 'n':
		{
			flag = 9;
			continue;
		}
		case 'w':
		{
			flag = 10;
			continue;
		}
		case 'z':
		{
			flag = 11;
			continue;
		}
		//case 't':
		//{
		//	flag = 12;
		//	//imshow("Captioned frame", frame);
		//	break;
		//}
		}
		if (key == 't')
		{
			string caption;
			Mat textimg;
			textimg = img;
			Point text_pos(300, 500);
			int font_size = 2;
			Scalar font_color(0, 0, 0);
			int font_weight = 2;
			cout << "Enter your caption: ";
			getline(cin, caption);
			putText(textimg, caption, text_pos, FONT_HERSHEY_PLAIN, font_size, font_color, font_weight);
			imshow("Caption", textimg);
			imwrite("Captioned Nature.jpg", textimg);
		}
		string name;
		if (key == 'q')
		{
			break;
		}
		else if (key == 's')
		{
			cout << "Image name: ";
			getline(cin, name);
			imwrite(name + ".jpg", result);
		}
	}
	
}
