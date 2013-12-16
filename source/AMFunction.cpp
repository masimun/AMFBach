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
	universe = SmallBasicSet::universe();
}

/* N = universe */
AMFunction::AMFunction(SmallBasicSet N) {
	universe = N;
}

AMFunction::AMFunction(SmallBasicSet s[], int size) {
	universe = SmallBasicSet::universe();
	for( int i = 0 ; i < size ; i++ ) {
		sets.insert(s[i]);
	}
}

AMFunction::~AMFunction() {
	// do nothing
}

SmallBasicSet AMFunction::span() const {
	SmallBasicSet span;
	for (SmallBasicSet s : sets ) {
		span = span.setunion(s);
	}
	return span;
}

vector<AMFunction> AMFunction::reduce(SmallBasicSet sbs) {
    int m = sbs.maximum();
    SmallBasicSet p = sbs.setdifference(m);
    vector<AMFunction> ret (2);
    AMFunction *pointer = ret.data();
    pointer[0] = (*this).project(p);
    AMFunction a1 = (*this);
    a1.removeAll(ret[0]);
    pointer[1] = a1.project(p);
    return ret;
}

AMFunction AMFunction::project(SmallBasicSet sbs) {
    AMFunction res = AMFunction((*this).universe);
    for (SmallBasicSet a : (*this).sets) {
        res.addSetConditional(a.setintersect(sbs));
    }
    return res;
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

bool AMFunction::isEmpty() const {
	return sets.empty();
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

/****************************************************
 * ALTER
 ****************************************************/

void AMFunction::setSets(set<SmallBasicSet> ss) {
	sets = ss;
}

/**
 * May break invariants
 */
void AMFunction::addSet(SmallBasicSet s) {
	sets.insert(s);
	bugstr = toString();
}

void AMFunction::removeAll(AMFunction amf) {
    set<SmallBasicSet> rs = amf.sets;
	for (SmallBasicSet s : rs) {
		sets.erase(s);
	}
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
	bugstr = toString();
}

/****************************************************
 * OPERATIONS
 ****************************************************/

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

/* (!) means not anti-monotonic! */
string AMFunction::toString() {
	if (isEmpty()) { return "0"; }
	string rep = "";
	for (SmallBasicSet s : sets) {
		rep += s.toString() + "-";
	}
	if (!isAntiMonotonic()) {
		rep += "(!)";
	}
	return rep;
}

AMFunction AMFunction::shallowclone() {
	AMFunction clone;
	for ( SmallBasicSet s : sets) {
		clone.addSet(s);
	}
	return clone;
}

AMFunction AMFunction::map(int inverse[]) {
	AMFunction res(universe);
	for (SmallBasicSet s : sets) {
		res.addSet(s.map(inverse));
	}
	return res;
}

AMFunction AMFunction::omicron(AMFunction tau, AMFunction alfa) {
	if ( tau.isEmpty() ) {
		if ( alfa.isEmpty() ) { return *this; }
		else { return AMFunction::emptyFunction(); }
	}
	AMFunction res(universe);
	res.addSet(span().setdifference(alfa.span()));
	for (SmallBasicSet s : alfa.getSets() ) {
		AMFunction current(universe);
		current.addSet(s);
		res = res.times(current.meet(tau));
	}
	return res.meet(*this);
}

AMFunction AMFunction::times(AMFunction other) const {
	if (isEmpty()) { return other; }
	else if (other.isEmpty()) { return (*this); }
	SmallBasicSet a = span();
	SmallBasicSet b = other.span();
	AMFunction res = AMFunction(universe);
	for (SmallBasicSet s1 : sets) {
		for (SmallBasicSet s2 : other.getSets() ) {
			res.addSetConditional((s1/b).setunion(s2/a).setunion(s1.setintersect(s2)));
		}
	}
	return res;
}

/****************************************************
 * COMPARE
 ****************************************************/

bool AMFunction::contains(SmallBasicSet s) {
	for (SmallBasicSet i : sets) {
		if (i.equals(s)) { return true; }
	}
	return false;
}

/* relies on ordered set */
bool AMFunction::equals(AMFunction other) const {
	return sets == other.getSets();
}

bool AMFunction::operator<(const AMFunction& other) const {
    return sets < other.getSets();
}

bool AMFunction::operator==(const AMFunction& other) const {
	return equals(other);
}

/**
 * Checks whether the AMFunction is less than or equal to other.
 */
bool AMFunction::leq(AMFunction other) const {
	for ( SmallBasicSet s1 : sets ) {
		bool contained = false;
		for ( SmallBasicSet s2 : other.getSets() ) {
			if (s2.hasAsSubset(s1)) { contained = true; break; }
		}
		if (!contained) { return false; }
	}
	return true;
}

/****************************************************
 * ALGO
 ****************************************************/
/*
unordered_set<vector<int>> AMFunction::symmetry_group() {
	unordered_set<vector<int>> res;
	SmallBasicSet sp = span();
	int maplen = sp.numberofelements();
	int max = sp.maximum();
	int min = sp.minimum();
	int* map = new int[maplen];
	int* inversemap = new int[max + 1];
	// iterate over set
	int pos = 0;
	int i = min;
	int bit = sp.getBit(i);
	while (i < max) {
		if ((bit & sp.getSet()) != 0) {
			map[pos] = i;
			inversemap[pos] = pos;
			pos++;
		}
		i++;
		bit <<= 1;
	}
	map[pos] = max;
	inversemap[pos] = pos;
	// permute
	PairPermutator perm(map,inversemap,maplen);
	int* p = new int[maplen+1];
	while (!perm.finished()) {
		perm.permute();
		AMFunction kand = this->map(inversemap);
		if ( (*this).equals(kand) ) {
			vector<int> v;
			std::copy(inversemap,inversemap+maplen,v.begin());
			res.insert(v);
		}
	}
	delete[] p;
	delete[] map;
	delete[] inversemap;
	return res;
}
*/

/****************************************************
 * CLASS
 ****************************************************/

/* N = universe */
AMFunction AMFunction::emptyFunction() {
	return AMFunction();
}

/* N = universe */
AMFunction AMFunction::emptySetFunction() {
	AMFunction amf = AMFunction();
	amf.addSet(SmallBasicSet());
	return amf;
}

/* N = universe */
AMFunction AMFunction::universeFunction(int n) {
	SmallBasicSet N = SmallBasicSet::universe(n);
	return universeFunction(N);
}

/* N = universe */
AMFunction AMFunction::universeFunction(SmallBasicSet N) {
	AMFunction amf = AMFunction(N);
	amf.addSet(N);
	return amf;
}

AMFunction AMFunction::singletonFunction(int l) {
	AMFunction amf;
	SmallBasicSet s;
	s.quickadd(l);
	amf.addSet(s);
	return amf;
}


