#include <stdio.h>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cout << "Enter Project.exe ImgeFileName\n";
		return -1;
	}

	Mat img, grayImg, cstImg;
	img = imread(argv[1], IMREAD_UNCHANGED);

	//check whether the image is available
	if (img.empty()) {
		printf("Error: Couldn't open the image file.\n");
		return -1;
	}

	int w = img.cols;
	int h = img.rows;
	int t = img.channels();
	int s = img.step;
	int t1 = 0;
	int t2 = 0;

	cvtColor(img, grayImg, COLOR_BGR2GRAY);
	cstImg = grayImg.clone();

	//contrast streching
	int x = 0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			x = grayImg.at<uchar>(i, j);
			if (x < 96) {
				cstImg.at<uchar>(i, j) = (uchar)(cvRound((double)(5 / 96) * (double)(x)));
			}
			else if (x < 209) {
				cstImg.at<uchar>(i, j) = (uchar)(cvRound(2.16 * ((double)(x)-96)) + 5);
			}
			else {
				cstImg.at<uchar>(i, j) = (uchar)(cvRound(0.108 * ((double)(x)-250)) + 250);
			}
		}
	}

	//create gray image histogram
	int hist[256];

	for (int i = 0; i < 256; i++) {
		hist[i] = 0;
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			t1 = grayImg.at<uchar>(i, j);
			hist[t1] = hist[t1] + 1;
		}
	}

	//print the hist array
	for (int i = 0; i < 256; i++)
	{
		cout << i << ":" << hist[i] << endl;
	}


	//find the maximum number of pixels from histogram array
	int gr_max = 0;
	for (int i = 0; i < 256; i++) {
		if (gr_max < hist[i])
			gr_max = hist[i];
	}

	//create an image to display the histogram
	int hist_width = 520;
	int hist_height = 601;
	Mat histImage(hist_height, hist_width, CV_8UC1, Scalar(255));

	//normalize the histogram array values between 0 and histogram window height and
	//draw the lines on histogram window
	int grHist[256];
	double gr_maxd = gr_max;
	for (int i = 0; i <= 255; i++) {
		grHist[i] = cvRound(double(hist[i] / gr_maxd) * 600);
		Point gr_pt1 = Point(2 * i, 600 - grHist[i]);
		Point gr_pt2 = Point(2 * i, 600);
		line(histImage, gr_pt1, gr_pt2, Scalar(0), 1, 8, 0);
	}

	//create contrasted image histogram
	int hist2[256];

	for (int i = 0; i < 256; i++) {
		hist2[i] = 0;
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			t2 = cstImg.at<uchar>(i, j);
			hist2[t2] = hist2[t2] + 1;
		}
	}

	int cst_max = 0;
	for (int i = 0; i < 256; i++) {
		if (cst_max < hist2[i])
			cst_max = hist2[i];
	}

	Mat histImage2(hist_height, hist_width, CV_8UC1, Scalar(255));

	int cstHist[256];

	double cst_maxd = cst_max;

	for (int i = 0; i <= 255; i++) {
		cstHist[i] = cvRound(double(hist2[i] / cst_maxd) * 600);
		Point cst_pt1 = Point(2 * i, 600 - cstHist[i]);
		Point cst_pt2 = Point(2 * i, 600);
		line(histImage2, cst_pt1, cst_pt2, Scalar(0), 1, 8, 0);
	}


	cout << "Width and Height :" << w << ", " << h << endl;
	imwrite("cst.jpg", cstImg);

	namedWindow("Original Image", WINDOW_NORMAL);
	namedWindow("High Contrast Image", WINDOW_AUTOSIZE);
	namedWindow("Original Histogram", WINDOW_AUTOSIZE);
	namedWindow("Contrasted Histogram", WINDOW_AUTOSIZE);

	imshow("Original Image", grayImg);
	imshow("High Contrast Image", cstImg);
	imshow("Original Histogram", histImage);
	imshow("Contrasted Histogram", histImage2);

	waitKey(0);

	img.release();
	grayImg.release();
	cstImg.release();
	histImage.release();
	histImage2.release();
	return 0;
}