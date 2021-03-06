/*
 * Solver_test.cpp
 *
 *  Created on: 3-dec.-2013
 *      Author: Daan
 */

using namespace std;
#include "Solver.h"
#include "MappingPermutator.h"
#include <iostream>
#include <unordered_set>
#include "BigInt.h"

void printarray(int arr[], int size) {
	cout <<"[";
	for(int i = 0; i < size-1; i++) {
		cout << arr[i] << ", ";
	}
	cout << arr[size-1] << "]";
}

void test_solver() {
	cout << Solver::combinations(6,3) << endl;
}


void test_hash() {
	// very test, much hash
	unordered_set<int> a;
}

void test_permutator() {
	int count = 1;
	int a[] = {1,2,3,4,5};
	int ai[6];
	ai[0] = 0;
	ai[1] = 0;
	ai[2] = 1;
	ai[3] = 2;
	ai[4] = 3;
	ai[5] = 4;
	MappingPermutator p(a,ai,5);
	while (p.has_next()) {
		p.permute();
		printarray(a, 5);
		printarray(ai, 6);
		cout << " - " << count++ << endl;
	}

}

void test_algo9() {
	int n = 6;
	// generate
	vector<map<AMFunction,long>*>* classes = Solver::algorithm9(n);
	map<AMFunction,long> functions;

	// collect
	for (int i = 0; i < (int) classes->capacity() ; i++ ) {
		long coeff = Solver::combinations(n, i);
		for( pair<AMFunction,long> p : *classes->at(i)) {
			Solver::mapstore(functions, p.first, p.second * coeff);
		}
	}

	// count
	long long sum = 0;
	for ( pair<AMFunction,long> p : functions ) {
		sum += p.second;
	}
	cout << sum << endl;
}

void test_pc2_dedekind() {
	int m = 5+2;
	bignum d = Solver::pc2_dedekind(m);
	cout << d << endl;
}

int main() {
    test_pc2_dedekind();
	//simple_decimal_bignum ward = "56713727820156410577229101238628035243";
//	BigInt dedekind = "56130437228687557907788";
//	cout << dedekind * dedekind << endl;
	return 0;
}

