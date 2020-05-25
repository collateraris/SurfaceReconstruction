#pragma once

#include <vector>

namespace my_sr_lib
{
    template<typename T>
	struct SPointXYZ
	{
		T x;
		T y;
		T z;
	};

	template<typename T>
	struct SPoint2D
	{
		T x;
		T y;
	};

    template<typename T>
	struct SMinMaxPoint
	{
		T maxX;
		T minX;
		T maxY;
		T minY;
		T maxZ;
		T minZ;
	};

	template<typename T>
	struct SBox2D
	{
		T maxX;
		T minX;
		T maxY;
		T minY;
	};

}