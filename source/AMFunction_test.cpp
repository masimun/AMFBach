/*
 * AMFunction_test.cpp
 *
 *  Created on: 28-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 */

#include <iostream>
using namespace std;
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include "parser.h"
#include "stdlib.h"
#include "ctime"
#include <bitset>

void test_amfunction() {
	cout << "# C++ FUNC TESTS - AMFUNCTION #" << endl;
	Parser p;
	SmallBasicSet s1 = p.parse("[3]", 1);
	SmallBasicSet s2 = p.parse("[13]", 2);
	SmallBasicSet s3 = p.parse("[2]", 1);

	AMFunction a1;
	a1.addSetConditional(s1);
	a1.addSetConditional(s2);
	a1.addSetConditional(s3);

	cout << a1.toString() << endl; // expected: AM

	SmallBasicSet s4 = p.parse("[12]",2);

	AMFunction a2;
	a2.addSet(s4);
	cout << a2.toString() << endl; // expected AM

	cout << "----" << endl;

	AMFunction a3 = a1 ^ a2;
	AMFunction a4 = a1 + a2;
	cout << a3.toString() << endl; // expected : [1]-[2]-(AM)
	cout << a4.toString() << endl;
}

int main() {
	test_amfunction();
	return 0;
}
