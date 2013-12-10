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
        friend class AMFunction;
        public:
            AMFunction* amf;
            AMFIterator(AMFInterval interval) {amf = &interval.from;};
            const reference operator*() {return *amf;}
        
    };
    
private:
	AMFunction from;
	AMFunction till;
    typedef AMFIterator iterator;
public:
	AMFInterval(AMFunction bottom, AMFunction top);
	virtual ~AMFInterval();

    //iterator
    iterator begin() {return iterator(*this);};
    iterator end() {return iterator(*this);};
    
    
	// query
	string toString();

	// class
	static AMFInterval fullspace(int n);
};

#endif /* AMFINTERVAL_H_ */
