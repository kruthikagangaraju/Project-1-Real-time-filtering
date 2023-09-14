#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "filters.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    VideoCapture* capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

   // cv::namedWindow("Video", 1); // identifies a window
    Mat frame;
    Mat grayframe;
	Mat greyVid;
	VideoWriter video;
	int flag = 0;
	Mat result;
	bool record = false;
	bool play = false;
	bool cap = false;
	

    for (;;) 
	{
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) 
		{
            printf("frame is empty\n");
            break;
        }
		
		if (flag == 0)//assigning new video frame "result" same feed as "frame" when filters not applied
		{
			result = frame;
		}
		if (flag == 1)//Greyscale video
		{
			greyVid = frame.clone();
			cvtColor(frame, greyVid, COLOR_BGR2GRAY);
			result = greyVid;
		}
		if (flag == 2)//Alternate Greyscale video
		{
			Mat greyV(frame.size(), CV_8UC1);
			alt_greyscale(frame, greyV);
			result = greyV;
		}
		if (flag == 3)//Blur video
		{
			Mat blurV(frame.size(), CV_8UC3);
			blur(frame, blurV);
			convertScaleAbs(blurV, blurV);
			result = blurV;
		}
		if (flag == 4)//Sobel filter in x direction
		{
			Mat sobelVx(frame.size(), CV_16SC3);
			sobel_x(frame, sobelVx);
			convertScaleAbs(sobelVx, sobelVx);
			result = sobelVx;
		}
		if (flag == 5)//Sobel filter in y direction
		{
			Mat sobelVy(frame.size(), CV_16SC3);
			sobel_y(frame, sobelVy);
			convertScaleAbs(sobelVy, sobelVy);
			result = sobelVy;
		}
		if (flag == 6)//gradient magnitude using sobelx and sobely
		{
			Mat gradmagV(frame.size(), CV_16SC3);
			Mat sVx(frame.size(), CV_16SC3);
			Mat sVy(frame.size(), CV_16SC3);
			sobel_x(frame, sVx);
			sobel_y(frame, sVy);
			gradMagnitude(sVx, sVy, gradmagV);
			convertScaleAbs(gradmagV, gradmagV);
			result = gradmagV;
		}
		if (flag == 7)//Quantized blur video
		{
			Mat blurqV(frame.size(), CV_8UC3);
			blurQuantize(frame, blurqV, 15);
			convertScaleAbs(blurqV, blurqV);
			result = blurqV;
		}
		if (flag == 8)//Cartoon video
		{
			Mat cartV(frame.size(), CV_8UC3);
			cartoon(frame, cartV, 18, 25);
			convertScaleAbs(cartV, cartV);
			result = cartV;
		}
		if (flag == 9)//Negative video
		{
			Mat negV(frame.size(), CV_8UC3);
			negative(frame, negV);
			convertScaleAbs(negV, negV);
			result = negV;
		}
		if (flag == 10)//Brightened video
		{
			Mat brghtV(frame.size(), CV_8UC3);
			bright(frame, brghtV, 100);
			result = brghtV;
		}
		if (flag == 11)//contrast video
		{
			Mat cnstrV(frame.size(), CV_8UC3);
			contrast(frame, cnstrV, -1,3,1);
			result = cnstrV;
		}

		if (record)//allows user to record the video till 'q' is pressed
		{
			
			if (!play)
			{
				int fps = 10;
				video.open("Special filter effects.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame.size());
				play = true;
				cout << "Recording started";
			}
			if (play)
			{
				video.write(result);
			}
		}
		else
		{
			video.release();
		}
	
		char key = waitKey(10);
		imshow("Webcam", frame);//display webcam frame
		imshow("Result", result);//displays resultant frame on which filters are applied

		switch (key)//takes keypress and performs necessary if block
		{
		case 'a':
		{
			flag = 0;
			continue;
		}
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
		case 'r':
		{
			record = !record;
			play = false;
			continue;
		}
		}
		
		string name;
		/*if (key == 't')
		{
			string caption;
			Mat result1 = frame;
			Point text_pos(200, 450);
			int font_size = 2;
			Scalar font_color(255, 255, 255);
			int font_weight = 2;
			cout << "Enter your caption: ";
			getline(cin, caption);
			putText(result1, caption, text_pos, FONT_HERSHEY_PLAIN, font_size, font_color, font_weight);
			imshow("Caption", result1);
			imwrite("Captioned image.jpg", result1);
		}*/
		if (key == 't')
		{
			VideoWriter writer("Captioned video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 20, Size((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),//creates a video file
				(int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT)));
			string caption;
			cout << "Enter your caption: ";//takes caption input from user
			getline(cin, caption);
			while (capdev->read(result))
			{
				cv::putText(result, caption, cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2); // Add the caption
				writer.write(result);//saves short video recording with caption
			}
			capdev->release();
			writer.release();

		}
		if (key == 'q')//quits
		{
			break;
		}
		else if (key == 's')//saves frame with desired name
		{
			cout << "Image name: ";
			getline(cin, name);
			imwrite(name + ".jpg", result);
		}
		
    }
    delete capdev;
    return(0);
}