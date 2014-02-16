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
	first = true;
}

PairPermutator::~PairPermutator() {
	// TODO Auto-generated destructor stub
}

void PairPermutator::permute() {
	int copy[length];
	for (int i = 0 ; i < length ; i++ ) { copy[i] = table[i]; }
	done = !next_permutation(table,table+length);
	int j = 0;
	while ( table[j] == copy[j] ) { j++; }
	int a = inverse[table[j]];
	inverse[table[j]] = inverse[copy[j]];
	inverse[copy[j]] = a;
}

bool PairPermutator::finished() {
	return done;
}
