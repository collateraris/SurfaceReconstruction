
#include "File/ReadPoints.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>

using namespace Engine::File;

const double EPSILON = 1e-6;

void CReadPoints::ParseFromText(const char* filename, std::vector<Engine::Algorithm::SPoint3D>& _points, Engine::Algorithm::SMinMaxPoint& _minmax, const int32_t _MULTIPLICATOR /* = 10000*/)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	double num[3];

	bool FirstMinMax = true;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		for (int i = 0; i < 3; ++i)
		{
			iss >> num[i];
		}

		int32_t x_ = static_cast<int32_t>(num[0] * _MULTIPLICATOR);
		int32_t y_ = static_cast<int32_t>(num[1] * _MULTIPLICATOR);
		int32_t z_ = static_cast<int32_t>(num[2] * _MULTIPLICATOR);

		if (FirstMinMax)
		{
			FirstMinMax = false;
			_minmax.maxX = _minmax.minX = x_;
			_minmax.maxY = _minmax.minY = y_;
			_minmax.maxZ = _minmax.minZ = z_;
		}

		if (_minmax.maxX < x_)
		{
			_minmax.maxX = x_;
		} 
		else if (_minmax.minX > x_)
		{
			_minmax.minX = x_;
		}

		if (_minmax.maxY < y_)
		{
			_minmax.maxY = y_;
		}
		else if (_minmax.minY > y_)
		{
			_minmax.minY = y_;
		}

		if (_minmax.maxZ < z_)
		{
			_minmax.maxZ = z_;
		}
		else if (_minmax.minZ > z_)
		{
			_minmax.minZ = z_;
		}

		_points.push_back(std::move(Engine::Algorithm::SPoint3D(x_, y_, z_)));
	}
}
