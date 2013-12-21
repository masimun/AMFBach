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
            AMFInterval *interval;
            AMFunction amf;
            SmallBasicSet span = (*interval).getTop().span();
            AMFIterator(AMFInterval intr,AMFunction funct) {interval = &intr; amf = funct;};
            const reference operator*() {return amf;}
            AMFIterator operator++() {
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
                    vector<AMFunction> alfaBottom = amf.reduce(span);
                    vector<AMFunction> alfaTop = (*interval).getTop().reduce(span);
                    vector<AMFunction> alfa (2);
                    vector<AMFunction> iterator (2);
                    // TODO hier aan verderwerken zie causie code
                }
                return (*this);
            };
            AMFIterator operator++(int junk) { return *this; };
            bool operator <=(const AMFunction otherAmf) { return amf.leq(otherAmf); };
            bool operator >(const AMFunction otherAmf) { return !(amf.leq(otherAmf)); };
        
    };
    
private:
	AMFunction from;
	AMFunction till;
    typedef AMFIterator iterator;
public:
	AMFInterval(AMFunction bottom, AMFunction top);
	virtual ~AMFInterval();

    //iterator
    iterator begin() {return iterator(*this, this->from);};
    iterator end() {return iterator(*this, this->till);};
    
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
