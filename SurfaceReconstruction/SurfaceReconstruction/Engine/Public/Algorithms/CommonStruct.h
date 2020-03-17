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

		virtual void MeshStorageData() = 0;

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) = 0;

	protected:
		virtual void IncludeInMesh() { bIsMesh = true; };

		bool bIsMesh = false;

		int32_t cubeSizeX;
		int32_t cubeSizeY;
		int32_t cubeSizeZ;
	};
	//struct for storage 1/8 cube
	// marching cube presents as 8 voxel
	struct SVertexVoxelUnit
	{
		std::vector<std::shared_ptr<SPoint3D>> vertices;
	};

	struct SVertexCubeField_0 : public SVertexCubeField
	{
		SVertexCubeField_0() = delete;

		SVertexCubeField_0(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) override;

		virtual void MeshStorageData() override;
	};

	struct SVertexCubeField_1 : public SVertexCubeField
	{
		SVertexCubeField_1() = delete;

		SVertexCubeField_1(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) override;

		virtual void MeshStorageData() override;
	};

	struct SVertexCubeField_2 : public SVertexCubeField
	{
		SVertexCubeField_2() = delete;

		SVertexCubeField_2(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) override;

		virtual void MeshStorageData() override;
	};

	struct SVertexCubeField_3 : public SVertexCubeField
	{
		SVertexCubeField_3() = delete;

		SVertexCubeField_3(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) override;

		virtual void MeshStorageData() override;
	};

	struct SVertexCubeField_4 : public SVertexCubeField
	{
		SVertexCubeField_4() = delete;

		SVertexCubeField_4(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) override;

		virtual void MeshStorageData() override;

	};

	struct SVertexCubeField_5 : public SVertexCubeField
	{
		SVertexCubeField_5() = delete;

		SVertexCubeField_5(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) override;

		virtual void MeshStorageData() override;

	};

	struct SVertexCubeField_6 : public SVertexCubeField
	{
		SVertexCubeField_6() = delete;

		SVertexCubeField_6(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) override;

		virtual void MeshStorageData() override;

	};

	struct SVertexCubeField_7 : public SVertexCubeField
	{
		SVertexCubeField_7() = delete;

		SVertexCubeField_7(int32_t _x, int32_t _y, int32_t _z, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(const std::shared_ptr<SPoint3D>& _point) override;

		virtual void MeshStorageData() override;

	};

	struct SMarchingCube
	{
		SMarchingCube() = delete;

		SMarchingCube(int32_t _startX, int32_t _startY, int32_t _startZ, int32_t _cubeSizeX, int32_t _cubeSizeY, int32_t _cubeSizeZ);

		void FillMeshSubSpace(const std::shared_ptr<SPoint3D>& _point);

	private:

		int32_t startX;
		int32_t startY;
		int32_t startZ;

		int32_t cubeSizeX;
		int32_t cubeSizeY;
		int32_t cubeSizeZ;

		bool bAllMeshFound;

		std::list<std::shared_ptr<SVertexCubeField>> mpVertices;
	};

}
