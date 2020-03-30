#pragma once

#include <memory>
#include <list>
#include <vector>

namespace Engine::Algorithm
{
	class CCommonStruct
	{
	public:
		static int GetSizeVoxelList();

		static void PrintVoxelsInObj(const char* filename);
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

		virtual void MeshStorageData() {};

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

		virtual void MeshStorageData() override;
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

		virtual void MeshStorageData() override;
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

		virtual void MeshStorageData() override;
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

		virtual void MeshStorageData() override;
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

		virtual void MeshStorageData() override;

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

		virtual void MeshStorageData() override;

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

		virtual void MeshStorageData() override;

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

		virtual void MeshStorageData() override;

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
		SVoxelData() = default;

		int32_t PushSubVoxelData(int32_t _startX, int32_t _startY, int32_t _startZ, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ);

		void AttachMeshSpace(const SPoint3D& _point, int32_t _subVoxelIndex);

	private:
		
		std::vector<SSubVoxelData> voxels;
	};

}
