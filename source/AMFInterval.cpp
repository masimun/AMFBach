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
AMFInterval::AMFIterator::AMFIterator(AMFInterval* interval) {
    intr=interval;
    theIt = (interval->getClosedIterator());
    if (!intr->isClosedAtTop()) ++(*theIt);
    if(theIt->hasNext()) {
        ++(*theIt);
        nxt = *(*theIt);
        thereIsNext = theIt->hasNext();
    }
    else thereIsNext = false;
}


iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFIterator::operator ++() {
    //if (!intr->getBottom().leq(intr->getTop()) ||
    //    (intr->getBottom().equals(intr->getTop())
    //   && (!intr->isClosedAtBottom() || !intr->isClosedAtTop()))){
    //        return *new AMFEmptyIterator(intr);
    //}
    //else if (intr->isClosedAtTop()) {
    //    AMFClosedIterator itr = *new AMFClosedIterator(intr);
    //    if (!intr->isClosedAtBottom()) ++itr;
    //    return itr;
    //}
    //else {
        AMFunction myNxt = nxt;
        ++(*theIt);
        nxt = *(*theIt);
        thereIsNext = theIt->hasNext();
        current = myNxt;
        return *this;
    //}
}

bool AMFInterval::AMFIterator::hasNext() {
    return thereIsNext;
}
/*******************************************
 * ITERATOR One or two element
 *******************************************/
AMFInterval::AMFOneOrTwoIterator::AMFOneOrTwoIterator(AMFInterval* intr ) {
    interval = intr;
    span = (interval->getTop()).span();
    theList = *new vector<AMFunction> (3);
    
    if (intr->getBottom().size() == 0) {
        pos = 0;
    }
    else {
        //for (set<SmallBasicSet>::iterator it = ((intr->getBottom()).getSets()).begin() ; it != ((intr->getBottom()).getSets()).end(); ++it){
        for (SmallBasicSet sbs : (intr->getBottom()).getSets() ) {
            if ((sbs.size()) == 0) {
                pos = 1;
                break;
            }
            else pos =2;
        }
    }
    
    if (intr->getTop().size() == 0) {
        last = 0;
    }
    else {
        for (SmallBasicSet sbs : (intr->getTop()).getSets() ){
            if ((sbs.size()) == 0) {
                last = 1;
                break;
            }
            else last =2;
        }
        
    }
    
    
    
    if (pos <= last) {
        //theList = new AMFunction[last + 1];
        if (pos == 0) {
            AMFunction amf;
            theList[0] = AMFunction((intr->getTop())); // empty function
            if (last > 0) {
                amf = *new AMFunction();
                amf.addSet(SmallBasicSet::universe()); // empty set
                theList[1] = amf;
            }
            if (last > 1) {
                amf = *new AMFunction();
                amf.addSet((intr->getTop()).span()); // set with one element
                theList[2] = amf;
            }
        }
        else if (pos == 1) {
            AMFunction amf;
            if (last > 0) {
                amf = *new AMFunction();
                amf.addSet(SmallBasicSet()); // empty set
                theList[1] = amf;
            }
            if (last > 1) {
                amf = *new AMFunction();
                amf.addSet((intr->getTop()).span()); // set with one element
                theList[2] = amf;
            }
            
        }
        else /* pos == 2 */ {
            AMFunction amf;
            if (last > 1) {
                amf = *new AMFunction();
                amf.addSet((intr->getTop()).span()); // set with one element
                theList[2] = amf;
            }
        }
    }
}

iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFOneOrTwoIterator::operator ++() {
    current = theList[pos++];
    return *this;
}

bool AMFInterval::AMFOneOrTwoIterator::hasNext() {
    return pos <= last;
}


/*******************************************
 * ITERATOR One element
 *******************************************/
AMFInterval::AMFOneElementIterator::AMFOneElementIterator(AMFInterval* intr ) {
    interval = intr;
}

iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFOneElementIterator::operator ++() {
    given = true;
    current = interval->getBottom();
    return *this;
}

bool AMFInterval::AMFOneElementIterator::hasNext() {
    return !given;
}

/*******************************************
 * ITERATOR Empty
 *******************************************/
AMFInterval::AMFEmptyIterator::AMFEmptyIterator(AMFInterval* intr ) {
    
}

iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFEmptyIterator::operator ++() {
    return *this;
}

bool AMFInterval::AMFEmptyIterator::hasNext() {
    return false;
}

/*******************************************
 * ITERATOR EXCEPTIONALCLOSEDITERATOR
 *******************************************/
AMFInterval::AMFExceptionalClosedIterator::AMFExceptionalClosedIterator(AMFInterval* intr ) {
    interval = intr;
    bottom = *(new AMFunction());
    bottom.addSet(SmallBasicSet());
    //AMFInterval i = *(new AMFInterval(bottom,intr->getTop(),true,true));
    normal = ((new AMFInterval(bottom,intr->getTop(),true,true))->getClosedIterator());
    
    //current = *new AMFunction();
    virgin = true;
}

iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFExceptionalClosedIterator::operator ++() {
    if (virgin) {
        virgin = false;
        //current = bottom;
        return *this;
    }
    else {
        ++(*normal);
        current = *(*normal);
        return *this;
    }
}

bool AMFInterval::AMFExceptionalClosedIterator::hasNext() {
  return (virgin || normal->hasNext());
}


/*******************************************
 * ITERATOR CLOSEDITERATOR
 *******************************************/
AMFInterval::AMFClosedIterator::AMFClosedIterator(AMFInterval* intr) {
    interval = intr;
    axes = interval->bestSplit();
    Xaxis = new AMFInterval((interval->getBottom()).project(axes[0]),(interval->getTop()).project(axes[0]),true,true);
    Yaxis = new AMFInterval((interval->getBottom()).project(axes[1]),(interval->getTop()).project(axes[1]),true,true);
    X = Xaxis->getIterator();
    Y= Yaxis->getIterator();
    if (X->hasNext() && Y->hasNext()) {
        ++(*X);
        ++(*Y);
        currentX = *(*X);
        currentY = *(*Y);
        currentIterator = (new AMFInterval( (currentX+currentY)+interval->getBottom(),
                                                 (currentX.times(currentY))^(interval->getTop()),true,true))->getIterator();
    }
    else {
        currentIterator = (new AMFInterval(*new AMFunction(),*new AMFunction(),false,false))->getIterator();
    }
}


iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFClosedIterator::operator ++() {
    if (interval->getBottom().size() == 0) {
        return *new AMFExceptionalClosedIterator(interval);
    }
    else if (interval->getTop().span().size() <= 1){
        return *new AMFOneOrTwoIterator(interval);
    }
    else if ((interval->getTop()).equals(interval->getBottom())) {
        return *new AMFOneElementIterator(interval);
    }
    long spanSize = interval->getTop().span().size();
    long minSizeBottom =spanSize;
    
    for (SmallBasicSet sbs : (interval->getBottom()).getSets()){
        if (sbs.size() < minSizeBottom) {
            minSizeBottom = sbs.size();
        }
    }
    if (interval->getTop().size() == 1
             && interval->getBottom().size() == spanSize
             && minSizeBottom + 1 == spanSize){
        return *new AMFGeneralClosedIterator(interval);
    }
    
    else{
    if (currentIterator->hasNext()) {
        ++(*currentIterator);
        current = *(*currentIterator);
        return *this;
    }
    if (X->hasNext()) {
        ++(*X);
        currentX = *(*X);
        currentIterator = (new AMFInterval((currentX+currentY) + (interval->getBottom()),
                        currentX.times(currentY)^(interval->getTop()),true,true))->getIterator();
        
        ++(*currentIterator);
        current = *(*currentIterator);
        return *this;
    }
    else if (Y->hasNext()) /* should always be true */ {
        X = Xaxis->getIterator();
        if (X->hasNext() && Y->hasNext()) {
            ++(*X);
            ++(*Y);
            currentX = *(*X);
            currentY = *(*Y);
            currentIterator = (new AMFInterval((currentX +currentY) + (interval->getBottom()),
                            currentX.times(currentY)^(interval->getTop()),true,true))->getIterator();
            
            ++(*currentIterator);
            current = *(*currentIterator);
            return *this;
        }
    }
    //should never happen
    return *this;
    }
    
}

bool AMFInterval::AMFClosedIterator::hasNext() {
    return currentIterator->hasNext() || X->hasNext() || Y->hasNext();
}




/*******************************************
 *General ITERATOR CLOSEDITERATOR
 *******************************************/
AMFInterval::AMFGeneralClosedIterator::AMFGeneralClosedIterator(AMFInterval* intr) {
    interval = intr;
    pos =0;
}


iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFGeneralClosedIterator::operator ++() {
    //span = (interval->getTop()).span();
    //if (interval->getBottom().size() == 0) {
    //    return *new AMFExceptionalClosedIterator(interval);
    //}
    //else if (span.size()<=1) {
    //    return *new AMFOneOrTwoIterator(interval);
    //}
    //else if(interval->getTop().equals(interval->getBottom())){
    //    return *new AMFOneElementIterator(interval);
    //}
    //else {
    //    if (interval->getTop().size() == 1
    //        && interval->getBottom().size() == spanSize
    //        && minSizeBottom + 1 == spanSize) {
            
            pos++;
            if (pos == 1 ) {
                current = interval->getBottom();
                return *this;
            }
            if(pos == 2) {
                current = interval->getTop();
                return *this;
            }
            else {
                //current = 0 ;
                //Must return null;
                return *this;
            }
        
    //    }
    //}
    //Remove
    return *this;
}

