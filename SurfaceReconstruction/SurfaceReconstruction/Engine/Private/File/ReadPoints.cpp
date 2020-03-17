
#include "File/ReadPoints.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>

using namespace Engine::File;

const double EPSILON = 1e-6;

void CReadPoints::ParseFromText(const char* filename, std::vector<std::shared_ptr<Engine::Algorithm::SPoint3D>>& _points, const int32_t _MULTIPLICATOR /* = 0*/)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	float_t num[3];

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		for (int i = 0; i < 3; ++i)
		{
			iss >> num[i];
		}
		if (_MULTIPLICATOR)
		{
			_points.push_back(std::make_shared<Engine::Algorithm::SPoint3D>(num[0] * _MULTIPLICATOR, num[1] * _MULTIPLICATOR, num[2] * _MULTIPLICATOR));
		}
		else
		{
			_points.push_back(std::make_shared<Engine::Algorithm::SPoint3D>(num[0] , num[1], num[2]));
		}
	}
}
