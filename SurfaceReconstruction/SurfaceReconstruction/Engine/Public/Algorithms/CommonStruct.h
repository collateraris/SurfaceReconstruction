#pragma once

#include <memory>
#include <list>
#include <vector>
#include <cassert>

namespace Engine::Algorithm
{
	struct SMinMaxPoint
	{
		int32_t maxX;
		int32_t minX;
		int32_t maxY;
		int32_t minY;
		int32_t maxZ;
		int32_t minZ;
	};

	struct SVertexVoxelUnit;

	class CCommonStruct
	{
	public:

		static void PrintVoxelsInObj(const char* filename, std::list<SVertexVoxelUnit>& storageVoxels);
	};

	struct SPoint3D
	{
		SPoint3D() {};

		SPoint3D(int32_t _x, int32_t _y, int32_t _z)
			: x{_x}
			, y{_y}
			, z{_z}
		{};

		virtual int32_t GetX() const { return x; };
		virtual int32_t GetY() const { return y; };
		virtual int32_t GetZ() const { return z; };

	protected:

		int32_t x;
		int32_t y;
		int32_t z;
	};

	struct SVertexCubeField : public SPoint3D
	{
		SVertexCubeField() {};

		virtual bool IsMesh() const { return bIsMesh; };

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) {};

		virtual bool IsContainPoint(const SPoint3D& _point) { return false; };

		virtual void IncludeInMesh() { bIsMesh = true; };

		virtual int32_t GetCubeSizeX() { return cubeSizeX; }
		virtual int32_t GetCubeSizeY() { return cubeSizeY; }
		virtual int32_t GetCubeSizeZ() { return cubeSizeZ; }

	protected:

		bool bIsMesh = false;

		int32_t cubeSizeX;
		int32_t cubeSizeY;
		int32_t cubeSizeZ;
	};
	//struct for storage 1/8 cube
	// marching cube presents as 8 voxel
	struct SVertexVoxelUnit
	{
		std::vector<SPoint3D> vertices;
	};

	struct SSubVoxelData : public SVertexCubeField
	{
		SSubVoxelData() = default;

		SSubVoxelData(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		bool bAllMeshFound = false;

		int32_t searchIndexX = -1;
		int32_t searchIndexY = -1;
		int32_t searchIndexZ = -1;

		// direction permittion
		bool leftDir = true;
		bool rightDir = true;
		bool topDir = true;
		bool buttomDir = true;
		bool backDir = true;
		bool frontDir = true;

	};

	struct SVoxelData
	{
		SVoxelData()
		{
			groupHeader = this;
		}

		int32_t PushSubVoxelData(int32_t _startX, int32_t _startY, int32_t _startZ, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ);

		void AttachMeshSpace(const SPoint3D& _point, int32_t _subVoxelIndex);

		void AttachMeshSpace(int32_t _subVoxelIndex);

		bool CompareGroup(std::shared_ptr<SVoxelData>& _srsData);

		void UnionGroup(std::shared_ptr<SVoxelData>& _srsData);

		void SetSearchIndexes(int32_t indX, int32_t indY, int32_t indZ, int32_t subVoxelIndex = 0)
		{
			//assert(subVoxelIndex < voxels.size());

			voxels[subVoxelIndex].searchIndexX = indX;
			voxels[subVoxelIndex].searchIndexY = indY;
			voxels[subVoxelIndex].searchIndexZ = indZ;
		}

		int32_t GetSearchIndexX(int32_t subVoxelIndex = 0)
		{
			//assert(subVoxelIndex < voxels.size());

			return voxels[subVoxelIndex].searchIndexX;
		}

		int32_t GetSearchIndexY(int32_t subVoxelIndex = 0)
		{
			//assert(subVoxelIndex < voxels.size());

			return voxels[subVoxelIndex].searchIndexY;
		}

		int32_t GetSearchIndexZ(int32_t subVoxelIndex = 0)
		{
			//assert(subVoxelIndex < voxels.size());

			return voxels[subVoxelIndex].searchIndexZ;
		}

		bool GetLeftDir(int32_t subVoxelIndex = 0)
		{
			return voxels[subVoxelIndex].leftDir;
		}

		bool GetRightDir(int32_t subVoxelIndex = 0)
		{
			return voxels[subVoxelIndex].rightDir;
		}

		bool GetTopDir(int32_t subVoxelIndex = 0)
		{
			return voxels[subVoxelIndex].topDir;
		}

		bool GetButtomDir(int32_t subVoxelIndex = 0)
		{
			return voxels[subVoxelIndex].buttomDir;
		}

		bool GetBackDir(int32_t subVoxelIndex = 0)
		{
			return voxels[subVoxelIndex].backDir;
		}

		bool GetFrontDir(int32_t subVoxelIndex = 0)
		{
			return voxels[subVoxelIndex].frontDir;
		}

		void SetLeftDir(bool val, int32_t subVoxelIndex = 0)
		{
			voxels[subVoxelIndex].leftDir = val;
		}

		void SetRightDir(bool val, int32_t subVoxelIndex = 0)
		{
			voxels[subVoxelIndex].rightDir = val;
		}

		void SetTopDir(bool val, int32_t subVoxelIndex = 0)
		{
			voxels[subVoxelIndex].topDir = val;
		}

		void SetButtomDir(bool val, int32_t subVoxelIndex = 0)
		{
			voxels[subVoxelIndex].buttomDir = val;
		}

		void SetBackDir(bool val, int32_t subVoxelIndex = 0)
		{
			voxels[subVoxelIndex].backDir = val;
		}

		void SetFrontDir(bool val, int32_t subVoxelIndex = 0)
		{
			voxels[subVoxelIndex].frontDir = val;
		}

		void GetAllMeshes(std::list<SVertexVoxelUnit>& allMeshesAsObj, int32_t _subVoxelIndex);

		std::vector<SSubVoxelData> voxels;

		bool bActive = true;

	private:
		friend class SVoxelData;

		SVoxelData* groupHeader = nullptr;
	};

}
