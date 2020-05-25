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
			VoroFortune::SVoronoiPoint2D<T> v1;
			VoroFortune::SVoronoiPoint2D<T> v2;
			VoroFortune::SVoronoiPoint2D<T> v3;
			VoroFortune::SVoronoiPoint2D<T> v4;
		};
	}

	class CDelaunayTriangulation
	{
	public:
		template<typename T>
		static void makeTriangulationBasedVoronoi(my_sr_lib::VoroFortune::SVoronoiDiagram2D<T>& voronoiDiagram, std::list<DelaunayTriangulation::STriangle<T>>& triangleList, T sqrMaxDistantion);
	};

	template<typename T>
	void CDelaunayTriangulation::makeTriangulationBasedVoronoi(my_sr_lib::VoroFortune::SVoronoiDiagram2D<T>& voronoiDiagram, std::list<DelaunayTriangulation::STriangle<T>>& triangleList, T sqrMaxDistantion)
	{
		std::size_t currSite;
		std::unordered_map<std::size_t, std::list<std::pair<std::size_t, VoroFortune::SVoronoiPoint2D<T>>>> siteMap;

		for (std::shared_ptr<my_sr_lib::VoroFortune::SSegment<T>> segment : voronoiDiagram.diagram)
		{
			// only two site here
			if (segment->siteSet.size() != 2)
				return; 

			std::size_t site1;
			std::size_t site2;
			my_sr_lib::VoroFortune::SVoronoiPoint2D<T>& v1 = segment->siteMap[site1];
			my_sr_lib::VoroFortune::SVoronoiPoint2D<T>& v2 = segment->siteMap[site2];
			auto siteIt = segment->siteSet.begin();
			site1 = *siteIt;
			site2 = *(++siteIt);
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
		for (std::shared_ptr<my_sr_lib::VoroFortune::SSegment<T>> segment : voronoiDiagram.diagram)
		{
			// only two site here
			if (segment->siteSet.size() != 2)
				return;

			std::size_t site1;
			std::size_t site2;
			auto siteIt = segment->siteSet.begin();
			site1 = *siteIt;
			site2 = *(++siteIt);
			my_sr_lib::VoroFortune::SVoronoiPoint2D<T>& v1 = segment->siteMap[site1];
			my_sr_lib::VoroFortune::SVoronoiPoint2D<T>& v2 = segment->siteMap[site2];

			SPointXYZ<T>& p1 = v1.originalPoints;
			SPointXYZ<T>& p2 = v2.originalPoints;

			T sqrDistantionp1p2 = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);

			if (sqrDistantionp1p2 > sqrMaxDistantion) continue;

			
			for (auto site1It = siteMap[site1].begin(); site1It != siteMap[site1].end(); ++site1It)
			{
				for (auto site2It = siteMap[site2].begin(); site2It != siteMap[site2].end(); ++site2It)
				{
					if ((*site1It).first == (*site2It).first)
					{
						my_sr_lib::VoroFortune::SVoronoiPoint2D<T>& v3 = (*site2It).second;

						SPointXYZ<T>& p3 = v3.originalPoints;

						T sqrDistantionp1p3 = (p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y) + (p1.z - p3.z) * (p1.z - p3.z);
						if (sqrDistantionp1p3 > sqrMaxDistantion) continue;

						T sqrDistantionp2p3 = (p2.x - p3.x) * (p2.x - p3.x) + (p2.y - p3.y) * (p2.y - p3.y) + (p2.z - p3.z) * (p2.z - p3.z);
						if (sqrDistantionp2p3 > sqrMaxDistantion) continue;

						my_sr_lib::VoroFortune::SVoronoiPoint2D<T> v4 = { 0, 0, {(v1.originalPoints.x + delta), (v1.originalPoints.y + delta), (v1.originalPoints.z + delta) } };
						
						triangleList.push_back({v1, v2, v3, v4});
					}
				}
			}
		}
	}
}