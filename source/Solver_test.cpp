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
#include <cstdlib>


void test_pc2_dedekind(int m) {
    Solver::pc2_dedekind(m);
}

int main() {
    test_pc2_dedekind(7);
    return 0;
}

/*
int main(int argc, const char* arg[]) {
	int  m = atoi(arg[1]);
	//cout << "Start calculating n=" << m<< endl;
	test_pc2_dedekind(m);
	
	//simple_decimal_bignum ward = "56713727820156410577229101238628035243";
    //	BigInt dedekind = "56130437228687557907788";
    //	cout << dedekind * dedekind << endl;
    return 0;
}
*/