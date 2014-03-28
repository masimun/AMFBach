/*
 * MappingPermutator.cpp
 *
 *  Created on: 18-feb.-2014
 *      Author: Daan
 */

#include "MappingPermutator.h"

MappingPermutator::MappingPermutator(int* the_map, int* the_inverse, int the_length) {
	map = the_map;
	inverse = the_inverse;
	length = the_length;
	done = false;
	next_insertion = 0;
	if ( length == 0 ) {
		sub_permutator = 0;
	} else {
		sub_permutator = new MappingPermutator(map,inverse,length-1);
	}
}

MappingPermutator::~MappingPermutator() {
	if ( length > 0 ) {
		// delete[] sub_permutator; TODO: memory leak
	} 
}

bool MappingPermutator::has_next() {
	if ( length == 0 ) {
		// return (!done);
		return !done;
	} else {
		bool ret = (sub_permutator->has_next()) || (next_insertion != (length - 1));
		if (!ret && !done && (next_insertion > 0)) {
			swap(next_insertion,next_insertion-1);
			done = true;
		}
		return ret;
	}
}

void MappingPermutator::permute() {
	if ( length == 0 ) {
		done = true;
	} else {
		if (sub_permutator->has_next()) {
			sub_permutator->permute();
		} else {
			if (next_insertion > 0) { swap(length-1, next_insertion-1); }
			swap(length-1,next_insertion);
			next_insertion++;
			// delete[] sub_permutator;
			sub_permutator = new MappingPermutator(map,inverse,length-1); // TODO: memory leak
			// sub_permutator->reset();
			sub_permutator->permute();
		}
	}
}

void MappingPermutator::swap(int i, int j) {
	int hi = map[i];
	int hj = map[j];
	map[j] = hi;
	map[i] = hj;
	inverse[hi] = j;
	inverse[hj] = i;
}

void MappingPermutator::reset() {
	next_insertion = 0;
	done = false;
}
