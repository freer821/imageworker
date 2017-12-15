// ImageWorker.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream> 
#include "PointsClassifier.h"
using namespace std;
using namespace cv;

void writePointToFile(const unordered_map<string, Point>& points, const string &filename = "points.txt")
{
	ofstream myfile;
	myfile.open(filename);

	for (auto & point : points) {
		myfile << "point: x " << point.second.x << ", y " << point.second.y << "\n";
	}

	myfile.close();

	cout << "Points are already wrritten to file!" << endl;
}

int main(int argc, char** argv)
{

	/**
	cout << "Please input the name of the image (default 'test.png'):";

	string filename = "";
	getline(cin, filename);

	if (filename.length() < 4) {
	filename = "test.png";
	}
	*/
	string filename = "test.png";

	Mat img = imread(filename);

	if (img.empty())
	{
		cout << "Image cannot be loaded..!!" << endl;
		return -1;
	}

	Mat imgGray(img.size().height, img.size().width, CV_8UC1, Scalar(0));

	for (int y = 0; y < img.size().height; y++) {
		for (int x = 0; x < img.size().width; x++) {
			Vec3b bgrPixel = img.at<Vec3b>(y, x);
			if (bgrPixel[0] == 255 && bgrPixel[1] == 0 && bgrPixel[2] == 255) {
				imgGray.at<uchar>(y, x) = 255;
			}
		}
	}

	PointsClassifier pc(imgGray);
	pc.findAllPointsGroup();
	pc.writePointsJsonInFile();

	vector<vector<Point>> pointsGroups = pc.getPointsGroups();
	if (pointsGroups.size() > 0) {
		for (auto const& groupPoints : pointsGroups) {
			for (auto const& point : groupPoints)
			{
				Vec3b & color = img.at<Vec3b>(point.y, point.x);
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
			}
		}
	}

	cout << "finished!!" << endl;

	/**
	cout << "Please input the name of the output (default 'output.png'):";
	getline(cin, filename);
	if (filename.length() < 4) {
	filename = "output.png";
	}
	*/
	filename = "output.png";
	imwrite(filename, img);
	return 0;
}