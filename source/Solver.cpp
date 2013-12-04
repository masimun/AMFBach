/*
 * Solver.cpp
 *
 *  Created on: 3-dec.-2013
 *      Author: Daan
 */

#include "Solver.h"

Solver::Solver() {
	// TODO Auto-generated constructor stub
}

Solver::~Solver() {
	// TODO Auto-generated destructor stub
}

void mapstore(map<AMFunction, long> M, AMFunction A, long i) {
	M.insert(make_pair(A,i));
}
void mapstore(map<AMFunction, long> M, AMFunction A) {
	M.insert(make_pair(A,0L));
}

/**
 * ( n | i ) = n! / i!(n-i)!
 */
long Solver::combinations(int n, int i) {
	if ( n < i ) { return 0; }
	long res = 1;
	int t = n - i;
	while (n > t) { res *= n; n--; }
	while (i > 0) { res /= i; i--; }
	return res;
}

vector<map<AMFunction,long>> Solver::algorithm9(int till) {
	vector<map<AMFunction,long>> res(till);
	map<AMFunction, long> res0;
	mapstore(res0,AMFunction::emptyFunction());
	mapstore(res0,AMFunction::emptySetFunction());
	res.push_back(res0);
	for (int n = 0; n < till; n++ ) {
		//res.push_back(algorithm7(n,res.at(n))); // calculate AMF(n+1) and add it to res.
	}
	return res;
}

// TODO: algo 7
