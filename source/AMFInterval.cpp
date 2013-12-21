/*
 * AMFInterval.cpp
 *
 *  Created on: 2-dec.-2013
 *      Author: Daan
 */

#include "AMFInterval.h"

AMFGraph AMFInterval::graph() {
	AMFGraph g;
	const edges_t* edges = g.getEdges();
	const AMFunction &r1 = this->getBottom();
	const AMFunction &r2 = this->getTop();
	for (SmallBasicSet r : r2.getSets()) {
		set<SmallBasicSet> cr;
		for (SmallBasicSet s : r2.getSets()) {
			if (!r1.ge(s.setintersect(s))) { cr.insert(s); };
		}
		// (*edges).insert(make_pair(r,cr));
	}
	return g;
}
	/**
	 * Compute the graph generated from the interval.
	 * The vertices of the graph are the  elements of top
	 * Two vertices A B are connected iff A intersection B is less than bottom
	 * The vertices
	 *
	 * @return sorted map mapping each of the elements in top to the set of immediately connected elements
	 */
//	public AMFGraph graph() {
//		AMFunction r1 = getBottom();
//		AMFunction r2 = getTop();
//		SortedMap<SmallBasicSet, SortedSet<SmallBasicSet>> ret = new TreeMap<SmallBasicSet, SortedSet<SmallBasicSet>>();
//		for (SmallBasicSet r : r2) {
//			SortedSet<SmallBasicSet> cr = new TreeSet<SmallBasicSet>();
//			for (SmallBasicSet s : r2) {
//				if (!r1.ge(r.intersection(s))) cr.add(s);
//			}
//			ret.put(r,cr);
//		}
//		return new AMFGraph(ret);
//	}

AMFInterval::AMFInterval(AMFunction bottom, AMFunction top) {
	from = bottom;
	till = top;
}

AMFInterval::~AMFInterval() {
	// TODO Auto-generated destructor stub
}

string AMFInterval::toString() {
	return "[" + from.toString() + "," + till.toString() + "]";
}

AMFInterval AMFInterval::fullspace(int n) {
	SmallBasicSet N = SmallBasicSet::universe(n);
	return AMFInterval(AMFunction::emptyFunction(),AMFunction::universeFunction(N));
}

AMFunction AMFInterval::getTop() {
    return from;
}

AMFunction AMFInterval::getBottom() {
    return till;
}

