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

		SPoint3D(float_t _x, float_t _y, float_t _z)
			: x{_x}
			, y{_y}
			, z{_z}
		{};

		virtual float_t GetX() const { return x; };
		virtual float_t GetY() const { return y; };
		virtual float_t GetZ() const { return z; };

	protected:

		float_t x;
		float_t y;
		float_t z;
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

		float_t cubeSizeX;
		float_t cubeSizeY;
		float_t cubeSizeZ;
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

		SVertexCubeField_0(float_t _x, float_t _y, float_t _z, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ)
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

		SVertexCubeField_1(float_t _x, float_t _y, float_t _z, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ)
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

		SVertexCubeField_2(float_t _x, float_t _y, float_t _z, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ)
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

		SVertexCubeField_3(float_t _x, float_t _y, float_t _z, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ)
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

		SVertexCubeField_4(float_t _x, float_t _y, float_t _z, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ)
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

		SVertexCubeField_5(float_t _x, float_t _y, float_t _z, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ)
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

		SVertexCubeField_6(float_t _x, float_t _y, float_t _z, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ)
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

		SVertexCubeField_7(float_t _x, float_t _y, float_t _z, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ)
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

		SMarchingCube(float_t _startX, float_t _startY, float_t _startZ, float_t _cubeSizeX, float_t _cubeSizeY, float_t _cubeSizeZ);

		void FillMeshSubSpace(const std::shared_ptr<SPoint3D>& _point);

	private:

		float_t startX;
		float_t startY;
		float_t startZ;

		float_t cubeSizeX;
		float_t cubeSizeY;
		float_t cubeSizeZ;

		bool bAllMeshFound;

		std::list<std::shared_ptr<SVertexCubeField>> mpVertices;
	};

}
