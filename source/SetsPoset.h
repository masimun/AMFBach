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
#include "AMFInterval.h"
#include <set>
#include <map>
#include <vector>
#include <climits>

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
	SetsPoset(AMFInterval* delta);
	virtual ~SetsPoset();
	void construct_level(AMFInterval* delta);
	void construct_cessors();
	void store(map<long,set<SmallBasicSet>>* h_level, SmallBasicSet s);

	// general
	bool is_empty();

	// size
	long get_lattice_size();
	long get_lattice_size(bool odd);
	int get_max_level_number();
	// ...



};

#endif /* SETSPOSET_H_ */
