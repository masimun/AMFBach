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
SetsPoset::SetsPoset(const AMFunction & bottom, const AMFunction & top) {
	construct_level(bottom,top);
	construct_cessors();
}

void SetsPoset::construct_level(const AMFunction & bottom, const AMFunction & top) {
	map<long,set<SmallBasicSet>> h_level;
	// AMFunction h = top;
	AMFunction* ph = new AMFunction(top);
	AMFunction hh;
	AMFunction g = bottom;
	if (top.gt(g)) {
		min_size = INT_MAX;
		max_size = INT_MIN;
		while (!(*ph).equals(g)) {
			AMFunction* hh = new AMFunction();
			for (SBS a : (*ph).getSets()) {
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
			delete ph;
			ph = hh;
		}
		delete ph;
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
	if ( h_level->find(size) == h_level->end() ) {
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
		if ( (int) get_level(i).size() == max ) {
			return i;
		}
	}
	return 1;
}

long SetsPoset::get_width() {
	long max = 0;
	for (int i = 1 ; i <= get_max_level() ; i++ ) {
		max = std::max((long) get_level(i).size(), max);
	}
	return max;
}

set<SmallBasicSet> SetsPoset::get_level(int n) {
	if ( n > (int) level.size() ) {
		return set<SmallBasicSet>();
	} else {
		return level[n-1];
	}
}

int SetsPoset::get_max_level() {
	return level.size();
}

long SetsPoset::get_lattice_size(bool odd) {
	int first_level;
	int exp;
	if (odd) {
		exp = 0;
		first_level = 1;
	}
	else {
		exp = (int) get_level(1).size();
		first_level = 2;
	}

	// for all levels firstLevel + 2k, compute the set of predecessors of the predecessors
	unordered_map<SBS,set<SBS>,SBS::hasher>* prepredec = new unordered_map<SBS,set<SBS>,SBS::hasher>();
	for (int i = first_level ; i <= this->get_max_level() ; i+=2 ) {
		for ( SBS s : get_level(i) ) {
			set<SBS> prepre;
			for ( SBS p : (*(this->predecessors.find(s))).second ) {
				for ( SBS r : (*(this->predecessors.find(p))).second ) {
					prepre.insert(r);
				}
			}
			prepredec->insert(make_pair(s,prepre));
		}
	}
	return get_lattice_size(exp,prepredec,new set<SBS>,first_level);
}

long pow(long exp) {
	return 1L << exp;
}

bool contains_all(const set<SmallBasicSet> & a , const set<SmallBasicSet> & b) {
	for ( SmallBasicSet s : b ) {
		if ( a.find(s) == a.end() ) {
			return false;
		}
	}
	return true;
}

long SetsPoset::get_lattice_size(int exp,
		unordered_map<SBS,set<SBS>,SBS::hasher>* prepredec,
		set<SBS>* lowerlevel, int l) {
	if ( l > get_max_level() ) {
		return pow(exp);
	} else {
		set<SBS> thislevel;
		set<SBS> good_successors;
		set<SBS> all_predecessors;
		for ( SBS s : get_level(l) ) {
			bool contains_all = true;
			for ( SBS r : (*(prepredec->find(s))).second ) {
				contains_all &= (lowerlevel->find(r) != lowerlevel->end());
			}
			if (contains_all) {
				thislevel.insert(s);
			}
		}
		SetIterator it(thislevel);
		long res = 0L;
		while(it.has_next()) {
			++it;
			set<SBS> alfa = *it;
			good_successors.clear();
			if ( l + 1 <= this->get_max_level() ) {
				set<SBS> level_above = this->get_level(l+1);
				for ( SBS t : level_above ) {
					if (contains_all(alfa,(*(this->predecessors.find(t))).second)) {
						good_successors.insert(t);
					}
				}
			}
			all_predecessors.clear();
			for ( SmallBasicSet s : alfa ) {
				for ( SmallBasicSet r : (*(this->predecessors.find(s))).second ) {
					all_predecessors.insert(r);
				}
			}
			int my_exp = good_successors.size();
			int low_exp = all_predecessors.size();
			res += pow(exp - low_exp) * get_lattice_size(my_exp,prepredec,&alfa,l+2);
		}
		return res;
	}
}

/**********************************
 * ITERATOR
 **********************************/

SetsPoset::SetIterator::SetIterator(const set<SBS> & thislevel) {
	current_set = &set1;
	next_set = &set2;
	elements.reserve(thislevel.size());
	for ( SmallBasicSet s : thislevel ) {
		next_set->insert(s);
		elements.push_back(s);
	}
	finished = false;
}

bool SetsPoset::SetIterator::has_next() {
	return !finished;
}

SetsPoset::SetIterator& SetsPoset::SetIterator::operator++() {
	// set the dereference value
	res.clear();
	for ( SBS s : *current_set ) {
		res.insert(s);
	}
	// current_set follows next_set
	// swap the pointers
	std::swap(next_set,current_set);
	int i;
	for (i = 0 ; i < (int) elements.capacity() ; i++) {
		if (current_set->find(elements.at(i)) != current_set->end()) {
			current_set->erase(elements.at(i));
		} else {
			current_set->insert(elements.at(i));
			break;
		}
	}
	for (i = 0 ; i < (int) elements.capacity() ; i++) {
		if (current_set->find(elements.at(i)) != current_set->end()) {
			current_set->erase(elements.at(i));
		} else {
			current_set->insert(elements.at(i));
			break;
		}
	}
	// if current becomes empty the iteration is finished
	if (current_set->empty()) {
		finished = true;
	}
	return *this;
}
