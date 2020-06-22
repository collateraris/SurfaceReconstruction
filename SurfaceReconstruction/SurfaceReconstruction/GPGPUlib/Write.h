#pragma once

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.h>

#include <iostream>
#include <fstream>

namespace GPGPUlib
{
	void PrintOBJ(const char* filename, const std::vector<cl_uint>& cube, int cubeSize = 1);
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