#include "Algorithms/BinaryTree.h"

using namespace Engine::Algorithm;

SBSTNodeBase::SBSTNodeBase(SNodeData _data)
{
	mData = _data;
}

void SBSTNodeBase::Find(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	if (mData.chunkValue <= axisKey && axisKey < mData.nextChunkValue)
	{
		Perform(_point);
	}
	else if (mData.deltaNode > END_DELTA_NODE)
	{
		if (axisKey < mData.chunkValue)
		{
			LeftNodeProc(axisKey, _point);
		}
		else if (mData.nextChunkValue <= axisKey)
		{
			RightNodeProc(axisKey, _point);
		}
	}
	
}


void SBSTNodeOx::LeftNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	if (leftChild.get() == nullptr)
	{
		SNodeData leftData;
		leftData = mData;
		leftData.chunkIndex = mData.chunkIndex - mData.deltaNode;
		leftData.deltaNode = mData.deltaNode >> 1;
		leftData.chunkValue = mData.startChunkValue + (leftData.chunkIndex - 1) * mData.deltaChunkValue;
		leftData.nextChunkValue = leftData.chunkValue + mData.deltaChunkValue;
		leftData.startX = leftData.chunkValue;
		leftChild = std::make_shared<SBSTNodeOx>(leftData);
	}
	leftChild->Find(axisKey, _point);
}

void SBSTNodeOx::RightNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	if (rightChild.get() == nullptr)
	{
		SNodeData rightData;
		rightData = mData;
		rightData.chunkIndex = mData.chunkIndex + mData.deltaNode;
		rightData.deltaNode = mData.deltaNode - (mData.deltaNode >> 1);
		rightData.chunkValue = mData.startChunkValue + (rightData.chunkIndex - 1) * mData.deltaChunkValue;
		rightData.nextChunkValue = rightData.chunkValue + mData.deltaChunkValue;
		rightData.startX = rightData.chunkValue;
		rightChild = std::make_shared<SBSTNodeOx>(rightData);
	}
	rightChild->Find(axisKey, _point);
}

void SBSTNodeOx::Perform(std::shared_ptr<SPoint3D> _point)
{
	if (sheet.get() == nullptr)
	{
		// example, if chunk number is 100. => chunckIndex for BST 100 / 2 = 50;
		//=> deltaNode 50/ 2 = 25 
		SNodeData sheetData = mData;
		sheetData.chunkIndex = mData.chunkNumber >> 1;
		sheetData.deltaNode = sheetData.chunkIndex >> 1;
		sheetData.deltaChunkValue = mData.cubeSizeY;
		sheetData.startChunkValue = mData.minOy;
		sheetData.chunkValue = sheetData.startChunkValue + (sheetData.chunkIndex - 1) * sheetData.deltaChunkValue;
		sheetData.nextChunkValue = sheetData.chunkValue + sheetData.deltaChunkValue;
		sheetData.startY = sheetData.chunkValue;
		sheet = std::make_shared<SBSTNodeOy>(sheetData);
	}
	sheet->Find(_point->GetY(), _point);
}

void SBSTNodeOy::LeftNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	if (leftChild.get() == nullptr)
	{
		SNodeData leftData;
		leftData = mData;
		leftData.chunkIndex = mData.chunkIndex - mData.deltaNode;
		leftData.deltaNode = mData.deltaNode >> 1;
		leftData.chunkValue = mData.startChunkValue + (leftData.chunkIndex - 1) * mData.deltaChunkValue;
		leftData.nextChunkValue = leftData.chunkValue + mData.deltaChunkValue;
		leftData.startY = leftData.chunkValue;
		leftChild = std::make_shared<SBSTNodeOy>(leftData);
	}
	leftChild->Find(axisKey, _point);
}

