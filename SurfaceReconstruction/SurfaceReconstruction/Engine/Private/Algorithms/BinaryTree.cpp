#include "Algorithms/BinaryTree.h"

#include <cassert>
#include <iostream>

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
	mData.maxOx = mData.minOx + mData.chunkNumber * mData.cubeSizeX;
	mData.maxOy = mData.minOy + mData.chunkNumber * mData.cubeSizeY;
	mData.maxOz = mData.minOz + mData.chunkNumber * mData.cubeSizeZ;
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

void SBSTContainer::GetVoxelFromPool(std::shared_ptr<SVoxelData>& _voxel)
{
	usedVoxels++;
	if (usedVoxels > (voxelsPool.size() - 1))
	{
		voxelsPool.push_back(std::move(std::make_shared<SVoxelData>()));
	}
	_voxel = voxelsPool[usedVoxels++];
}

void SBSTContainer::TriggerVoxel(std::shared_ptr<SBSTChunkOz>& issueChunk)
{
	if (issueChunk->voxel.get() == nullptr)
	{
		GetVoxelFromPool(issueChunk->voxel);
		SNodeData& data = issueChunk->mData;
		issueChunk->subVoxelIndex = issueChunk->voxel->PushSubVoxelData(data.startX, data.startY, data.startZ, data.cubeSizeX, data.cubeSizeY, data.cubeSizeZ);
		issueChunk->voxel->SetSearchIndexes(data.searchIndexX, data.searchIndexY, data.searchIndexZ);
	}
}

void SBSTContainer::AttachMeshSpace(std::shared_ptr<SBSTChunkOz>& issueChunk, const SPoint3D& _point)
{
	issueChunk->voxel->AttachMeshSpace(_point, issueChunk->subVoxelIndex);
}

void SBSTContainer::Find(const SPoint3D& _point)
{
	int32_t axisKey = _point.GetX();

	if (axisKey < mData.minOx || axisKey > mData.maxOx)
	{
		return;
	}

	uint8_t depthTree = mData.depthTree;

	int16_t searchIndex = mData.rootNodeIndex;

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

			if (axisKey < mData.minOy || axisKey > mData.maxOy)
			{
				return;
			}

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
				leftData.searchIndexX = searchIndex;
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
				rightData.searchIndexX = searchIndex;
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

			if (axisKey < mData.minOz || axisKey > mData.maxOz)
			{
				return;
			}

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
				leftData.searchIndexY = searchIndex;
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
				rightData.searchIndexY = searchIndex;
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
				leftData.searchIndexZ = searchIndex;
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
				rightData.searchIndexZ = searchIndex;
				nextChunk = std::move(std::make_shared<SBSTChunkOz>());
				nextChunk->SetData(std::move(rightData));
			}
		}
	}
}

void SBSTContainer::CreateSolidMesh(int32_t numSteps/* = 10*/, int32_t neutralSteps /* = 1*/)
{
	this->neutralSteps = neutralSteps;
	for (int32_t st = 0; st < numSteps; st++)
	{
		for (std::size_t vx = 0; vx < usedVoxels; ++vx)
		{
			std::shared_ptr<SVoxelData>& vxData = voxelsPool[vx];
			int32_t voxelSize = vxData->voxels.size();
			if (voxelSize > 0){
				SearchNeighbourX(vxData, st + 1);
			    for (int j = 0; j <= st; ++j)
				for (int32_t i = 1; i < voxelSize; ++i)
				{
					SearchNeighbourX(vxData, j + 1, i);
				}
			}
		}
	}
}

void SBSTContainer::SearchNeighbourX(std::shared_ptr<SVoxelData>& vxData, int32_t stepSize, int32_t subVoxelIndex/* = 0*/)
{
	int32_t chunkSize = chunksOx.size() - 1;
	int32_t searchIndexX = vxData->GetSearchIndexX(subVoxelIndex);

	int32_t lowerBound = vxData->GetBackDir(subVoxelIndex) ? -stepSize : 0;
	int32_t upperBound = vxData->GetFrontDir(subVoxelIndex) ? stepSize : 0;

	for (int32_t i = lowerBound; i <= upperBound; ++i) {
		int32_t checkIndexX = searchIndexX + i;
		if (checkIndexX < 0 || checkIndexX > chunkSize)
		{
			continue;
		}

		if (chunksOx[checkIndexX].get() == nullptr)
		{
			continue;
		}

		SearchNeighbourY(vxData, stepSize, checkIndexX, subVoxelIndex);
	}
}

