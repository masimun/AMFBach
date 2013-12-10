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
#include "Tests.hpp"

void test_amfunction();
void test_times();
void test_omicron();

int main() {
	test_omicron();
	return 0;
}

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

	cout << "tesing parser for amf" << endl;
	cout << p.parse_amf("{[12],[3],[45]}").toString() << endl;
}

void test_times() {
	cout << "# C++ FUNC TESTS - AMFUNCTION TIMES #" << endl;
	Parser p;
	SmallBasicSet s1 = p.parse("[3]", 1);
	SmallBasicSet s2 = p.parse("[13]", 2);
	SmallBasicSet s3 = p.parse("[2]", 1);
	SmallBasicSet s4 = p.parse("[12]",2);
	SmallBasicSet s5 = p.parse("[2]", 1);
	SmallBasicSet s6 = p.parse("[1]", 1);

	AMFunction a1;
	a1.addSet(s6);
	cout << a1.toString() << " : " << a1.span() << endl;
	AMFunction a2;
	a2.addSet(s3);
	cout << a2.toString() << " : " << a2.span() << endl;
	AMFunction a3;
	a3.addSet(s6);
	a3.addSet(s3);
	cout << a3.toString() << " : " << a3.span() << endl;
	AMFunction a4;
	a4.addSet(s1);
	cout << a4.toString() << " : " << a4.span() << endl;

	AMFunction a1x2 = a1.times(a2);
	AMFunction a3x4 = a3.times(a4);
	cout << a1x2.toString() << endl;
	cout << a3x4.toString() << endl;
}

void test_omicron() {

	Parser p;

	AMFunction testAlfa[] = {
					p.parse_amf("{[1,2],[3,4]}")
					,p.parse_amf("{[1,2],[2,3],[3,4,5]}")
					,p.parse_amf("{[1,2],[2,3,4,5]}")
					,p.parse_amf("{[1,2],[2,3,4,5]}")
					,p.parse_amf("{}")
	};
	AMFunction testTau[] = {
					p.parse_amf("{[1],[3,4]}")
					,p.parse_amf("{[1],[3],[4,5]}")
					,p.parse_amf("{[1],[2,3],[3,4],[2,4,5]}")
					,p.parse_amf("{[1],[2,3],[3,4],[2,4,5]}")
					,p.parse_amf("{}")
	};
	AMFunction testSpan[] = {
					p.parse_amf("{[1,2,3,4,5]}")
					,p.parse_amf("{[1,2,3,4,5,6,7]}")
					,p.parse_amf("{[1,2,3,4,5]}")
					,p.parse_amf("{[1,2,3],[2,3,4,5]}")
					,p.parse_amf("")
	};
	AMFunction testAnswer[] = {
						p.parse_amf("{[1, 3, 4, 5]}")
					,p.parse_amf("{[1, 3, 6, 7], [1, 4, 5, 6, 7]}")
					,p.parse_amf("{[2, 3], [1, 3, 4], [1, 4, 5], [2, 4, 5]}")
					,p.parse_amf("{[1, 3], [2, 3], [3, 4], [2, 4, 5]}")
					,p.parse_amf("")
	};

	cout << "# C++ FUNC TESTS - AMFUNCTION OMICRON #" << endl;
	for (int i = 0; i < 5; i++) {
		AMFunction top, alfa, tau;
		top = testSpan[i];
		alfa = testAlfa[i];
		tau = testTau[i];
		AMFunction o = top.omicron(tau,alfa);
		cout << top.toString() << ".omicron(" << tau.toString() << "," << alfa.toString() << ") = " << o.toString() << endl;
		test::ASSERT_EQUAL(testAnswer[i],o);
	}
}
