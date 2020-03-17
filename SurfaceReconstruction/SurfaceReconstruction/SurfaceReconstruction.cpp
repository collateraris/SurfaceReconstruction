﻿#include <iostream>

#include "Algorithms/CommonStruct.h"
#include "Algorithms/BinaryTree.h"
#include "File/ReadPoints.h"

#include <algorithm>
#include <chrono>
#include <omp.h>

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
//#pragma omp parallel for
    for (std::shared_ptr<Engine::Algorithm::SPoint3D>& p : _points)
    {
        bst.Find(p);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    std::cout << CCommonStruct::GetSizeVoxelList() << std::endl;
    CCommonStruct::PrintVoxelsInObj("object.obj");
}

