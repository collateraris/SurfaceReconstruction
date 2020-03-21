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
	//chunksOx.reserve(mData.chunkNumber + 1);

	//start (root) is last pos
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeX;
	data.startChunkValue = mData.minOx;
	SBSTChunkBase::CreateRootNodeData(data, rootData);
	mData.depthTree = static_cast<uint8_t>(log2(mData.chunkNumber)) + 1;
	rootData.depthTree = mData.depthTree;
	rootData.startX = rootData.chunkValue;

	//chunksOx[mData.chunkNumber] = std::move(std::make_shared<SBSTChunkOx>(rootData));
	chunksOx[mData.chunkNumber] = std::move(SBSTChunkOx(rootData));
	//chunksOx.emplace(std::move(std::make_pair(mData.chunkNumber, SBSTChunkOx(rootData))));

	mData.rootNodeIndex = rootData.chunkIndex;
}

void SBSTContainer::Find(const SPoint3D& _point)
{
	uint8_t depthTree = mData.depthTree;
	int16_t searchIndex = mData.rootNodeIndex;

	float_t axisKey = _point.GetX();

	SNodeData leftData;
	SNodeData rightData;

	for (uint8_t i = 0; i < depthTree; ++i)
	{
		//assert(chunksOx[searchIndex].get() != nullptr);
		//assert(chunksOx.find(searchIndex) != chunksOx.end());
		assert(!chunksOx[searchIndex].IsEmpty());

		const SNodeData& data = chunksOx[searchIndex].GetData();

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			chunksOx[searchIndex].Find(_point.GetY(), _point);
			break;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			//if (chunksOx.find(searchIndex) == chunksOx.end())
			if (chunksOx[searchIndex].IsEmpty())
			{
				SBSTChunkBase::CreateLeftNodeData(data, leftData);
				leftData.startX = leftData.chunkValue;
				//chunksOx[searchIndex] = std::move(std::make_shared<SBSTChunkOx>(leftData));
				//chunksOx.emplace(std::move(std::make_pair(searchIndex, SBSTChunkOx(leftData))));
				chunksOx[searchIndex] = std::move(SBSTChunkOx(leftData));
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			//if (chunksOx.find(searchIndex) == chunksOx.end())
			if (chunksOx[searchIndex].IsEmpty())
			{
				SBSTChunkBase::CreateRightNodeData(data, rightData);
				rightData.startX = rightData.chunkValue;
				//chunksOx[searchIndex] = std::move(std::make_shared<SBSTChunkOx>(rightData));
				//chunksOx.emplace(std::move(std::make_pair(searchIndex, SBSTChunkOx(rightData))));
				chunksOx[searchIndex] = std::move(SBSTChunkOx(rightData));
			}
		}
	}
}

SBSTChunkOx::SBSTChunkOx(SNodeData _data)
{
	mData = std::move(_data);

	bIsEmpty = false;
	chunksOy.resize(mData.chunkNumber + 1);
	//chunksOy.reserve(mData.chunkNumber + 1);

	//start (root) is last pos
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeY;
	data.startChunkValue = mData.minOy;
	SBSTChunkBase::CreateRootNodeData(data, rootData);
	rootData.startY = rootData.chunkValue;
	rootData.depthTree = mData.depthTree;

	//chunksOy[mData.chunkNumber] = std::move(std::make_shared<SBSTChunkOy>(rootData));
	//chunksOy.emplace(std::move(std::make_pair(mData.chunkNumber, SBSTChunkOy(rootData
	chunksOy[mData.chunkNumber] = std::move(SBSTChunkOy(rootData));

	mData.rootNodeIndex = rootData.chunkIndex;
}

