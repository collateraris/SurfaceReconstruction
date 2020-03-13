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

	base = std::make_shared<SBSTChunkBase>();
	//start (root) is last pos
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeX;
	data.startChunkValue = mData.minOx;
	base->CreateRootNodeData(data, rootData);
	mData.depthTree = log2(mData.chunkNumber) + 1;
	rootData.depthTree = mData.depthTree;
	rootData.startX = rootData.chunkValue;

	chunksOx[mData.chunkNumber] = std::make_shared<SBSTChunkOx>(rootData);

	mData.rootNodeIndex = rootData.chunkIndex;
}

void SBSTContainer::Find(const std::shared_ptr<SPoint3D>& _point)
{
	int depthTree = mData.depthTree;
	int searchIndex = mData.rootNodeIndex;

	double axisKey = _point->GetX();

	SNodeData leftData;
	SNodeData rightData;

	for (int i = 0; i < depthTree; ++i)
	{
		assert(chunksOx[searchIndex].get() != nullptr);

		const SNodeData& data = chunksOx[searchIndex]->GetData();

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			chunksOx[searchIndex]->Find(_point->GetY(), _point);
			break;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			if (chunksOx[searchIndex].get() == nullptr)
			{
				base->CreateLeftNodeData(data, leftData);
				leftData.startX = leftData.chunkValue;
				chunksOx[searchIndex] = std::make_shared<SBSTChunkOx>(leftData);
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			if (chunksOx[searchIndex].get() == nullptr)
			{
				base->CreateRightNodeData(data, rightData);
				rightData.startX = rightData.chunkValue;
				chunksOx[searchIndex] = std::make_shared<SBSTChunkOx>(rightData);
			}
		}
	}
}

SBSTChunkOx::SBSTChunkOx(SNodeData _data)
{
	mData = std::move(_data);

	chunksOy.resize(mData.chunkNumber + 1);

	base = std::make_shared<SBSTChunkBase>();
	//start (root) is last pos
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeY;
	data.startChunkValue = mData.minOy;
	base->CreateRootNodeData(data, rootData);
	rootData.startY = rootData.chunkValue;
	rootData.depthTree = mData.depthTree;

	chunksOy[mData.chunkNumber] = std::make_shared<SBSTChunkOy>(rootData);

	mData.rootNodeIndex = rootData.chunkIndex;
}

void SBSTChunkOx::Find(double axisKey,const std::shared_ptr<SPoint3D>& _point)
{
	int depthTree = mData.depthTree;
	int searchIndex = mData.rootNodeIndex;

	SNodeData leftData;
	SNodeData rightData;

	for (int i = 0; i < depthTree; ++i)
	{
		assert(chunksOy[searchIndex].get() != nullptr);

		const SNodeData& data = chunksOy[searchIndex]->GetData();

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			chunksOy[searchIndex]->Find(_point->GetZ(), _point);
			break;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			if (chunksOy[searchIndex].get() == nullptr)
			{
				base->CreateLeftNodeData(data, leftData);
				leftData.startY = leftData.chunkValue;
				chunksOy[searchIndex] = std::make_shared<SBSTChunkOy>(leftData);
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			if (chunksOy[searchIndex].get() == nullptr)
			{
				base->CreateRightNodeData(data, rightData);
				rightData.startY = rightData.chunkValue;
				chunksOy[searchIndex] = std::make_shared<SBSTChunkOy>(rightData);
			}
		}
	}
}

SBSTChunkOy::SBSTChunkOy(SNodeData _data)
{
	mData = std::move(_data);

	chunksOz.resize(mData.chunkNumber + 1);

	base = std::make_shared<SBSTChunkBase>();
	//start (root) is last pos
	SNodeData rootData;
	SNodeData data = mData;
	data.deltaChunkValue = mData.cubeSizeZ;
	data.startChunkValue = mData.minOz;
	base->CreateRootNodeData(data, rootData);
	rootData.startZ = rootData.chunkValue;
	rootData.depthTree = mData.depthTree;

	chunksOz[mData.chunkNumber] = std::make_shared<SBSTChunkOz>(rootData);

	mData.rootNodeIndex = rootData.chunkIndex;
}

void SBSTChunkOy::Find(double axisKey, const std::shared_ptr<SPoint3D>& _point)
{
	int depthTree = mData.depthTree;
	int searchIndex = mData.rootNodeIndex;

	SNodeData leftData;
	SNodeData rightData;

	for (int i = 0; i < depthTree; ++i)
	{
		assert(chunksOz[searchIndex].get() != nullptr);

		const SNodeData& data = chunksOz[searchIndex]->GetData();

		if (data.chunkValue <= axisKey && axisKey <= data.nextChunkValue)
		{
			chunksOz[searchIndex]->Perform(_point);
			break;
		}

		if (axisKey < data.chunkValue)
		{
			searchIndex = data.leftNodeIndex;
			if (chunksOz[searchIndex].get() == nullptr)
			{
				base->CreateLeftNodeData(data, leftData);
				leftData.startZ = leftData.chunkValue;
				chunksOz[searchIndex] = std::make_shared<SBSTChunkOz>(leftData);
			}
		}
		else if (data.nextChunkValue < axisKey)
		{
			searchIndex = data.rightNodeIndex;
			if (chunksOz[searchIndex].get() == nullptr)
			{
				base->CreateRightNodeData(data, rightData);
				rightData.startZ = rightData.chunkValue;
				chunksOz[searchIndex] = std::make_shared<SBSTChunkOz>(rightData);
			}
		}
	}
}

SBSTChunkOz::SBSTChunkOz(SNodeData _data)
{
	mData = std::move(_data);

	cubes = std::make_shared<SMarchingCube>(mData.startX, mData.startY, mData.startZ, mData.cubeSizeX, mData.cubeSizeY, mData.cubeSizeZ);
}

void SBSTChunkOz::Perform(const std::shared_ptr<SPoint3D>& _point)
{
	cubes->FillMeshSubSpace(_point);
}
