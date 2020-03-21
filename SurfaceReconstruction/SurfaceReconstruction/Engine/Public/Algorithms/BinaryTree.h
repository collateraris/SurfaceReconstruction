#pragma once

#include "CommonStruct.h"
#include <memory>
#include <vector>
#include <unordered_map>

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

		inline static void CreateLeftNodeData(const SNodeData& fromData, SNodeData& leftData);

		inline static void CreateRightNodeData(const SNodeData& fromData, SNodeData& rightData);

		inline static void CreateRootNodeData(const SNodeData& fromData, SNodeData& rootData) ;
	};

	struct SBSTChunkOx;

	struct SBSTContainer
	{
		SBSTContainer(SNodeData _data);

		void Find(const SPoint3D& _point);

	private:

		//std::vector<std::shared_ptr<SBSTChunkOx>> chunksOx;
		//std::unordered_map<uint32_t, SBSTChunkOx> chunksOx;
		std::vector<SBSTChunkOx> chunksOx;

		SNodeData mData;
	};

	struct SBSTChunkOy;

	struct SBSTChunkOx
	{
		SBSTChunkOx() = default;

		SBSTChunkOx(SNodeData _data);

		void Find(int32_t axisKey, const SPoint3D& _point);

		const SNodeData& GetData()
		{
			return mData;
		}

		bool IsEmpty() { return bIsEmpty; };

	private:
		 //std::vector< std::shared_ptr<SBSTChunkOy>> chunksOy;
		//std::unordered_map<uint32_t, SBSTChunkOy> chunksOy;
		std::vector<SBSTChunkOy> chunksOy;
		 
		 SNodeData mData;

		 bool bIsEmpty = true;
	};

	struct SBSTChunkOz;

	struct SBSTChunkOy
	{
		SBSTChunkOy() = default;

		SBSTChunkOy(SNodeData _data);

		void Find(int32_t axisKey,const SPoint3D& _point);

		const SNodeData& GetData()
		{
			return mData;
		}

		bool IsEmpty() { return bIsEmpty; };

	private:

		std::vector<std::shared_ptr<SBSTChunkOz>> chunksOz;
		//std::unordered_map<uint32_t, SBSTChunkOz> chunksOz;

		SNodeData mData;

		bool bIsEmpty = true;
	};

	struct SBSTChunkOz
	{
		SBSTChunkOz() = default;

		SBSTChunkOz(SNodeData _data);

		void Perform(const SPoint3D& _point);

		const SNodeData& GetData()
		{
			return mData;
		}

	private:

		SMarchingCube cubes;

		SNodeData mData;
	};
}