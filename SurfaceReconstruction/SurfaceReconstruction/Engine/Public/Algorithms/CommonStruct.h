#pragma once

#include <memory>
#include <list>
#include <vector>

namespace Engine::Algorithm
{
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

	struct SVertexCubeField_0 : public SVertexCubeField
	{
		SVertexCubeField_0() = default;

		SVertexCubeField_0(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;
	};

	struct SVertexCubeField_1 : public SVertexCubeField
	{
		SVertexCubeField_1() = default;

		SVertexCubeField_1(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;
	};

	struct SVertexCubeField_2 : public SVertexCubeField
	{
		SVertexCubeField_2() = default;

		SVertexCubeField_2(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;
	};

	struct SVertexCubeField_3 : public SVertexCubeField
	{
		SVertexCubeField_3() = default;

		SVertexCubeField_3(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;
	};

	struct SVertexCubeField_4 : public SVertexCubeField
	{
		SVertexCubeField_4() = default;

		SVertexCubeField_4(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;

	};

	struct SVertexCubeField_5 : public SVertexCubeField
	{
		SVertexCubeField_5() = default;

		SVertexCubeField_5(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;

	};

	struct SVertexCubeField_6 : public SVertexCubeField
	{
		SVertexCubeField_6() = default;

		SVertexCubeField_6(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;

	};

	struct SVertexCubeField_7 : public SVertexCubeField
	{
		SVertexCubeField_7() = default;

		SVertexCubeField_7(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const SPoint3D& _point) override;

		virtual void MeshStorageData(std::list<SVertexVoxelUnit>& allMeshesAsObj) override;

	};

	struct SSubVoxelData
	{
		SSubVoxelData() = default;

		bool bAllMeshFound = false;

		SVertexCubeField_0 mV0;
		SVertexCubeField_1 mV1;
		SVertexCubeField_2 mV2;
		SVertexCubeField_3 mV3;
		SVertexCubeField_4 mV4;
		SVertexCubeField_5 mV5;
		SVertexCubeField_6 mV6;
		SVertexCubeField_7 mV7;
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

		void SetSearchIndexes(int32_t indX, int32_t indY, int32_t indZ)
		{
			searchIndexX = indX;
			searchIndexY = indY;
			searchIndexZ = indZ;
		}

		int32_t GetSearchIndexX()
		{
			return searchIndexX;
		}

		int32_t GetSearchIndexY()
		{
			return searchIndexY;
		}

		int32_t GetSearchIndexZ()
		{
			return searchIndexZ;
		}

		void GetAllMeshes(std::list<SVertexVoxelUnit>& allMeshesAsObj, int32_t _subVoxelIndex);

		std::vector<SSubVoxelData> voxels;

	private:
		friend class SVoxelData;

		SVoxelData* groupHeader = nullptr;
		
		//std::vector<SSubVoxelData> voxels;

		//
		int16_t searchIndexX = -1;
		int16_t searchIndexY = -1;
		int16_t searchIndexZ = -1;

	public:
		// direction permittion
		bool leftDir = true;
		bool rightDir = true;
		bool topDir = true;
		bool buttomDir = true;
		bool backDir = true;
		bool frontDir = true;
	};

}
