#include "stdafx.h"
#include "PointsClassifier.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>

namespace
{
	string PointToString(const Point &p)
	{
		ostringstream ss;
		ss << setfill('0') << setw(4) << p.x << setfill('0') << setw(4) << p.y;

		return ss.str();

	}

	bool checkPointInGroup(const string &key, const unordered_map<string, Point>& grouppedPoints)
	{
		unordered_map<string, Point>::const_iterator got = grouppedPoints.find(key);

		if (got == grouppedPoints.end())
		{
			return false;
		}

		return true;
	}

	string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}

}
PointsClassifier::PointsClassifier(const Mat & img) : _img(img), _imgH(img.size().height), _imgW(img.size().width)
{
	_cal_img_space = Mat(_imgH + 2, _imgW + 2, CV_8UC1, Scalar(0));
	for (int y = 0; y < _imgH; y++) {
		for (int x = 0; x < _imgW; x++) {
			if (_img.at<uchar>(y, x) == 255) {
				_cal_img_space.at<uchar>(y + 1, x + 1) = 1;
				Point tmp_p = Point(x, y);
				allPoints.insert({ PointToString(tmp_p),tmp_p });
			}
		}
	}
}

PointsClassifier::~PointsClassifier()
{
}

void PointsClassifier::setCalNeighborsOfPoint(Point p, Point* points)
{
	const int x = p.x + 1;
	const int y = p.y + 1;

	points[0].x = x - 1;
	points[0].y = y - 1;

	points[1].x = x;
	points[1].y = y - 1;

	points[2].x = x + 1;
	points[2].y = y - 1;

	points[3].x = x - 1;
	points[3].y = y;
	points[4].x = x + 1;
	points[4].y = y;

	points[5].x = x - 1;
	points[5].y = y + 1;

	points[6].x = x;
	points[6].y = y + 1;

	points[7].x = x + 1;
	points[7].y = y + 1;
}

vector<Point>  PointsClassifier::findRailwayNeighborsOfPoint(const Point& center_p)
{
	string p_key = PointToString(center_p);

	// cal center point
	allPoints.erase(p_key);
	_cal_img_space.at<uchar>(center_p.y + 1, center_p.x + 1) = 0;

	// cal Neighbor points
	vector<Point> new_neighbors;
	Point* points = new Point[8];
	setCalNeighborsOfPoint(center_p, points);
	for (int i = 0; i < 8; i++)
	{
		Point p = points[i];
		Point orig_p = Point(p.x - 1, p.y - 1);
		p_key = PointToString(orig_p);
		if (_cal_img_space.at<uchar>(p.y, p.x))
		{
			new_neighbors.push_back(orig_p);
			allPoints.erase(p_key);
			_cal_img_space.at<uchar>(p.y, p.x) = 0;
		}

	}
	delete[] points;

	return new_neighbors;


}

vector<Point> PointsClassifier::findOneGroupPoints(Point start_p)
{

	cout << " begin to find one realway at point: x " << start_p.x << ", y " << start_p.y << endl;

	vector<Point> grouppedPoints;

	vector<Point> neighbors{ start_p };
	grouppedPoints.push_back(start_p);

	while (neighbors.size() > 0) {
		vector<Point> new_neighbors;
		thread *t = new thread[neighbors.size()];
		for (auto & point : neighbors) {
			vector<Point> results = findRailwayNeighborsOfPoint(point);
			new_neighbors.insert(new_neighbors.end(), results.begin(), results.end());
		}
		grouppedPoints.insert(grouppedPoints.end(), new_neighbors.begin(), new_neighbors.end());
		neighbors = new_neighbors;
	}

	cout << " one realway found with size  " << grouppedPoints.size() << endl;

	return grouppedPoints;

}

void  PointsClassifier::findAllPointsGroup()
{
	if (allPoints.size() > 0)
	{
		while (allPoints.size() > 0) {
			cout << allPoints.size() << " railway points left! " << endl;
			Point start_p = allPoints.begin()->second;
			vector<Point> grouppedPoints = findOneGroupPoints(start_p);
			pointsGroups.push_back(grouppedPoints);

		}
	}
	else
	{
		cout << " no railway points found! " << endl;
	}
}

vector<vector<Point>> PointsClassifier::getPointsGroups()
{
	return pointsGroups;
}

void PointsClassifier::writePointsJsonInFile(const string & filename)
{
	cout << "Beginning to write Point Json into "<< filename << endl;

	ostringstream ss;
	ss << "{ \"groups\":[";

	if (pointsGroups.size() > 0) {
		for (auto const& groupPoints : pointsGroups) {
			ss << "{ \"group\":[";
			for (auto const& point : groupPoints)
			{
				ss << "{ \"x\":" << point.x << ",\"y\":" << point.y << "},";
			}
			ss << "]},";
		}
	}

	ss << "]}";

	string jsonstring = ReplaceAll(ss.str(), ",]", "]");
	ofstream myfile;
	myfile.open(filename);
	myfile << jsonstring;
	myfile.close();

	cout << "Points Json are already wrritten to file!" << endl;
}
