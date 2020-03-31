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

		int32_t maxOx;
		int32_t maxOy;
		int32_t maxOz;
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

		//
		int16_t searchIndexX = -1;
		int16_t searchIndexY = -1;
		int16_t searchIndexZ = -1;
	};

	struct SBSTChunkBase
	{
		SBSTChunkBase() {};

		inline static void CreateLeftNodeData(const SNodeData& fromData, SNodeData& leftData);

		inline static void CreateRightNodeData(const SNodeData& fromData, SNodeData& rightData);

		inline static void CreateRootNodeData(const SNodeData& fromData, SNodeData& rootData) ;
	};

	struct SBSTChunkOx;

	struct SBSTChunkOz;

	class SBSTContainer
	{
	public:
		SBSTContainer(SNodeData _data);

		void InitCubesPool(int32_t _poolSize);

		void Find(const SPoint3D& _point);

		void CreateSolidMesh(int32_t numSteps = 10, int32_t neutralSteps = 1);

		void GetAllMeshes(std::list<SVertexVoxelUnit>& allMeshesAsObj);

	private:

		void TriggerVoxel(std::shared_ptr<SBSTChunkOz>& issueChunk);

		void AttachMeshSpace(std::shared_ptr<SBSTChunkOz>& issueChunk, const SPoint3D& _point);

		void SearchNeighbourX(std::shared_ptr<SVoxelData>& vxData, int32_t stepSize);
		void SearchNeighbourY(std::shared_ptr<SVoxelData>& vxData, int32_t stepSize, int32_t chunkIndexX);
		void SearchNeighbourZ(std::shared_ptr<SVoxelData>& vxData, int32_t stepSize, int32_t chunkIndexX, int32_t chunkIndexY);

		void AddSubVoxel(std::shared_ptr<SVoxelData>& vxData, int32_t stepSize, int32_t chunkIndexX, int32_t chunkIndexY, int32_t chunkIndexZ);

		std::vector<std::shared_ptr<SBSTChunkOx>> chunksOx;

		std::vector<std::shared_ptr<SVoxelData>> voxelsPool;

		int32_t usedVoxels = 0;

		int32_t neutralSteps = 1;

		SNodeData mData;
	};

	struct SBSTChunkOy;

	struct SBSTChunkOx
	{
		SBSTChunkOx() = default;

		void SetData(SNodeData&& mData);

		std::vector<std::shared_ptr<SBSTChunkOy>> chunksOy;
		 
		SNodeData mData;
	};

	struct SBSTChunkOy
	{
		SBSTChunkOy() = default;

		void SetData(SNodeData&& mData);

		std::vector<std::shared_ptr<SBSTChunkOz>> chunksOz;

		SNodeData mData;
	};

	struct SBSTChunkOz
	{
		SBSTChunkOz() = default;

		std::shared_ptr<SVoxelData> voxel;

		int32_t subVoxelIndex = 0;

		void SetData(SNodeData&& data)
		{
			mData = std::move(data);
		}

		SNodeData mData;
	};
}