bool AMFInterval::AMFGeneralClosedIterator::hasNext() {
    return (pos < 2);
}



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
	if (!getBottom().leq(getTop())) {
		return 0;
	} else if (getBottom().equals(getTop())) {
		if (isClosedAtBottom() && isClosedAtTop()) {
			return 1;
		} else {
			return 0;
		}
	} else {
		SetsPoset poset(this);
		return poset.get_lattice_size();
	}
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

vector<SmallBasicSet> AMFInterval::bestSplit() {
    vector<SmallBasicSet> res (2);
    SmallBasicSet best = *new SmallBasicSet();
    SmallBasicSet span = getTop().span();
    long spanSize = span.size();
    if (getTop().size()>1) {
        AMFunction difference = (getTop().minus(getBottom()));
        long value = spanSize;
        for(SmallBasicSet a : difference.getSets()) {
            if(abs(a.size() - spanSize/2) < value){
                value =abs(a.size()-spanSize/2);
                best = a;
            }
        }
    }
    else {
        best = bestSubset(span,spanSize/2,getBottom());
    }
    
    res[0] = best;
    res[1] = span.setdifference(best);
    
    return res;
}

SmallBasicSet AMFInterval::bestSubset(SmallBasicSet span,long target,AMFunction bottom) {
    SmallBasicSet best;
    long value = 2*target;
    //SmallBasicSet* sp = new SmallBasicSet();
    //AMFunction* bot = new AMFunction();
    //sp = &span;
    //bot = &bottom;
    if(!(bottom.ge(span))) {
        best = span;
        value = abs(best.size() - target);
    }
    if (best == 0|| best.size() <= target) {
        return best;
    }
    
    for(SmallBasicSet::SBSIterator it= span.getIterator(); it.hasNext() ; ++it) {
        SmallBasicSet candidate;
        SmallBasicSet diff = span.difference(*it);
        //cout << diff.toString() << endl;
        //cout << bottom.toString() << endl;
        candidate = bestSubset(diff, target, bottom);
        if(!(candidate == 0)) {
            long newValue= abs(candidate.size() - target);
            if (newValue < value) {
                best = candidate;
                value = newValue;
            }
        }
    }
    return best;
}

string AMFInterval::toString() {
	return "[" + from.toString() + "," + till.toString() + "]";
}

AMFInterval AMFInterval::fullspace(int n) {
	SmallBasicSet N = SmallBasicSet::universe(n);
	return AMFInterval(AMFunction::emptyFunction(),AMFunction::universeFunction(N));
}

AMFInterval::GeneralIterator * AMFInterval::getIterator() {
    //if (!this->getBottom().leq(this->getTop()) ||
    //    (this->getBottom().equals(this->getTop())
    //     && (!this->isClosedAtBottom() || !this->isClosedAtTop()))){
    //        return AMFEmptyIterator(this);
    //    }
    
    //if (isClosedAtTop()) {
    //    AMFClosedIterator itr = *new AMFClosedIterator(this);
    //    if (!isClosedAtBottom()) ++itr;
    //    return itr;
    //}
    if (!getBottom().leq(getTop()) ||
        (getBottom().equals(getTop())
         && (!isClosedAtBottom() || !isClosedAtTop()))){
            return new AMFEmptyIterator(this);
        }
    else if (closedAtTop) {
        GeneralIterator * itr = (this->getClosedIterator());
        if (!isClosedAtBottom()) {
            ++(itr);
        }
        return (itr);
    }
    else {
        return new AMFIterator(this);
    }


}
AMFInterval::GeneralIterator * AMFInterval::getClosedIterator() {
    if(getBottom().size() == 0){
        return new AMFExceptionalClosedIterator(this);
    }
    if(getTop().span().size() <= 1){
        return new AMFOneOrTwoIterator(this);
    }
    if(getTop().equals(getBottom())){
        return new AMFOneElementIterator(this);
    }
    long spanSize = getTop().span().size();
    long minSizeBottom = spanSize;
    for (SmallBasicSet sbs : (getBottom()).getSets()){
        if (sbs.size() < minSizeBottom) {
            minSizeBottom = sbs.size();
        }
    }
    if (getTop().size() == 1
        && getBottom().size() == spanSize
        && minSizeBottom + 1 == spanSize){
        return new AMFGeneralClosedIterator(this);
    }
    else {
        return new AMFClosedIterator(this);
    }

}

AMFunction AMFInterval::getTop() {
    return till;
}

AMFunction AMFInterval::getBottom() {
    return from;
}

SmallBasicSet AMFInterval::getUniverse() {
    return (this->getTop()).getUniverse();
}

bool AMFInterval::isClosedAtBottom() {
    return closedAtBottom;
}

bool AMFInterval::isClosedAtTop() {
    return closedAtTop;
}
