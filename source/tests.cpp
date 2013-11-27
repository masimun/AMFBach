/*
 * tests.cpp
 *
 *  Created on: 19-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 *
 * Deze file is bedoeld als een soort 'script' die de
 * testen (en later ook het algoritme) zal bevatten.
 * De parallelisering zal hier ook gebeuren.
 */

#include <iostream>
using namespace std;
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include "parser.h"
#include "stdlib.h"
#include "ctime"

/**
 * Testmodule om simpele tests uit te voeren op klassen,
 * later testen in afzonderlijke files steken.
 */
void test_smallbasicset() {
	cout << "# TESTS - SMALLBASICSET #" << endl;
	Parser p;
	SmallBasicSet s1 = p.parse("[1234]", 4);
	SmallBasicSet s2 = p.parse("[465]", 3);
	cout << s1.toString() << endl;
	cout << s2.toString() << endl;
	SmallBasicSet s3 = s1.setunion(s2);
	cout << s3.toString() << endl;
	SmallBasicSet s4 = s1.setdifference(s2);
	cout << s4.toString() << endl;
	SmallBasicSet s5 = s1.setintersect(s2);
	cout << s5.toString() << endl;

	cout << s1.hasAsSubset(s5) << endl; // expected: 1
	cout << s1.hasAsSubset(s3) << endl; // expected: 0
}

void test_amfunction() {
	cout << "# TESTS - AMFUNCTION #" << endl;
	Parser p;
	SmallBasicSet s1 = p.parse("[1]", 1);
	SmallBasicSet s2 = p.parse("[23]", 2);

	AMFunction a1;
	a1.addSet(s1);
	a1.addSet(s2);

	for ( SmallBasicSet s : a1.getSets() ) {
		cout << s.toString() << endl;
	}

	cout << "antimonotonic:" << a1.isAntiMonotonic() << endl; // expected: 1
}

void test_smallbasicsetint() {
    cout << "# TESTS - SMALLBASICSET OP INTEGERS#" << endl;
    int x = 2;
    int y = 3;
    cout << (x | y) << endl;
    // zou een error moeten geven omdat 65536 > s.maxintvalue
    //SmallBasicSet s = SmallBasicSet(65536);
    //cout << s.maxintvalue() << endl;
    
    
}

void test_smallbasicsetstress() {
    int aantaloperaties=100;
    SmallBasicSet s[100];
    cout << "# TESTS - STRESS TEST SMALL BASIC SETS #" << endl;
    cout << "-Start timing-" << endl;
    clock_t begin = clock();
    //maken van  small sets
    for (int i=1; i<=100; i++) {
        SmallBasicSet si = SmallBasicSet(i);
        s[i]= si;
    }
    //uitvoeren van  operaties op random sets
    for (int j = 0; j<aantaloperaties; j++) {
        int irand = rand() % 3 + 1;
        int irand2 = rand() % 100 + 0;
        int irand3 = rand() % 100 + 0;
        switch ( irand )
        {
            case 1:
                cout << s[irand2].toString() << " U " << s[irand3].toString() << " = " << (s[irand2].setunion(s[irand3])).toString() << endl;
                break;
            case 2:
                cout << s[irand2].toString() << " intersect " << s[irand3].toString() << " = " << (s[irand2].setintersect(s[irand3])).toString() << endl;
                ;
                break;
            //case 3:
            //    cout << s[irand2].toString() << " / " << s[irand3].toString() << " = " << (s[irand2].setdifference(s[irand3])).toString() << endl;
            //    break;
            case 3:
                cout << s[irand2].toString() << " subset " << s[irand3].toString() << " = " << (s[irand2].hasAsSubset(s[irand3])) << endl;
                break;
        }
    }

    clock_t end = clock();
    
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "-End timing-" << endl;
    cout << "# TESTS - ran in " << elapsed_secs << " sec #"<< endl;
}
/**
 * Testmodule om een beetje in te experimenteren
 */
void test_general() {
	// do some stuff
}

int main() {
	// run the tests
	test_smallbasicset();
	test_amfunction();
	test_general();
    //test_smallbasicsetint();
    test_smallbasicsetstress();
	return 0;
}
