#pragma once

#include "CommonStruct.h"
#include <memory>

namespace Engine::Algorithm
{
	struct SNodeData
	{
		//points space divided on chunk. we want to know chunck Index each axis (oX, oY, oZ)
		// and know chunk value in current axis coordinates;
		int deltaNode;
		int chunkIndex;
		double chunkValue;
		double nextChunkValue;
		int chunkNumber;
		// difference between n and n-1 chunk value // 
		double deltaChunkValue;
		double startChunkValue;
		// minimal point of cloud
		double minOx;
		double minOy;
		double minOz;
		// start position of marching cube (it should be vertex 0 (-1, -1, -1 in normalized coordinates))
		double startX;
		double startY;
		double startZ;
		// marhing cube size
		double cubeSizeX;
		double cubeSizeY;
		double cubeSizeZ;
	};

	struct SBSTNodeBase
	{
		SBSTNodeBase(SNodeData _data);

		virtual void Find(double axisKey, std::shared_ptr<SPoint3D> _point);

	protected:
		virtual void LeftNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point);
		virtual void RightNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point);

		virtual void Perform(std::shared_ptr<SPoint3D> _point) {};

		const int END_DELTA_NODE = 1;

		SNodeData mData;

		std::shared_ptr<SBSTNodeBase> leftChild;
		std::shared_ptr<SBSTNodeBase> rightChild;
	};

	struct SBSTNodeOy;

	// tree operation for oX axis
	struct SBSTNodeOx : public SBSTNodeBase
	{
		protected:
			virtual void LeftNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point) override;
			virtual void RightNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point) override;
			virtual void Perform(std::shared_ptr<SPoint3D> _point) override;

			std::shared_ptr<SBSTNodeOy> sheet;
	};

	struct SBSTNodeOz;

	// tree operation for oY axis
	struct SBSTNodeOy : public SBSTNodeBase
	{
	protected:
		virtual void LeftNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point) override;
		virtual void RightNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point) override;
		virtual void Perform(std::shared_ptr<SPoint3D> _point) override;

		std::shared_ptr<SBSTNodeOz> sheet;
	};

	// tree operation for oz axis
	// here we can perform operation with marching cube 
	// and signed mesh subspace
	struct SBSTNodeOz : public SBSTNodeBase
	{
	protected:
		virtual void LeftNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point) override;
		virtual void RightNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point) override;
		virtual void Perform(std::shared_ptr<SPoint3D> _point) override;

		std::shared_ptr<SMarchingCube> sheet;
	};

	//binary search tree for surface reconstuction
	class CBSTforSR
	{
		public:
			CBSTforSR(SNodeData data);

			void Find(std::shared_ptr<SPoint3D> _point);

		protected:

			std::shared_ptr<SBSTNodeOx> rootNode;

		private:
			CBSTforSR() = delete;
	};
}