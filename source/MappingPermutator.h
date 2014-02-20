/*
 * MappingPermutator.h
 *
 *  Created on: 18-feb.-2014
 *      Author: Daan
 */

#ifndef MAPPINGPERMUTATOR_H_
#define MAPPINGPERMUTATOR_H_

using namespace std;
#include <vector>

/**
 * Permutes a mapping and its inverse in place
 */
class MappingPermutator {
private:
	int* map;
	int* inverse;
	int length;
	int next_insertion;
	bool done;
	MappingPermutator* sub_permutator;

	void swap(int i, int j);
public:
	MappingPermutator(int* the_map, int* the_inverse, int the_length);
	virtual ~MappingPermutator();

	void permute();
	bool has_next();
	void reset();
};

#endif /* MAPPINGPERMUTATOR_H_ */
