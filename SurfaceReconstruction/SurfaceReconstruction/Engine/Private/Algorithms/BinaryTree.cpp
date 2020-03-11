#include "Algorithms/BinaryTree.h"

using namespace Engine::Algorithm;


void SBSPTNodeBase::Search(double _SearchAxisVal, std::shared_ptr<SPoint3D> _point)
{
	if (nodeVal == END_NODE_VAL)
	{
		SearchResult(_point);
	}
	else if ( _SearchAxisVal <= nodeValInAxis)
	{
		leftSubNode->Search(_SearchAxisVal, _point);
	}
	else
	{
		rightSubNode->Search(_SearchAxisVal, _point);
	}
}

void CBSPTBase::Search(double _SearchAxisVal, std::shared_ptr<SPoint3D> _point)
{
	rootNode->Search(_SearchAxisVal, _point);
}