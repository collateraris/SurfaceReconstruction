#include <iostream>

#include "GPGPUlib/Read.h"
#include "GPGPUlib/Sort.h"

#include <algorithm>
#include <chrono>
#include <vector>
#include <fstream>

int main()
{
    GPGPUlib::SPointsData pointsData;
    GPGPUlib::ReadPoint("bunnyData.xyz", pointsData);
    std::cout << pointsData.pointsOfX.size();
    GPGPUlib::IntervalSort(pointsData, 512);
    std::cout << GPGPUlib::IntervalSortChecker(pointsData, 512);
    return 0;
}

