
#include "File/ReadPoints.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>

using namespace Engine::File;

const double EPSILON = 1e-6;

void CReadPoints::ParseFromText(const char* filename, std::vector<Engine::Algorithm::SPoint3D>& _points, const int32_t _MULTIPLICATOR /* = 10000*/)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	double num[3];

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		for (int i = 0; i < 3; ++i)
		{
			iss >> num[i];
		}

		_points.push_back(std::move(Engine::Algorithm::SPoint3D(static_cast<int32_t>(num[0] * _MULTIPLICATOR), static_cast<int32_t>(num[1] * _MULTIPLICATOR), static_cast<int32_t>(num[2] * _MULTIPLICATOR))));
	}
}
