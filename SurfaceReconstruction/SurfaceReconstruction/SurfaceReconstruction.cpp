#include <iostream>

#include "Algorithms/CommonStruct.h"
#include "Algorithms/BinaryTree.h"

using namespace Engine::Algorithm;

int main()
{
    SPoint3D p1(-0.15, -0.5, 0.44);
    SPoint3D p2(0.35, 0.01, 0.37);
    SPoint3D p3(0.01, 0.364, -0.9);

    SNodeData data;
    data.minOx = -.15;
    data.minOy = 0.01;
    data.minOz = -0.9;
    data.chunkNumber = 16;
    data.cubeSizeX = (0.35 + 0.15) * 0.1;
    data.cubeSizeY = (0.364 + 0.5) * 0.1;
    data.cubeSizeZ = (0.44 + 0.9) * 0.1;

    CBSTforSR bst(data);
    bst.Find(std::make_shared<SPoint3D>(p1));
    bst.Find(std::make_shared<SPoint3D>(p2));
    bst.Find(std::make_shared<SPoint3D>(p3));
    bst.Find(std::make_shared<SPoint3D>(p1));

}

