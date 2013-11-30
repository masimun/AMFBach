/*
 * AMFunction.cpp
 *
 *  Created on: 25-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 *
 *
 */
#include "AMFunction.h"
#include <iostream>

AMFunction::AMFunction() {
	// do nothing
}

AMFunction::AMFunction(SmallBasicSet s[], int size) {
	for( int i = 0 ; i < size ; i++ ) {
		sets.insert(s[i]);
	}
}

AMFunction::~AMFunction() {
	// do nothing
}

SmallBasicSet AMFunction::span() {
	SmallBasicSet span;
	for (SmallBasicSet s : sets ) {
		span = span.setunion(s);
	}
	return span;
}

bool AMFunction::isAntiMonotonic() {
	bool amf = true;
	for (SmallBasicSet a : sets ) {
		for (SmallBasicSet b : sets ) {
			amf &= !(a.hasAsSubset(b)) || a.equals(b);
		}
	}
	return amf;
}

void AMFunction::makeAntiMonotonic() {
	list<SmallBasicSet> subsets;
	for (SmallBasicSet a : sets) {
		for (SmallBasicSet b : sets) {
			if (a.hasAsSubset(b)) {
				subsets.push_front(b);
			}
		}
	}
	removeSets(subsets);
}

set<SmallBasicSet> AMFunction::getSets() const {
	return sets;
}

void AMFunction::setSets(set<SmallBasicSet> ss) {
	sets = ss;
}

/**
 * May break invariants
 */
void AMFunction::addSet(SmallBasicSet s) {
	sets.insert(s);
}

void AMFunction::removeSets(list<SmallBasicSet> rs) {
	for (SmallBasicSet s : rs) {
		sets.erase(s);
	}
}

/**
 * Does not break invariants:
 *
 * if isAntiMonotonic() was true before,
 * it will still be after adding (s)
 *
 * doesn't add (s) if there is a superset of (s)
 * removes all subsets of (s) uppon adding.
 */

void AMFunction::addSetConditional(SmallBasicSet s) {
	list<SmallBasicSet> subsets;
	for (SmallBasicSet a : sets ) {
		if (s.hasAsSubset(a)) { subsets.push_front(a); } // found a subset --> REMOVE
		if (a.hasAsSubset(s)) { return; } // found a superset --> STOP!
	}
	removeSets(subsets);
	sets.insert(s);
}

AMFunction AMFunction::join(AMFunction other) const {
	AMFunction a;
	for (SmallBasicSet s1 : sets ) {
		a.addSetConditional(s1);
	}
	for (SmallBasicSet s2 : other.getSets() ) {
		a.addSetConditional(s2);
	}
	return a;
}

AMFunction AMFunction::operator+(AMFunction other) {
	return join(other);
}

AMFunction AMFunction::meet(AMFunction other) const {
	AMFunction a;
	for (SmallBasicSet s1 : sets ) {
		for (SmallBasicSet s2 : other.getSets() ) {
            a.addSetConditional(s1.setintersect(s2));
		}
	}
	return a;
}

AMFunction AMFunction::operator^(AMFunction other) {
	return meet(other);
}

string AMFunction::toString() {
	string rep = "";
	for (SmallBasicSet s : sets) {
		rep += s.toString() + "-";
	}
	if (isAntiMonotonic()) {
		rep += "(AM)";
	} else {
		rep += "(Not AM)";
	}
	return rep;
}

/**
 * TODO: good clone
 */
AMFunction AMFunction::badclone() {
	AMFunction clone;
	for ( SmallBasicSet s : sets) {
		clone.addSet(s);
	}
	return clone;
}

bool AMFunction::contains(SmallBasicSet s) {
	for (SmallBasicSet i : sets) {
		if (i.equals(s)) { return true; }
	}
	return false;
}

/**
 * relies on ordered set
 */
bool AMFunction::equals(AMFunction other) {
//	bool equal = true;
//	set<SmallBasicSet>::iterator ita = sets.begin();
//	set<SmallBasicSet>::iterator itb = other.getSets().begin();
//	while (ita != sets.end()) {
//		if (itb == sets.end()) { return false; }
//		cout << (*ita).getSet() << "--" << (*itb).getSet() << endl;
//		equal &= ((*ita).equals(*itb));
//		ita++;
//		itb++;
//	}
//	return equal;
	return sets == other.getSets();
}
