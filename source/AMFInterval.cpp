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
    AMFunction myNxt = nxt;
    ++(*theIt);
    nxt = *(*theIt);
    thereIsNext = theIt->hasNext();
    current = myNxt;
    return *this;
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
    normal = ((new AMFInterval(bottom,intr->getTop(),true,true))->getClosedIterator());
    virgin = true;
}

iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFExceptionalClosedIterator::operator ++() {
    if (virgin) {
        virgin = false;
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
        return *this;
    }
    return *this;
}

bool AMFInterval::AMFGeneralClosedIterator::hasNext() {
    return (pos < 2);
}

/*
    Cp.']'.
 */
AMFInterval::GeneralFastIterator::~GeneralFastIterator() {
    
}



/*******************************************
 *Fast Non Empty iterator
 *******************************************/
AMFInterval::AMFFastNonEmptyIterator::AMFFastNonEmptyIterator(AMFInterval* intr) {
    interval = intr;
    span = interval->getTop().span();
    maxSpan = AMFunction::singletonFunction(span.maximum());
    current = interval->getBottom();
    alfaBottom = current.reduce(span);
    alfaTop = interval->getTop().reduce(span);
    
    
    itr0 = (new AMFInterval(alfaBottom[0],alfaTop[0]))->getFastIterator();
    ++(*itr0);
    alfa0 = *(*itr0);
    itr1 = (new AMFInterval(alfaBottom[1],alfa0.meet(alfaTop[1])))->getFastIterator();
    ++(*itr1);
    alfa1 = *(*itr1);
}

AMFunction AMFInterval::AMFFastNonEmptyIterator::nextCurrent() {
    if (alfa1.isEmpty()) return alfa0;
    else return alfa0.join(alfa1.times(maxSpan));
}

iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFFastNonEmptyIterator::operator ++() {
    ret = current;
    if (itr1->hasNext()) {
        ++(*itr1);
        alfa1 = *(*itr1);
        current = nextCurrent();
    }
    else if (itr0->hasNext()) {
        //delete[] itr1;
        ++(*itr0);
        alfa0 = *(*itr0);
        itr1 = ((new AMFInterval(alfaBottom[1],alfa0.meet(alfaTop[1])))->getFastIterator());
        if (!itr1->hasNext()) isFinished = true;
        else {
            ++(*itr1);
            alfa1 = *(*itr1);
            current = nextCurrent();
        }
    }
    else {
        //delete[] itr0;
        //delete[] itr1;
        //delete[] interval;
        isFinished = true;
    }
    return *this;
}
bool AMFInterval::AMFFastNonEmptyIterator::hasNext() {
    if (isFinished){
        //delete[] itr0;
        //delete[] itr1;
        //delete[] interval;
        return false;
    }
    return true;
}

AMFInterval::AMFFastNonEmptyIterator::~AMFFastNonEmptyIterator() {
    delete[] interval;
    delete[] itr0;
    delete[] itr1;
}




/*******************************************
 *Fast Empty span iterator
 *******************************************/
AMFInterval::AMFFastEmptySpanIterator::AMFFastEmptySpanIterator(AMFInterval* intr) {
    interval = intr;
    current = interval->getBottom();
    nxt = interval->getBottom();
}
iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFFastEmptySpanIterator::operator ++() {
    AMFunction ret = nxt;
    if(nxt.equals(interval->getTop())) isFinished = true;
    else nxt = interval->getTop();
    current = ret;
    return *this;
}

bool AMFInterval::AMFFastEmptySpanIterator::hasNext() {
    return !isFinished;
}

AMFInterval::AMFFastEmptySpanIterator::~AMFFastEmptySpanIterator() {
    delete[] interval;
}

/*******************************************
 * ITERATOR Fast Empty
 *******************************************/
AMFInterval::AMFFastEmptyIterator::AMFFastEmptyIterator() {
    
}

iterator<forward_iterator_tag, AMFunction> AMFInterval::AMFFastEmptyIterator::operator ++() {
    return *this;
}

bool AMFInterval::AMFFastEmptyIterator::hasNext() {
    return false;
}

AMFInterval::AMFFastEmptyIterator::~AMFFastEmptyIterator() {
    delete[] this;
}

//AMFGraph AMFInterval::graph() {
//	AMFGraph g;
//	edges_t* edges = g.getEdges();
//	const AMFunction &r1 = this->getBottom();
//	const AMFunction &r2 = this->getTop();
//	for (SmallBasicSet r : r2.getSets()) {
//		set<SmallBasicSet> cr;
//		for (SmallBasicSet s : r2.getSets()) {
//			if (!r1.ge(s.setintersect(s))) { cr.insert(s); };
//		}
//		edges->insert(make_pair(r,cr));
//	}
//	return g;
//}

edges_t AMFInterval::edges() {
//	AMFGraph g;
	edges_t edges;
    AMFunction r1 = getBottom();
    AMFunction r2 = getTop();
	for (SmallBasicSet r : r2.getSets()) {
		set<SmallBasicSet> cr;
		for (SmallBasicSet s : r2.getSets()) {
			if (!r1.ge(r.setintersect(s))) { cr.insert(s); };
		}
		edges.insert(make_pair(r,cr));
	}
	return edges;
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
		return SetsPoset(getBottom(),getTop()).get_lattice_size();
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
    if(!(bottom.ge(span))) {
        best = span;
        value = abs(best.size() - target);
    }
    if (best == 0|| best.size() <= target) {
        return best;
    }
    
    SmallBasicSet::iterator it = span.getIterator();
    while (it.hasNext()) {
    	++it;
        SmallBasicSet candidate;
        SmallBasicSet diff = span.difference(*it);
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
AMFInterval::GeneralIterator* AMFInterval::getClosedIterator() {
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

AMFInterval::GeneralFastIterator * AMFInterval::getFastIterator() {
    if (getBottom().leq(getTop())){
        SmallBasicSet span = getTop().span();
        if (span.isemptyset()) {
            return new AMFFastEmptySpanIterator(this);
        }
        else {
            return new AMFFastNonEmptyIterator(this);
        }
        
    }
    else {
        return new AMFFastEmptyIterator();
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
