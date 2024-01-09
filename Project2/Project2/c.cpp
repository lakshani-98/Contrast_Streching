#include <stdio.h>
#include <iostream>
#include<opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat img = imread(argv[1], 1);

	//check whether the image is available 
	if (!img.data) {
		cout << "Could not find the image!" << endl;
		return -1;
	}

	int height = img.rows;
	int width = img.cols;
	int widthstep = img.step;
	int ch = img.channels();

	//Print height, width, channels
	printf("height: %d, width:%d, Channels: %d", height, width, ch);

	//Making a pixel into white
	//img.at<uchar>(10, 10) = 255;

	Mat gray(height, width, CV_8UC1, Scalar(0));
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Mat copy(height, width, CV_8UC3, Scalar(0, 0, 0));

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			copy.at<Vec3b>(y, x) = img.at<Vec3b>(y, x);
		}
	}

	//allocate memory for all the intensity values
    int hist[256];

	//Initialize "0" to all the array elements
	for (int i = 0; i < 256; i++)
	{
		hist[i] = 0;
	}

	//Count one by one pixel for each intensity value
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			hist[(int)gray.at<uchar>(y, x)] = hist[(int)gray.at<uchar>(y, x)]+ 1;	
		}
	}


	//print the hist array
	for (int i = 0; i < 256; i++)
	{
		cout << i << ":" << hist[i] << endl;

	}

	//create an image to display the histogram
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255));

	//Find the maximum number of pixels from histogram array
	int max = hist[0];
	for (int i = 0; i < 256; i++)
	{
		if (max < hist[i]) {
			max = hist[i];
		}        
	}

	//Normalize the histogram array values between 0 and histogram window height
	for (int i = 0; i < 256; i++)
	{
		hist[i] = ((double)hist[i] / max) * histImage.rows;
	}

	//Draw the lines on histogram window
	for (int i = 0; i < 256; i++)
	{
		line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), hist_h - hist[i]), Scalar(0), 1, 8, 0);
	}


	namedWindow("Image");
	imshow("Image", img);

	namedWindow("GRAY");
	imshow("GRAY", gray);

	namedWindow("Copy");
	imshow("COPY", copy);

	namedWindow("Histogram");
	imshow("Histogram", histImage);

	waitKey(0);
	return 0;

}
