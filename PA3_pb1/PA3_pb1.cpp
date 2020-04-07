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

struct node
{
	int data;
	struct node* next;
};

class linked_list
{
	private:
		node* head, * tail;

	public:
		linked_list()
		{
			head = NULL;
			tail = NULL;
		}

		void add_node(int n)
		{
			node* tmp = new node;
			tmp -> data = n;
			tmp -> next = NULL;

			if (head == NULL)
			{
				head = tmp;
				tail = tmp;
			}
			else
			{
				tail -> next = tmp;
				tail = tail -> next;
			}
		}

		void print() 
		{
			node* ptr;
			ptr = head;
			
			while (ptr != NULL) {
				cout << ptr -> data << " ";
				ptr = ptr -> next;
			}
		}
};

void hough_circles(Mat img, vector<Vec3f> circles, int Radius)
{
	
}


int distance(int xi, int yi, int x0, int y0, int r)
{
	return abs( sqrt( pow((xi - x0),2) + pow((yi - y0), 2) ) - r );
}

int fitting_error(int n)
{
	int sum = 0;
	int i;

	for (i = 0; i < n; i++) 
	{
		sum += 1;
	}

	sum /= n;
	return sum;
}

int main(int argc, char** argv)
{
	linked_list a;
	a.add_node(1);
	a.add_node(2);
	a.add_node(3);
	a.add_node(5);
	a.add_node(7);
	a.add_node(10);
	cout << "The linked list is: ";
	a.print();
	cout << endl << endl;

	// Read in images
	const int image_number = 14;
	string images[image_number];
	images[0] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\test1.pgm";
	images[1] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\test2.pgm";
	images[2] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\test3.pgm";
	images[3] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\test4.pgm";
	images[4] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\test5.pgm";
	images[5] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\C1.pgm";
	images[6] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\C2.pgm";
	images[7] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\C3.pgm";
	images[8] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\C4.pgm";
	images[9] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\C5.pgm";
	images[10] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\C6.pgm";
	images[11] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\overlap1.pgm";
	images[12] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\overlap2.pgm";
	images[13] = "C:\\Users\\Lee\\source\\repos\\PA3_pb1\\PA3_pb1\\Circles\\overlap3.pgm";
	

	int i;
	for (i = 0; i < image_number; i++)
	{
		Mat img = imread(images[i], IMREAD_GRAYSCALE);
		imshow("Original Image", img);

		cout << images[i] << endl;
		cout << "Width: " << img.cols << endl;
		cout << "Height: " << img.rows << endl << endl;


		// Gaussian Blur to Reduce noise
		Mat img_blur;
		GaussianBlur(img, img_blur, Size(9, 9), 2, 2);
		imshow("Gaussian Blurred Image", img_blur);


		// Sobel Edge Detection
		// Generate grad_x and grad_y
		Mat grad_x, grad_y;
		Mat abs_grad_x, abs_grad_y;
		Mat img_grad;

		// Gradient X
		Sobel(img_blur, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
		convertScaleAbs(grad_x, abs_grad_x);

		// Gradient Y
		Sobel(img_blur, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
		convertScaleAbs(grad_y, abs_grad_y);

		// Total Gradient (approximate)
		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, img_grad);
		imshow("Sobel Edge Detection", img_grad);


		// Thresholding
		Mat img_threshold;
		double thresh = 50;
		double maxValue = 255;

		threshold(img_grad, img_threshold, thresh, maxValue, THRESH_BINARY);
		imshow("Thresholding", img_threshold);


		// Apply the Hough Transform to find the circles
		vector<Vec3f> circles;
		hough_circles(img_threshold, circles);
		//HoughCircles(img_threshold, circles, HOUGH_GRADIENT, 1,
		//	img_blur.rows / 8,
		//	250, 40, 0, 0);

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
	}

	return 0;
}