#include <iostream>

#include "Algorithms/CommonStruct.h"
#include "Algorithms/BinaryTree.h"
#include "File/ReadPoints.h"

#include "MyLib/Struct.h"
#include "MyLib/ReadXYZ.h"
#include "MyLib/VoronoiFortune.h"
#include "MyLib/DelaunayTriangulation.h"
#include "MyLib/WriteObj.h"

#include <algorithm>
#include <chrono>
#include <vector>
#include <fstream>


using namespace Engine::Algorithm;
using namespace Engine::File;



void BST(SBSTContainer& bst, std::vector<Engine::Algorithm::SPoint3D>& _points)
{
    for (Engine::Algorithm::SPoint3D& p : _points)
    {
        bst.Find(p);
    }
}

double rnd() { return double(rand()) / RAND_MAX; }

int main()
{
    std::vector<my_sr_lib::SPointXYZ<float>> points;
    my_sr_lib::SMinMaxPoint<float> minmax;

    const int32_t MULTIPLICATOR = 10000000;
    my_sr_lib::read_xyz("object.pts", points, minmax, MULTIPLICATOR);
    auto start = std::chrono::steady_clock::now();
    //cube size)
    std::size_t cubeSize = 1024;
    std::size_t numberTraverse = cubeSize * 3;
    float deltaX = (minmax.maxX - minmax.minX) / cubeSize;
    float invDeltaX = 1.f / deltaX;
    float deltaY = (minmax.maxY - minmax.minY) / cubeSize;
    float invDeltaY = 1.f / deltaY;
    float deltaZ = (minmax.maxZ - minmax.minZ) / cubeSize;
    float invDeltaZ = 1.f / deltaZ;
    std::size_t maxCubeInDistantion = 50;
    float sqrMaxDistantion = (maxCubeInDistantion * deltaX) * (maxCubeInDistantion * deltaX) + (maxCubeInDistantion * deltaY) * (maxCubeInDistantion * deltaY) + (maxCubeInDistantion * deltaZ) * (maxCubeInDistantion * deltaZ);
    std::vector<std::vector<my_sr_lib::VoroFortune::SVoronoiPoint2D<float>>> pointsForVoronoi(numberTraverse + 1);

    for (my_sr_lib::SPointXYZ<float>& p: points)
    {
        // x axis yz coord
        std::size_t index;
        if ((index = p.x * invDeltaX) <= cubeSize)
        {
            pointsForVoronoi[index].push_back({ p.y, p.z, p });
        }

        // y axis xz coord
        if ((index = p.y * invDeltaY) <= cubeSize)
        {
            pointsForVoronoi[cubeSize + index].push_back({ p.x, p.z, p });
        }

        // z axis xy coord
        if ((index = p.z * invDeltaZ) <= cubeSize)
        {
            pointsForVoronoi[cubeSize * 2 + index].push_back({ p.x, p.y, p });
        }
    }

    std::vector<my_sr_lib::VoroFortune::SVoronoiDiagram2D<float>> voronoiDiagram(numberTraverse + 1);
    std::vector < std::list<my_sr_lib::DelaunayTriangulation::STriangle<float>>> triangleList(numberTraverse + 1);
    my_sr_lib::SBox2D<float> boundingBox = {minmax.maxX, minmax.minX, minmax.maxY, minmax.minY};
    for (std::size_t index = 0; index < pointsForVoronoi.size(); ++index)
    {
        my_sr_lib::CVoronoiFortune::VoronoiDiagramFortune2D(pointsForVoronoi[index], voronoiDiagram[index], boundingBox);
    }

    for (std::size_t index = 0; index < pointsForVoronoi.size(); ++index)
    {
        my_sr_lib::CDelaunayTriangulation::makeTriangulationBasedVoronoi(voronoiDiagram[index], triangleList[index], sqrMaxDistantion);
    }
    my_sr_lib::print_triangulation_as_obj("build.obj", triangleList);


    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "CVoronoiFortune::VoronoiDiagramFortune2D: " << elapsed_seconds.count() << "s\n";
    return  0;

    /*
    unsigned int i;
    double x, y, z, rsq, r;
    voronoicell v;

    v.init(-1, 1, -1, 1, -1, 1);

    for (i = 0; i < 250; i++)
    {
        x = 2 * rnd() - 1;
        y = 2 * rnd() - 1;
        z = 2 * rnd() - 1;
        rsq = x * x + y * y + z * z;
        if (rsq > 0.01 && rsq < 1)
        {
            r = 1 / sqrt(rsq); x *= r; y *= r; z *= r;
            v.plane(x, y, z, 1);
        }
    }

    v.draw_gnuplot(0, 0, 0, "single_cell.gnu");

    std::vector<int> f_vert;
    std::vector<double> nor;
    v.face_orders(f_vert);
    v.normals(nor);

    const char* parity[2] = { "even","odd" };
    FILE* fp = safe_fopen("odd_even_pl.pov", "w");
    for (i = 0; i < f_vert.size(); i++)
        fprintf(fp, "plane{<%g,%g,%g>,0.5 texture{t_%s}}\n"
           , nor[3 * i], nor[3 * i + 1], nor[3 * i + 2]
           , parity[f_vert[i] & 1]);
    fclose(fp);
    v.draw_pov(0, 0, 0, "odd_even_v.pov");
    */
    /*
    const int32_t MULTIPLICATOR = 10000000;
    //const int32_t MULTIPLICATOR = 100000;
    std::vector<Engine::Algorithm::SPoint3D> _points;
    Engine::Algorithm::SMinMaxPoint minmax;

    //CReadPoints::ParseFromText("object.pts", _points, minmax, MULTIPLICATOR);
    //CReadPoints::ParseFromText("bunnyData.pts", _points, minmax, MULTIPLICATOR); //pump.ptx
    //CReadPoints::ParseFromText("vertebra.pts", _points, minmax, MULTIPLICATOR);
    CReadPoints::ParseFromText("face.xyz", _points, minmax, MULTIPLICATOR);

    std::cout << "Point size " << _points.size() << std::endl;

    SNodeData data;
    data.minOx = minmax.minX;
    data.minOy = minmax.minY;
    data.minOz = minmax.minZ;
    data.chunkNumber = 128;
    data.cubeSizeX = (minmax.maxX - minmax.minX) / data.chunkNumber;
    data.cubeSizeY = (minmax.maxY - minmax.minY) / data.chunkNumber;
    data.cubeSizeZ = (minmax.maxZ - minmax.minZ) / data.chunkNumber;

    SBSTContainer bst(data);

    {
        auto start = std::chrono::steady_clock::now();
        bst.InitCubesPool(50000);
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
        bst.CreateSolidMesh(5, 0);
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
   // */
}

