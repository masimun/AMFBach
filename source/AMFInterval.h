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
private:
	AMFunction from;
	AMFunction till;
public:
	AMFInterval(AMFunction bottom, AMFunction top);
	virtual ~AMFInterval();

	// query
	string toString();

	// class
	static AMFInterval fullspace(int n);
};

#endif /* AMFINTERVAL_H_ */
