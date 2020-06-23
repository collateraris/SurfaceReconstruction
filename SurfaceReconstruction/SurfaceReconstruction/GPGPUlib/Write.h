#pragma once

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.h>

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <list>

namespace GPGPUlib
{
	void PrintOBJ(const char* filename, const std::vector<cl_uint>& cube, int cubeSize = 1);

	void PrintOBJ(const char* filename, const std::unordered_map<cl_uint, std::list<cl_uint>>& triangulation, int cubeSize = 1);
}

void GPGPUlib::PrintOBJ(const char* filename, const std::vector<cl_uint>& cube, int cubeSize/* = 1*/)
{
	//int size = x * width * height + y * height + z;

	const int VERTICES_CUBE_NUMBER = 8;

	std::ofstream out;
	out.open(filename);

	if (!out.is_open())
	{
		std::cout << "Error open obj file!" << std::endl;
		return;
	}

	unsigned int offset = 0;
	for (cl_uint pos : cube)
	{
		if (pos == 0)
			continue;

		int x = pos / (cubeSize * cubeSize);
		int y = (pos - x * cubeSize * cubeSize)/ cubeSize;
		int z = pos - x * cubeSize * cubeSize - y * cubeSize;

		//v0
		out << "v " << x
			<< " " << y
			<< " " << z
			<< std::endl;
		
		//v1
		out << "v " << x
			<< " " << y + 1
			<< " " << z
			<< std::endl;

		//v2
		out << "v " << x + 1
			<< " " << y + 1
			<< " " << z
			<< std::endl;

		//v3
		out << "v " << x + 1
			<< " " << y
			<< " " << z
			<< std::endl;

		//v4
		out << "v " << x 
			<< " " << y
			<< " " << z + 1
			<< std::endl;

		//v5
		out << "v " << x
			<< " " << y + 1
			<< " " << z + 1
			<< std::endl;

		//v6
		out << "v " << x + 1
			<< " " << y + 1
			<< " " << z + 1
			<< std::endl;

		//v7
		out << "v " << x + 1
			<< " " << y
			<< " " << z + 1
			<< std::endl;
		out << "f " << 1 + offset
			<< " " << 2 + offset
			<< " " << 3 + offset
			<< " " << 4 + offset
			<< std::endl;

		out << "f " << 1 + offset
			<< " " << 2 + offset
			<< " " << 6 + offset
			<< " " << 5 + offset
			<< std::endl;

		out << "f " << 1 + offset
			<< " " << 5 + offset
			<< " " << 8 + offset
			<< " " << 4 + offset
			<< std::endl;

		out << "f " << 2 + offset
			<< " " << 6 + offset
			<< " " << 7 + offset
			<< " " << 3 + offset
			<< std::endl;

		out << "f " << 5 + offset
			<< " " << 6 + offset
			<< " " << 7 + offset
			<< " " << 8 + offset
			<< std::endl;

		out << "f " << 4 + offset
			<< " " << 3 + offset
			<< " " << 7 + offset
			<< " " << 8 + offset
			<< std::endl;

		offset += VERTICES_CUBE_NUMBER;

	}
	std::cout << "Obj file Done!" << std::endl;
	out.close();
}

void GPGPUlib::PrintOBJ(const char* filename, const std::unordered_map<cl_uint, std::list<cl_uint>>& triangulation, int cubeSize/* = 1*/)
{
	//int size = x * width * height + y * height + z;

	const int VERTICES_TRIANGLE_NUMBER = 3;

	std::ofstream out;
	out.open(filename);

	if (!out.is_open())
	{
		std::cout << "Error open obj file!" << std::endl;
		return;
	}

	unsigned int offset = 0;

	for (auto it = triangulation.begin(); it != triangulation.end(); ++it)
	{
		int v0 = (*it).first;
		int v1 = 0; 
		int v2 = 0;

		int v0x = v0 / (cubeSize * cubeSize);
		int v0y = (v0 - v0x * cubeSize * cubeSize) / cubeSize;
		int v0z = v0 - v0x * cubeSize * cubeSize - v0y * cubeSize;
		for (const cl_uint& vertices: (*it).second)
		{
			v1 = vertices;
			if (v2 == 0)
			{
				v2 = v1;
				continue;
			}

			//v0
			out << "v " << v0x
				<< " " << v0y
				<< " " << v0z
				<< std::endl;

			int v1x = v1 / (cubeSize * cubeSize);
			int v1y = (v1 - v1x * cubeSize * cubeSize) / cubeSize;
			int v1z = v1 - v1x * cubeSize * cubeSize - v1y * cubeSize;

			//v1
			out << "v " << v1x
				<< " " << v1y
				<< " " << v1z
				<< std::endl;

			int v2x = v2 / (cubeSize * cubeSize);
			int v2y = (v2 - v2x * cubeSize * cubeSize) / cubeSize;
			int v2z = v2 - v2x * cubeSize * cubeSize - v2y * cubeSize;

			//v2
			out << "v " << v2x
				<< " " << v2y
				<< " " << v2z
				<< std::endl;

			out << "f " << 1 + offset
				<< " " << 2 + offset
				<< " " << 3 + offset
				<< std::endl;

			offset += VERTICES_TRIANGLE_NUMBER;

			v2 = v1;
		}

		v1 = v2;
		//v0
		out << "v " << v0x
			<< " " << v0y
			<< " " << v0z
			<< std::endl;

		int v1x = v1 / (cubeSize * cubeSize);
		int v1y = (v1 - v1x * cubeSize * cubeSize) / cubeSize;
		int v1z = v1 - v1x * cubeSize * cubeSize - v1y * cubeSize;

		//v1
		out << "v " << v1x
			<< " " << v1y
			<< " " << v1z
			<< std::endl;

		v2 = (*it).second.front();

		int v2x = v2 / (cubeSize * cubeSize);
		int v2y = (v2 - v2x * cubeSize * cubeSize) / cubeSize;
		int v2z = v2 - v2x * cubeSize * cubeSize - v2y * cubeSize;

		//v2
		out << "v " << v2x
			<< " " << v2y
			<< " " << v2z
			<< std::endl;

		out << "f " << 1 + offset
			<< " " << 2 + offset
			<< " " << 3 + offset
			<< std::endl;

		offset += VERTICES_TRIANGLE_NUMBER;
	}

	std::cout << "Obj file Done!" << std::endl;
	out.close();
}