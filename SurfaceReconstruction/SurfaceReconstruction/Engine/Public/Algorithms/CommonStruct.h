#pragma once

namespace Engine::Algorithm
{
	struct SPoint3D
	{
		SPoint3D() = delete;

		SPoint3D(double _x, double _y, double _z)
			: x{_x}
			, y{_y}
			, z{_z}
		{};

		double GetX() { return x; };
		double GetY() { return y; };
		double GetZ() { return z; };

	private:

		double x;
		double y;
		double z;
	};


}
