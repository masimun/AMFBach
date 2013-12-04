/*
 * AMFInterval.cpp
 *
 *  Created on: 2-dec.-2013
 *      Author: Daan
 */

#include "AMFInterval.h"

AMFInterval::AMFInterval(AMFunction bottom, AMFunction top) {
	from = bottom;
	till = top;
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
