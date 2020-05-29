#include <iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

float O_distance(float * l, float * m, int n) {
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum = sum + pow((*(l + i) - *(m + i)), 2);
	}
	return sum;
}

void HOG(Mat & src, int blocksize, int nX, int nY, float * hist) {
	Mat gx, gy;
	Mat mag, angle;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	int n = 0;
	for (int i = 0; i < nY; i++) {

		for (int j = 0; j < nX; j++) {

			int n_x = j * blocksize, n_y = i * blocksize;

			for (int l = n_x; l < blocksize + n_x; l++) {

				for (int m = n_y; m < blocksize + n_y; m++) {

					int region = (angle.at<float>(m, l)) / 40;
					*(hist + region + n) += mag.at<float>(m, l);
				}
			}
			n += 9;
		}
	}
}

int main() {

	Mat srcMat = imread("D:\\template.png", 0);
	Mat result = imread("D:\\img.png", 0);

	if (!srcMat.data) {
		cout << "fail" << endl;
	}if (!result.data) {
		cout << "fail2" << endl;
	}

	int blocksize = 16;
	int nX = srcMat.cols / blocksize;
	int nY = srcMat.rows / blocksize;
	int bins = 9 * nX*nY;

	double minR = 1e20;
	int minX;
	int minY;
	int height = result.rows;
	int width = result.cols;
	int nheight = srcMat.rows;
	int nwidth = srcMat.cols;

	float * rhist = new float[bins];
	memset(rhist, 0, sizeof(float)*bins);

	HOG(srcMat, blocksize, nX, nY, rhist);

	for (int i = 0; i < height - nheight + 1; i++) {

		for (int j = 0; j < width - nwidth + 1; ) {

			Rect rect(j, i, nwidth, nheight);
			Mat iroi = result(rect);
			float * roi_hist = new float[bins];
			memset(roi_hist, 0, sizeof(float)*bins);
			HOG(iroi, blocksize, nX, nY, roi_hist);

			double distance = O_distance(rhist, roi_hist, bins);

			if (distance < minR)
			{
				minR = distance;
				minX = j;
				minY = i;
			}
			delete[]  roi_hist;
			j = j + 5;
		}
		i = i + 5;
	}
	delete[] rhist;
	Rect rect(minX, minY, nwidth, nheight);
	rectangle(result, rect, Scalar(0, 0, 255), 2, 8, 0);
	imshow("result", result);
	waitKey(0);
	return 0;
}