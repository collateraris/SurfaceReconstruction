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
		static void ParseFromText(const char* filename, std::vector<std::shared_ptr<Engine::Algorithm::SPoint3D>>& _points);
	};
}