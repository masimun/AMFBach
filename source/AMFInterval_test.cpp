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
#include <array>
#include "AMFInterval.h"

void test_amfinterval() {
	cout << (AMFInterval::fullspace(4)).toString() << endl;
}


void test_iterator() {
    Parser p;
    AMFInterval amf(p.parse_amf("{}"),p.parse_amf("{[123]}"));
    AMFInterval::AMFIterator freddy = amf.begin();
    cout << freddy.amf.toString() << endl;
    //AMFInterval::AMFIterator f2 = freddy++;
    
<<<<<<< HEAD
=======
    ++freddy;

    // AMFInterval::AMFIterator f2 = freddy++;
>>>>>>> 816033ce8a9e10f2e1abfd7a37b35fa2c69e5b57

    vector<AMFInterval::AMFIterator> c;
    AMFInterval::AMFIterator * piterator = c.data();
    
    //c.push_back(freddy);
    piterator[0] = freddy;
    
    cout << piterator[0].amf.toString();
    
    //c.insert(freddy);
    //c.insert(freddy++);
        
    //AMFInterval::AMFIterator* t = v.data();
    
    //cout << f2.amf.toString() <<endl;

    
    
    //for (AMFInterval::AMFIterator freddy = amf.begin(); freddy.hasNext(); freddy++) {
    //    AMFunction b = (freddy.amf);
    //    cout << (b.toString()) << endl;
    //}
    //cout << p.parse_amf("{[1,2,3]}").toString() << endl;
    //AMFunction b = (freddy.amf);
    //cout << (b.toString()) << endl;
    ///AMFInterval::AMFIterator f2 = freddy++;
    //cout << f2.amf.toString();
    
    //cout <<  b.toString();
    //cout << (*amf.end()).toString() << endl;
    
}

int main() {
    
    //set<int> *s = new set<int>;
    //(*s).insert(4);
    //(*s).insert(6);
    
    //cout << *(++(*s).begin());
    
    //vector<int> vec (5);
    //int* p = vec.data();
    //p[0] = 10;
    //p[1] = 2;
    
    //cout << vec[1];
    
    test_iterator();
    return 0;
}
