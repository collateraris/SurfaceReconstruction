#include "Algorithms/BinaryTree.h"

#include <cassert>

using namespace Engine::Algorithm;

void SBSTChunkBase::CreateLeftNodeData(const SNodeData& fromData, SNodeData& leftData)
{
	leftData = fromData;
	leftData.chunkIndex = fromData.leftNodeIndex;
	leftData.deltaNode =  fromData.deltaNode - (fromData.deltaNode >> 1);
	leftData.chunkValue = fromData.startChunkValue + (leftData.chunkIndex - 1) * fromData.deltaChunkValue;
	leftData.nextChunkValue = leftData.chunkValue + fromData.deltaChunkValue;
	leftData.leftNodeIndex = leftData.chunkIndex - leftData.deltaNode;
	leftData.rightNodeIndex = leftData.chunkIndex + leftData.deltaNode;
}

void SBSTChunkBase::CreateRightNodeData(const SNodeData& fromData, SNodeData& rightData)
{
	rightData = fromData;
	rightData.chunkIndex = fromData.rightNodeIndex;
	rightData.deltaNode = fromData.deltaNode - (fromData.deltaNode >> 1);
	rightData.chunkValue = fromData.startChunkValue + (rightData.chunkIndex - 1) * fromData.deltaChunkValue;
	rightData.nextChunkValue = rightData.chunkValue + fromData.deltaChunkValue;
	rightData.leftNodeIndex = rightData.chunkIndex - rightData.deltaNode;
	rightData.rightNodeIndex = rightData.chunkIndex + rightData.deltaNode;
}

void SBSTChunkBase::CreateRootNodeData(const SNodeData& fromData, SNodeData& rootData) 
{
	rootData = fromData;
	rootData.chunkIndex = fromData.chunkNumber;
	rootData.deltaNode = fromData.chunkNumber - (fromData.chunkNumber >> 1);
	rootData.chunkValue = fromData.startChunkValue + (rootData.chunkIndex - 1) * fromData.deltaChunkValue;
	rootData.nextChunkValue = rootData.chunkValue + fromData.deltaChunkValue;
	rootData.leftNodeIndex = rootData.chunkIndex - rootData.deltaNode;
	rootData.rightNodeIndex = -1;
}

SBSTContainer::SBSTContainer(SNodeData _data)
{
	mData = std::move(_data);

	chunksOx.resize(mData.chunkNumber + 1);

	//start (root) is last pos
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeX;
	data.startChunkValue = mData.minOx;
	SBSTChunkBase::CreateRootNodeData(data, rootData);
	mData.depthTree = static_cast<uint8_t>(log2(mData.chunkNumber)) + 1;
	rootData.depthTree = mData.depthTree;
	rootData.startX = rootData.chunkValue;
	mData.rootNodeIndex = rootData.chunkIndex;

	chunksOx[mData.chunkNumber] = std::move(std::make_shared<SBSTChunkOx>());
	chunksOx[mData.chunkNumber]->SetData(std::move(rootData));
}

void SBSTContainer::InitCubesPool(int32_t _poolSize)
{
	voxelsPool.reserve(_poolSize + 1);

	for (int32_t i = 0; i < _poolSize; ++i)
	{
		voxelsPool.push_back(std::move(std::make_shared<SVoxelData>()));
	}
}

void SBSTContainer::TriggerVoxel(std::shared_ptr<SBSTChunkOz>& issueChunk)
{
	if (issueChunk->voxel == nullptr)
	{
		issueChunk->voxel = voxelsPool[usedVoxels++];

		SNodeData data = issueChunk->mData;
		issueChunk->subVoxelIndex = issueChunk->voxel->PushSubVoxelData(data.startX, data.startY, data.startZ, data.cubeSizeX, data.cubeSizeY, data.cubeSizeZ);
	}
}

void SBSTContainer::AttachMeshSpace(std::shared_ptr<SBSTChunkOz>& issueChunk, const SPoint3D& _point)
{
	issueChunk->voxel->AttachMeshSpace(_point, issueChunk->subVoxelIndex);
}

