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
#include <cmath>
#include <list>

void test_amfunction() {
	cout << "# C++ FUNC TESTS - AMFUNCTION #" << endl;
	Parser p;
	SmallBasicSet s1 = p.parse("[3]", 1);
	SmallBasicSet s2 = p.parse("[13]", 2);
	SmallBasicSet s3 = p.parse("[2]", 1);
	SmallBasicSet s4 = p.parse("[12]",2);
	SmallBasicSet s8 = p.parse("[2]", 1);
	SmallBasicSet s9 = p.parse("[1]", 1);

	AMFunction a1;
	a1.addSetConditional(s1);
	a1.addSetConditional(s2);
	a1.addSetConditional(s3);

	cout << a1.toString() << endl; // expected: AM

	AMFunction a2;
	a2.addSet(s4);
	cout << a2.toString() << endl; // expected AM

	cout << "----" << endl;

	AMFunction a3 = a1 ^ a2;
	AMFunction a4 = a1 + a2;
	cout << a3.toString() << endl; // expected : [1]-[2]-(AM)
	cout << a4.toString() << endl;

	AMFunction a5;
	a5.addSet(s8);
	a5.addSet(s9);
	cout << a5.toString() << endl;
	cout << a3.equals(a4) << endl; // expected : 0

	cout << "----" << endl;

	AMFunction a6;
	a6.addSetConditional(s1);
	a6.addSetConditional(s9);
	AMFunction a7;
	a7.addSetConditional(s2);
	cout << a6.toString() << endl;
	cout << a7.toString() << endl;

	cout << a6.leq(a7) << endl; // expected: 1
	cout << a7.leq(a6) << endl; // expected: 0
}

bool contains(list<AMFunction> as, AMFunction a) {
	for ( AMFunction b : as ) {
		if (b.equals(a)) {
			return true;
		}
	}
	return false;
}

void verynaivededekind() {
	int n = 3; // does not work for 4... and 5 is to hard to calculate.
	int sbsamount = pow(2,n);
	SmallBasicSet sbs[sbsamount];
	for (int i = 0 ; i < sbsamount ; i++) {
		sbs[i] = SmallBasicSet(i);
		cout << sbs[i].toString() << endl;
	}
	cout << "----" << endl;
	int ncount = n;
	int accdede = 1;
	cout << "lege AMF - 1" << endl;
	list<AMFunction> amfs;
	amfs.push_front(AMFunction());
	int itno = 0;
	while (ncount > 0) {
		cout << itno++ << endl;
		list<AMFunction> amfs_new;
		for (AMFunction a : amfs) {
			for (SmallBasicSet s : sbs) {
				AMFunction a_new = a.badclone();
				if (!a_new.contains(s)) {
					a_new.addSet(s);
					if (a_new.isAntiMonotonic() && !contains(amfs_new,a_new)) {
						cout << a_new.toString() << " - " << ++accdede << endl;
						amfs_new.push_front(a_new);
					}
				}
			}
		}
		amfs = amfs_new;
		ncount--;
	}
	cout << "Dedekind number for n = " << n << ": " << accdede;
}

int main() {
	test_amfunction();
	return 0;
}
