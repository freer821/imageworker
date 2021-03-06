// ImageWorker.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream> 
#include "PointsClassifier.h"
#include <experimental/filesystem> // C++-standard header file name  
#include <filesystem> // Microsoft-specific implementation header file name  
using namespace std::experimental::filesystem::v1;
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

void convertImageToPointsJson(const string& imagename, const string& output_path)
{
	cout << "begin to read Image: " << imagename << endl;
	Mat img = imread(imagename);

	if (img.empty())
	{
		cout << "Image cannot be loaded..!!" << endl;
		return;
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

	path p(imagename);
	const string jsonfile = output_path + "\\" + p.stem().string() + ".json";
	PointsClassifier pc(imgGray);
	pc.findAllPointsGroup();
	pc.writePointsJsonInFile(jsonfile);
}

int main(int argc, char** argv)
{

	cout << "Please input the full path to the images (default 'data'):";

	string filepath = "";
	getline(cin, filepath);

	for (auto & p : directory_iterator(filepath))
	{
		ostringstream oss;
		oss << p;

		convertImageToPointsJson(oss.str(), filepath);
	}
	getline(cin, filepath);
	/**
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

	filename = "output.png";
	imwrite(filename, img);
	*/
	return 0;
}