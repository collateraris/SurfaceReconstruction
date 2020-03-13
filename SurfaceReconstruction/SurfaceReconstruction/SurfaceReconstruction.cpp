#include <iostream>

#include "Algorithms/CommonStruct.h"
#include "Algorithms/BinaryTree.h"
#include "File/ReadPoints.h"

#include <algorithm>
#include <chrono>

using namespace Engine::Algorithm;
using namespace Engine::File;

int main()
{
    std::vector<std::shared_ptr<Engine::Algorithm::SPoint3D>> _points;

    CReadPoints::ParseFromText("object.pts", _points);

    SNodeData data;
    data.minOx = -10.;
    data.minOy = -10.;
    data.minOz = -10.;
    data.chunkNumber = 512;
    data.cubeSizeX = (20.) / data.chunkNumber;
    data.cubeSizeY = (20.) / data.chunkNumber;
    data.cubeSizeZ = (20.) / data.chunkNumber;

    SBSTContainer bst(data);

    auto start = std::chrono::steady_clock::now();
    for (auto it = _points.begin(); it != _points.end(); ++it)
    {
        bst.Find(*it);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    std::cout << CCommonStruct::GetSizeVoxelList() << std::endl;
    CCommonStruct::PrintVoxelsInObj("object.obj");
}