void SBSTContainer::SearchNeighbourY(std::shared_ptr<SVoxelData>& vxData, int32_t stepSize, int32_t chunkIndexX, int32_t subVoxelIndex/* = 0*/)
{
	std::vector<std::shared_ptr<SBSTChunkOy>>& chunksOy = chunksOx[chunkIndexX]->chunksOy;
	int32_t chunkSize = chunksOy.size() - 1;
	int32_t searchIndexY = vxData->GetSearchIndexY(subVoxelIndex);

	int32_t lowerBound = vxData->GetLeftDir(subVoxelIndex) ? -stepSize : 0;
	int32_t upperBound = vxData->GetRightDir(subVoxelIndex) ? stepSize : 0;

	for (int32_t i = lowerBound; i <= upperBound; ++i)
	{
		int32_t checkIndexY = searchIndexY + i;
		if (checkIndexY < 0 || checkIndexY > chunkSize)
		{
			continue;
		}

		std::shared_ptr<SBSTChunkOy>& chunkY = chunksOy[checkIndexY];

		if (chunkY.get() == nullptr)
		{
			continue;
		}

		SearchNeighbourZ(vxData, stepSize, chunkIndexX, checkIndexY, subVoxelIndex);
	}
}

void SBSTContainer::SearchNeighbourZ(std::shared_ptr<SVoxelData>& vxData, int32_t stepSize, int32_t chunkIndexX, int32_t chunkIndexY, int32_t subVoxelIndex/* = 0*/)
{
	std::vector<std::shared_ptr<SBSTChunkOy>>& chunksOy = chunksOx[chunkIndexX]->chunksOy;
	std::vector<std::shared_ptr<SBSTChunkOz>>& chunksOz = chunksOy[chunkIndexY]->chunksOz;
	int32_t chunkSize = chunksOz.size() - 1;
	int32_t searchIndexX = vxData->GetSearchIndexX(subVoxelIndex);
	int32_t searchIndexY = vxData->GetSearchIndexY(subVoxelIndex);
	int32_t searchIndexZ = vxData->GetSearchIndexZ(subVoxelIndex);

	int32_t neutralLowerSizeX = searchIndexX - neutralSteps;
	int32_t neutralUperSizeX = searchIndexX + neutralSteps;
	int32_t neutralLowerSizeY = searchIndexY - neutralSteps;
	int32_t neutralUperSizeY = searchIndexY + neutralSteps;
	int32_t neutralLowerSizeZ = searchIndexZ - neutralSteps;
	int32_t neutralUperSizeZ = searchIndexZ + neutralSteps;

	int32_t lowerBound = vxData->GetButtomDir(subVoxelIndex) ? -stepSize : 0;
	int32_t upperBound = vxData->GetTopDir(subVoxelIndex) ? stepSize : 0;

	for (int32_t i = lowerBound; i <= upperBound; ++i)
	{
		int32_t checkIndexZ = searchIndexZ + i;
		if (checkIndexZ < 0 || checkIndexZ > chunkSize)
		{
			continue;
		}

		std::shared_ptr<SBSTChunkOz>& chunkZ = chunksOz[checkIndexZ];
		if (chunkZ.get() == nullptr)
		{
			continue;
		}

		if (chunkZ->voxel.get() == nullptr)
		{
			continue;
		}

		/*if (((chunkIndexX < neutralLowerSizeX || chunkIndexX > neutralUperSizeX) ||
			(chunkIndexY < neutralLowerSizeY || chunkIndexY > neutralUperSizeY) ||
			(checkIndexZ < neutralLowerSizeZ || checkIndexZ > neutralUperSizeZ)) &&
			vxData->CompareGroup(chunkZ->voxel))
		{
			continue;
		}*/

		// Compare directional;
		//if (chunkIndexX < neutralLowerSizeX || chunkIndexX > neutralUperSizeX) {
			if (searchIndexX > chunkIndexX)
			{
				vxData->SetBackDir(false, subVoxelIndex);
			}
			else
			{
				vxData->SetFrontDir(false, subVoxelIndex);
			}
		//}

		//if (chunkIndexY < neutralLowerSizeY || chunkIndexY > neutralUperSizeY)
		{
			if (searchIndexY > chunkIndexY)
			{
				vxData->SetLeftDir(false, subVoxelIndex);
			}
			else
			{
				vxData->SetRightDir(false, subVoxelIndex);
			}
		}

		//if (checkIndexZ < neutralLowerSizeZ || checkIndexZ > neutralUperSizeZ)
		{
			if (searchIndexZ > checkIndexZ)
			{
				vxData->SetButtomDir(false, subVoxelIndex);
			}
			else
			{
				vxData->SetTopDir(false, subVoxelIndex);
			}
		//}

		// Found neighbour with another group!
		//if (!vxData->CompareGroup(chunkZ->voxel))
		{
			//vxData->UnionGroup(chunkZ->voxel);
		}

		AddSubVoxel(vxData, stepSize, chunkIndexX, chunkIndexY, checkIndexZ, subVoxelIndex);
	}
}

void SBSTContainer::AddSubVoxel(std::shared_ptr<SVoxelData>& vxData, int32_t stepSize, int32_t chunkIndexX, int32_t chunkIndexY, int32_t chunkIndexZ, int32_t subVoxelIndex/* = 0*/)
{
	int32_t searchIndexX = vxData->GetSearchIndexX(subVoxelIndex);
	int32_t searchIndexY = vxData->GetSearchIndexY(subVoxelIndex);
	int32_t searchIndexZ = vxData->GetSearchIndexZ(subVoxelIndex);

	int32_t signX = searchIndexX > chunkIndexX ? 1 : -1;
	int32_t signY = searchIndexY > chunkIndexY ? 1 : -1;
	int32_t signZ = searchIndexZ > chunkIndexZ ? 1 : -1;

	int32_t newChunkX = chunkIndexX;
	int32_t newChunkY = chunkIndexY;
	int32_t newChunkZ = chunkIndexZ;

	int32_t chunkSize = chunksOx.size() - 1;

	for (int32_t i = 0; i < stepSize; ++i)
	{
		newChunkX = (newChunkX == searchIndexX) ? newChunkX : (newChunkX + signX);
		newChunkY = (newChunkY == searchIndexY) ? newChunkY : (newChunkY + signY);
		newChunkZ = (newChunkZ == searchIndexZ) ? newChunkZ : (newChunkZ + signZ);

		if (newChunkX < 0 || newChunkX > chunkSize)
		{
			continue;
		}

		if (newChunkY < 0 || newChunkY > chunkSize)
		{
			continue;
		}

		if (newChunkZ < 0 || newChunkZ > chunkSize)
		{
			continue;
		}

		std::shared_ptr<SBSTChunkOx>& chunkX = chunksOx[newChunkX];
		if (chunkX.get() == nullptr)
		{
			chunkX = std::move(std::make_shared<SBSTChunkOx>());
			chunkX->chunksOy.resize(chunkSize + 1);
		}

		std::vector<std::shared_ptr<SBSTChunkOy>>& chunksOy = chunkX->chunksOy;
		std::shared_ptr<SBSTChunkOy>& chunkY = chunksOy[newChunkY];
		if (chunkY.get() == nullptr)
		{
			chunkY = std::move(std::make_shared<SBSTChunkOy>());
			chunkY->chunksOz.resize(chunkSize + 1);
		}

		std::vector<std::shared_ptr<SBSTChunkOz>>& chunksOz = chunkY->chunksOz;
		std::shared_ptr<SBSTChunkOz>& chunkZ = chunksOz[newChunkZ];

		if (chunkZ.get() == nullptr)
		{
			chunkZ = std::move(std::make_shared<SBSTChunkOz>());

			int32_t startX = mData.minOx + (newChunkX - 1) * mData.cubeSizeX;
			int32_t startY = mData.minOy + (newChunkY - 1) * mData.cubeSizeY;
			int32_t startZ = mData.minOz + (newChunkZ - 1) * mData.cubeSizeZ;
			chunkZ->subVoxelIndex = vxData->PushSubVoxelData(startX, startY, startZ, mData.cubeSizeX, mData.cubeSizeY, mData.cubeSizeZ);
			vxData->SetSearchIndexes(newChunkX, newChunkY, newChunkZ, chunkZ->subVoxelIndex);
			vxData->AttachMeshSpace(chunkZ->subVoxelIndex);
			chunkZ->voxel = vxData;
		}
	}
}

void SBSTContainer::GetAllMeshes(std::list<SVertexVoxelUnit>& allMeshesAsObj)
{
	for (std::size_t vx = 0; vx < usedVoxels; ++vx)
	{
		std::shared_ptr<SVoxelData>& vxData = voxelsPool[vx];
		std::size_t subVoxelSize = vxData->voxels.size();

		for (std::size_t svx = 0; svx < subVoxelSize; ++svx)
		{
			vxData->GetAllMeshes(allMeshesAsObj, svx);
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

