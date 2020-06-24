#pragma once

#include <CL/cl.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

namespace GPGPUlib
{
	struct SPointsData
	{
		std::vector<cl_float> pointsOfX = {};
		std::vector<cl_float> pointsOfY = {};
		std::vector<cl_float> pointsOfZ = {};
	};

	void ReadPoint(const char* filename, SPointsData& outputData);
}

void GPGPUlib::ReadPoint(const char* filename, GPGPUlib::SPointsData& outputData)
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

		outputData.pointsOfX.push_back(num[0]);
		outputData.pointsOfY.push_back(num[1]);
		outputData.pointsOfZ.push_back(num[2]);

	}
}