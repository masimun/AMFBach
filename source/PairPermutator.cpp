/*
 * PairPermutator.cpp
 *
 *  Created on: 5-dec.-2013
 *      Author: Daan
 */

#include "PairPermutator.h"

PairPermutator::PairPermutator(int* tab, int* inv , int len) {
	table = tab;
	inverse = inv;
	length = len;
	done = false;
}

PairPermutator::~PairPermutator() {
	// TODO Auto-generated destructor stub
}

void PairPermutator::permute() {
	done = !next_permutation(table,table+length);
	next_permutation(inverse+1,inverse+1+length);
}

bool PairPermutator::finished() {
	return done;
}
