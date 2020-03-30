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

int32_t SVoxelData::PushSubVoxelData(int32_t _startX, int32_t _startY, int32_t _startZ, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
{
	int32_t startX = _startX;
	int32_t startY = _startY;
	int32_t startZ = _startZ;
	int32_t cubeSizeX = _cubeSizeX;
	int32_t cubeSizeY = _cubeSizeY;
	int32_t cubeSizeZ = _cubeSizeZ;

	int32_t cubeHalfSizeX = cubeSizeX >> 1;
	int32_t cubeHalfSizeY = cubeSizeY >> 1;
	int32_t cubeHalfSizeZ = cubeSizeZ >> 1;
  
	SSubVoxelData voxelData;

	voxelData.mV0 = std::move(SVertexCubeField_0(startX, startY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	voxelData.mV1 = std::move(SVertexCubeField_1(startX, startY + cubeSizeY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	voxelData.mV2 = std::move(SVertexCubeField_2(startX + cubeSizeX, startY + cubeSizeY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	voxelData.mV3 = std::move(SVertexCubeField_3(startX + cubeSizeX, startY, startZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	voxelData.mV4 = std::move(SVertexCubeField_4(startX, startY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	voxelData.mV5 = std::move(SVertexCubeField_5(startX, startY + cubeSizeY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	voxelData.mV6 = std::move(SVertexCubeField_6(startX + cubeSizeX, startY + cubeSizeY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));
	voxelData.mV7 = std::move(SVertexCubeField_7(startX + cubeSizeX, startY, startZ + cubeSizeZ, cubeHalfSizeX, cubeHalfSizeY, cubeHalfSizeZ));

	voxels.push_back(std::move(voxelData));

	return voxels.size() - 1;
}

void SVoxelData::AttachMeshSpace(const SPoint3D& _point, int32_t _subVoxelIndex)
{
	SSubVoxelData& subVoxel = voxels[_subVoxelIndex];

	if (!subVoxel.mV0.IsMesh() && subVoxel.mV0.IsContainPoint(_point))
	{
		subVoxel.mV0.IncludeInMesh();
		return;
	}

	if (!subVoxel.mV1.IsMesh() && subVoxel.mV1.IsContainPoint(_point))
	{
		subVoxel.mV1.IncludeInMesh();
		return;
	}

	if (!subVoxel.mV2.IsMesh() && subVoxel.mV2.IsContainPoint(_point))
	{
		subVoxel.mV2.IncludeInMesh();
		return;
	}

	if (!subVoxel.mV3.IsMesh() && subVoxel.mV3.IsContainPoint(_point))
	{
		subVoxel.mV3.IncludeInMesh();
		return;
	}


	if (!subVoxel.mV4.IsMesh() && subVoxel.mV4.IsContainPoint(_point))
	{
		subVoxel.mV4.IncludeInMesh();
		return;
	}

	if (!subVoxel.mV5.IsMesh() && subVoxel.mV5.IsContainPoint(_point))
	{
		subVoxel.mV5.IncludeInMesh();
		return;
	}

	if (!subVoxel.mV6.IsMesh() && subVoxel.mV6.IsContainPoint(_point))
	{
		subVoxel.mV6.IncludeInMesh();
		return;
	}

	if (!subVoxel.mV7.IsMesh() && subVoxel.mV7.IsContainPoint(_point))
	{
		subVoxel.mV7.IncludeInMesh();
		return;
	}
}
