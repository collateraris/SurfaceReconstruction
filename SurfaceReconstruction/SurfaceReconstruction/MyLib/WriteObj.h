#pragma once

#include <list>
#include <vector>
#include <iostream>
#include <fstream>

#include "DelaunayTriangulation.h"

namespace my_sr_lib
{
	template<typename T>
	void print_triangulation_as_obj(const char* filename, std::vector<std::list<DelaunayTriangulation::STriangle<T>>>& triangulationLists);


	template<typename T>
	void print_triangulation_as_obj(const char* filename, std::vector<std::list<DelaunayTriangulation::STriangle<T>>>& triangulationLists)
	{
		std::ofstream out;
		out.open(filename);

		if (out.is_open())
		{
			const int VERTICES_BATCH_NUMBER = 4;
			unsigned int offset = 0;
			for (auto& list: triangulationLists)
			{
				for (DelaunayTriangulation::STriangle<T>& triangle : list)
				{
					out << "v " << triangle.v1.originalPoints.x
						<< " " << triangle.v1.originalPoints.y
						<< " " << triangle.v1.originalPoints.z
						<< std::endl;

					out << "v " << triangle.v2.originalPoints.x
						<< " " << triangle.v2.originalPoints.y
						<< " " << triangle.v2.originalPoints.z
						<< std::endl;

					out << "v " << triangle.v3.originalPoints.x
						<< " " << triangle.v3.originalPoints.y
						<< " " << triangle.v3.originalPoints.z
						<< std::endl;

					out << "v " << triangle.v4.originalPoints.x
						<< " " << triangle.v4.originalPoints.y
						<< " " << triangle.v4.originalPoints.z
						<< std::endl;

					out << "f " << 1 + offset
						<< " " << 2 + offset
						<< " " << 3 + offset
						<< std::endl;

					out << "f " << 1 + offset
						<< " " << 2 + offset
						<< " " << 4 + offset
						<< std::endl;

					out << "f " << 1 + offset
						<< " " << 3 + offset
						<< " " << 4 + offset
						<< std::endl;

					out << "f " << 2 + offset
						<< " " << 3 + offset
						<< " " << 4 + offset
						<< std::endl;

					offset += VERTICES_BATCH_NUMBER;
				}
			}
		}

		out.close();
	}
}