#include <iostream>

#include "Algorithms/CommonStruct.h"
#include "Algorithms/BinaryTree.h"
#include "File/ReadPoints.h"

#include <algorithm>
#include <chrono>

using namespace Engine::Algorithm;
using namespace Engine::File;

void BST(SBSTContainer& bst, std::vector<Engine::Algorithm::SPoint3D>& _points)
{
    for (Engine::Algorithm::SPoint3D& p : _points)
    {
        bst.Find(p);
    }
}


int main()
{
    const int32_t MULTIPLICATOR = 100000000;
    std::vector<Engine::Algorithm::SPoint3D> _points;

    CReadPoints::ParseFromText("object.pts", _points, MULTIPLICATOR);

    SNodeData data;
    data.minOx = -10 * MULTIPLICATOR;
    data.minOy = -10 * MULTIPLICATOR;
    data.minOz = -10 * MULTIPLICATOR;
    data.chunkNumber = 2048;
    data.cubeSizeX = (20 * MULTIPLICATOR) / data.chunkNumber;
    data.cubeSizeY = (20 * MULTIPLICATOR) / data.chunkNumber;
    data.cubeSizeZ = (20 * MULTIPLICATOR) / data.chunkNumber;

    SBSTContainer bst(data);
    bst.InitCubesPool(_points.size());
    auto start = std::chrono::steady_clock::now();
    BST(bst, _points);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    std::cout << CCommonStruct::GetSizeVoxelList() << std::endl;
    CCommonStruct::PrintVoxelsInObj("object.obj");
    system("pause");
}

