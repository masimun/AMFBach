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

/**
 * A (closed) interval of anti-monotonic functions.
 */
class AMFInterval {
public:
    class AMFIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        //AMFunction amf;
        SmallBasicSet span;// = (*interval).getTop().span();
        long spanSize = span.size();
        long pos;
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
        const reference operator*();
        //AMFunction next(AMFunction top);
        AMFIterator operator++();
        //AMFIterator operator++(int junk) { return operator++(); };
        bool hasNext();
        // bool operator <=(const AMFunction otherAmf) { return amf.leq(otherAmf); };
        // bool operator >(const AMFunction otherAmf) { return !(amf.leq(otherAmf));};
        //bool operator ==(const AMFIterator other) { return amf.equals(other.amf); }
        //bool operator !=(const AMFIterator other) { return !amf.equals(other.amf); }

    };
    
    class AMFClosedIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        int pos, last;
        vector<AMFunction> theList;
        
        AMFClosedIterator(AMFInterval* intr);
        const reference operator*();
        AMFClosedIterator operator++();
        bool hasNext();
    };
    
    class AMFExceptionalClosedIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        AMFunction bottom;
        bool virgin;
        AMFInterval* interval;
        //AMFIterator normal;

        
        AMFExceptionalClosedIterator(AMFInterval* intr);
        const reference operator*();
        AMFExceptionalClosedIterator operator++();
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
    AMFIterator getIterator() 	{return AMFIterator(this);};
    //iterator end() 		{return iterator(this, this->till);};
    
    //graph
    AMFGraph graph();
    long long lattice_size();
    
    //getters
    AMFunction getTop();
    AMFunction getBottom();
    bool isClosedAtBottom();
    bool isClosedAtTop();
    
	// query
	string toString();

	// class
	static AMFInterval fullspace(int n);
};

#endif /* AMFINTERVAL_H_ */
