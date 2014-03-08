/*
 * AMFInterval.cpp
 *
 *  Created on: 2-dec.-2013
 *      Author: Daan, Max
 */

#include "AMFInterval.h"


/*******************************************
 * ITERATOR
 *******************************************/
AMFInterval::AMFIterator::AMFIterator(AMFInterval* intr) {
    interval = intr;
}


//AMFInterval::AMFIterator AMFInterval::AMFIterator::operator ++() {
//
//}

//bool AMFInterval::AMFIterator::hasNext() {
//    return (!interval->getBottom().leq(interval->getTop()) || (interval->getBottom().equals(interval->getTop()) && (!interval->isClosedAtBottom() || !interval->isClosedAtTop())));
//}

/*******************************************
 * ITERATOR EXCEPTIONALCLOSEDITERATOR
 *******************************************/
//AMFInterval::AMFExceptionalClosedIterator::AMFExceptionalClosedIterator(AMFInterval* intr ) {
//    interval = intr;
//    bottom = AMFunction();
//    bottom.addSet(SmallBasicSet());
//    virgin = true;
    //AMFIterator* normal = new AMFIterator((AMFInterval(bottom,intr->getTop(),true,true)));
//    AMFInterval intervaltoit = AMFInterval(bottom,intr->getTop(),true,true);
//    AMFIterator normal =  intervaltoit.getIterator();
    
//}

//AMFInterval::AMFExceptionalClosedIterator AMFInterval::AMFExceptionalClosedIterator::operator ++() {
    
//}

//bool AMFInterval::AMFExceptionalClosedIterator::hasNext() {
//    return (virgin || normal.hasNext());
//}


/*******************************************
 * ITERATOR CLOSEDITERATOR
 *******************************************/
//AMFInterval::AMFClosedIterator::AMFClosedIterator(AMFInterval* intr) {
//    interval = intr;
    
//    if ( (interval->getBottom().size() ) == 0) { // size could be wrong
//        pos = 0;
//    }
//    else {
//        for (SmallBasicSet::SBSIterator = interval->getTop()-> ; ) {
             //list.begin(); it != list.end(); ++list)
            // iteratre over all the smallbasic sets in top
//    }
    

//}

//AMFInterval::AMFClosedIterator AMFInterval::AMFClosedIterator::operator ++() {
//
//}

//bool AMFInterval::AMFClosedIterator::hasNext() {
//
//}



/***********************************************************************************************
 FAST ITERATOR (NOT WORKING)
 
 
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
            
            alfa.push_back((iter[0]).next(this->interval->getTop())); // nest van this of van iter[0] ?
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

AMFunction AMFInterval::AMFIterator::next(AMFunction top) {
    
    AMFunction ret = amf;
    
    //@DAAN KAN JE KEER KIJKEN NAAR DIE getTop() kijken ik versta nie waarom ik daar iedere keer nulptr exception heb
    
    if (amf.equals(top)) {
        amf = AMFunction::emptyFunction();
    }
    else {
       amf = top;
    }
    return ret;
    
}

AMFInterval::AMFIterator AMFInterval::AMFIterator::operator++() {
    
    if (this->hasNext()) {
        /*
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
       // * /
    
        
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
************************************************************************************************************/


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

long long AMFInterval::lattice_size() {
	return 42; // TODO: implement
}

AMFInterval::AMFInterval(AMFunction bottom, AMFunction top) {
    from = bottom;
	till = top;
    closedAtBottom = true;
    closedAtTop = true;
    
}

AMFInterval::AMFInterval(AMFunction bottom, AMFunction top,bool closedTop, bool closedBot) {
	from = bottom;
	till = top;
    closedAtBottom = closedBot;
    closedAtTop = closedTop;
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

bool AMFInterval::isClosedAtBottom() {
    return closedAtBottom;
}

bool AMFInterval::isClosedAtTop() {
    return closedAtTop;
}
