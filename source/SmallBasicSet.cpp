/*
 * SmallBasicSet.cpp
 *
 *  Created on: 20-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 */
#include "SmallBasicSet.h"
#include <bitset>

SmallBasicSet::SmallBasicSet() : numberofelem(0) {
    set = 0;
}

SmallBasicSet::SmallBasicSet(int a[], int asize) : numberofelem(asize) {
	set = 0;
	setSet(a,asize);
}

SmallBasicSet::SmallBasicSet(uint_fast16_t a) {
    set = a;
    numberofelem = bitset<16>(a).count();
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

string int_to_string(int a) {
	stringstream ss;
	ss << a;
	string str = ss.str();
	return str;
}

string SmallBasicSet::toString() {
	stringstream ss;
    int copyset = set;
	ss << "{";
	for (int i = MAXELEMENT ; i >= 0 ; i-- ) {
        int temp = copyset - getBit(i);
		if (temp >= 0) {
			ss << int_to_string(i);
            copyset = temp;
		}
	}
	ss << "}";
	return ss.str();
}

string SmallBasicSet::toBitString() {
	return std::bitset<13>(set).to_string();
}

/*******************************************
 * OPERATIONS
 *******************************************/

/**
 * map this set according to the transformation in table: i -> table[i] + 1
 * TODO: verify this is working correctly
 */
SmallBasicSet SmallBasicSet::map(int table[]) const {
	SmallBasicSet res;
	int i = 1;
	int bit = 1;
	while (i <= MAXELEMENT) {
		if ((bit & set) > 0) { res.quickadd(table[i] + 1); }
		bit <<= 1; i++;
	}
	return res;
}
void SmallBasicSet::quickadd(int a) {
	set |= getBit(a);
}

SmallBasicSet SmallBasicSet::setunion(SmallBasicSet other) const {
	SmallBasicSet unie = set | other.set;
    return unie;
}

SmallBasicSet SmallBasicSet::setdifference(SmallBasicSet other) const {
	SmallBasicSet difference = set & ~ other.set;
	return difference;
}

SmallBasicSet SmallBasicSet::setintersect(SmallBasicSet other) const {
	SmallBasicSet intersect = set & other.set;
	return intersect;
}

/*******************************************
 * MISC
 *******************************************/

bool SmallBasicSet::equals(SmallBasicSet other) const {
	return (set == other.set);
}

bool SmallBasicSet::hasAsSubset(SmallBasicSet other) const {
	return equals(setunion(other));
}

bool SmallBasicSet::operator<(const SmallBasicSet& other) const {
    return set < other.set;
}

bool SmallBasicSet::operator==(const SmallBasicSet& other) const {
	return equals(other);
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
    if(asize >0){
    for (int i = 0; i<asize ; i++) {
        set += getBit(setvalues[i]);
    }
    }
}

uint_fast16_t SmallBasicSet::getSet() const {
	return set;
}

/*******************************************
 * CLASS
 *******************************************/

SmallBasicSet SmallBasicSet::universe() {
	return SmallBasicSet((1 << MAXELEMENT) - 1);
}

SmallBasicSet SmallBasicSet::universe(int n) {
	return SmallBasicSet((1 << n) - 1);
}
