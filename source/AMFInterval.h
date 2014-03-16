/*
 * AMFInterval.h
 *
 *  Created on: 2-dec.-2013
 *      Author: Daan
 */

#ifndef AMFINTERVAL_H_
#define AMFINTERVAL_H_

using namespace std;
#include <string>
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include "AMFGraph.h"
#include <utility>
#include <stdlib.h>

/**
 * A (closed) interval of anti-monotonic functions.
 */
class AMFInterval {
public:
    
    class AMFClosedIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        vector<SmallBasicSet> axes;
        AMFunction::AMFiterator X;
        AMFunction::AMFiterator Y;
        AMFInterval* Xaxis;
        AMFInterval* Yaxis;
        AMFunction currentX,currentY;
        AMFunction::AMFiterator currentIterator;
        
        AMFClosedIterator(AMFInterval* intr);
        AMFunction getCurrent(){return current;}
        const reference operator*();
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();
    };
    
    class AMFGeneralClosedIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        long spanSize;
        int pos;
        long minSizeBottom;
        vector<AMFunction> theList;
        SmallBasicSet span;
        
        AMFGeneralClosedIterator(AMFInterval* intr);
        AMFunction getCurrent(){return current;}
        const reference operator*();
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();
    };
    
    class AMFIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFInterval* intr;
        AMFunction current;
        bool thereIsNext;
        AMFClosedIterator theIt = *new AMFClosedIterator(intr);
        AMFunction nxt;
        //AMFunction amf;
        //SmallBasicSet span;// = (*interval).getTop().span();
        vector<AMFunction> theList;
        long pos,last;
        //AMFunction maxSpan;
        //AMFunction ret;
        
        //vector<AMFunction> alfaBottom;
        //vector<AMFunction> alfaTop;
        //vector<AMFunction> alfa;
        //vector<AMFIterator> iter;
        
        //AMFunction* pAlfaBottom = alfaBottom.data();
        //AMFunction* pAlfaTop = alfaTop.data();
        //AMFunction* pAlfa = alfa.data();
        //AMFIterator* pIterator = iter.data();
        
        AMFIterator(AMFInterval* intr);
        AMFunction getCurrent(){return current;}
        //const reference operator*();
        //AMFunction next(AMFunction top);
        iterator<forward_iterator_tag, AMFunction> operator++();
        //AMFIterator operator++(int junk) { return operator++(); };
        bool hasNext();
        // bool operator <=(const AMFunction otherAmf) { return amf.leq(otherAmf); };
        // bool operator >(const AMFunction otherAmf) { return !(amf.leq(otherAmf));};
        //bool operator ==(const AMFIterator other) { return amf.equals(other.amf); }
        //bool operator !=(const AMFIterator other) { return !amf.equals(other.amf); }

    };
    
    class AMFEmptyIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        int pos, last;
        vector<AMFunction> theList;
        
        AMFEmptyIterator(AMFInterval* intr);
        const reference operator*();
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();
    };
    
    class AMFOneOrTwoIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        int pos, last;
        vector<AMFunction> theList;
        SmallBasicSet span;
        
        AMFOneOrTwoIterator(AMFInterval* intr);
        AMFunction getCurrent(){return current;}
        const reference operator*();
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();
    };
    
    class AMFOneElementIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        bool given = false;
        
        AMFOneElementIterator(AMFInterval* intr);
        AMFunction getCurrent(){return current;}
        const reference operator*();
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();
    };
    
    
    
    class AMFExceptionalClosedIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFunction current;
        AMFunction bottom;
        bool virgin;
        AMFInterval* interval;
        //AMFIterator normal;

        
        AMFExceptionalClosedIterator(AMFInterval* intr);
        AMFunction getCurrent(){return current;}
        const reference operator*();
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();
    };
    


private:
	AMFunction from;
	AMFunction till;
    bool closedAtTop;
    bool closedAtBottom;

public:
	AMFInterval(AMFunction bottom, AMFunction top);
    AMFInterval(AMFunction bottom, AMFunction top, bool closedTop, bool closedBot);

	virtual ~AMFInterval();

    //iterator
    iterator<forward_iterator_tag, AMFunction> getIterator();
    //iterator end() 		{return iterator(this, this->till);};
    
    //methode
    vector<SmallBasicSet> bestSplit();
    SmallBasicSet bestSubset(SmallBasicSet span, long target, AMFunction bottom);
    
    //graph
    AMFGraph graph();
    long long lattice_size();
    
    //getters
    AMFunction getTop();
    AMFunction getBottom();
    SmallBasicSet getUniverse();
    bool isClosedAtBottom();
    bool isClosedAtTop();
    
	// query
	string toString();

	// class
	static AMFInterval fullspace(int n);
};

#endif /* AMFINTERVAL_H_ */
