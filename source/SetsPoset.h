/*
 * SetsPoset.h
 *
 *  Created on: 22-feb.-2014
 *      Author: Daan
 */

#ifndef SETSPOSET_H_
#define SETSPOSET_H_

using namespace std;
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include <set>
#include <map>
#include <vector>
#include <climits>
#include <unordered_map>
#include <unordered_set>

class SetsPoset {
private:
	typedef SmallBasicSet SBS;

	vector<set<SBS>> level;
	map<SBS,set<SBS>> successors;
	map<SBS,set<SBS>> predecessors;
	map<SBS,set<SBS>> before;
	map<SBS,set<SBS>> after;
	int min_size;
	int max_size;

public:
	// constructing
	SetsPoset(const AMFunction & bottom, const AMFunction & top);
	virtual ~SetsPoset();
	void construct_level(const AMFunction & bottom, const AMFunction & top);
	void construct_cessors();
	void store(map<long,set<SmallBasicSet>>* h_level, SmallBasicSet s);

	// general
	bool is_empty();

	// size
	long get_lattice_size();
	long get_lattice_size(bool odd);
	long get_lattice_size(int exp,
						  unordered_map<SBS,set<SBS>,SBS::hasher>* prepredec,
						  set<SBS>* lowerlevel,
						  int l);

	int get_maximal_level_number();
	int get_max_level();
	set<SBS> get_level(int i);
	long get_width();

	// iterator
	class SetIterator : public iterator<forward_iterator_tag, SBS> {
	private:
		set<SBS> set1;
		set<SBS> set2;
		set<SBS>* current_set;
		set<SBS>* next_set;
		vector<SBS> elements;
		set<SBS> res;
		bool finished;
	public:
		SetIterator(const set<SBS> & thislevel);
		bool has_next();
		const set<SBS>& operator*() { return res; };
		SetIterator& operator++();
	};


};

#endif /* SETSPOSET_H_ */
