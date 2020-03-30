#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <stdio.h> 
#include <stdlib.h>

using namespace cv;
using namespace std;

#define pi 3.141592653589793

int distance(int xi, int yi, int x0, int y0, int r)
{
	return abs( sqrt( pow((xi - x0),2) + pow((yi - y0), 2) ) - r );
}

int fitting_error()
{
	return 0;
}

int main(int argc, char** argv)
{
	// Read in image
	string img_name = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\C1.pgm";
	Mat img = imread(img_name, IMREAD_GRAYSCALE);
	imshow("Original Image", img);

	cout << "Width: " << img.cols << endl;
	cout << "Height: " << img.rows << endl;


	// Gaussian Blur to Reduce noise
	Mat img_blur;
	GaussianBlur(img, img_blur, Size(9, 9), 2, 2);
	imshow("Gaussian Blurred Image", img_blur);


	// Sobel Edge Detection
	// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat grad;

	// Gradient X
	Sobel(img_blur, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	// Gradient Y
	Sobel(img_blur, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	// Create window
	namedWindow("Sobel Edge Detection");
	imshow("Sobel Edge Detection", grad);


	// Apply the Hough Transform to find the circles
	vector<Vec3f> circles;
	HoughCircles(grad, circles, HOUGH_GRADIENT, 1,
				img_blur.rows / 8, 
				220, 55, 0, 0);

	// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		
		// Circle center
		circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		
		// Circle outline
		circle(img, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}


	// Show your results
	namedWindow("Hough Circle Transform");
	imshow("Hough Circle Transform", img);

	waitKey(0);
	return 0;
}