void SBSTContainer::Find(const SPoint3D& _point)
{
	uint8_t depthTree = mData.depthTree;

	int16_t searchIndex = mData.rootNodeIndex;

	int32_t axisKey = _point.GetX();

	SNodeData leftData;
	SNodeData rightData;

	std::vector<std::shared_ptr<SBSTChunkOy>>* chunksOy = nullptr;
	std::vector<std::shared_ptr<SBSTChunkOz>>* chunksOz = nullptr;

	for (uint8_t i = 0; i < depthTree; ++i)
	{
		std::shared_ptr<SBSTChunkOx>& issueChunk = chunksOx[searchIndex];
		assert(issueChunk.get() != nullptr);

		const SNodeData& data = issueChunk->mData;

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			axisKey = _point.GetY();
			chunksOy = &issueChunk->chunksOy;
			break;
		}

		if (i == depthTree - 1)
		{
			return;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			std::shared_ptr<SBSTChunkOx>& nextChunk = chunksOx[searchIndex];
			if (nextChunk.get() == nullptr)
			{
				SBSTChunkBase::CreateLeftNodeData(data, leftData);
				leftData.startX = leftData.chunkValue;
				nextChunk = std::move(std::make_shared<SBSTChunkOx>());
				nextChunk->SetData(std::move(leftData));
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			std::shared_ptr<SBSTChunkOx>& nextChunk = chunksOx[searchIndex];
			if (nextChunk.get() == nullptr)
			{
				SBSTChunkBase::CreateRightNodeData(data, rightData);
				rightData.startX = rightData.chunkValue;
				nextChunk = std::move(std::make_shared<SBSTChunkOx>());
				nextChunk->SetData(std::move(rightData));
			}
		}

	}

	// for oy depth
	searchIndex = mData.rootNodeIndex;
	for (uint8_t i = 0; i < depthTree; ++i)
	{
		std::shared_ptr<SBSTChunkOy>& issueChunk = (*chunksOy)[searchIndex];
		assert(issueChunk.get() != nullptr);

		const SNodeData& data = issueChunk->mData;

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			axisKey = _point.GetZ();
			chunksOz = &issueChunk->chunksOz;
			break;
		}

		if (i == depthTree - 1)
		{
			return;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			std::shared_ptr<SBSTChunkOy>& nextChunk = (*chunksOy)[searchIndex];
			if (nextChunk.get() == nullptr)	
			{
				SBSTChunkBase::CreateLeftNodeData(data, leftData);
				leftData.startY = leftData.chunkValue;
				nextChunk = std::move(std::make_shared<SBSTChunkOy>());
				nextChunk->SetData(std::move(leftData));
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			std::shared_ptr<SBSTChunkOy>& nextChunk = (*chunksOy)[searchIndex];
			if (nextChunk.get() == nullptr)
			{
				SBSTChunkBase::CreateRightNodeData(data, rightData);
				rightData.startY = rightData.chunkValue;
				nextChunk = std::move(std::make_shared<SBSTChunkOy>());
				nextChunk->SetData(std::move(rightData));
			}
		}
	}

	// for oz depth
	searchIndex = mData.rootNodeIndex;
	for (uint8_t i = 0; i < depthTree; ++i)
	{
		std::shared_ptr<SBSTChunkOz>& issueChunk = (*chunksOz)[searchIndex];
		assert(issueChunk.get() != nullptr);

		const SNodeData& data = issueChunk->mData;

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			TriggerVoxel(issueChunk);
			AttachMeshSpace(issueChunk, _point);
			break;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			std::shared_ptr<SBSTChunkOz>& nextChunk = (*chunksOz)[searchIndex];
			if (nextChunk.get() == nullptr)
			{
				SBSTChunkBase::CreateLeftNodeData(data, leftData);
				leftData.startZ = leftData.chunkValue;
				nextChunk = std::move(std::make_shared<SBSTChunkOz>());
				nextChunk->SetData(std::move(leftData));
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			std::shared_ptr<SBSTChunkOz>& nextChunk = (*chunksOz)[searchIndex];
			if (nextChunk.get() == nullptr)
			{
				SBSTChunkBase::CreateRightNodeData(data, rightData);
				rightData.startZ = rightData.chunkValue;
				nextChunk = std::move(std::make_shared<SBSTChunkOz>());
				nextChunk->SetData(std::move(rightData));
			}
		}
	}
}

void SBSTChunkOx::SetData(SNodeData&& _data)
{
	mData = std::move(_data);

	chunksOy.resize(mData.chunkNumber + 1);
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeY;
	data.startChunkValue = mData.minOy;
	SBSTChunkBase::CreateRootNodeData(data, rootData);
	rootData.startY = rootData.chunkValue;
	rootData.depthTree = mData.depthTree;
	mData.rootNodeIndex = rootData.chunkIndex;

	chunksOy[mData.chunkNumber] = std::move(std::make_shared<SBSTChunkOy>());
	chunksOy[mData.chunkNumber]->SetData(std::move(rootData));
}

void SBSTChunkOy::SetData(SNodeData&& _data)
{
	mData = std::move(_data);

	chunksOz.resize(mData.chunkNumber + 1);

	//start (root) is last pos
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeZ;
	data.startChunkValue = mData.minOz;
	SBSTChunkBase::CreateRootNodeData(data, rootData);
	rootData.startZ = rootData.chunkValue;
	rootData.depthTree = mData.depthTree;
	mData.rootNodeIndex = rootData.chunkIndex;

	chunksOz[mData.chunkNumber] = std::move(std::make_shared<SBSTChunkOz>());
	chunksOz[mData.chunkNumber]->SetData(std::move(rootData));
}

