#include <iostream> 
#include "PointsClassifier.h"
using namespace std;

inline bool exists_file(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}

inline string get_filename_without_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    size_t lastbackslash = filename.find_last_of("/");
	if (lastbackslash == std::string::npos) 
	{
		lastbackslash = 0;
	}
    if (lastdot == std::string::npos) return filename;
    return filename.substr(lastbackslash, lastdot); 
}



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

void convertImageToPointsJson(const string& imagename)
{
	const string jsonfile = get_filename_without_extension(imagename) + ".json";
	PointsClassifier pc(imagename);
	pc.findAllPointsGroup();
	pc.writePointsJsonInFile(jsonfile);
}

int main(int argc, char** argv)
{

	if (argc > 1) {
		string filepath = argv[1];
		convertImageToPointsJson(filepath);
	} else {
		cout << "no image is input!!" << endl;
	}
	
	return 0;
}
