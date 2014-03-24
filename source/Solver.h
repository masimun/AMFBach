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
#include "AMFInterval.h"
#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
#include "Toolkit.hpp"
using namespace std;

class Solver {
public:
	Solver();
	virtual ~Solver();

	// UTILITY
	typedef long long bigint;
	static long combinations(int n, int i);
	static long comb(int n, int i);
	static void mapstore(map<AMFunction, long>& M, AMFunction A, long i);
	static void mapstore(map<AMFunction, long>& M, AMFunction A);

	// ALGORITHMS
	static vector<map<AMFunction,long>> algorithm9(int n);
	static vector<map<AMFunction,long>> equivalence_classes(int n); // == algorithm 9
	static map<AMFunction,long> algorithm7(int n, map<AMFunction,long> S);
	long long pc2_dedekind(int n);

	// COEFFICIENTS
    static long long PatricksCoefficient(AMFunction r1, AMFunction r2);
};

#endif /* SOLVER_H_ */
