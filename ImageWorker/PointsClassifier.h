#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <unordered_map>
using namespace std;
using namespace cv;

class PointsClassifier
{
private:
	Mat _img;
	int _imgH;
	int _imgW;
	Mat _cal_img_space;
	unordered_map<string, Point> allPoints;
	vector<vector<Point>> pointsGroups;
	void setCalNeighborsOfPoint(Point p, Point* points);
	vector<Point>  findRailwayNeighborsOfPoint(const Point& center_p);
	vector<Point>  findOneGroupPoints(Point start_p);

public:
	PointsClassifier(const Mat & img);
	~PointsClassifier();
	void  findAllPointsGroup();
	vector<vector<Point>> getPointsGroups();
	void writePointsJsonInFile(const string &filename = "points.json");
};

