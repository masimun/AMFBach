/*
 * AMFInterval.cpp
 *
 *  Created on: 2-dec.-2013
 *      Author: Daan, Max
 */

#include "AMFInterval.h"


AMFInterval::AMFIterator::AMFIterator(AMFInterval* intr,AMFunction funct) {
    interval = intr;
    //Geeft een niet lege iterator terug
    if ((interval->getBottom()).leq(interval->getTop())) {
        amf = funct; //current
        string test = interval->getTop().toString();
        span = interval->getTop().span();
        
        
        if (span.isemptyset()) {
            ret = amf;
            amf = (*interval).getBottom();
            if (amf.equals((*interval).getTop())) {
                amf = AMFunction::emptyFunction(); // moet null zijn.
            }
            else {
                amf = (*interval).getTop();
            }
            
        }
        else {
            maxSpan = AMFunction::singletonFunction(span.maximum()); // @Dit geeft lik de verkeerde waarde terug?
            // We willen hier niet wat het grootste element van de span kan zijn maar wat het grootste is die er in zit.
            amf = (*interval).getBottom();
            span =(*interval).getTop().span();
            alfaBottom = (amf.reduce(span));
            alfaTop = ((*interval).getTop().reduce(span));
            
            //vector<AMFIterator> pIterator; // KAN LIK GEEN ITERATORS IN DIE VECTOR STOPPEN RAAR MAAR WAAR
            
            
            pAlfaBottom = alfaBottom.data();
            pAlfaTop = alfaTop.data();
            pAlfa = alfa.data();
            pIterator = iter.data();
            
            
            //debug
            //int debug_alfaBottomsize = alfaBottom.size();
            
            
            AMFIterator pIterator0 = (AMFInterval(pAlfaBottom[0],pAlfaTop[0])).getIterator();
            iter.push_back(pIterator0); // nulpointer error!
            alfa.push_back((iter[0]).next());
            AMFIterator pIterator1 = (AMFInterval(pAlfaBottom[1],(pAlfa[0].meet(pAlfaTop[1])))).getIterator();
            iter.push_back(pIterator1);
            //pAlfa[1] = (iter[1]).amf;
            
            
            
        }
        
        
        
        
    }
    
    // Geef een lege iterator terug
    else {
        amf = AMFunction::emptyFunction();
    }
}

AMFunction AMFInterval::AMFIterator::next() {
    
    AMFunction ret = amf;
    
    //@ DAAN KAN JE KEER KIJKEN NAAR DIE getTop() kijken ik versta nie waarom ik daar iedere keer nulptr exception heb
    if (amf.equals(interval->getTop())) {
        amf = AMFunction::emptyFunction();
    }
    else {
       amf = interval->getTop();
    }
    return ret;
    
}

AMFInterval::AMFIterator AMFInterval::AMFIterator::operator++() {
    
    if (this->hasNext()) {
        /**
        if (span.isemptyset()) {
            ret = amf;
            amf = (*interval).getBottom();
            if (amf.equals((*interval).getTop())) {
                amf = AMFunction::emptyFunction(); // moet null zijn.
            }
            else {
                amf = (*interval).getTop();
            }
            
        }
        else {
            maxSpan = AMFunction::singletonFunction(span.maximum()); // @Dit geeft lik de verkeerde waarde terug?
            // We willen hier niet wat het grootste element van de span kan zijn maar wat het grootste is die er in zit.
            amf = (*interval).getBottom();
            span =(*interval).getTop().span();
            alfaBottom = (amf.reduce(span));
            alfaTop = ((*interval).getTop().reduce(span));
            
            //vector<AMFIterator> pIterator; // KAN LIK GEEN ITERATORS IN DIE VECTOR STOPPEN RAAR MAAR WAAR
            
            
            pAlfaBottom = alfaBottom.data();
            pAlfaTop = alfaTop.data();
            pAlfa = alfa.data();
            pIterator = iter.data();
            
            
            //debug
            //int debug_alfaBottomsize = alfaBottom.size();
            
            
            AMFIterator pIterator0 = (AMFInterval(pAlfaBottom[0],pAlfaTop[0])).getIterator();
            iter.push_back(pIterator0); // nulpointer error!
            pAlfa[0] = (iter[0]).amf;
            AMFIterator pIterator1 = (AMFInterval(pAlfaBottom[1],pAlfa[0].meet(pAlfaTop[1]))).getIterator();
            iter.push_back(pIterator1);
            pAlfa[1] = (iter[1]).amf;
            
        
         
        }
       **/
    
        
        if(pIterator[1].hasNext()) {
            pAlfa[1] = (pIterator[1]++).amf;
            if(pAlfa[1].isEmpty()) {amf = pAlfa[0];}
            else {amf = pAlfa[0].join(pAlfa[1].times(maxSpan));}
        }
        else if (pIterator[0].hasNext()) {
            pAlfa[0] = (pIterator[0]++).amf;
            pIterator[1] = (AMFInterval(pAlfaBottom[1],pAlfa[0].meet(pAlfaTop[1]))).getIterator();
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
            amf = AMFunction().emptyFunction();
        }
        amf = ret;
        
    
    
        return (*this);
        
    }
    
    else {
        throw exception();
    }
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
    return till;
}

AMFunction AMFInterval::getBottom() {
    return from;
}

