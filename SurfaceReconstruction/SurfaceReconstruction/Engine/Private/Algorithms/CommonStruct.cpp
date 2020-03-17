#include "Algorithms/CommonStruct.h"

#include <iostream>
#include <fstream>

using namespace Engine::Algorithm;

std::list<SVertexVoxelUnit> g_storageVoxels;

int CCommonStruct::GetSizeVoxelList()
{
	return g_storageVoxels.size();
}

void CCommonStruct::PrintVoxelsInObj(const char* filename)
{
	std::ofstream out;
	out.open(filename);

	if (out.is_open())
	{
		const int VERTICES_BATCH_NUMBER = 8;
		unsigned int offset = 0;
		for (auto& it : g_storageVoxels)
		{
			for (auto& v : it.vertices)
			{
				out << "v " << v.GetX()
					<< " "  << v.GetY()
					<< " "  << v.GetZ()
					<< std::endl;
			}

			out << "f " << 1 + offset
				<< " " << 2 + offset
				<< " " << 3 + offset
				<< " " << 4 + offset
				<< std::endl;

			out << "f " << 1 + offset
				<< " " << 2 + offset
				<< " " << 6 + offset
				<< " " << 5 + offset
				<< std::endl;

			out << "f " << 1 + offset
				<< " " << 5 + offset
				<< " " << 8 + offset
				<< " " << 4 + offset
				<< std::endl;

			out << "f " << 2 + offset
				<< " " << 6 + offset
				<< " " << 7 + offset
				<< " " << 3 + offset
				<< std::endl;

			out << "f " << 5 + offset
				<< " " << 6 + offset
				<< " " << 7 + offset
				<< " " << 8 + offset
				<< std::endl;

			out << "f " << 4 + offset
				<< " " << 3 + offset
				<< " " << 7 + offset
				<< " " << 8 + offset
				<< std::endl;

			offset += VERTICES_BATCH_NUMBER;
		}

		std::cout << "Obj file Done!" << std::endl;
	}
	else
	{
		std::cout << "Error open obj file!" << std::endl;
	}
}

