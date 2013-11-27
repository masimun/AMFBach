/*
 * AMFunction.h
 *
 *  Created on: 25-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 */
#ifndef AMFUNCTION_H_
#define AMFUNCTION_H_

using namespace std;
#include <set>
#include <list>
#include "SmallBasicSet.h"
#include <iostream>

/**
 * Deze klasse stelt een antimonotonic function voor.
 *
 * Invariant: isAntiMonotonic()
 */
class AMFunction {
private:
	SmallBasicSet universe = SmallBasicSet::universe(); // set of integers on which the function operates
	set<SmallBasicSet> sets;
public:
	AMFunction();
	AMFunction(SmallBasicSet s[], int size);
	virtual ~AMFunction();

	// query
	SmallBasicSet span();
	bool isAntiMonotonic();
	set<SmallBasicSet> getSets() const;

	// alter
	void addSet(SmallBasicSet s);
	void addSetConditional(SmallBasicSet s);

	// operations
	AMFunction join(AMFunction other) const;
	AMFunction meet(AMFunction other) const;
};

#endif /* AMFUNCTION_H_ */
