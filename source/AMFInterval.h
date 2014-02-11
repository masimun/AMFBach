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
        AMFunction amf;
        SmallBasicSet span;// = (*interval).getTop().span();
        AMFunction maxSpan;
        AMFunction ret;
        
        vector<AMFunction> alfaBottom;
        vector<AMFunction> alfaTop;
        vector<AMFunction> alfa;
        vector<AMFIterator> iter;
        
        AMFunction* pAlfaBottom = alfaBottom.data();
        AMFunction* pAlfaTop = alfaTop.data();
        AMFunction* pAlfa = alfa.data();
        AMFIterator* pIterator = iter.data();
        
        AMFIterator(AMFInterval* intr,AMFunction funct);
        const reference operator*() {return amf;}
        AMFunction operator++();
        AMFunction operator++(int junk) { return operator++(); };
        bool hasNext() {return !(amf.isEmpty());};
        // bool operator <=(const AMFunction otherAmf) { return amf.leq(otherAmf); };
        // bool operator >(const AMFunction otherAmf) { return !(amf.leq(otherAmf));};
        bool operator ==(const AMFIterator other) { return amf.equals(other.amf); }
        bool operator !=(const AMFIterator other) { return !amf.equals(other.amf); }

    };
    
private:
	AMFunction from;
	AMFunction till;

public:
	AMFInterval(AMFunction bottom, AMFunction top);
	virtual ~AMFInterval();

    //iterator
	typedef AMFIterator iterator;
    iterator getIterator() 	{return iterator(this, this->till);};
    //iterator end() 		{return iterator(this, this->till);};
    
    //graph
    AMFGraph graph();
    
    //getters
    AMFunction getTop();
    AMFunction getBottom();
    
	// query
	string toString();

	// class
	static AMFInterval fullspace(int n);
};

#endif /* AMFINTERVAL_H_ */
