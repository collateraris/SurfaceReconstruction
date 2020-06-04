#pragma once

#include <list>
#include <unordered_map>

#include "VoronoiFortune.h"

namespace my_sr_lib
{
	namespace DelaunayTriangulation
	{
		template<typename T>
		struct STriangle
		{
			VoronoiPoint2D<T> v1;
			VoronoiPoint2D<T> v2;
			VoronoiPoint2D<T> v3;
			VoronoiPoint2D<T> v4;
		};
	}

	class CDelaunayTriangulation
	{
	public:
		template<typename T>
		static void makeTriangulationBasedVoronoi(my_sr_lib::VoronoiDiagram2D<T>& voronoiDiagram, std::list<DelaunayTriangulation::STriangle<T>>& triangleList, T sqrMaxDistantion);
	};

	template<typename T>
	void CDelaunayTriangulation::makeTriangulationBasedVoronoi(my_sr_lib::VoronoiDiagram2D<T>& voronoiDiagram, std::list<DelaunayTriangulation::STriangle<T>>& triangleList, T sqrMaxDistantion)
	{
		std::size_t currSite;
		std::unordered_map<std::size_t, std::list<std::pair<std::size_t, VoronoiPoint2D<T>>>> siteMap;
		
		for (auto* segment : voronoiDiagram.diagram)
		{
			if (segment->siteSet.size() != 2 || segment->siteMap.size() != 2)
			{
				segment->bIsDone = false;
				continue;
			}
			continue;
			std::size_t site1;
			std::size_t site2;
			auto siteIt = segment->siteSet.begin();
			site1 = *siteIt;
			site2 = *(++siteIt);
			my_sr_lib::SPointXYZ<T>& v1 = segment->siteMap[site1].originalPoints;
			my_sr_lib::SPointXYZ<T>& v2 = segment->siteMap[site2].originalPoints;
			T sqrDistantionp2p3 = (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z);
			if (sqrDistantionp2p3 > sqrMaxDistantion)
			{
				segment->bIsDone = false;
				continue;
			}
		}
		

		for (auto* segment : voronoiDiagram.diagram)
		{
			if (!segment->bIsDone)
				continue;

			std::size_t site1;
			std::size_t site2;
			auto siteIt = segment->siteSet.begin();
			site1 = *siteIt;
			site2 = *(++siteIt);
			my_sr_lib::VoronoiPoint2D<T>& v1 = segment->siteMap[site1];
			my_sr_lib::VoronoiPoint2D<T>& v2 = segment->siteMap[site2];
			if (siteMap.find(site1) == siteMap.end())
				siteMap[site1] = { std::make_pair(site2, v2) };
			else
				siteMap[site1].push_back(std::make_pair(site2, v2));

			if (siteMap.find(site2) == siteMap.end())
				siteMap[site2] = { std::make_pair(site1, v1) };
			else
				siteMap[site2].push_back(std::make_pair(site1, v1));
		}

		float delta = 0.5;

		//search common member
		for (auto* segment : voronoiDiagram.diagram)
		{
			if (!segment->bIsDone)
				continue;

			std::size_t site1;
			std::size_t site2;
			auto siteIt = segment->siteSet.begin();
			site1 = *siteIt;
			site2 = *(++siteIt);
			my_sr_lib::VoronoiPoint2D<T>& v1 = segment->siteMap[site1];
			my_sr_lib::VoronoiPoint2D<T>& v2 = segment->siteMap[site2];
		
			for (auto site1It = siteMap[site1].begin(); site1It != siteMap[site1].end(); ++site1It)
			{
				for (auto site2It = siteMap[site2].begin(); site2It != siteMap[site2].end(); ++site2It)
				{
					if ((*site1It).first == (*site2It).first)
					{
						my_sr_lib::VoronoiPoint2D<T>& v3 = (*site2It).second;

						SPointXYZ<T>& p3 = v3.originalPoints;

						my_sr_lib::VoronoiPoint2D<T> v4 = { 0, 0, {(v1.originalPoints.x + delta), (v1.originalPoints.y + delta), (v1.originalPoints.z + delta) } };
						
						triangleList.push_back({v1, v2, v3, v4});
					}
				}
			}
		}
	}
}