#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>

#include "Struct.h"


namespace my_sr_lib
{
	const double EPSILON = 1e-6;

	template<typename T>
	void read_xyz(const char* filename, std::vector<my_sr_lib::SPointXYZ<T>>& _points, SMinMaxPoint<T>& _minmax, std::size_t multiplicator = -1);

	template<typename T>
	void read_xyz(const char* filename, std::vector<my_sr_lib::SPointXYZ<T>>& _points, SMinMaxPoint<T>& _minmax, std::size_t multiplicator)
	{
		std::ifstream in;
		in.open(filename, std::ifstream::in);
		if (in.fail()) return;
		std::string line;
		double num[3];

		bool FirstMinMax = true;
		bool UseMultiplicator = multiplicator != -1;

		while (!in.eof()) {
			std::getline(in, line);
			std::istringstream iss(line.c_str());
			for (int i = 0; i < 3; ++i)
			{
				iss >> num[i];
			}

			T x_, y_, z_;
			if (UseMultiplicator)
			{
				x_ = static_cast<T>(num[0] * multiplicator);
				y_ = static_cast<T>(num[1] * multiplicator);
				z_ = static_cast<T>(num[2] * multiplicator);
			}
			else
			{
				x_ = static_cast<T>(num[0]);
				y_ = static_cast<T>(num[1]);
				z_ = static_cast<T>(num[2]);
			}

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

			_points.push_back({ x_, y_, z_ });
		}
	}
}
