/*
 * SetsPoset.cpp
 *
 *  Created on: 22-feb.-2014
 *      Author: Daan
 */

#include "SetsPoset.h"

/**
 * Builds a poset of sets from an interval of AMF
 */
SetsPoset::SetsPoset(AMFInterval* delta) {
	map<long,set<SmallBasicSet>> h_level;
	AMFunction h = delta->getTop();
	AMFunction hh;
	AMFunction g = delta->getBottom();
	if (h.gt(g)) {
		min_size = INT_MAX;
		max_size = INT_MIN;
		while (!h.equals(g)) {
			hh = AMFunction();
			for (SmallBasicSet a : h.getSets()) {
				if(g.contains(a)) {
					hh.addSetConditional(a);
				} else {
					max_size = std::max(max_size, a.numberofelements());
					min_size = std::min(min_size, a.numberofelements());
					store(&h_level, a);
					for (SmallBasicSet s : AMFunction::immediate_subsets(a).getSets()) {
						hh.addSetConditional(s);
					}
				}
			}
			h = hh; // debug: check if data doesn't get overwritten
		}
		level.resize(max_size - min_size + 1);
		for (long k = min_size ; k <= max_size ; k++ ) {
			if ( h_level.find(k) != h_level.end() ) {
				level[k - min_size] = h_level.find(k)->second;
			} else {
				level[k - min_size] = set<SmallBasicSet>();
			}
		}
	} else {
		min_size = 0;
		max_size = 0;
		level[0] = set<SmallBasicSet>();
	}

	construct_cessors();
}

void SetsPoset::construct_cessors() {
	//...
}

SetsPoset::~SetsPoset() {
	// TODO Auto-generated destructor stub
}

void SetsPoset::store(map<long,set<SmallBasicSet>>* h_level, SmallBasicSet s) {

}

