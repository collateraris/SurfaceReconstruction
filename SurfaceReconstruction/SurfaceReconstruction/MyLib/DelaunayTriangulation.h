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
		static void makeTriangulationBasedVoronoi(my_sr_lib::VoroFortune::SVoronoiDiagram2D<T>& voronoiDiagram, std::list<DelaunayTriangulation::STriangle<T>>& triangleList);
	};

	template<typename T>
	void CDelaunayTriangulation::makeTriangulationBasedVoronoi(my_sr_lib::VoroFortune::SVoronoiDiagram2D<T>& voronoiDiagram, std::list<DelaunayTriangulation::STriangle<T>>& triangleList)
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
			
			for (auto site1It = siteMap[site1].begin(); site1It != siteMap[site1].end(); ++site1It)
			{
				for (auto site2It = siteMap[site2].begin(); site2It != siteMap[site2].end(); ++site2It)
				{
					if ((*site1It).first == (*site2It).first)
					{
						my_sr_lib::VoroFortune::SVoronoiPoint2D<T> v3 = (*site2It).second;

						if (std::abs(v3.originalPoints.x) < EPSILON && std::abs(v3.originalPoints.y) < EPSILON && std::abs(v3.originalPoints.z) < EPSILON)
							continue;

						my_sr_lib::VoroFortune::SVoronoiPoint2D<T> v4 = { 0, 0, {(v1.originalPoints.x + v2.originalPoints.x + v3.originalPoints.x) * 0.01, (v1.originalPoints.y + v2.originalPoints.y + v3.originalPoints.y) * 0.01, (v1.originalPoints.z + v2.originalPoints.z + v3.originalPoints.z) * 0.01 } };
						triangleList.push_back({v1, v2, v3, v4});
					}
				}
			}
		}
	}
}