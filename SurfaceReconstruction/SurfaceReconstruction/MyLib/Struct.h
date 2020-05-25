#pragma once

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
	struct SMinMaxPoint
	{
		T maxX;
		T minX;
		T maxY;
		T minY;
		T maxZ;
		T minZ;
	};
}