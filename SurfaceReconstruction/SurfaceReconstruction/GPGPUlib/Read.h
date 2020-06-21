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
		cl_float minX = 0.;
		cl_float maxX = 0.;
		cl_float minY = 0.;
		cl_float maxY = 0.;
		cl_float minZ = 0.;
		cl_float maxZ = 0.;
	};

	void ReadPoint(const char* filename, SPointsData& outputData);
}

void GPGPUlib::ReadPoint(const char* filename, GPGPUlib::SPointsData& outputData)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	float num[3];


	std::getline(in, line);
	std::istringstream iss(line.c_str());
	for (int i = 0; i < 3; ++i)
	{
		iss >> num[i];
	}

	outputData.minX = outputData.minX = num[0];
	outputData.maxY = outputData.minY = num[1];
	outputData.maxZ = outputData.minZ = num[2];

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

		if (outputData.maxX < num[0])
		{
			outputData.maxX = num[0];
		}
		if (outputData.minX > num[0])
		{
			outputData.minX = num[0];
		}

		if (outputData.maxY < num[1])
		{
			outputData.maxY = num[1];
		}
		if (outputData.minY > num[1])
		{
			outputData.minY = num[1];
		}

		if (outputData.maxZ < num[2])
		{
			outputData.maxZ = num[2];
		}
		if (outputData.minZ > num[2])
		{
			outputData.minZ = num[2];
		}
	}
}