/*
 * SmallBasicSet.cpp
 *
 *  Created on: 20-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 */
#include "SmallBasicSet.h"
#include <bitset>

const int SmallBasicSet::bits[13] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096};

SmallBasicSet::SmallBasicSet() {
    set = 0;
}

SmallBasicSet::SmallBasicSet(int a[], int asize) {
	set = 0;
	setSet(a,asize);
}

SmallBasicSet::SmallBasicSet(uint_fast16_t a) {
    set = a;
}

SmallBasicSet::~SmallBasicSet() {
	// destructor
}

int SmallBasicSet::numberofelements() const{
	return bitset<MAXELEMENT>(set).count();
}

int SmallBasicSet::maximum() const {
	int max = MAXELEMENT;
	int bit = get_bit(MAXELEMENT);
	while ( max > 0 ) {
		if ((bit & set) != 0) { break; }
		bit >>= 1;
		max--;
	}
	return max;
}

int SmallBasicSet::minimum() const {
	int min = 1;
	int bit = 1;
	while ( min <= MAXELEMENT ) {
		if ((bit & set) != 0) { break; }
		bit <<= 1;
		min++;
	}
	return min;
}

int SmallBasicSet::maxelement() const {
	return MAXELEMENT;
}

int SmallBasicSet::maxintvalue() const {
	return MAXSET;
}

string int_to_string(int a) {
	stringstream ss;
	ss << a;
	string str = ss.str();
	return str;
}

string SmallBasicSet::toString() const {
	if ( numberofelements() == 0 ) {
		return "0";
	}
	stringstream ss;
    SBSIterator it = getIterator();
    while(it.hasNext()) {
    	++it;
    	ss << *it;
    }
	return ss.str();
}

string SmallBasicSet::toBitString() const {
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

SmallBasicSet SmallBasicSet::minmap(vector<int> & map, int* & next) const {
	int res = 0;
	SBSIterator it = this->getIterator();
	while( it.hasNext() ) {
		++it;
		int x = map[*it];
		if ( x == 0 ) {
			map[*it] = *next;
			res |= this->get_bit(*next);
			next++;
		} else {
			res |= this->get_bit(x);
		}
	}
	return SmallBasicSet(res);
}

void SmallBasicSet::quickadd(int a) {
	set |= get_bit(a);
}

SmallBasicSet SmallBasicSet::setunion(const SmallBasicSet & other) const {
	SmallBasicSet unie(set | other.set);
    return unie;
}

SmallBasicSet SmallBasicSet::setdifference(const SmallBasicSet & other) const {
    SmallBasicSet difference(set & (~ other.getSet()));
	return difference;
}

/**
 * Return an sbs wich is this sbs without given integer
 */
SmallBasicSet SmallBasicSet::difference(int other) const {
    SmallBasicSet difference(set & (~ get_bit(other)));
	return difference;
}

SmallBasicSet SmallBasicSet::setintersect(const SmallBasicSet & other) const {
	SmallBasicSet intersect(set & other.set);
	return intersect;
}

SmallBasicSet SmallBasicSet::operator/(const SmallBasicSet & other) const {
	return setdifference(other);
}

/*******************************************
 * MISC
 *******************************************/

bool SmallBasicSet::isemptyset() const {
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

int SmallBasicSet::get_bit(int positie) {
    return bits[std::max(positie-1,0)];
}

void SmallBasicSet::setSet(bool elements[]) {
    int n = 0;
    for (int i = 0; i < MAXELEMENT; ++i) {
        n += (elements[i] ? get_bit(i) : 0);
    }
    set = n;
}

void SmallBasicSet::setSet(int setvalues[], int asize) {
    if(asize >0){
    for (int i = 0; i<asize ; i++) {
        set += get_bit(setvalues[i]);
    }
    }
}

uint_fast16_t SmallBasicSet::getSet() const {
	return set;
}

int SmallBasicSet::size() const{
    return numberofelements();
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

SmallBasicSet::SBSIterator::SBSIterator(const SmallBasicSet* const set) {
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

/*******************************************
 * HASHER
 *******************************************/

size_t SmallBasicSet::hasher::operator() ( const SmallBasicSet & sbs ) const {
	return (size_t) (sbs.getSet() % MAXSET);
}