void SBSTChunkOx::Find(int32_t axisKey,const SPoint3D& _point)
{
	uint8_t depthTree = mData.depthTree;
	int16_t searchIndex = mData.rootNodeIndex;

	SNodeData leftData;
	SNodeData rightData;

	for (uint8_t i = 0; i < depthTree; ++i)
	{
		//assert(chunksOy[searchIndex].get() != nullptr);
		//assert(chunksOy.find(searchIndex) != chunksOy.end());
		assert(!chunksOy[searchIndex].IsEmpty());

		const SNodeData& data = chunksOy[searchIndex].GetData();

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			chunksOy[searchIndex].Find(_point.GetZ(), _point);
			break;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			//if (chunksOy.find(searchIndex) == chunksOy.end())
			if (chunksOy[searchIndex].IsEmpty())
			{
				SBSTChunkBase::CreateLeftNodeData(data, leftData);
				leftData.startY = leftData.chunkValue;
				//chunksOy[searchIndex] = std::move(std::make_shared<SBSTChunkOy>(leftData));
				//chunksOy.emplace(std::move(std::make_pair(searchIndex, SBSTChunkOy(leftData))));
				chunksOy[searchIndex] = std::move(SBSTChunkOy(leftData));
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			//if (chunksOy.find(searchIndex) == chunksOy.end())
			if (chunksOy[searchIndex].IsEmpty())
			{
				SBSTChunkBase::CreateRightNodeData(data, rightData);
				rightData.startY = rightData.chunkValue;
				//chunksOy[searchIndex] = std::move(std::make_shared<SBSTChunkOy>(rightData));
				//chunksOy.emplace(std::move(std::make_pair(searchIndex, SBSTChunkOy(rightData))));
				chunksOy[searchIndex] = std::move(SBSTChunkOy(rightData));
			}
		}
	}
}

SBSTChunkOy::SBSTChunkOy(SNodeData _data)
{
	mData = std::move(_data);

	bIsEmpty = false;
	chunksOz.resize(mData.chunkNumber + 1);
	//chunksOz.reserve(mData.chunkNumber + 1);

	//start (root) is last pos
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeZ;
	data.startChunkValue = mData.minOz;
	SBSTChunkBase::CreateRootNodeData(data, rootData);
	rootData.startZ = rootData.chunkValue;
	rootData.depthTree = mData.depthTree;

	chunksOz[mData.chunkNumber] = std::move(std::make_shared<SBSTChunkOz>(rootData));
	//chunksOz.emplace(std::move(std::make_pair(mData.chunkNumber, SBSTChunkOz(rootData))));

	mData.rootNodeIndex = rootData.chunkIndex;
}

void SBSTChunkOy::Find(int32_t axisKey, const SPoint3D& _point)
{
	uint8_t depthTree = mData.depthTree;
	int16_t searchIndex = mData.rootNodeIndex;

	SNodeData leftData;
	SNodeData rightData;

	for (uint8_t i = 0; i < depthTree; ++i)
	{
		assert(chunksOz[searchIndex].get() != nullptr);
		//assert(chunksOz.find(searchIndex) != chunksOz.end());

		const SNodeData& data = chunksOz[searchIndex]->GetData();

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			chunksOz[searchIndex]->Perform(_point);
			break;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			//if (chunksOz.find(searchIndex) == chunksOz.end())
			if (chunksOz[searchIndex].get() == nullptr)
			{
				SBSTChunkBase::CreateLeftNodeData(data, leftData);
				leftData.startZ = leftData.chunkValue;
				chunksOz[searchIndex] = std::move(std::make_shared<SBSTChunkOz>(leftData));
				//chunksOz.emplace(std::move(std::make_pair(searchIndex, SBSTChunkOz(leftData))));
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			//if (chunksOz.find(searchIndex) == chunksOz.end())
			if (chunksOz[searchIndex].get() == nullptr)
			{
				SBSTChunkBase::CreateRightNodeData(data, rightData);
				rightData.startZ = rightData.chunkValue;
				chunksOz[searchIndex] = std::move(std::make_shared<SBSTChunkOz>(rightData));
				//chunksOz.emplace(std::move(std::make_pair(searchIndex, SBSTChunkOz(rightData))));
			}
		}
	}
}

SBSTChunkOz::SBSTChunkOz(SNodeData _data)
{
	mData = std::move(_data);

	cubes = std::move(SMarchingCube(mData.startX, mData.startY, mData.startZ, mData.cubeSizeX, mData.cubeSizeY, mData.cubeSizeZ));
}

void SBSTChunkOz::Perform(const SPoint3D& _point)
{
	cubes.FillMeshSubSpace(_point);
}
