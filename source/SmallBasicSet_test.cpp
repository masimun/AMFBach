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
#include <bitset>
#include "Tests.hpp"

void test_smallbasicset();
void test_smallbasicset_map();
void test_smallbasicsetint();
double speed_test_smallbasicsetstress();
void test_speed();
/*
int main() {
	Parser p;
	SmallBasicSet s1 = p.parse("[379]", 3);
	SmallBasicSet::iterator it = s1.getIterator();
	while (it.hasNext()) {
		++it;
		cout << *it << endl;
	}
	return 0;
}
*/
void test_smallbasicset_map() {
	Parser p;
	int inv1[] = {0,0,1,2,3,4};
	int inv2[] = {0,4,3,2,1,0};
	int inv3[] = {0,1,2,3,4,0};
	SmallBasicSet s = p.parse("[523]",3);
	cout << s.map(inv1) << endl;
	cout << s.map(inv2) << endl;
	cout << s.map(inv3) << endl;
	SmallBasicSet u = p.parse("[14]",2);
	SmallBasicSet v = p.parse("[135]",3);
	AMFunction a = p.parse_amf("{[14],[523]}");
	test::ASSERT_EQUAL(a.map(inv2).toString(),(string)"{431}-{52}-");
}


/**
 * Testmodule om simpele tests uit te voeren op klassen,
 * later testen in afzonderlijke files steken.
 */
void test_smallbasicset() {
	cout << "# C++ FUNC TESTS - SMALLBASICSET #" << endl;
	Parser p;
	SmallBasicSet s1 = p.parse("[1234]", 4);
	SmallBasicSet s2 = p.parse("[465]", 3);
	cout << s1 << endl;
	cout << s2 << endl;

	cout << "// --- operations test" << endl;
	SmallBasicSet s3 = s1.setunion(s2);
	cout << s3 << endl;
	SmallBasicSet s4 = s1.setdifference(s2);
	cout << s4 << endl;
	cout << s2 / s1 << endl;
	SmallBasicSet s5 = s1.setintersect(s2);
	cout << s5 << endl;

	cout << "// --- subset test" << endl;

	cout << s1.hasAsSubset(s5) << endl; // expected: 1
	cout << s1.hasAsSubset(s3) << endl; // expected: 0

	cout << "// --- bitwise output test" << endl;
	cout << s1.toBitString() << endl;

	cout << "// --- count/max/min tests" << endl;
	cout << s1.numberofelements() << endl; // expected: 4
	cout << (s2 / s1).numberofelements() << endl; // expected: 2
	cout << p.parse("[974]",3).minimum() << endl; // expected: 4
	cout << p.parse("[14286]",5).maximum() << endl; // expected: 8
    cout << p.parse("[123]",3).maximum() << endl; // expected: 3
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
    cout << "# C++ STRESS TESTS - TEST SMALL BASIC SETS #" << endl;
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
    
    

double speed_test_smallbasicsetstress() {
    int aantaloperaties=100000;
    SmallBasicSet s[500];
    cout << "# C++ STRESS TESTS - TEST SMALL BASIC SETS #" << endl;
    cout << "-Start timing-" << endl;
    clock_t begin = clock();
    //maken van  small sets
    for (int i=1; i<=500; i++) {
        SmallBasicSet si = SmallBasicSet(i);
        s[i]= si;
    }
    //uitvoeren van  operaties op random sets
    for (int j = 0; j<aantaloperaties; j++) {
        int irand = rand() % 3 + 1;
        int irand2 = rand() % 500 + 0;
        int irand3 = rand() % 500 + 0;
        switch ( irand )
        {
            case 1:
                s[irand2].setunion(s[irand3]);
                break;
            case 2:
                s[irand2].setintersect(s[irand3]);
                break;
            //case 3:
            //    cout << s[irand2].toString() << " / " << s[irand3].toString() << " = " << (s[irand2].setdifference(s[irand3])).toString() << endl;
            //    break;
            case 3:
                s[irand2].hasAsSubset(s[irand3]);
                break;
        }
    }

    clock_t end = clock();
    
    double elapsed_secs = double (end - begin) / (CLOCKS_PER_SEC / 1000);
    cout << "-End timing-" << endl;
    cout << "# TESTS - ran in " << elapsed_secs << " msec #"<< endl;
    return elapsed_secs;
}
/**
 * Testmodule om een beetje in te experimenteren
 */
void test_speed() {
    
    double gem = 0;
    for(int i = 300 ; i > 0 ; i--){
        gem = gem + speed_test_smallbasicsetstress();
    }
    cout << "gem runtime is: " << gem/300 << "ms" << endl;
    
}
