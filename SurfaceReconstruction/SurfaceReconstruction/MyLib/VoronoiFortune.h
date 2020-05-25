#pragma once

#include <memory>
#include <algorithm>
#include <set>
#include <queue>
#include <functional>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Struct.h"

namespace my_sr_lib
{
	namespace VoroFortune
	{
		const double EPSILON = 1e-3;

		template<typename T>
		struct SVoronoiPoint2D
		{
			T x;
			T y;

			SPointXYZ<T> originalPoints;
		};

		template<typename T>
		struct SSegment
		{
			SPoint2D<T> mStart, mEnd;
			bool bIsDone;

			SSegment(SPoint2D<T>& p)
				: mStart(p), bIsDone(false) {};

			void finish(const SPoint2D<T>& _end)
			{
				if (bIsDone) return;
				mEnd = _end;
				bIsDone = true;
			}

			std::unordered_set<std::size_t> siteSet;
			std::unordered_map<std::size_t, SVoronoiPoint2D<T>> siteMap;
		};

		template<typename T>
		struct SArc;

		template<typename T>
		struct SCircleEvent
		{
			T mX;
			SPoint2D<T> center;
			std::shared_ptr<SArc<T>> arc;
			bool bIsValid;

			SCircleEvent(T xCoord, SPoint2D<T>& p, std::shared_ptr<SArc<T>>& a)
				: mX(xCoord), center(p), arc(a), bIsValid(true) {};
		};

		template<typename T>
		struct SArc
		{
			SArc(SVoronoiPoint2D<T>& _p, std::shared_ptr<SArc<T>> a = nullptr, std::shared_ptr<SArc<T>> b = nullptr)
				: mPoint(_p), mPrev(a), mNext(b) {};

			SVoronoiPoint2D<T> mPoint;
			std::shared_ptr<SArc<T>> mPrev = nullptr;
			std::shared_ptr<SArc<T>> mNext = nullptr;
			std::shared_ptr<SCircleEvent<T>> mCircleEvent = nullptr;

			std::shared_ptr<SSegment<T>> mSegment1 = nullptr;
			std::shared_ptr<SSegment<T>> mSegment2 = nullptr;
		};

		template<typename T>
		struct SVoronoiDiagram2D
		{
			std::vector<std::shared_ptr<SSegment<T>>> diagram;

			std::unordered_map<std::size_t, std::list<std::shared_ptr<SSegment<T>>>> siteSegmentMap;
		};

		// "Greater than" comparison, for reverse sorting in priority queue.
		template<typename T>
		struct gt {
			bool operator()(VoroFortune::SVoronoiPoint2D<T>& a, VoroFortune::SVoronoiPoint2D<T>& b) { return (std::abs(a.x - b.x) < EPSILON) ? a.y > b.y : a.x > b.x; }
			bool operator()(std::shared_ptr<SCircleEvent<T>>& a, std::shared_ptr<SCircleEvent<T>>& b) { return a->mX > b->mX; }
		};
	}

	class CVoronoiFortune
	{
	public:

		template<typename T>
		static void VoronoiDiagramFortune2D(std::vector<VoroFortune::SVoronoiPoint2D<T>>& points, VoroFortune::SVoronoiDiagram2D<T>& voronoiDiagram, const SBox2D<T>& boundingBox);
	};

	template<typename T>
	void CVoronoiFortune::VoronoiDiagramFortune2D(std::vector<VoroFortune::SVoronoiPoint2D<T>>& points, VoroFortune::SVoronoiDiagram2D<T>& voronoiDiagram, const SBox2D<T>& boundingBox)
	{
		if (points.size() == 0) return;

		std::shared_ptr<VoroFortune::SArc<T>> rootArcTree;

		voronoiDiagram.siteSegmentMap.reserve(points.size());

		std::priority_queue<VoroFortune::SVoronoiPoint2D<T>, std::vector<VoroFortune::SVoronoiPoint2D<T>>, VoroFortune::gt<T>> siteEvents;
		for (VoroFortune::SVoronoiPoint2D<T>& p : points)
		{
			siteEvents.push(p);
		}
	
		std::priority_queue<std::shared_ptr<VoroFortune::SCircleEvent<T>>, std::vector<std::shared_ptr<VoroFortune::SCircleEvent<T>>>, VoroFortune::gt<T>> circleEvents;
	
		// Find the rightmost point on the circle through a,b,c.
		std::function<bool(VoroFortune::SVoronoiPoint2D<T>&, VoroFortune::SVoronoiPoint2D<T>&, VoroFortune::SVoronoiPoint2D<T>&, T&, SPoint2D<T>&)> circle
			= [&](VoroFortune::SVoronoiPoint2D<T>& a, VoroFortune::SVoronoiPoint2D<T>& b, VoroFortune::SVoronoiPoint2D<T>& c, T& x, SPoint2D<T>& origin) -> bool
		{
			// Check that bc is a "right turn" from ab.
			if ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) > 0.)
				return false;

