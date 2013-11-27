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
#include "SmallBasicSet.h"
#include <iostream>

/**
 * Deze klasse stelt een antimonotonic function voor.
 *
 * Invariant: isAntiMonotonic()
 */
class AMFunction {
	set<SmallBasicSet> sets;
public:
	AMFunction();
	virtual ~AMFunction();

	// query
	SmallBasicSet span();
	bool isAntiMonotonic();
	set<SmallBasicSet> getSets() const;

	// alter
	void addSet(SmallBasicSet s);

};

#endif /* AMFUNCTION_H_ */
