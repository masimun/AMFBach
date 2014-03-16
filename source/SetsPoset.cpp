/*
 * SetsPoset.cpp
 *
 *  Created on: 22-feb.-2014
 *      Author: Daan
 */

#include "SetsPoset.h"

/****************************************************
 * CONSTRUCTING
 ****************************************************/

/**
 * Builds a poset of sets from an interval of AMF
 */
SetsPoset::SetsPoset(AMFInterval* delta) {
	construct_level(delta);
	construct_cessors();
}

void SetsPoset::construct_level(AMFInterval* delta) {
	map<long,set<SmallBasicSet>> h_level;
	AMFunction h = delta->getTop();
	AMFunction* ph = &h;
	AMFunction hh;
	AMFunction g = delta->getBottom();
	if (h.gt(g)) {
		min_size = INT_MAX;
		max_size = INT_MIN;
		while (!(*ph).equals(g)) {
			AMFunction* hh = new AMFunction();
			for (SBS a : h.getSets()) {
				if(g.contains(a)) {
					hh->addSetConditional(a);
				} else {
					max_size = std::max(max_size, a.numberofelements());
					min_size = std::min(min_size, a.numberofelements());
					store(&h_level, a);
					for (SBS s : AMFunction::immediate_subsets(a).getSets()) {
						hh->addSetConditional(s);
					}
				}
			}
			delete[] ph;
			ph = hh;
		}
		delete[] ph;
		level.resize(max_size - min_size + 1);
		for (long k = min_size ; k <= max_size ; k++ ) {
			if ( h_level.find(k) != h_level.end() ) {
				level[k - min_size] = h_level.find(k)->second;
			} else {
				level[k - min_size] = set<SBS>();
			}
		}
	} else {
		min_size = 0;
		max_size = 0;
		level[0] = set<SBS>();
	}
}

void SetsPoset::construct_cessors() {
	for (int k = 0 ; k < (int) level.capacity() ; k++ ) {
		for (SBS t : level[k]) {
			predecessors.insert(make_pair(t,set<SBS>()));
			successors.insert(make_pair(t,set<SBS>()));
		}
	}
	for (int k = 0 ; k < (int) level.capacity() - 1 ; k++ ) {
		for (SBS s : level[k]) {
			map<SBS,set<SBS>>::iterator s_pair = successors.find(s);
			for (SBS t : level[k+1]) {
				if (t.hasAsSubset(s)) {
					(s_pair->second).insert(t);
					((predecessors.find(t))->second).insert(s);
				}
			}
		}
	}
}

SetsPoset::~SetsPoset() {
	// TODO Auto-generated destructor stub
}

void SetsPoset::store(map<long,set<SmallBasicSet>>* h_level, SmallBasicSet s) {
	int size = s.numberofelements();
	if ( h_level->find(size) != h_level->end() ) {
		h_level->insert(make_pair(size,set<SBS>()));
	}
	h_level->find(size)->second.insert(s);
}

/****************************************************
 * GENERAL
 ****************************************************/

bool SetsPoset::is_empty() {
	return (successors.empty());
}

/****************************************************
 * SIZE
 ****************************************************/

long SetsPoset::get_lattice_size() {
	if (is_empty()) { return 1; }
	else { return get_lattice_size((get_maximal_level_number() % 2) != 1); }
}

int SetsPoset::get_maximal_level_number() {
	long max = get_width();
	for( int i = 1 ; i <= get_max_level() ; i++ ) {
		if ( get_level(i).size() == max ) {
			return i;
		}
	}
	return 1;
}

long SetsPoset::get_width() {
	long max = 0;
	for (int i = 1 ; 1 <= get_max_level() ; i++ ) {
		max = std::max((long) get_level(i).size(), max);
	}
	return max;
}

set<SmallBasicSet> SetsPoset::get_level(int n) {
	if ( n > level.capacity() ) {
		return set<SmallBasicSet>();
	} else {
		return level[n-1];
	}
}

int SetsPoset::get_max_level() {
	return level.capacity();
}

long SetsPoset::get_lattice_size(bool odd) {
	int first_level;
	int exp;
	if (odd) {
		exp = 0;
	}
	else {
		exp = (int) get_level(1).size();
		first_level = 2;
	}

	// for all levels firstLevel + 2k, compute the set of predecessors of the predecessors
	//unordered_map<SBS,set<SBS>>* prepredec = new unordered_map<SBS,set<SBS>>();

    return true;
}
