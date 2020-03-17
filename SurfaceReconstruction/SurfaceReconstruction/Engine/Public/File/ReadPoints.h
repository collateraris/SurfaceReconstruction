#pragma once

#include "Algorithms/CommonStruct.h"
#include <vector>
#include <string>
#include <memory>

namespace Engine::File
{
	class CReadPoints
	{
	public:
		static void ParseFromText(const char* filename, std::vector<Engine::Algorithm::SPoint3D>& _points, const int32_t _MULTIPLICATOR = 10000);
	};
}