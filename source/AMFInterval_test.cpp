/*
 * AMFInterval_test.cpp
 *
 *  Created on: 2-dec.-2013
 *      Author: Daan
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
#include "AMFInterval.h"

void test_amfinterval() {
	cout << (AMFInterval::fullspace(4)).toString() << endl;
}


void test_iterator() {
    Parser p;
    AMFInterval amf(p.parse_amf("{[1]}"), p.parse_amf("{}"));
    AMFInterval::AMFIterator freddy = amf.begin();
    //cout << p.parse_amf("{[1,2,3]}").toString() << endl;
    AMFunction b = (freddy.amf);
    cout << (b.toString()) << endl;
    AMFInterval::AMFIterator f2 = freddy++;
    cout << f2.amf.toString();
    
    //cout <<  b.toString();
    //cout << (*amf.end()).toString() << endl;
    
}

int main() {
    int[3] ret = ret[3];
    ret[0] = 5;
    ret[1] = 6;
    cout << ret[1] << endl;
    
    //test_iterator();
    return 0;
}