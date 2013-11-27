/*
 * AMFunction.cpp
 *
 *  Created on: 25-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 */
#include "AMFunction.h"
#include <iostream>

AMFunction::AMFunction() {
	// do nothing
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

void AMFunction::addSet(SmallBasicSet s) {
	sets.insert(s);
}
