#pragma once

#include "CommonStruct.h"
#include <memory>
#include <vector>

namespace Engine::Algorithm
{
	struct SNodeData
	{
		//points space divided on chunk. we want to know chunck Index each axis (oX, oY, oZ)
		// and know chunk value in current axis coordinates;
		int16_t deltaNode;
		int16_t chunkIndex;
		int32_t chunkValue;
		int32_t nextChunkValue;
		int16_t chunkNumber;
		// difference between n and n-1 chunk value // 
		int32_t deltaChunkValue;
		int32_t startChunkValue;
		// minimal point of cloud
		int32_t minOx;
		int32_t minOy;
		int32_t minOz;
		// start position of marching cube (it should be vertex 0 (-1, -1, -1 in normalized coordinates))
		int32_t startX;
		int32_t startY;
		int32_t startZ;
		// marhing cube size
		int32_t cubeSizeX;
		int32_t cubeSizeY;
		int32_t cubeSizeZ;

		// navigation
		int16_t leftNodeIndex = -1;
		int16_t rightNodeIndex = -1;
		int16_t rootNodeIndex = -1;

		//
		uint8_t depthTree;
	};

	struct SBSTChunkBase
	{
		SBSTChunkBase() {};

		void CreateLeftNodeData(const SNodeData& fromData, SNodeData& leftData) const;

		void CreateRightNodeData(const SNodeData& fromData, SNodeData& rightData) const;

		void CreateRootNodeData(const SNodeData& fromData, SNodeData& rootData) const;
	};

	struct SBSTChunkOx;

	struct SBSTContainer
	{
		SBSTContainer(SNodeData _data);

		void Find(const std::shared_ptr<SPoint3D>& _point);

	private:

		std::vector<std::shared_ptr<SBSTChunkOx>> chunksOx;

		SNodeData mData;

		std::shared_ptr<SBSTChunkBase> base;
	};

	struct SBSTChunkOy;

	struct SBSTChunkOx
	{
		SBSTChunkOx(SNodeData _data);

		void Find(int32_t axisKey, const std::shared_ptr<SPoint3D>& _point);

		const SNodeData& GetData()
		{
			return mData;
		}

	private:
		 std::vector<std::shared_ptr<SBSTChunkOy>> chunksOy;
		 
		 SNodeData mData;

		 std::shared_ptr<SBSTChunkBase> base;
	};

	struct SBSTChunkOz;

	struct SBSTChunkOy
	{
		SBSTChunkOy(SNodeData _data);

		void Find(int32_t axisKey,const std::shared_ptr<SPoint3D>& _point);

		const SNodeData& GetData()
		{
			return mData;
		}

	private:

		std::vector<std::shared_ptr<SBSTChunkOz>> chunksOz;

		SNodeData mData;

		std::shared_ptr<SBSTChunkBase> base;
	};

	struct SBSTChunkOz
	{
		SBSTChunkOz(SNodeData _data);

		void Perform(const std::shared_ptr<SPoint3D>& _point);

		const SNodeData& GetData()
		{
			return mData;
		}

	private:

		std::shared_ptr<SMarchingCube> cubes;

		SNodeData mData;

		std::shared_ptr<SBSTChunkBase> base;
	};
}