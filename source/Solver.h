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
#include "AMFGraph.h"
#include "BigInt.h"
#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
#include "mpi.h"

using namespace std;

class Solver {
public:
	Solver();
	virtual ~Solver();
    
    
	// UTILITY
	static long combinations(int n, int i);
	static long comb(int n, int i);
	static void mapstore(map<AMFunction, long>& M, AMFunction A, long i);
	static void mapstore(map<AMFunction, long>& M, AMFunction A);
	static void mapstore(map<AMFunction, bignum>& M, AMFunction A, bignum i);
    static edges_t graph(AMFunction r1, AMFunction r2);
    
	// ALGORITHMS
	static vector<map<AMFunction,long>*>* algorithm9(int n);
	static vector<map<AMFunction,long>*>* equivalence_classes(int n); // == algorithm 9
	static map<AMFunction,long>* algorithm7(int n, const map<AMFunction,long>* const S);
	static void pc2_dedekind(int n);
    
	// COEFFICIENTS
    static bignum PatricksCoefficient(const AMFunction & r1, const AMFunction & r2);
    
    // OTHER
    static void verynaivededekind();
};

#endif /* SOLVER_H_ */
