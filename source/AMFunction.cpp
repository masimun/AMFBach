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
	for (set<SmallBasicSet>::iterator it = sets.begin(); it != sets.end() ; ++it ) {
		span = span.setunion(*it);
	}
	return span;
}

bool AMFunction::isAntiMonotonic() {
	bool amf = true;
	for (set<SmallBasicSet>::iterator a = sets.begin(); a != sets.end() ; ++a ) {
		for (set<SmallBasicSet>::iterator b = sets.begin(); b != sets.end() ; ++b ) {
			amf &= (!((*a).hasAsSubset(*b)) || (*a).equals(*b));
		}
	}
	/**
	 * Aan Max: is jouw C-compiler C++11 ?
	 * anders kunnen we dit herschrijven als:
	 *
	 * 	for ( SmallBasicSet a : sets ) {
	 * 		for ( SmallBasicSet b : sets ) {
	 * 			amf &= !((*a).hasAsSubset(*b));
	 * 		}
	 * 	}
	 *
	 * 	wat net iets duidelijker is :)
	 *
	 * 	(TODO: remove this comment)
	 */
	return amf;
}

set<SmallBasicSet> AMFunction::getSets() const {
	return sets;
}

/**
 * May break invariants
 */
void AMFunction::addSet(SmallBasicSet s) {
	sets.insert(s);
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
	for (set<SmallBasicSet>::iterator it = sets.begin() ; it != sets.end() ; ++it ) {
		if (s.hasAsSubset(*it)) { subsets.push_front(*it); } // found a subset --> REMOVE
		if ((*it).hasAsSubset(s)) { return; } // found a superset --> STOP!
	}
	for (list<SmallBasicSet>::iterator sub = subsets.begin(); sub != subsets.end() ; ++sub ) {
		sets.erase(*sub);
	}
	sets.insert(s);
}

AMFunction AMFunction::join(AMFunction other) const {
	AMFunction a;
	for (set<SmallBasicSet>::iterator it = sets.begin() ; it != sets.end() ; ++it ) {
		a.addSetConditional(*it);
	}
	for (set<SmallBasicSet>::iterator it = other.getSets().begin() ; it != other.getSets().end() ; ++it ) {
		a.addSetConditional(*it);
	}
	return a;
}

AMFunction AMFunction::operator+(AMFunction other) {
	return join(other);
}

AMFunction AMFunction::meet(AMFunction other) const {
	AMFunction a;
	for (set<SmallBasicSet>::iterator x = sets.begin() ; x != sets.end() ; ++x ) {
		for (set<SmallBasicSet>::iterator y = other.getSets().begin() ; y != other.getSets().end() ; ++y ) {
			a.addSetConditional((*x).setintersect(*y));
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
