#include "Algorithms/CommonStruct.h"

using namespace Engine::Algorithm;

bool SVertexCubeField_0::IsContainPoint(std::shared_ptr<SPoint3D> _point)
{
	if ( x <= _point->GetX() && _point->GetX() < (x + cubeSizeX)
	  && y <= _point->GetY() && _point->GetY() < (y + cubeSizeY)
	  && z <= _point->GetZ() &&	_point->GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

bool SVertexCubeField_1::IsContainPoint(std::shared_ptr<SPoint3D> _point)
{
	if (x <= _point->GetX() && _point->GetX() < (x + cubeSizeX)
		&& y >= _point->GetY() && _point->GetY() > (y - cubeSizeY)
		&& z <= _point->GetZ() && _point->GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

bool SVertexCubeField_2::IsContainPoint(std::shared_ptr<SPoint3D> _point)
{
	if (x >= _point->GetX() && _point->GetX() > (x - cubeSizeX)
		&& y >= _point->GetY() && _point->GetY() > (y - cubeSizeY)
		&& z <= _point->GetZ() && _point->GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

bool SVertexCubeField_3::IsContainPoint(std::shared_ptr<SPoint3D> _point)
{
	if (x >= _point->GetX() && _point->GetX() > (x - cubeSizeX)
		&& y <= _point->GetY() && _point->GetY() < (y + cubeSizeY)
		&& z <= _point->GetZ() && _point->GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

bool SVertexCubeField_4::IsContainPoint(std::shared_ptr<SPoint3D> _point)
{
	if (x <= _point->GetX() && _point->GetX() < (x + cubeSizeX)
		&& y <= _point->GetY() && _point->GetY() < (y + cubeSizeY)
		&& z >= _point->GetZ() && _point->GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

bool SVertexCubeField_5::IsContainPoint(std::shared_ptr<SPoint3D> _point)
{
	if (x <= _point->GetX() && _point->GetX() < (x + cubeSizeX)
		&& y >= _point->GetY() && _point->GetY() > (y - cubeSizeY)
		&& z >= _point->GetZ() && _point->GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

bool SVertexCubeField_6::IsContainPoint(std::shared_ptr<SPoint3D> _point)
{
	if (x >= _point->GetX() && _point->GetX() > (x - cubeSizeX)
		&& y >= _point->GetY() && _point->GetY() > (y - cubeSizeY)
		&& z >= _point->GetZ() && _point->GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

bool SVertexCubeField_7::IsContainPoint(std::shared_ptr<SPoint3D> _point)
{
	if (x >= _point->GetX() && _point->GetX() > (x - cubeSizeX)
		&& y <= _point->GetY() && _point->GetY() < (y + cubeSizeY)
		&& z >= _point->GetZ() && _point->GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}


