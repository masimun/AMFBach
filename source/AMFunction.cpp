/*
 * AMFunction.cpp
 *
 *  Created on: 25-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 *
 *
 */
#include "AMFunction.h"
#include "Parser.h"
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

long AMFunction::size() const {
    return sets.size();
}

SmallBasicSet AMFunction::span() const {
	SmallBasicSet span;
	for (SmallBasicSet s : sets ) {
		span = span.setunion(s);
	}
	return span;
}

bool AMFunction::isAntiMonotonic() const {
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

set<SmallBasicSet> AMFunction::getSets() const {
	return sets;
}

/* (!) means not anti-monotonic! */
string AMFunction::toString() {
	if (isEmpty()) { return "[]"; }
	string rep = "[";
	for (SmallBasicSet s : sets) {
		rep += s.toString() + ",";
	}
	rep.pop_back();
	rep += "]";
	if (!isAntiMonotonic()) {
		rep += "(!)";
	}
	return rep;
}

SmallBasicSet AMFunction::getUniverse() {
    return universe;
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
void AMFunction::addSet(const SmallBasicSet & s) {
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
 * removes all subsets of (s) upon adding.
 */

void AMFunction::addSetConditional(const SmallBasicSet & s) {
	list<SmallBasicSet> subsets;
	for (SmallBasicSet a : sets ) {
		if (s.hasAsSubset(a)) { subsets.push_front(a); } // found a subset --> REMOVE
		if (a.hasAsSubset(s)) { return; } // found a superset --> STOP!
	}
	removeSets(subsets);
	sets.insert(s);
	bugstr = toString();
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

/****************************************************
 * OPERATIONS
 ****************************************************/

AMFunction AMFunction::join(const AMFunction & other) const {
	AMFunction a;
	for (SmallBasicSet s1 : sets ) {
		a.addSetConditional(s1);
	}
	for (SmallBasicSet s2 : other.getSets() ) {
		a.addSetConditional(s2);
	}
	return a;
}

AMFunction AMFunction::operator+(const AMFunction & other) const {
	return join(other);
}

AMFunction AMFunction::meet(const AMFunction & other) const {
	AMFunction a;
	for (SmallBasicSet s1 : sets ) {
		for (SmallBasicSet s2 : other.getSets() ) {
            a.addSetConditional(s1.setintersect(s2));
		}
	}
	return a;
}

AMFunction AMFunction::operator^(const AMFunction & other) const {
	return meet(other);
}

AMFunction AMFunction::shallowclone() {
	AMFunction clone;
	for ( SmallBasicSet s : sets) {
		clone.addSet(s);
	}
	return clone;
}

AMFunction AMFunction::map(int inverse[]) const {
	AMFunction res(universe);
	for (SmallBasicSet s : sets) {
		res.addSet(s.map(inverse));
	}
	return res;
}

AMFunction AMFunction::omicron(const AMFunction & tau, const AMFunction & alfa) const {
	if ( tau.isEmpty() ) {
		if ( alfa.isEmpty() ) { return *this; }
		else { return AMFunction::empty_function(); }
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

AMFunction AMFunction::times(const AMFunction & other) const {
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

AMFunction AMFunction::project(const SmallBasicSet & sbs) const {
    AMFunction res = AMFunction(this->universe);
    for (SmallBasicSet a : this->sets) {
        res.addSetConditional(a.setintersect(sbs));
    }
    return res;
}

AMFunction AMFunction::minus(const AMFunction & f) const {
    AMFunction res = AMFunction(universe);
    for (SmallBasicSet x : (*this).sets) {
        bool found = false;
        for (SmallBasicSet t : (f).sets) {
            if (t.hasAsSubset(x)) found = true;
        }
        if (!found) res.addSet(x);
    }
    return res;
}

/****************************************************
 * COMPARE
 ****************************************************/

bool AMFunction::contains(const SmallBasicSet & s) const {
	return sets.find(s) != sets.end();
}

/* relies on ordered set */
bool AMFunction::equals(const AMFunction & other) const {
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
bool AMFunction::leq(const AMFunction & other) const {
	for ( SmallBasicSet s1 : sets ) {
		bool contained = false;
		for ( SmallBasicSet s2 : other.getSets() ) {
			if (s2.hasAsSubset(s1)) { contained = true; break; }
		}
		if (!contained) { return false; }
	}
	return true;
}

bool AMFunction::gt(const AMFunction & other) const {
	return !(leq(other));
}

/**
 *  Check whether this AMFunction is greater or equal to the one with only x as an element
 */
bool AMFunction::ge(const SmallBasicSet & x) const {
	for ( SmallBasicSet s : sets ) {
		if (s.hasAsSubset(x)) {
			return true;
		}
	}
	return false;
}

/****************************************************
 * ALGO
 ****************************************************/

/**
 * returns the symmetry group of this AMFunction (treeset)
 */
AMFunction::perm_t AMFunction::symmetry_group() const {
	set<vector<int>,lex_compare> res;
	SmallBasicSet sp = span();
	int maplen = sp.numberofelements();
	int max = sp.maximum();
	int min = sp.minimum();
	int* map = new int[maplen];
	int* inversemap = new int[max + 1];
	// iterate over set
	int pos = 0;
	int i = min;
	int bit = sp.get_bit(i);
	inversemap[0] = 0;
	while (i < max) {
		if ((bit & sp.getSet()) != 0) {
			map[pos] = i;
			inversemap[i] = pos;
			pos++;
		}
		i++;
		bit <<= 1;
	}
	map[pos] = max;
	inversemap[i] = pos;
	// permute
	MappingPermutator perm(map,inversemap,maplen);
	while (perm.has_next()) {
		perm.permute();
		AMFunction kand = this->map(inversemap);
		if ( (*this).equals(kand) ) {
			vector<int> v (max+1);
			std::copy(inversemap,inversemap+max+1,v.begin());
			res.insert(v);
		}
	}
	delete[] map;
	delete[] inversemap;
	return res;
}

vector<AMFunction> AMFunction::reduce(const SmallBasicSet & sbs) const {
    int m = sbs.maximum();
    SmallBasicSet p = sbs.difference(m);
    vector<AMFunction> ret (2);
    AMFunction* pointer = ret.data();
    pointer[0] = this->project(p);
    AMFunction a1 = (*this);
    a1.removeAll(ret[0]);
    pointer[1] = a1.project(p);
    return ret;
}

AMFunction AMFunction::standard(const perm_t & permutations) const {
	AMFunction best = (*this);
	for ( vector<int> p : permutations ) {
		AMFunction kand = this->map(p.data());
		if (kand < best) {
			best = kand;
		}
	}
	return best;
}

AMFunction AMFunction::lexi_standard() const {
	AMFunction res;
	SmallBasicSet sp = span();
	int size = sp.numberofelements();
	int max = sp.maximum();
	vector<int> map(max+1);
	vector<set<SmallBasicSet>> size_partition(size+1);
	for ( SmallBasicSet s : this->getSets() ) {
		size_partition[s.numberofelements()].insert(s);
	}
	int x = 1;
	int* next = &x;
	for ( int i = 1 ; i <= size ; i++ ) {
		for ( SmallBasicSet s : size_partition[i] ) {
			res.addSet(s.minmap(map,next));
		}
	}
	return res;
}

AMFunction AMFunction::standard() const {
	SmallBasicSet sp = span();
	int maplen = sp.numberofelements();
	int max = sp.maximum();
	int* map = new int[maplen];
	int* inversemap = new int[max + 1];
	for ( int i = 0 ; i < max + 1 ; i++ ) { inversemap[i] = 0; }
	// iterate over set
	int pos = 0;
	SmallBasicSet::iterator it = sp.getIterator();
	while (it.hasNext()) {
		++it;
		map[pos] = *it;
		inversemap[*it] = pos++;
	}
	// permute
	AMFunction best = (*this);
	MappingPermutator perm(map,inversemap,maplen);
	while (perm.has_next()) {
		perm.permute();
		AMFunction kand = this->map(inversemap);
		if ( kand < best ) {
			best = kand;
		}
	}
	delete[] map;
	delete[] inversemap;
	return best;
}

/****************************************************
 * CLASS
 ****************************************************/

/* N = universe */
AMFunction AMFunction::empty_function() {
	return AMFunction();
}

/* N = universe */
AMFunction AMFunction::empty_set_function() {
	AMFunction amf = AMFunction();
	amf.addSet(SmallBasicSet());
	return amf;
}

/* N = universe */
AMFunction AMFunction::universe_function(int n) {
	SmallBasicSet N = SmallBasicSet::universe(n);
	return universe_function(N);
}

/* N = universe */
AMFunction AMFunction::universe_function(const SmallBasicSet & N) {
	AMFunction amf = AMFunction(N);
	amf.addSet(N);
	return amf;
}

AMFunction AMFunction::singleton_function(int l) {
	AMFunction amf;
	SmallBasicSet s;
	s.quickadd(l);
	amf.addSet(s);
	return amf;
}

AMFunction AMFunction::immediate_subsets(const SmallBasicSet & s) {
	AMFunction amf;
	SmallBasicSet::iterator it = s.getIterator();
	while(it.hasNext()) {
		++it;
		amf.addSet(s.difference(*it));
	}
	return amf;
}

/*******************************************
 * HASHER
 *******************************************/

size_t AMFunction::hasher::operator() (const AMFunction & amf ) const {
	uint_fast16_t hash = 0;
	for ( SmallBasicSet s : amf.getSets() ) {
		hash ^= s.getSet();
	}
	return hash;
}

