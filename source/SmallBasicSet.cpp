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

int SmallBasicSet::numberofelements() {
	return bitset<MAXELEMENT>(set).count();
}

int SmallBasicSet::maximum() {
	int max = MAXELEMENT;
	int bit = getBit(MAXELEMENT);
	while ( max > 0 ) {
		if ((bit & set) != 0) { break; }
		bit >>= 1;
		max--;
	}
	return max;
}

int SmallBasicSet::minimum() {
	int min = 1;
	int bit = 1;
	while ( min <= MAXELEMENT ) {
		if ((bit & set) != 0) { break; }
		bit <<= 1;
		min++;
	}
	return min;
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

string SmallBasicSet::toString() const {
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
	return std::bitset<MAXELEMENT>(set).to_string();
}

std::ostream& operator<<(std::ostream &strm, const SmallBasicSet &s) {
  return strm << s.toString();
}

/*******************************************
 * OPERATIONS
 *******************************************/

/**
 * map this set according to the transformation in table: i -> table[i] + 1
 * that is: for every integer i in the set, the integer table[i] + 1
 * will be in the resulting set. required: maximum <= size(table)
 * TODO: verify this is working correctly
 */
SmallBasicSet SmallBasicSet::map(int table[]) const {
	SmallBasicSet res;
	int i = 1;
	int bit = 1;
	while (i <= MAXELEMENT) {
		if ((bit & set) != 0) { res.quickadd(table[i] + 1); }
		bit <<= 1; i++;
	}
	return res;
}
void SmallBasicSet::quickadd(int a) {
	set |= getBit(a);
}

SmallBasicSet SmallBasicSet::setunion(SmallBasicSet other) const {
	SmallBasicSet unie(set | other.set);
    return unie;
}

SmallBasicSet SmallBasicSet::setdifference(SmallBasicSet other) const {
    SmallBasicSet difference(set & (~ other.getSet()));
	return difference;
}

SmallBasicSet SmallBasicSet::difference(int other) const {
    SmallBasicSet difference(set & (~ getBit(other)));
	return difference;
}

SmallBasicSet SmallBasicSet::setintersect(SmallBasicSet other) const {
	SmallBasicSet intersect(set & other.set);
	return intersect;
}

SmallBasicSet SmallBasicSet::operator/(const SmallBasicSet& other) const {
	return setdifference(other);
}

/*******************************************
 * MISC
 *******************************************/

bool SmallBasicSet::isemptyset() {
    return set==0;
}

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

int SmallBasicSet::getBit(int positie) const {
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

/*******************************************
 * ITERATOR
 *******************************************/
SmallBasicSet::SBSIterator::SBSIterator(SmallBasicSet* set) {
	sbs = set;
	current = 1;
	bit = 1;
	while ( current <= MAXELEMENT ) {
		if ((bit & sbs->set) != 0) { break; }
		bit <<= 1;
		current++;
	}
	prev = current;
}

SmallBasicSet::SBSIterator SmallBasicSet::SBSIterator::operator ++() {
	prev = current;
	bit <<= 1;
	current++;
	while(( bit & sbs->set) == 0 && current <= sbs->MAXELEMENT) {
		bit <<= 1;
		current++;
	}
	return (*this);
}

bool SmallBasicSet::SBSIterator::hasNext() {
	return ( current <= (sbs->MAXELEMENT) );
}
