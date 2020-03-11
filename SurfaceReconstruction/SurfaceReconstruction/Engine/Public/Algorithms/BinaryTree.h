#pragma once

#include "CommonStruct.h"
#include <memory>

namespace Engine::Algorithm
{
	struct SBSPTNodeBase
	{
		SBSPTNodeBase() {};

		virtual void Search(double _SearchAxisVal, std::shared_ptr<SPoint3D> _point);

		virtual void SearchResult(std::shared_ptr<SPoint3D> _point) = 0;

	protected:

		virtual void Init() = 0;

		const int END_NODE_VAL = 1;

		int nodeVal;
		int offset;
		double nodeValInAxis;

		std::shared_ptr<SBSPTNodeBase> leftSubNode;
		std::shared_ptr<SBSPTNodeBase> rightSubNode;
	};

	class CBSPTBase
	{
		public:
			virtual void Search(double _SearchAxisVal, std::shared_ptr<SPoint3D> _point);

		protected:

			virtual void Init() = 0;

			std::shared_ptr<SBSPTNodeBase> rootNode;
	};
}