			// Algorithm from O'Rourke 2ed p. 189.
			T A = b.x - a.x,
				B = b.y - a.y,
				C = c.x - a.x,
				D = c.y - a.y,
				E = A * (a.x + b.x) + B * (a.y + b.y),
				F = C * (a.x + c.x) + D * (a.y + c.y),
				G = 2. * (A * (c.y - b.y) - B * (c.x - b.x));

			if (std::abs(G) < EPSILON) return false; // Points are co-linear.

			// Point origin is the center of the circle.
			origin.x = (D * E - B * F) / G;
			origin.y = (A * F - C * E) / G;

			// o.x plus radius equals max x coordinate.
			x = origin.x + std::sqrt((a.x - origin.x) * (a.x - origin.x)
				+ (a.y - origin.y) * (a.y - origin.y));

			return true;
		};

		// Look for a new circle event for arc i.
		std::function<void(std::shared_ptr<VoroFortune::SArc<T>>&, T)> check_circle_event
			= [&](std::shared_ptr<VoroFortune::SArc<T>>& arc, T x0)
		{
			// Invalidate any old event.
			if (arc->mCircleEvent.get() && std::abs(arc->mCircleEvent->mX - x0) > EPSILON)
			{
				arc->mCircleEvent->bIsValid = false;
			}
			arc->mCircleEvent.reset();

			if (!arc->mPrev.get() || !arc->mNext.get())
				return;

			T x;
			SPoint2D<T> origin;
			if (circle(arc->mPrev->mPoint, arc->mPoint, arc->mNext->mPoint, x, origin) && x > x0)
			{
				// Create new event.
				arc->mCircleEvent = std::make_shared<VoroFortune::SCircleEvent<T>>(x, origin, arc);
				circleEvents.push(arc->mCircleEvent);
			}
		};

		// Where do two parabolas intersect?
		std::function<SPoint2D<T>(VoroFortune::SVoronoiPoint2D<T>&, VoroFortune::SVoronoiPoint2D<T>&, T)> intersection
			= [&](VoroFortune::SVoronoiPoint2D<T>& _p0, VoroFortune::SVoronoiPoint2D<T>& _p1, T l)
			->SPoint2D<T>
		{
			SPoint2D<T> result;
			VoroFortune::SVoronoiPoint2D<T> p = _p0;

			if (std::abs(_p0.x - _p1.x) < EPSILON)
			{
				result.y = (_p0.y + _p1.y) * 0.5;
			}
			else if (std::abs(_p1.x - l) < EPSILON)
			{
				result.y = _p1.y;
			}
			else if (std::abs(_p0.x - l) < EPSILON)
			{
				result.y = _p0.y;
				p = _p1;
			}
			else
			{
				T z0 = 2. * (_p0.x - l);
				T z1 = 2. * (_p1.x - l);

				T a = 1. / z0 - 1. / z1;
				T b = -2. * (_p0.y / z0 - _p1.y / z1);
				T c = (_p0.y * _p0.y + _p0.x * _p0.x - l * l) / z0
					- (_p1.y * _p1.y + _p1.x * _p1.x - l * l) / z1;

				result.y = (-b - std::sqrt(b * b - 4 * a * c)) / (2 * a);
			}

			result.x = (p.x * p.x + (p.y - result.y) * (p.y - result.y) - l * l) / (2. * p.x - 2. * l);
			return result;
		};

		// Will a new parabola at point p intersect with arc i?
		std::function<bool(VoroFortune::SVoronoiPoint2D<T>&, std::shared_ptr<VoroFortune::SArc<T>>&, SPoint2D<T>&)> intersect
			= [&](VoroFortune::SVoronoiPoint2D<T>& _p, std::shared_ptr<VoroFortune::SArc<T>>& arc, SPoint2D<T>& result) -> bool
		{
			if (std::abs(arc->mPoint.x - _p.x) < EPSILON) return false;

			double a, b;
			if (arc->mPrev.get())
				a = intersection(arc->mPrev->mPoint, arc->mPoint, _p.x).y;
			if (arc->mNext.get())
				b = intersection(arc->mPoint, arc->mNext->mPoint, _p.x).y;

			if ((arc->mPrev.get() || a <= _p.y) && (arc->mNext.get() || _p.y <= b))
			{
				result.y = _p.y;

				result.x = (arc->mPoint.x * arc->mPoint.x +
					(arc->mPoint.y - result.y) * (arc->mPoint.y - result.y)
					- _p.x * _p.x) / (2. * arc->mPoint.x - 2. * _p.x);

				return true;
			}

			return false;
		};

		std::function<void(VoroFortune::SVoronoiPoint2D<T>&)> front_insert = [&](VoroFortune::SVoronoiPoint2D<T>& _p)
		{
			if (!rootArcTree.get())
			{
				rootArcTree = std::make_shared<VoroFortune::SArc<T>>(_p);
				return;
			}

			for (std::shared_ptr<VoroFortune::SArc<T>> arc = rootArcTree; arc.get(); arc = arc->mNext)
			{
				SPoint2D<T> z, zz;
				if (intersect(_p, arc, z))
				{
					// New parabola intersects arc i.  If necessary, duplicate i.
					if (arc->mNext.get() && !intersect(_p, arc->mNext, zz))
					{
						arc->mNext->mPrev = std::make_shared<VoroFortune::SArc<T>>(arc->mPoint, arc, arc->mNext);
						arc->mNext = arc->mNext->mPrev;
					}
					else
					{
						arc->mNext = std::make_shared<VoroFortune::SArc<T>>(arc->mPoint, arc);
					}
					arc->mNext->mSegment2 = arc->mSegment2;

					// Add p between i and i->next.
					arc->mNext->mPrev = std::make_shared<VoroFortune::SArc<T>>(_p, arc, arc->mNext);
					arc->mNext = arc->mNext->mPrev;

					arc = arc->mNext; // Now i points to the new arc.

					// Add new half-edges connected to i's endpoints.
					arc->mPrev->mSegment2 = arc->mSegment1 = std::make_shared<VoroFortune::SSegment<T>>(z);
					voronoiDiagram.diagram.push_back(arc->mPrev->mSegment2);
					arc->mNext->mSegment1 = arc->mSegment2 = std::make_shared<VoroFortune::SSegment<T>>(z);
					voronoiDiagram.diagram.push_back(arc->mNext->mSegment1);

					{
						std::size_t siteHashVal = std::hash<T>{}(arc->mPoint.x) ^ std::hash<T> {}(arc->mPoint.y);
						arc->mSegment1->siteSet.insert(siteHashVal);
						arc->mSegment1->siteMap[siteHashVal] = arc->mPoint;

						arc->mSegment2->siteSet.insert(siteHashVal);
						arc->mSegment2->siteMap[siteHashVal] = arc->mPoint;

					}

					{
						std::size_t siteHashVal = std::hash<T>{}(arc->mPrev->mPoint.x) ^ std::hash<T> {}(arc->mPrev->mPoint.y);
						arc->mSegment1->siteSet.insert(siteHashVal);
						arc->mSegment1->siteMap[siteHashVal] = arc->mPrev->mPoint;

						arc->mPrev->mSegment2->siteSet.insert(siteHashVal);
						arc->mPrev->mSegment2->siteMap[siteHashVal] = arc->mPrev->mPoint;
					}

					{
						std::size_t siteHashVal = std::hash<T>{}(arc->mNext->mPoint.x) ^ std::hash<T> {}(arc->mNext->mPoint.y);
						arc->mSegment2->siteSet.insert(siteHashVal);
						arc->mSegment2->siteMap[siteHashVal] = arc->mNext->mPoint;


						arc->mNext->mSegment1->siteSet.insert(siteHashVal);
						arc->mNext->mSegment1->siteMap[siteHashVal] = arc->mNext->mPoint;

					}

					// Check for new circle events around the new arc:
					check_circle_event(arc, _p.x);
					check_circle_event(arc->mPrev, _p.x);
					check_circle_event(arc->mNext, _p.x);
					return;
				}
			}

			// Special case: If p never intersects an arc, append it to the list.
			std::shared_ptr<VoroFortune::SArc<T>> i;
			for (i = rootArcTree; i->mNext.get(); i = i->mNext);
			i->mNext = std::make_shared<VoroFortune::SArc<T>>(_p, i);

			// Insert segment between p and i
			SPoint2D<T> start;
			start.x = boundingBox.minX;
			start.y = (i->mNext->mPoint.y + i->mPoint.y) * 0.5;
			i->mSegment2 = i->mNext->mSegment1 = std::make_shared<VoroFortune::SSegment<T>>(start);
			voronoiDiagram.diagram.push_back(i->mSegment2);
			voronoiDiagram.diagram.push_back(i->mNext->mSegment1);

			if (i->mSegment2.get())
			{
				{
					std::size_t siteHashVal = std::hash<T>{}(i->mPoint.x) ^ std::hash<T> {}(i->mPoint.y);
					i->mSegment2->siteSet.insert(siteHashVal);
					i->mSegment2->siteMap[siteHashVal] = i->mPoint;

				}

				{
					std::size_t siteHashVal = std::hash<T>{}(i->mNext->mPoint.x) ^ std::hash<T> {}(i->mNext->mPoint.y);
					i->mNext->mSegment1->siteSet.insert(siteHashVal);
					i->mNext->mSegment1->siteMap[siteHashVal] = i->mNext->mPoint;

				}
			}
		};

		std::function<void()> process_site_event = [&]()
		{
			VoroFortune::SVoronoiPoint2D<T> p = siteEvents.top();
			siteEvents.pop();

			front_insert(p);
		};

		std::function<void()> process_circle_event = [&]()
		{
			std::shared_ptr<VoroFortune::SCircleEvent<T>> e = circleEvents.top();
			circleEvents.pop();

			if (e->bIsValid)
			{
				std::shared_ptr<VoroFortune::SSegment<T>> segment = std::make_shared<VoroFortune::SSegment<T>>(e->center);
				voronoiDiagram.diagram.push_back(segment);
				// Remove the associated arc from the front.
				std::shared_ptr<VoroFortune::SArc<T>> arc = e->arc;
				if (arc->mPrev.get())
				{
					arc->mPrev->mNext = arc->mNext;
					arc->mPrev->mSegment2 = segment;
					std::size_t siteHashVal = std::hash<T>{}(arc->mPrev->mPoint.x) ^ std::hash<T> {}(arc->mPrev->mPoint.y);
					segment->siteSet.insert(siteHashVal);
					segment->siteMap[siteHashVal] = arc->mPrev->mPoint;

				}

				if (arc->mNext.get())
				{
					arc->mNext->mPrev = arc->mPrev;
					arc->mNext->mSegment1 = segment;
					std::size_t siteHashVal = std::hash<T>{}(arc->mNext->mPoint.x) ^ std::hash<T> {}(arc->mNext->mPoint.y);
					segment->siteSet.insert(siteHashVal);
					segment->siteMap[siteHashVal] = arc->mNext->mPoint;

				}

				// Finish the edges before and after a.
				if (arc->mSegment1.get())
				{
					arc->mSegment1->finish(e->center);
					std::size_t siteHashVal = std::hash<T>{}(arc->mPoint.x) ^ std::hash<T> {}(arc->mPoint.y);
					arc->mSegment1->siteSet.insert(siteHashVal);
					arc->mSegment1->siteMap[siteHashVal] = arc->mPoint;
				}
				if (arc->mSegment2.get())
				{
					arc->mSegment2->finish(e->center);
					std::size_t siteHashVal = std::hash<T>{}(arc->mPoint.x) ^ std::hash<T> {}(arc->mPoint.y);
					arc->mSegment2->siteSet.insert(siteHashVal);
					arc->mSegment2->siteMap[siteHashVal] = arc->mPoint;
				}

				// Recheck circle events on either side of p:
				if (arc->mPrev.get()) check_circle_event(arc->mPrev, e->mX);
				if (arc->mNext.get()) check_circle_event(arc->mNext, e->mX);
			}

			e.reset();
		};

		std::function<void()> finish_edges = [&]()
		{
			// Advance the sweep line so no parabolas can cross the bounding box.
			T l = boundingBox.maxX + (boundingBox.maxX - boundingBox.minX) + (boundingBox.maxY - boundingBox.minY);

			// Extend each remaining segment to the new parabola intersections.
			for (std::shared_ptr<VoroFortune::SArc<T>> arc = rootArcTree; arc.get(); arc = arc->mNext)
				if (arc->mSegment2.get() && arc->mNext.get())
					arc->mSegment2->finish(intersection(arc->mPoint, arc->mNext->mPoint, l * 2.));

			std::size_t siteMapKey;
			for (std::shared_ptr<VoroFortune::SSegment<T>> seg : voronoiDiagram.diagram)
			{
				// here must be two vertex only
				for (auto it = seg->siteMap.begin(); it != seg->siteMap.end(); it++)
				{
					siteMapKey = (*it).first;
					auto foundedSiteMapIt = voronoiDiagram.siteSegmentMap.find(siteMapKey);
					if (foundedSiteMapIt == voronoiDiagram.siteSegmentMap.end())
					{
						std::list<std::shared_ptr<VoroFortune::SSegment<T>>> segmentList;
						voronoiDiagram.siteSegmentMap[siteMapKey] = std::move(segmentList);
					}
					voronoiDiagram.siteSegmentMap[siteMapKey].push_back(seg);
				}
			}
		};

		while (!siteEvents.empty())
		{
			if (!circleEvents.empty() && circleEvents.top()->mX <= siteEvents.top().x)
			{
				process_circle_event();
			}
			else
			{
				process_site_event();
			}
		}

		while (!circleEvents.empty())
		{
			process_circle_event();
		}

		finish_edges();
	}

}