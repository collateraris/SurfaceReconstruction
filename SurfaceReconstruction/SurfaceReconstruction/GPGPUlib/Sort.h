#pragma once

#include "Read.h" 

#include <algorithm>

namespace GPGPUlib
{
	struct SPoint
	{
		cl_float x;
		cl_float y;
		cl_float z;
	};

	void IntervalSort(SPointsData& sortedData, int cubeSize);

	bool IntervalSortChecker(SPointsData& sortedData, int cubeSize);
}

void GPGPUlib::IntervalSort(GPGPUlib::SPointsData& sortedData, int cubeSize)
{
	std::vector<GPGPUlib::SPoint> points;
	for (int i = 0; i < sortedData.pointsOfX.size(); ++i)
	{
		float x = sortedData.pointsOfX[i];
		float y = sortedData.pointsOfY[i];
		float z = sortedData.pointsOfZ[i];
		points.push_back({x, y, z});
	}

	//sorting by x

	std::sort(points.begin(), points.end(), [&](const SPoint& a, const SPoint& b)
	{
		return a.x < b.x;
	});

	{
		cl_float deltaX = sortedData.maxX - sortedData.minX;
		int prev_i = 0;
		int i;
		float step = sortedData.minX + deltaX;
		//sorting by y inner [x1, x2] chunks
		for (i = 0; i < cubeSize; ++i)
		{
			if (points[i].x > step)
			{
				std::sort(points.begin() + prev_i, points.begin() +(i - 1), [&](const SPoint& a, const SPoint& b)
				{
					return a.y < b.y;
				});
				prev_i = i;
				step += deltaX;
			}
		}

		std::sort(points.begin() + prev_i, points.begin() +(i - 1), [&](const SPoint& a, const SPoint& b)
		{
			return a.y < b.y;
		});
	}

	{
		cl_float deltaX = sortedData.maxX - sortedData.minX;
		cl_float deltaY = sortedData.maxY - sortedData.minY;
		float stepX = sortedData.minX + deltaX;
		for (int i = 0; i < cubeSize; ++i)
		{
			if (points[i].x > stepX)
			{
				int prev_j = 0;
				int j;
				cl_float stepY = sortedData.minY + deltaY;
				for (j = 0; j < cubeSize; ++j)
				{
					if (points[j].y > stepY)
					{
						std::sort(points.begin() + prev_j, points.begin() + (j - 1), [&](const SPoint& a, const SPoint& b)
						{
							return a.z < b.z;
						});
						prev_j = j;
						stepY += deltaY;
					}
				}

				std::sort(points.begin() + prev_j, points.begin() +(j - 1), [&](const SPoint& a, const SPoint& b)
				{
					return a.z < b.z;
				});
			}
		}

	}

	for (int i = 0; i < points.size(); ++i)
	{
		sortedData.pointsOfX[i] = points[i].x;
		sortedData.pointsOfY[i] = points[i].y;
		sortedData.pointsOfZ[i] = points[i].z;
	}
}

bool GPGPUlib::IntervalSortChecker(GPGPUlib::SPointsData& sortedData, int cubeSize)
{
	cl_float deltaX = sortedData.maxX - sortedData.minX;
	cl_float deltaY = sortedData.maxY - sortedData.minY;
	float stepX = sortedData.minX + deltaX;
	for (int i = 0; i < cubeSize; ++i)
	{
		if (sortedData.pointsOfX[i] > stepX)
		{
			int prev_j = 0;
			int j;
			cl_float stepY = sortedData.minY + deltaY;
			for (j = 0; j < cubeSize; ++j)
			{
				if (sortedData.pointsOfY[j] > stepY)
				{
					for (int h = prev_j; h < j - 1; ++h)
					{
						if (sortedData.pointsOfZ[h] > sortedData.pointsOfZ[h + 1])
							return false;
					}
					prev_j = j;
					stepY += deltaY;
				}
			}
		}
	}
	return true;
}