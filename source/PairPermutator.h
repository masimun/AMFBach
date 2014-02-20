/*
 * PairPermutator.h
 *
 *  Created on: 5-dec.-2013
 *      Author: Daan
 *
 *  DEPECRATED
 */

#ifndef PAIRPERMUTATOR_H_
#define PAIRPERMUTATOR_H_

#include <algorithm>
#include <utility>

using namespace std;

/**
 * DEPECRATED: USE MappingPermutator INSTEAD
 */
class PairPermutator {
private:
	int* table;
	int* inverse;
	int length;
	bool done;
	bool first;
public:
	PairPermutator(int* tab, int* inv, int len);
	virtual ~PairPermutator();

	void permute();
	bool finished();
};

#endif /* PAIRPERMUTATOR_H_ */
