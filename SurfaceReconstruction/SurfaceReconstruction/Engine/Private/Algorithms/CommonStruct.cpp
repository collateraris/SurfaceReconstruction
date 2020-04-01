#include "Algorithms/CommonStruct.h"

#include <iostream>
#include <fstream>

using namespace Engine::Algorithm;

void CCommonStruct::PrintVoxelsInObj(const char* filename, std::list<SVertexVoxelUnit>& storageVoxels)
{
	std::ofstream out;
	out.open(filename);

	if (out.is_open())
	{
		const int VERTICES_BATCH_NUMBER = 8;
		unsigned int offset = 0;
		for (auto& it : storageVoxels)
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

bool SSubVoxelData::IsContainPoint(const SPoint3D& _point)
{
	int32_t pointX = _point.GetX();
	int32_t pointY = _point.GetY();
	int32_t pointZ = _point.GetZ();

	if ( x <= pointX && pointX < (x + cubeSizeX)
	  && y <= pointY && pointY < (y + cubeSizeY)
	  && z <= pointZ &&	pointZ < (z + cubeSizeZ))
	{
		return true;
	}
	return false;
}

void SSubVoxelData::MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj)
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
	allMeshesAsObj.push_back(std::move(unit));
}



int32_t SVoxelData::PushSubVoxelData(int32_t _startX, int32_t _startY, int32_t _startZ, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
{
	int32_t startX = _startX;
	int32_t startY = _startY;
	int32_t startZ = _startZ;
	int32_t cubeSizeX = _cubeSizeX;
	int32_t cubeSizeY = _cubeSizeY;
	int32_t cubeSizeZ = _cubeSizeZ;

	SSubVoxelData voxelData(startX, startY, startZ, cubeSizeX, cubeSizeY, cubeSizeZ);

	voxels.push_back(std::move(voxelData));

	return voxels.size() - 1;
}

void SVoxelData::AttachMeshSpace(const SPoint3D& _point, int32_t _subVoxelIndex)
{
	SSubVoxelData& subVoxel = voxels[_subVoxelIndex];
	if (!subVoxel.IsMesh() && subVoxel.IsContainPoint(_point))
	{
		subVoxel.IncludeInMesh();
	}
}

void SVoxelData::AttachMeshSpace(int32_t _subVoxelIndex)
{
	SSubVoxelData& subVoxel = voxels[_subVoxelIndex];
	subVoxel.IncludeInMesh();
}

void SVoxelData::GetAllMeshes(std::list<SVertexVoxelUnit>& allMeshesAsObj, int32_t _subVoxelIndex)
{
	SSubVoxelData& subVoxel = voxels[_subVoxelIndex];
	subVoxel.MeshStorageData(allMeshesAsObj);
}

bool SVoxelData::CompareGroup(std::shared_ptr<SVoxelData>& _srsData)
{
	if (&_srsData->groupHeader->groupHeader == &this->groupHeader->groupHeader)
	{
		return true;
	}
	return false;
}

void SVoxelData::UnionGroup(std::shared_ptr<SVoxelData>& _srsData)
{
	_srsData->groupHeader->groupHeader = this->groupHeader->groupHeader;
}
