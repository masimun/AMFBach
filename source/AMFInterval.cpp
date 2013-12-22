/*
 * AMFInterval.cpp
 *
 *  Created on: 2-dec.-2013
 *      Author: Daan, Max
 */

#include "AMFInterval.h"


AMFInterval::AMFIterator::AMFIterator(AMFInterval intr,AMFunction funct) {
    interval = &intr;
    amf = funct;
    span = (*interval).getTop().span();
}


AMFInterval::AMFIterator AMFInterval::AMFIterator::operator++() {
    if (span.isemptyset()) {
        amf = (*interval).getBottom();
        if (amf.equals((*interval).getTop())) {
            amf = AMFunction::emptyFunction(); // moet null zijn.
        }
        else {
            amf = (*interval).getTop();
        }
    }
    else {
        AMFunction maxSpan = AMFunction::singletonFunction(span.maximum()); // @MAX: static functions moet je met :: oproepen
        amf = (*interval).getBottom();
        span =(*interval).getTop().span();
        vector<AMFunction> alfaBottom =( amf.reduce(span));
        vector<AMFunction> alfaTop = ((*interval).getTop().reduce(span));
        vector<AMFunction> alfa (2);
        
        
        vector<AMFIterator> itr; // KAN LIK GEEN ITERATORS IN DIE VECTOR STOPPEN RAAR MAAR WAAR
        
        AMFunction* pAlfaBottom = alfaBottom.data();
        AMFunction* pAlfaTop = alfaTop.data();
        AMFunction* pAlfa = alfa.data();
        
        AMFIterator* pIterator = itr.data();
        pIterator[0] = (AMFInterval::AMFInterval(pAlfaBottom[0],pAlfaTop[0]).begin());
        pAlfa[0] = (pIterator[0]++).amf;
        pIterator[1] = (AMFInterval::AMFInterval(pAlfaBottom[1],pAlfa[0].meet(pAlfaTop[1])).begin());
        pAlfa[1] = (pIterator[1]++).amf;
        
        AMFunction ret = amf;
        
        if(pIterator[1].hasNext()) {
            pAlfa[1] = (pIterator[1]++).amf;
            if(pAlfa[1].isEmpty()) {amf = pAlfa[0];}
            else {amf = pAlfa[0].join(pAlfa[1].times(maxSpan));}
        }
        else if (pIterator[0].hasNext()) {
            pAlfa[0] = (pIterator[0]++).amf;
            pIterator[1] = (AMFInterval::AMFInterval(pAlfaBottom[1],pAlfa[0].meet(pAlfaTop[1])).begin());
            if (!pIterator[1].hasNext()) {
                amf = AMFunction().emptyFunction();
            }
            else {
                pAlfa[1] = (pIterator[1]++).amf;
                if(pAlfa[1].isEmpty()) {amf = pAlfa[0];}
                else {amf = pAlfa[0].join(pAlfa[1].times(maxSpan));}
            }
        }
        else {
            amf =AMFunction().emptyFunction();
        }
        amf = ret;
        
    }
    
    return (*this);
}
    

AMFGraph AMFInterval::graph() {
	AMFGraph g;
	edges_t* edges = g.getEdges();
	const AMFunction &r1 = this->getBottom();
	const AMFunction &r2 = this->getTop();
	for (SmallBasicSet r : r2.getSets()) {
		set<SmallBasicSet> cr;
		for (SmallBasicSet s : r2.getSets()) {
			if (!r1.ge(s.setintersect(s))) { cr.insert(s); };
		}
		edges->insert(make_pair(r,cr));
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

