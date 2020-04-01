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
    Engine::Algorithm::SMinMaxPoint minmax;

    CReadPoints::ParseFromText("object.pts", _points, minmax, MULTIPLICATOR);

    std::cout << "Point size " << _points.size() << std::endl;

    SNodeData data;
    data.minOx = minmax.minX;
    data.minOy = minmax.minY;
    data.minOz = minmax.minZ;
    data.chunkNumber = 256;
    data.cubeSizeX = (minmax.maxX - minmax.minX) / data.chunkNumber;
    data.cubeSizeY = (minmax.maxY - minmax.minY) / data.chunkNumber;
    data.cubeSizeZ = (minmax.maxZ - minmax.minZ) / data.chunkNumber;

    SBSTContainer bst(data);

    {
        auto start = std::chrono::steady_clock::now();
        bst.InitCubesPool(_points.size());
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Create voxel pool elapsed time: " << elapsed_seconds.count() << "s\n";
    }
 
    {
        auto start = std::chrono::steady_clock::now();
        BST(bst, _points);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Create voxel tree elapsed time: " << elapsed_seconds.count() << "s\n";
    }

    {
        auto start = std::chrono::steady_clock::now();
        bst.CreateSolidMesh(3, 0);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Create solid mesh elapsed time: " << elapsed_seconds.count() << "s\n";
    }

    std::list<SVertexVoxelUnit> allMeshesAsObj;

    {
        auto start = std::chrono::steady_clock::now();
        bst.GetAllMeshes(allMeshesAsObj);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Getting all meshes elapsed time: " << elapsed_seconds.count() << "s\n";
    }

    std::cout << allMeshesAsObj.size() << std::endl;
    CCommonStruct::PrintVoxelsInObj("object.obj", allMeshesAsObj);
    system("pause");
}