bool SVertexCubeField_0::IsContainPoint(const SPoint3D& _point)
{
	if ( x <= _point.GetX() && _point.GetX() < (x + cubeSizeX)
	  && y <= _point.GetY() && _point.GetY() < (y + cubeSizeY)
	  && z <= _point.GetZ() &&	_point.GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_0::MeshStorageData()
{
	IncludeInMesh();
	SVertexVoxelUnit unit;
	unit.vertices.push_back(std::move(SPoint3D(x, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x, y + cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y + cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x, y, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y + cubeSizeY, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y + cubeSizeY, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y, z + cubeSizeZ)));
	g_storageVoxels.push_back(std::move(unit));
}

bool SVertexCubeField_1::IsContainPoint(const SPoint3D& _point)
{
	if (x <= _point.GetX() && _point.GetX() < (x + cubeSizeX)
		&& y >= _point.GetY() && _point.GetY() > (y - cubeSizeY)
		&& z <= _point.GetZ() && _point.GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_1::MeshStorageData()
{
	IncludeInMesh();
	SVertexVoxelUnit unit;
	unit.vertices.push_back(std::move(SPoint3D(x, y - cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y - cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x, y - cubeSizeY, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y - cubeSizeY, z + cubeSizeZ)));
	g_storageVoxels.push_back(std::move(unit));
}

bool SVertexCubeField_2::IsContainPoint(const SPoint3D& _point)
{
	if (x >= _point.GetX() && _point.GetX() > (x - cubeSizeX)
		&& y >= _point.GetY() && _point.GetY() > (y - cubeSizeY)
		&& z <= _point.GetZ() && _point.GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_2::MeshStorageData()
{
	IncludeInMesh();
	SVertexVoxelUnit unit;
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y - cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x, y , z)));
	unit.vertices.push_back(std::move(SPoint3D(x , y - cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x- cubeSizeX, y - cubeSizeY, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x , y , z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y - cubeSizeY, z + cubeSizeZ)));
	g_storageVoxels.push_back(std::move(unit));
}

bool SVertexCubeField_3::IsContainPoint(const SPoint3D& _point)
{
	if (x >= _point.GetX() && _point.GetX() > (x - cubeSizeX)
		&& y <= _point.GetY() && _point.GetY() < (y + cubeSizeY)
		&& z <= _point.GetZ() && _point.GetZ() < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_3::MeshStorageData()
{
	IncludeInMesh();
	SVertexVoxelUnit unit;
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y + cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x , y + cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x , y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y + cubeSizeY, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y + cubeSizeY, z + cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y, z + cubeSizeZ)));
	g_storageVoxels.push_back(std::move(unit));
}

bool SVertexCubeField_4::IsContainPoint(const SPoint3D& _point)
{
	if (x <= _point.GetX() && _point.GetX() < (x + cubeSizeX)
		&& y <= _point.GetY() && _point.GetY() < (y + cubeSizeY)
		&& z >= _point.GetZ() && _point.GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_4::MeshStorageData()
{
	IncludeInMesh();
}

bool SVertexCubeField_5::IsContainPoint(const SPoint3D& _point)
{
	if (x <= _point.GetX() && _point.GetX() < (x + cubeSizeX)
		&& y >= _point.GetY() && _point.GetY() > (y - cubeSizeY)
		&& z >= _point.GetZ() && _point.GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_5::MeshStorageData()
{
	IncludeInMesh();
	SVertexVoxelUnit unit;
	unit.vertices.push_back(std::move(SPoint3D(x, y - cubeSizeY, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y , z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y - cubeSizeY, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y - cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x, y , z)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x + cubeSizeX, y - cubeSizeY, z)));
	g_storageVoxels.push_back(std::move(unit));
}

bool SVertexCubeField_6::IsContainPoint(const SPoint3D& _point)
{
	if (x >= _point.GetX() && _point.GetX() > (x - cubeSizeX)
		&& y >= _point.GetY() && _point.GetY() > (y - cubeSizeY)
		&& z >= _point.GetZ() && _point.GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_6::MeshStorageData()
{
	IncludeInMesh();
	SVertexVoxelUnit unit;
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y - cubeSizeY, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x, y - cubeSizeY, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y - cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x , y , z )));
	unit.vertices.push_back(std::move(SPoint3D(x, y - cubeSizeY, z)));
	g_storageVoxels.push_back(std::move(unit));
}

bool SVertexCubeField_7::IsContainPoint(const SPoint3D& _point)
{
	if (x >= _point.GetX() && _point.GetX() > (x - cubeSizeX)
		&& y <= _point.GetY() && _point.GetY() < (y + cubeSizeY)
		&& z >= _point.GetZ() && _point.GetZ() > (z - cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SVertexCubeField_7::MeshStorageData()
{
	IncludeInMesh();
	SVertexVoxelUnit unit;
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y, z- cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y + cubeSizeY, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x , y + cubeSizeY, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x , y, z - cubeSizeZ)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y, z)));
	unit.vertices.push_back(std::move(SPoint3D(x - cubeSizeX, y + cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x, y + cubeSizeY, z)));
	unit.vertices.push_back(std::move(SPoint3D(x , y, z )));
	g_storageVoxels.push_back(std::move(unit));
}

SMarchingCube::SMarchingCube(int32_t _startX, int32_t _startY, int32_t _startZ, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
	: startX{_startX}
	, startY{_startY}
	, startZ{_startZ}
	, cubeSizeX{_cubeSizeX}
	, cubeSizeY{ _cubeSizeY }
	, cubeSizeZ{ _cubeSizeZ }
{
	int32_t cubeHalfSizeX = cubeSizeX >> 1;
	int32_t cubeHalfSizeY = cubeSizeY >> 1;
	int32_t cubeHalfSizeZ = cubeSizeZ >> 1;

	bAllMeshFound = false;

	mV0 = std::move(SVertexCubeField_0(startX, startY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mV1 = std::move(SVertexCubeField_1(startX, startY + cubeSizeY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mV2 = std::move(SVertexCubeField_2(startX + cubeSizeX, startY + cubeSizeY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mV3 = std::move(SVertexCubeField_3(startX + cubeSizeX, startY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mV4 = std::move(SVertexCubeField_4(startX, startY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mV5 = std::move(SVertexCubeField_5(startX, startY + cubeSizeY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mV6 = std::move(SVertexCubeField_6(startX + cubeSizeX, startY + cubeSizeY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	mV7 = std::move(SVertexCubeField_7(startX + cubeSizeX, startY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
}

void SMarchingCube::FillMeshSubSpace(const SPoint3D& _point)
{
	if (bAllMeshFound)
	{
		return;
	}

	if (!mV0.IsMesh() && mV0.IsContainPoint(_point))
	{
		mV0.MeshStorageData();
		return;
	}

	if (!mV1.IsMesh() && mV1.IsContainPoint(_point))
	{
		mV1.MeshStorageData();
		return;
	}

	if (!mV2.IsMesh() && mV2.IsContainPoint(_point))
	{
		mV2.MeshStorageData();
		return;
	}

	if (!mV3.IsMesh() && mV3.IsContainPoint(_point))
	{
		mV3.MeshStorageData();
		return;
	}


	if (!mV4.IsMesh() && mV4.IsContainPoint(_point))
	{
		mV4.MeshStorageData();
		return;
	}

	if (!mV5.IsMesh() && mV5.IsContainPoint(_point))
	{
		mV5.MeshStorageData();
		return;
	}

	if (!mV6.IsMesh() && mV6.IsContainPoint(_point))
	{
		mV6.MeshStorageData();
		return;
	}

	if (!mV7.IsMesh() && mV7.IsContainPoint(_point))
	{
		mV7.MeshStorageData();
		return;
	}
	bAllMeshFound = true;
}
