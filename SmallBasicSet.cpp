/*
 * SmallBasicSet.cpp
 *
 *  Created on: 20-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 */
#include "SmallBasicSet.h"

string int_to_string(int a); // forward declaration

SmallBasicSet::SmallBasicSet() : numberofelem(0) {
	for (int i = 1 ; i < MAXELEMENT ; i++ ) {
		elements[i] = false;
	}
    set = 0;
}

SmallBasicSet::SmallBasicSet(int a[], int asize) : numberofelem(asize) {
	setSet(a,asize);
    /**
    for (int i = 1 ; i < MAXELEMENT ; i++ ) {
		elements[i] = false;
	}
	for (int i = 0 ; i < asize ; i++ ) {
		elements[a[i]] = true;
	}
    **/
}

SmallBasicSet::SmallBasicSet(uint_fast16_t a) {
    set = a;
}

SmallBasicSet::~SmallBasicSet() {
	// destructor
}

int SmallBasicSet::maxelement() {
	return MAXELEMENT;
}

int SmallBasicSet::maxintvalue() {
	return MAXINT;
}

//dit kan waarschijnlijk nog efficienter.
string SmallBasicSet::toString() {
	stringstream ss;
    int copyset = set;
	ss << "[";
	for (int i = MAXELEMENT ; i >= 0 ; i-- ) {
        int temp = copyset - getBit(i);
		if (temp >= 0) {
			ss << int_to_string(i);
            copyset = temp;
		}
	}
	ss << "]";
	return ss.str();
}

SmallBasicSet SmallBasicSet::setunion(SmallBasicSet other) const {
	SmallBasicSet unie = set | other.set;
    /**
	for (int i = 0 ; i < MAXELEMENT ; i++ ) {
		unie.elements[i] = elements[i] || other.elements[i];
	}
	return unie;
    **/
    return unie;
}

SmallBasicSet SmallBasicSet::setdifference(SmallBasicSet other) const {
	SmallBasicSet difference = set & ~ other.set;
	/**
    for (int i = 0 ; i < MAXELEMENT ; i++ ) {
		difference.elements[i] = elements[i] && !other.elements[i];
	}
    **/
	return difference;
}

SmallBasicSet SmallBasicSet::setintersect(SmallBasicSet other) const {
	SmallBasicSet intersect = set & other.set;
	/**
     for (int i = 0 ; i < MAXELEMENT ; i++ ) {
		intersect.elements[i] = elements[i] && other.elements[i];
	}
     **/
	return intersect;
}

bool SmallBasicSet::equals(SmallBasicSet other) const {
	/**
    bool equal = true;
	for (int i = 0 ; i < MAXELEMENT ; i++ ) {
		equal &= elements[i] == other.elements[i];
	}
     **/
	return (set == other.set);
}

bool SmallBasicSet::hasAsSubset(SmallBasicSet other) const {
	return equals(setunion(other));
}

bool SmallBasicSet::operator<(const SmallBasicSet& other) const {
    return id < other.id;
}

bool SmallBasicSet::operator==(const SmallBasicSet& other) const {
	return id == other.id;
}

int SmallBasicSet::getBit(int positie) {
    return bits[positie-1];
}

void SmallBasicSet::setSet(bool elements[]) {
    int n = 0;
    for (int i = 0; i < MAXELEMENT; ++i) {
        n += (elements[i] ? getBit(i) : 0);
    }
    set = n;
}

void SmallBasicSet::setSet(int setvalues[], int asize) {
    for (int i = 0; i<asize ; i++) {
        set += getBit(setvalues[i]);
    }
}


string int_to_string(int a) {
	stringstream ss;
	ss << a;
	string str = ss.str();
	return str;
}
