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
	template<typename T>
	struct SPointXY
	{
		T x;
		T y;
	};

	template<typename T>
	struct VoronoiPoint2D
	{
		T x;
		T y;

		SPointXYZ<T> originalPoints;
	};

	template<typename T>
	struct Segment
	{
		SPointXY<T> mStart, mEnd;
		bool bIsDone;

		Segment(SPointXY<T>& p)
			: mStart(p), bIsDone(false) {};

		void finish(const SPointXY<T>& _end)
		{
			if (bIsDone) return;
			mEnd = _end;
			bIsDone = true;
		}

		std::unordered_set<std::size_t> siteSet = {};
		std::unordered_map<std::size_t, VoronoiPoint2D<T>> siteMap = {};
	};

	template<typename T>
	struct Arc;
	template<typename T>
	struct CircleEvent
	{
		T mX;
		SPointXY<T> center;
		Arc<T>* arc = nullptr;
		bool bIsValid;

		CircleEvent(T xCoord, SPointXY<T>& p, Arc<T>* a)
			: mX(xCoord), center(p), arc(a), bIsValid(true) {};
	};

	template<typename T>
	struct Arc
	{
		Arc(VoronoiPoint2D<T>& _p, Arc<T>* a = nullptr, Arc<T>* b = nullptr)
			: mPoint(_p), mPrev(a), mNext(b) {};

		VoronoiPoint2D<T> mPoint;
		Arc<T>* mPrev = nullptr;
		Arc<T>* mNext = nullptr;
		CircleEvent<T>* mCircleEvent = nullptr;

		Segment<T>* mSegment1 = nullptr;
		Segment<T>* mSegment2 = nullptr;

	};

	template<typename T>
	struct VoronoiDiagram2D
	{
		std::vector<Segment<T>*> diagram;

		std::unordered_map<std::size_t, std::list<Segment<T>*>> siteSegmentMap;
	};

	// "Greater than" comparison, for reverse sorting in priority queue.
	template<typename T>
	struct gt {
		bool operator()(VoronoiPoint2D<T>& a, VoronoiPoint2D<T>& b) { return (std::abs(a.x - b.x) < EPSILON) ? a.y > b.y : a.x > b.x; }
		bool operator()(CircleEvent<T>* a, CircleEvent<T>* b) { return a->mX > b->mX; }
	};


	class CVoronoiFortune
	{
	public:

		template<typename T>
		static void VoronoiDiagramFortune2D(std::vector<VoronoiPoint2D<T>>& points, VoronoiDiagram2D<T>& voronoiDiagram, const SBox2D<T>& boundingBox);
	};

	template<typename T>
	void CVoronoiFortune::VoronoiDiagramFortune2D(std::vector<VoronoiPoint2D<T>>& points, VoronoiDiagram2D<T>& voronoiDiagram, const SBox2D<T>& boundingBox)
	{
		if (points.size() == 0) return;

		std::list<Arc<T>*> arcTracker;
		std::list<CircleEvent<T>*> circleEventTracker;

		Arc<T>* rootArcTree = nullptr;

		voronoiDiagram.siteSegmentMap.reserve(points.size());

		std::priority_queue<VoronoiPoint2D<T>, std::vector<VoronoiPoint2D<T>>, gt<T>> siteEvents(begin(points), end(points));

		std::priority_queue<CircleEvent<T>*, std::vector<CircleEvent<T>*>, gt<T>> circleEvents;

		// Find the rightmost point on the circle through a,b,c.
		std::function<bool(VoronoiPoint2D<T>&, VoronoiPoint2D<T>&, VoronoiPoint2D<T>&, T&, SPointXY<T>&)> circle
			= [&](VoronoiPoint2D<T>& a, VoronoiPoint2D<T>& b, VoronoiPoint2D<T>& c, T& x, SPointXY<T>& origin) -> bool
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
		std::function<void(Arc<T>*, T)> check_circle_event
			= [&](Arc<T>* arc, T x0)
		{
			// Invalidate any old event.
			if (arc->mCircleEvent && std::abs(arc->mCircleEvent->mX - x0) > EPSILON)
			{
				arc->mCircleEvent->bIsValid = false;
			}
			arc->mCircleEvent = nullptr;

			if (!arc->mPrev || !arc->mNext)
				return;

			T x;
			SPointXY<T> origin;
			if (circle(arc->mPrev->mPoint, arc->mPoint, arc->mNext->mPoint, x, origin) && x > x0)
			{
				// Create new event.
				arc->mCircleEvent = new CircleEvent<T>(x, origin, arc);
				circleEvents.push(arc->mCircleEvent);
				circleEventTracker.push_back(arc->mCircleEvent);
			}
		};

		// Where do two parabolas intersect?
		std::function<SPointXY<T>(VoronoiPoint2D<T>&, VoronoiPoint2D<T>&, T)> intersection
			= [&](VoronoiPoint2D<T>& _p0, VoronoiPoint2D<T>& _p1, T l)
			->SPointXY<T>
		{
			SPointXY<T> result;
			VoronoiPoint2D<T> p = _p0;

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
		std::function<bool(VoronoiPoint2D<T>&, Arc<T>*, SPointXY<T>&)> intersect
			= [&](VoronoiPoint2D<T>& _p, Arc<T>* arc, SPointXY<T>& result) -> bool
		{
			if (std::abs(arc->mPoint.x - _p.x) < EPSILON) return false;

			T a, b;
			if (arc->mPrev)
				a = intersection(arc->mPrev->mPoint, arc->mPoint, _p.x).y;
			if (arc->mNext)
				b = intersection(arc->mPoint, arc->mNext->mPoint, _p.x).y;

			if ((!arc->mPrev || a <= _p.y) && (!arc->mNext || _p.y <= b))
			{
				result.y = _p.y;

				result.x = (arc->mPoint.x * arc->mPoint.x +
					(arc->mPoint.y - result.y) * (arc->mPoint.y - result.y)
					- _p.x * _p.x) / (2. * arc->mPoint.x - 2. * _p.x);

				return true;
			}

			return false;
		};

		std::function<void(VoronoiPoint2D<T>&)> front_insert = [&](VoronoiPoint2D<T>& _p)
		{
			if (!rootArcTree)
			{
				rootArcTree = new Arc(_p);
				arcTracker.push_back(rootArcTree);
				return;
			}

			for (Arc<T>* arc = rootArcTree; arc; arc = arc->mNext)
			{
				SPointXY<T> z, zz;
				if (intersect(_p, arc, z))
				{
					// New parabola intersects arc i.  If necessary, duplicate i.
					if (arc->mNext && !intersect(_p, arc->mNext, zz))
					{
						arc->mNext->mPrev = new Arc(arc->mPoint, arc, arc->mNext);
						arcTracker.push_back(arc->mNext->mPrev);
						arc->mNext = arc->mNext->mPrev;
					}
					else
					{
						arc->mNext = new Arc(arc->mPoint, arc);
						arcTracker.push_back(arc->mNext);
					}
					arc->mNext->mSegment2 = arc->mSegment2;

					// Add p between i and i->next.
					arc->mNext->mPrev = new Arc(_p, arc, arc->mNext);
					arcTracker.push_back(arc->mNext->mPrev);
					arc->mNext = arc->mNext->mPrev;

					arc = arc->mNext; // Now i points to the new arc.

					// Add new half-edges connected to i's endpoints.
					arc->mPrev->mSegment2 = arc->mSegment1 = new Segment(z);
					voronoiDiagram.diagram.push_back(arc->mPrev->mSegment2);
					arc->mNext->mSegment1 = arc->mSegment2 = new Segment(z);
					voronoiDiagram.diagram.push_back(arc->mNext->mSegment1);

					{
						std::size_t siteHashVal = std::hash<double>{}(arc->mPoint.x) ^ std::hash<double> {}(arc->mPoint.y);
						arc->mSegment1->siteSet.insert(siteHashVal);
						arc->mSegment1->siteMap[siteHashVal] = arc->mPoint;

						arc->mSegment2->siteSet.insert(siteHashVal);
						arc->mSegment2->siteMap[siteHashVal] = arc->mPoint;

					}

					{
						std::size_t siteHashVal = std::hash<double>{}(arc->mPrev->mPoint.x) ^ std::hash<double> {}(arc->mPrev->mPoint.y);
						arc->mSegment1->siteSet.insert(siteHashVal);
						arc->mSegment1->siteMap[siteHashVal] = arc->mPrev->mPoint;

						arc->mPrev->mSegment2->siteSet.insert(siteHashVal);
						arc->mPrev->mSegment2->siteMap[siteHashVal] = arc->mPrev->mPoint;
					}

					{
						std::size_t siteHashVal = std::hash<double>{}(arc->mNext->mPoint.x) ^ std::hash<double> {}(arc->mNext->mPoint.y);
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
			Arc<T>* i = nullptr;
			for (i = rootArcTree; i->mNext; i = i->mNext);
			i->mNext = new Arc(_p, i);
			arcTracker.push_back(i->mNext);

			// Insert segment between p and i
			SPointXY<T> start;
			start.x = boundingBox.minX;
			start.y = (i->mNext->mPoint.y + i->mPoint.y) * 0.5;
			i->mSegment2 = i->mNext->mSegment1 = new Segment(start);
			voronoiDiagram.diagram.push_back(i->mSegment2);
			voronoiDiagram.diagram.push_back(i->mNext->mSegment1);

			if (i->mSegment2)
			{
				{
					std::size_t siteHashVal = std::hash<double>{}(i->mPoint.x) ^ std::hash<double> {}(i->mPoint.y);
					i->mSegment2->siteSet.insert(siteHashVal);
					i->mSegment2->siteMap[siteHashVal] = i->mPoint;

				}

				{
					std::size_t siteHashVal = std::hash<double>{}(i->mNext->mPoint.x) ^ std::hash<double> {}(i->mNext->mPoint.y);
					i->mNext->mSegment1->siteSet.insert(siteHashVal);
					i->mNext->mSegment1->siteMap[siteHashVal] = i->mNext->mPoint;

				}
			}
		};

		std::function<void()> process_site_event = [&]()
		{
			VoronoiPoint2D<T> p = siteEvents.top();
			siteEvents.pop();

			front_insert(p);
		};

		std::function<void()> process_circle_event = [&]()
		{
			CircleEvent<T>* e = circleEvents.top();
			circleEvents.pop();

			if (e->bIsValid)
			{
				Segment<T>* segment = new Segment(e->center);
				voronoiDiagram.diagram.push_back(segment);
				// Remove the associated arc from the front.
				Arc<T>* arc = e->arc;
				if (arc->mPrev)
				{
					arc->mPrev->mNext = arc->mNext;
					arc->mPrev->mSegment2 = segment;
					std::size_t siteHashVal = std::hash<double>{}(arc->mPrev->mPoint.x) ^ std::hash<double> {}(arc->mPrev->mPoint.y);
					segment->siteSet.insert(siteHashVal);
					segment->siteMap[siteHashVal] = arc->mPrev->mPoint;

				}

				if (arc->mNext)
				{
					arc->mNext->mPrev = arc->mPrev;
					arc->mNext->mSegment1 = segment;
					std::size_t siteHashVal = std::hash<double>{}(arc->mNext->mPoint.x) ^ std::hash<double> {}(arc->mNext->mPoint.y);
					segment->siteSet.insert(siteHashVal);
					segment->siteMap[siteHashVal] = arc->mNext->mPoint;

				}

				// Finish the edges before and after a.
				if (arc->mSegment1)
				{
					arc->mSegment1->finish(e->center);
					std::size_t siteHashVal = std::hash<double>{}(arc->mPoint.x) ^ std::hash<double> {}(arc->mPoint.y);
					arc->mSegment1->siteSet.insert(siteHashVal);
					arc->mSegment1->siteMap[siteHashVal] = arc->mPoint;

				}
				if (arc->mSegment2)
				{
					arc->mSegment2->finish(e->center);
					std::size_t siteHashVal = std::hash<double>{}(arc->mPoint.x) ^ std::hash<double> {}(arc->mPoint.y);
					arc->mSegment2->siteSet.insert(siteHashVal);
					arc->mSegment2->siteMap[siteHashVal] = arc->mPoint;

				}


				// Recheck circle events on either side of p:
				if (arc->mPrev) check_circle_event(arc->mPrev, e->mX);
				if (arc->mNext) check_circle_event(arc->mNext, e->mX);
			}
		};


		std::function<void()> finish_edges = [&]()
		{
			// Advance the sweep line so no parabolas can cross the bounding box.
			double l = boundingBox.maxX + (boundingBox.maxX - boundingBox.minX) + (boundingBox.maxY - boundingBox.minY);

			// Extend each remaining segment to the new parabola intersections.
			for (Arc<T>* arc = rootArcTree; arc; arc = arc->mNext)
				if (arc->mSegment2)
					arc->mSegment2->finish(intersection(arc->mPoint, arc->mNext->mPoint, l * 2.));

			std::size_t siteMapKey;
			for (Segment<T>* seg : voronoiDiagram.diagram)
			{
				// here must be two vertex only
				for (auto it = seg->siteMap.begin(); it != seg->siteMap.end(); it++)
				{
					siteMapKey = (*it).first;
					auto foundedSiteMapIt = voronoiDiagram.siteSegmentMap.find(siteMapKey);
					if (foundedSiteMapIt == voronoiDiagram.siteSegmentMap.end())
					{
						std::list<Segment<T>*> segmentList;
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

		for (auto arc : arcTracker)
		{
			if (arc)
				delete arc;
		}

		for (auto event : circleEventTracker)
		{
			if (event)
				delete event;
		}
	}

}