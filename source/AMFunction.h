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
 * This class represents an anti-monotonic function.
 *
 * Invariant: isAntiMonotonic()
 */
class AMFunction {
private:
	SmallBasicSet universe = SmallBasicSet::universe(); // set of integers on which the function operates
	set<SmallBasicSet> sets;
public:
	AMFunction();
	AMFunction(SmallBasicSet N);
	AMFunction(SmallBasicSet s[], int size);
	virtual ~AMFunction();

	// query
	SmallBasicSet span();
	bool isAntiMonotonic();
	bool isEmpty();
	set<SmallBasicSet> getSets() const;
	string toString();
	bool contains(SmallBasicSet s);

	// alter
	void setSets(set<SmallBasicSet> ss);
	void addSet(SmallBasicSet s);
	void removeSets(list<SmallBasicSet> rs);
	void addSetConditional(SmallBasicSet s);
	void makeAntiMonotonic();

	// compare
	bool leq(AMFunction other);
	bool equals(AMFunction other);

	// operations
	AMFunction badclone();
	AMFunction join(AMFunction other) const;
	AMFunction meet(AMFunction other) const;
	AMFunction times(AMFunction other) const;
	AMFunction operator^(AMFunction other); // meet
	AMFunction operator+(AMFunction other); // join

	// class
	static AMFunction emptyFunction(SmallBasicSet N);
	static AMFunction universeFunction(SmallBasicSet N);
	static AMFunction universeFunction(int n);
	static AMFunction emptySetFunction(SmallBasicSet N);
};

#endif /* AMFUNCTION_H_ */
