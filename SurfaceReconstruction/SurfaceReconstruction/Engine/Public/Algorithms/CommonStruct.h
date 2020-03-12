#pragma once

#include <memory>
#include <list>

namespace Engine::Algorithm
{
	struct SPoint3D
	{
		SPoint3D() {};

		SPoint3D(double _x, double _y, double _z)
			: x{_x}
			, y{_y}
			, z{_z}
		{};

		virtual double GetX() const { return x; };
		virtual double GetY() const { return y; };
		virtual double GetZ() const { return z; };

	protected:

		double x;
		double y;
		double z;
	};

	struct SVertexCubeField : public SPoint3D
	{
		SVertexCubeField() {};

		virtual bool IsMesh() const { return bIsMesh; };

		virtual void IncludeInMesh() { bIsMesh = true; };

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) = 0;

	protected:

		bool bIsMesh = false;

		double cubeSizeX;
		double cubeSizeY;
		double cubeSizeZ;
	};

	struct SVertexCubeField_0 : public SVertexCubeField
	{
		SVertexCubeField_0() = delete;

		SVertexCubeField_0(double _x, double _y, double _z, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) override;
	};

	struct SVertexCubeField_1 : public SVertexCubeField
	{
		SVertexCubeField_1() = delete;

		SVertexCubeField_1(double _x, double _y, double _z, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) override;
	};

	struct SVertexCubeField_2 : public SVertexCubeField
	{
		SVertexCubeField_2() = delete;

		SVertexCubeField_2(double _x, double _y, double _z, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) override;
	};

	struct SVertexCubeField_3 : public SVertexCubeField
	{
		SVertexCubeField_3() = delete;

		SVertexCubeField_3(double _x, double _y, double _z, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) override;
	};

	struct SVertexCubeField_4 : public SVertexCubeField
	{
		SVertexCubeField_4() = delete;

		SVertexCubeField_4(double _x, double _y, double _z, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) override;
	};

	struct SVertexCubeField_5 : public SVertexCubeField
	{
		SVertexCubeField_5() = delete;

		SVertexCubeField_5(double _x, double _y, double _z, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) override;
	};

	struct SVertexCubeField_6 : public SVertexCubeField
	{
		SVertexCubeField_6() = delete;

		SVertexCubeField_6(double _x, double _y, double _z, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) override;
	};

	struct SVertexCubeField_7 : public SVertexCubeField
	{
		SVertexCubeField_7() = delete;

		SVertexCubeField_7(double _x, double _y, double _z, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ)
		{
			x = _x;
			y = _y;
			z = _z;
			cubeSizeX = _cubeSizeX;
			cubeSizeY = _cubeSizeY;
			cubeSizeZ = _cubeSizeZ;
		};

		virtual bool IsContainPoint(std::shared_ptr<SPoint3D> _point) override;
	};

	struct SMarchingCube
	{
		SMarchingCube() = delete;

		SMarchingCube(double _startX, double _startY, double _startZ, double _cubeSizeX, double _cubeSizeY, double _cubeSizeZ);

		void FillMeshSubSpace(std::shared_ptr<SPoint3D> _point);

	private:

		double startX;
		double startY;
		double startZ;

		double cubeSizeX;
		double cubeSizeY;
		double cubeSizeZ;

		bool bAllMeshFound;

		std::list<std::shared_ptr<SVertexCubeField>> mpVertices;
	};

}