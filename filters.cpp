#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "filters.hpp"

using namespace cv;
using namespace std;

Mat grey_scale(Mat &src, Mat &dst)
{
	cvtColor(src, dst, COLOR_BGR2GRAY); //predefined function converts RGB to greyscale
	return dst;
}

int alt_greyscale(Mat &src, Mat &dst)
{
	src.copyTo(dst);
	for (int i = 0; i < dst.rows; i++)
	{
		Vec3b *rptr = dst.ptr<Vec3b>(i);//creating a row pointer for dst image matrix
		for (int j = 0; j < dst.cols; j++)
		{
			rptr[j][0] = rptr[j][0];//extracting value of each channel of each pixel
			rptr[j][1] = rptr[j][0];//changing value of each channel to one channel (blue)
			rptr[j][2] = rptr[j][0];
		}
	}
	return 0;
}

int blur(Mat &src, Mat &dst)
{
	Mat temp1(src.size(), CV_8UC3);//creating temp matrix same size as source
	int kernel[5] = { 1,2,4,2,1 }; //1x5 kernel matrix to operate once horizontally and once vertically
	for (int i = 2; i < src.rows - 2; i++)
	{
		for (int j = 2; j < src.cols - 2; j++)
		{
			for (int c = 0; c < 3; c++)
			{
				int s = 0;
				for (int k = j - 2; k <= j + 2; k++)
				{
					Vec3b blurim = src.at<Vec3b>(i, k);//calculating for pixels for 5 rows from x-2 to x+2
					s = s + (int)blurim[c] * kernel[k - (j - 2)];//performing convulation for 5 rows since kernel is 1x5
				}
				temp1.at<Vec3b>(i, j)[c] = (int)s / 10;//assigning values to a temp image
			}
			
		}
	}
	Mat temp2(temp1.size(), CV_8UC3);
	for (int i = 2; i < temp1.rows - 2; i++)//performing the same operations but for columns
	{
		for (int j = 2; j < temp1.cols - 2; j++)
		{
			for (int c = 0; c < 3; c++)
			{
				int s = 0;
				for (int k = i - 2; k <= i + 2; k++)
				{
					Vec3b blurim = src.at<Vec3b>(k, j);
					s = s + (int)blurim[c] * kernel[k - (i - 2)];
				}
				temp2.at<Vec3b>(i, j)[c] = (int)s / 10;
			}
		}
	}
	temp2.copyTo(dst);//copying the image to the dst/final image
	return 0;
}

int sobel_x(Mat &src, Mat &dst)
{
	int kernel1[] = { -1, 0, 1 }; //horizontal matrix
	int kernel2[] = { 1, 2, 1 }; //vertical matrix
	Mat temp(src.size(), CV_16SC3);
	for (int i = 1; i < src.rows - 1; i++) 
	{
		for (int j = 1; j < src.cols - 1; j++) 
		{
			int sb1 = 0;
			int sg1 = 0;
			int sr1 = 0;
			for (int k = i - 1; k <= i + 1; k++) 
			{
				Vec3b sblx = src.at<Vec3b>(k, j);//calculating for pixels for 3 rows from x-1 to x+1
				sb1 = sb1 + sblx[0] * kernel1[k - (i - 1)];
				sg1 = sg1 + sblx[1] * kernel1[k - (i - 1)];
				sr1 = sr1 + sblx[2] * kernel1[k - (i - 1)];
			}
			temp.at<Vec3s>(i, j)[0] = abs(sb1);
			temp.at<Vec3s>(i, j)[1] = abs(sg1);
			temp.at<Vec3s>(i, j)[2] = abs(sr1);
		}
	}
	for (int i = 1; i < temp.rows - 1; i++) 
	{
		for (int j = 1; j < temp.cols - 1; j++) 
		{
			int sb2 = 0;
			int sg2 = 0;
			int sr2 = 0;
			for (int k = j - 1; k <= j + 1; k++) 
			{
				Vec3s sblx = temp.at<Vec3s>(i, k);
				sb2 = sb2 + sblx[0] * kernel2[k - (j - 1)];
				sg2 += sg2 + sblx[1] * kernel2[k - (j - 1)];
				sr2 += sr2 + sblx[2] * kernel2[k - (j - 1)];
			}
			dst.at<Vec3s>(i, j)[0] = abs(sb2 / 4);
			dst.at<Vec3s>(i, j)[1] = abs(sg2 / 4);
			dst.at<Vec3s>(i, j)[2] = abs(sr2 / 4);
		}
	}
	return 0;
}