void SBSTNodeOy::RightNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	if (rightChild.get() == nullptr)
	{
		SNodeData rightData;
		rightData = mData;
		rightData.chunkIndex = mData.chunkIndex + mData.deltaNode;
		rightData.deltaNode = mData.deltaNode - (mData.deltaNode >> 1);
		rightData.chunkValue = mData.startChunkValue + (rightData.chunkIndex - 1) * mData.deltaChunkValue;
		rightData.nextChunkValue = rightData.chunkValue + mData.deltaChunkValue;
		rightData.startY = rightData.chunkValue;
		rightChild = std::make_shared<SBSTNodeOy>(rightData);
	}
	rightChild->Find(axisKey, _point);
}

void SBSTNodeOy::Perform(std::shared_ptr<SPoint3D> _point)
{
	if (sheet.get() == nullptr)
	{
		// example, if chunk number is 100. => chunckIndex for BST 100 / 2 = 50;
		//=> deltaNode 50/ 2 = 25 
		SNodeData sheetData = mData;
		sheetData.chunkIndex = mData.chunkNumber >> 1;
		sheetData.deltaNode = sheetData.chunkIndex >> 1;
		sheetData.deltaChunkValue = mData.cubeSizeZ;
		sheetData.startChunkValue = mData.minOz;
		sheetData.chunkValue = sheetData.startChunkValue + (sheetData.chunkIndex - 1) * sheetData.deltaChunkValue;
		sheetData.nextChunkValue = sheetData.chunkValue + sheetData.deltaChunkValue;
		sheetData.startZ = sheetData.chunkValue;
		sheet = std::make_shared<SBSTNodeOz>(sheetData);
	}
	sheet->Find(_point->GetZ(), _point);
}

void SBSTNodeOz::LeftNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	if (leftChild.get() == nullptr)
	{
		SNodeData leftData;
		leftData = mData;
		leftData.chunkIndex = mData.chunkIndex - mData.deltaNode;
		leftData.deltaNode = mData.deltaNode >> 1;
		leftData.chunkValue = mData.startChunkValue + (leftData.chunkIndex - 1) * mData.deltaChunkValue;
		leftData.nextChunkValue = leftData.chunkValue + mData.deltaChunkValue;
		leftData.startZ = leftData.chunkValue;
		leftChild = std::make_shared<SBSTNodeOy>(leftData);
	}
	leftChild->Find(axisKey, _point);
}

void SBSTNodeOz::RightNodeProc(double axisKey, std::shared_ptr<SPoint3D> _point)
{
	if (rightChild.get() == nullptr)
	{
		SNodeData rightData;
		rightData = mData;
		rightData.chunkIndex = mData.chunkIndex + mData.deltaNode;
		rightData.deltaNode = mData.deltaNode - (mData.deltaNode >> 1);
		rightData.chunkValue = mData.startChunkValue + (rightData.chunkIndex - 1) * mData.deltaChunkValue;
		rightData.nextChunkValue = rightData.chunkValue + mData.deltaChunkValue;
		rightData.startZ = rightData.chunkValue;
		rightChild = std::make_shared<SBSTNodeOy>(rightData);
	}
	rightChild->Find(axisKey, _point);
}

void SBSTNodeOz::Perform(std::shared_ptr<SPoint3D> _point)
{
	if (sheet.get() == nullptr)
	{
		sheet = std::make_shared<SMarchingCube>(mData.startX, mData.startY, mData.startZ, mData.cubeSizeX, mData.cubeSizeY, mData.cubeSizeZ);
	}
	sheet->FillMeshSubSpace(_point);
}

CBSTforSR::CBSTforSR(SNodeData data)
{
	SNodeData rootData = data;
	rootData.chunkIndex = data.chunkNumber >> 1;
	rootData.deltaNode = rootData.chunkIndex >> 1;
	rootData.deltaChunkValue = data.cubeSizeX;
	rootData.startChunkValue = data.minOx;
	rootData.chunkValue = rootData.startChunkValue + (rootData.chunkIndex - 1) * rootData.deltaChunkValue;
	rootData.nextChunkValue = rootData.chunkValue + rootData.deltaChunkValue;

	rootNode = std::make_shared<SBSTNodeOx>(rootData);
}

void CBSTforSR::Find(std::shared_ptr<SPoint3D> _point)
{
	rootNode->Find(_point->GetX(), _point);
}