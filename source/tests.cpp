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

/**
 * Testmodule om simpele tests uit te voeren op klassen,
 * later testen in afzonderlijke files steken.
 */
void test_smallbasicset() {
	cout << "# C++ FUNC TESTS - SMALLBASICSET #" << endl;
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

	cout << "// --- bitwise output test" << endl;
	cout << s1.toBitString() << endl;
	int r = 7;
	cout << std::bitset<16>(r << 1).to_string() <<endl;
	cout << std::bitset<16>(0x1).to_string() << endl;
	cout << std::bitset<16>((1 << 13) - 1) << endl;
}

void test_amfunction() {
	cout << "# C++ FUNC TESTS - AMFUNCTION #" << endl;
	Parser p;
	SmallBasicSet s1 = p.parse("[3]", 1);
	SmallBasicSet s2 = p.parse("[13]", 2);
	SmallBasicSet s3 = p.parse("[2]", 1);

	cout << s1.toString() << endl;
	cout << s2.toString() << endl;
	cout << s3.toString() << endl;

	AMFunction a1;
	a1.addSetConditional(s1);
	a1.addSetConditional(s2);
	a1.addSetConditional(s3);

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

int main() {
	// run the tests
	// test_smallbasicset();
	test_amfunction();
	//test_general();
    //test_smallbasicsetint();
    //test_smallbasicsetstress();
    //test_speed();
	return 0;
}