int sobel_y(Mat &src, Mat &dst)//same calculation as sobelx, kernel matrix different
{
	int kernel1[] = { 1, 2, 1 }; //horizontal
	int kernel2[] = { -1, 0, 1 }; //vertical

	Mat temp(src.size(), CV_16SC3);
	for (int i = 1; i < src.rows - 1; i++) 
	{
		for (int j = 1; j < src.cols - 1; j++) 
		{
			int sb1 = 0;
			int sg1 = 0;
			int sr1 = 0;
			for (int k = i - 1; k <= i + 1; k++) 
			{
				Vec3b sbly = src.at<Vec3b>(k, j);
				sb1 = sb1 + sbly[0] * kernel1[k - (i - 1)];
				sg1 = sg1 + sbly[1] * kernel1[k - (i - 1)];
				sr1 = sr1 + sbly[2] * kernel1[k - (i - 1)];
			}
			temp.at<Vec3s>(i, j)[0] = abs(sb1 / 4);
			temp.at<Vec3s>(i, j)[1] = abs(sg1 / 4);
			temp.at<Vec3s>(i, j)[2] = abs(sr1 / 4);
		}
	}
	for (int i = 1; i < temp.rows - 1; i++) 
	{
		for (int j = 1; j < temp.cols - 1; j++) 
		{
			int sb2 = 0;
			int sg2 = 0;
			int sr2 = 0;
			for (int k = j - 1; k <= j + 1; k++) 
			{
				Vec3s sbly = temp.at<Vec3s>(i, k);
				int filter_point = kernel2[k - (j - 1)];
				sb2 = sb2 + sbly[0] * kernel2[k - (j - 1)];
				sg2 = sg2 + sbly[1] * kernel2[k - (j - 1)];
				sr2 = sr2 + sbly[2] * kernel2[k - (j - 1)];
			}
			dst.at<Vec3s>(i, j)[0] = abs(sb2);
			dst.at<Vec3s>(i, j)[1] = abs(sg2);
			dst.at<Vec3s>(i, j)[2] = abs(sr2);
		}
	}
	return 0;
}

int gradMagnitude(Mat &srcx, Mat &srcy, Mat &dst)
{
	for (int i = 0; i < srcx.rows; i++)
	{
		for (int j = 0; j < srcx.cols; j++)
		{
			Vec3s x = srcx.at<Vec3s>(i, j);
			Vec3s y = srcy.at<Vec3s>(i, j);

			for (int c = 0; c < 3; c++)
			{
				dst.at<Vec3s>(i, j)[c] = sqrtf(pow(x[c], 2) + pow(y[c], 2));//using sobelx and sobely values to calculate
			}
		}
	}
	return 0;
}

int blurQuantize(Mat &src, Mat &dst, int levels)
{
	Mat blurq(src.size(), CV_8UC3);
	blur(src, blurq);
	int b = 255 / levels;
	int xt = 0;
	int xf = 0;
	for (int i = 0; i < blurq.rows; i++)
	{
		for (int j = 0; j < blurq.cols; j++)
		{
			Vec3b bq = blurq.at<Vec3b>(i, j);
			for (int c = 0; c < 3; c++)
			{
				xt = bq[c] / b;
				xf = xt * b;
				dst.at<Vec3b>(i, j)[c] = xf;//assigning specific value to all pixels within threshold values
			}
		}
	}
	return 0;
}

int cartoon(Mat &src, Mat &dst, int levels, int threshold)
{
	Mat mag(src.size(), CV_16UC3);
	Mat sx(src.size(), CV_16UC3);
	Mat sy(src.size(), CV_16UC3);
	sobel_x(src, sx);
	sobel_y(src, sy);
	gradMagnitude(sx, sy, mag);
	blurQuantize(src, dst, levels);

	for (int i = 0; i < mag.rows; i++) 
	{
		for (int j = 0; j < mag.cols; j++) 
		{
			Vec3b pixel = dst.at<Vec3b>(i, j);
			Vec3s magnitude = mag.at<Vec3s>(i, j);

			for (int c = 0; c < 3; c++) {

				if (magnitude[c] > threshold) 
				{
					dst.at<Vec3b>(i, j)[c] = 0;//changing all pixel values baove threshold to black
				}
			}
		}
	}
	return 0;
}

int bright(Mat &src, Mat &dst, int brightval)
{
	dst = src + Scalar(brightval, brightval, brightval);
	return 0;
}

int contrast(Mat &src, Mat &dst, int conval1, int conval2, int conval3)
{
	src.convertTo(dst, conval1, conval2, conval3);
	return 0;
}

int negative(Mat &src, Mat &dst)
{
	src.copyTo(dst);
	for (int i = 0; i < dst.rows; i++)
	{
		Vec3b* rptr = dst.ptr<Vec3b>(i);
		for (int j = 0; j < dst.cols; j++)
		{
			rptr[j][0] = 255 - rptr[j][0];//computing inverted values for each colour channel
			rptr[j][1] = 255 - rptr[j][1];
			rptr[j][2] = 255 - rptr[j][2];
		}
	}
	return 0;
}