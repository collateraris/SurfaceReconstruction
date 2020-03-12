#include "Algorithms/CommonStruct.h"

using namespace Engine::Algorithm;

bool SVertexCubeField_0::IsContainPoint(const std::shared_ptr<SPoint3D>& _point)
{
	if ( x <= _point->GetX() && _point->GetX() < (x + cubeSizeX)
	  && y <= _point->GetY() && _point->GetY() < (y + cubeSizeY)
	  && z <= _point->GetZ() &&	_point->GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_0::MeshStorageData()
{
	IncludeInMesh();
	SVertexVoxelUnit unit;
	unit.v0 = std::make_shared<SPoint3D>(x, y, z);
	unit.v1 = std::make_shared<SPoint3D>(x, y + cubeSizeY, z);
}

bool SVertexCubeField_1::IsContainPoint(const std::shared_ptr<SPoint3D>& _point)
{
	if (x <= _point->GetX() && _point->GetX() < (x + cubeSizeX)
		&& y >= _point->GetY() && _point->GetY() > (y - cubeSizeY)
		&& z <= _point->GetZ() && _point->GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_1::MeshStorageData()
{
	IncludeInMesh();
}

bool SVertexCubeField_2::IsContainPoint(const std::shared_ptr<SPoint3D>& _point)
{
	if (x >= _point->GetX() && _point->GetX() > (x - cubeSizeX)
		&& y >= _point->GetY() && _point->GetY() > (y - cubeSizeY)
		&& z <= _point->GetZ() && _point->GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_2::MeshStorageData()
{
	IncludeInMesh();
}

bool SVertexCubeField_3::IsContainPoint(const std::shared_ptr<SPoint3D>& _point)
{
	if (x >= _point->GetX() && _point->GetX() > (x - cubeSizeX)
		&& y <= _point->GetY() && _point->GetY() < (y + cubeSizeY)
		&& z <= _point->GetZ() && _point->GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_3::MeshStorageData()
{
	IncludeInMesh();
}

bool SVertexCubeField_4::IsContainPoint(const std::shared_ptr<SPoint3D>& _point)
{
	if (x <= _point->GetX() && _point->GetX() < (x + cubeSizeX)
		&& y <= _point->GetY() && _point->GetY() < (y + cubeSizeY)
		&& z >= _point->GetZ() && _point->GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_4::MeshStorageData()
{
	IncludeInMesh();
}

bool SVertexCubeField_5::IsContainPoint(const std::shared_ptr<SPoint3D>& _point)
{
	if (x <= _point->GetX() && _point->GetX() < (x + cubeSizeX)
		&& y >= _point->GetY() && _point->GetY() > (y - cubeSizeY)
		&& z >= _point->GetZ() && _point->GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_5::MeshStorageData()
{
	IncludeInMesh();
}

bool SVertexCubeField_6::IsContainPoint(const std::shared_ptr<SPoint3D>& _point)
{
	if (x >= _point->GetX() && _point->GetX() > (x - cubeSizeX)
		&& y >= _point->GetY() && _point->GetY() > (y - cubeSizeY)
		&& z >= _point->GetZ() && _point->GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_6::MeshStorageData()
{
	IncludeInMesh();
}

bool SVertexCubeField_7::IsContainPoint(const std::shared_ptr<SPoint3D>& _point)
{
	if (x >= _point->GetX() && _point->GetX() > (x - cubeSizeX)
		&& y <= _point->GetY() && _point->GetY() < (y + cubeSizeY)
		&& z >= _point->GetZ() && _point->GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_7::MeshStorageData()
{
	IncludeInMesh();
}

SMarchingCube::SMarchingCube(double _startX, double _startY, double _startZ, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
	: startX{_startX}
	, startY{_startY}
	, startZ{_startZ}
	, cubeSizeX{_cubeSizeX}
	, cubeSizeY{ _cubeSizeY }
	, cubeSizeZ{ _cubeSizeZ }
{
	double cubeHalfSizeX = cubeSizeX * 0.5;
	double cubeHalfSizeY = cubeSizeY * 0.5;
	double cubeHalfSizeZ = cubeSizeZ * 0.5;

	bAllMeshFound = false;

	mpVertices.push_back(std::make_shared<SVertexCubeField_0>(startX, startY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mpVertices.push_back(std::make_shared<SVertexCubeField_1>(startX, startY + cubeSizeY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mpVertices.push_back(std::make_shared<SVertexCubeField_2>(startX + cubeSizeX, startY + cubeSizeY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mpVertices.push_back(std::make_shared<SVertexCubeField_3>(startX + cubeSizeX, startY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mpVertices.push_back(std::make_shared<SVertexCubeField_4>(startX, startY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mpVertices.push_back(std::make_shared<SVertexCubeField_5>(startX, startY + cubeSizeY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mpVertices.push_back(std::make_shared<SVertexCubeField_6>(startX + cubeSizeX, startY + cubeSizeY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mpVertices.push_back(std::make_shared<SVertexCubeField_7>(startX + cubeSizeX, startY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
}

void SMarchingCube::FillMeshSubSpace(const std::shared_ptr<SPoint3D>& _point)
{
	if (bAllMeshFound)
	{
		return;
	}

	for (auto& pV : mpVertices)
	{
		if (!pV->IsMesh() && pV->IsContainPoint(_point))
		{
			pV->MeshStorageData();
			return;
		}
	}

	bAllMeshFound = true;
}
