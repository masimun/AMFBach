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
            AMFIterator(AMFInterval intr,AMFunction funct) {interval = &intr; amf = funct;};
            const reference operator*() {return amf;}
            AMFIterator operator++() { AMFIterator i = *this; return i; }
            AMFIterator operator++(int junk) { return *this; }
            bool operator <=(const AMFunction otherAmf) { return amf.leq(otherAmf); }
            bool operator >(const AMFunction otherAmf) { return !(amf.leq(otherAmf)); }
        
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
    
    
	// query
	string toString();

	// class
	static AMFInterval fullspace(int n);
};

#endif /* AMFINTERVAL_H_ */
