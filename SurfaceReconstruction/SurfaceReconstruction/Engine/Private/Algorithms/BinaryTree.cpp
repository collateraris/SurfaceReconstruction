#include "Algorithms/BinaryTree.h"

#include <cassert>

using namespace Engine::Algorithm;



SBSTContainer::SBSTContainer(SNodeData _data)
{
	mData = std::move(_data);

	chunksOx.resize(mData.chunkNumber);

	base = std::make_shared<SBSTChunkBase>();
	//start (root) is last pos
	SNodeData rootData;
	base->CreateRootNodeData(mData, rootData);

	chunksOx[mData.chunkNumber] = std::make_shared<SBSTChunkOx>(rootData);
	
	mData.depthTree = log2(mData.chunkNumber) + 1;
	mData.rightNodeIndex = rootData.chunkIndex;
}

void SBSTContainer::Find(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	int depthTree = mData.depthTree;
	int searchIndex = mData.rootNodeIndex;

	SNodeData leftData;
	SNodeData rightData;

	for (int i = 0; i < depthTree; ++i)
	{
		assert(chunksOx[searchIndex].get() == nullptr);

		const SNodeData& data = chunksOx[searchIndex]->GetData();

		if (data.chunkValue <= axisKey && axisKey < data.nextChunkValue)
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
				chunksOx[searchIndex] = std::make_shared<SBSTChunkOx>(leftData);
			}
		}
		else if (data.nextChunkValue <= axisKey)
		{
			searchIndex = data.rightNodeIndex;
			if (chunksOx[searchIndex].get() == nullptr)
			{
				base->CreateRightNodeData(data, rightData);
				chunksOx[searchIndex] = std::make_shared<SBSTChunkOx>(rightData);
			}
		}
	}
}

void SBSTChunkOx::Find(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	int depthTree = mData.depthTree;
	int searchIndex = mData.rootNodeIndex;

	SNodeData leftData;
	SNodeData rightData;

	for (int i = 0; i < depthTree; ++i)
	{
		assert(chunksOy[searchIndex].get() == nullptr);

		const SNodeData& data = chunksOy[searchIndex]->GetData();

		if (data.chunkValue <= axisKey && axisKey < data.nextChunkValue)
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
				chunksOy[searchIndex] = std::make_shared<SBSTChunkOx>(leftData);
			}
		}
		else if (data.nextChunkValue <= axisKey)
		{
			searchIndex = data.rightNodeIndex;
			if (chunksOy[searchIndex].get() == nullptr)
			{
				base->CreateRightNodeData(data, rightData);
				chunksOy[searchIndex] = std::make_shared<SBSTChunkOx>(rightData);
			}
		}
	}
}

void SBSTChunkOy::Find(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	int depthTree = mData.depthTree;
	int searchIndex = mData.rootNodeIndex;

	SNodeData leftData;
	SNodeData rightData;

	for (int i = 0; i < depthTree; ++i)
	{
		assert(chunksOz[searchIndex].get() == nullptr);

		const SNodeData& data = chunksOz[searchIndex]->GetData();

		if (data.chunkValue <= axisKey && axisKey < data.nextChunkValue)
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
				chunksOz[searchIndex] = std::make_shared<SBSTChunkOx>(leftData);
			}
		}
		else if (data.nextChunkValue <= axisKey)
		{
			searchIndex = data.rightNodeIndex;
			if (chunksOz[searchIndex].get() == nullptr)
			{
				base->CreateRightNodeData(data, rightData);
				chunksOz[searchIndex] = std::make_shared<SBSTChunkOx>(rightData);
			}
		}
	}
}

void SBSTChunkOz::Perform(std::shared_ptr<SPoint3D> _point)
{
	cubes->FillMeshSubSpace(_point);
}
