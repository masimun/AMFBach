/*
 * Solver_test.cpp
 *
 *  Created on: 3-dec.-2013
 *      Author: Daan
 */

using namespace std;
#include "Solver.h"
#include "PairPermutator.h"
#include <iostream>
#include <tr1/unordered_set>

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

<<<<<<< HEAD
//int main() {
//	test_solver();
//}
=======
void test_hash() {
	// very test, much hash
	tr1::unordered_set<int> a;
}

void test_permutator() {
	int a[] = {1,2,3,4,5};
	int ai[6];
	ai[0] = 0;
	ai[1] = 0;
	ai[2] = 1;
	ai[3] = 2;
	ai[4] = 3;
	ai[5] = 4;
	PairPermutator p(a,ai,5);
	while (!p.finished()) {
		printarray(a, 5);
		printarray(ai, 6);
		cout << endl;
		p.permute();
	}

}
>>>>>>> 584c6a9f415021d1d8b29885af5550e34d32b30e

//int main() {
//	test_permutator();
//	return 0;
//}

