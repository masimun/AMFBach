/*
 * Solver.h
 *
 *  Created on: 3-dec.-2013
 *      Author: Daan
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <set>
#include <list>
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Solver {
public:
	Solver();
	virtual ~Solver();

	// ALGORITHMS
	vector<map<AMFunction,long>> algorithm9(int n);
	vector<map<AMFunction,long>> equivalence_classes(int n); // == algorithm 9
	map<AMFunction,long> algorithm7(long n, map<AMFunction,long> S);

	// COEFFICIENTS
};

#endif /* SOLVER_H_ */