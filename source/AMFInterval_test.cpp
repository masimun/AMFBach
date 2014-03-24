
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

void test_fastiterator(){
    Parser p;
    AMFInterval amf(p.parse_amf(""),p.parse_amf("{[12345]}"));
    long i = 0;
    AMFInterval::GeneralFastIterator& amfit = (*(amf.getFastIterator()));
    while(amfit.hasNext()) {
        i++;
        (++amfit);
        //AMFunction a = (*amfit);
        //cout  <<i << " \t " << a.toString() << endl;
        
    }
    cout << i << endl;
}


void test_iterator() {
    Parser p;
    AMFInterval amf(p.parse_amf(""),p.parse_amf("{[12345]}"));
    //AMFInterval amf(p.parse_amf("{[12][23]}"),p.parse_amf("{[123]}"));
    long i = 0;
    AMFInterval::GeneralIterator& amfit = (*(amf.getIterator()));
    //for( AMFInterval::GeneralIterator& amfit = (*(amf.getIterator()));amfit.hasNext();++amfit ) {
    while(amfit.hasNext()) {
        i++;
        (++amfit);
        AMFunction a = (*amfit);
        //cout  <<i << " \t " << a.toString() << endl;
    }
    cout << i << endl;
    

    
}

void test_bestSubSet() {
    
    
    
    Parser p;
    //SmallBasicSet span = *new SmallBasicSet(12);
    //AMFunction bottom = p.parse_amf("{[]}");
    ///long spanSize = 1;
    AMFInterval amf(p.parse_amf("{[]}"),p.parse_amf("{[12]}"));
    //SmallBasicSet best = *amf.bestSubset(span, spanSize, bottom);
    vector<SmallBasicSet> split (2);
    
    split =  amf.bestSplit();
    cout << split[0].toString() << endl;
    cout << split[1].toString() << endl;
    
    // Raar dat er hier geen waardes in komen
    

    
    
    AMFInterval* Xaxis = new AMFInterval((amf.getBottom()).project(split[0]),(amf.getTop()).project(split[0]),true,true);
    AMFInterval* Yaxis = new AMFInterval((amf.getBottom()).project(split[1]),(amf.getTop()).project(split[1]),true,true);
    
    cout << Xaxis->toString() << endl;
    cout << Yaxis->toString() << endl;
    AMFInterval::GeneralIterator& X = *(Xaxis->getIterator());
    AMFInterval::GeneralIterator& Y= *(Yaxis->getIterator());
    
    
    long i=0;
    //for( AMFInterval::GeneralIterator& amfit = (*(amf.getIterator()));amfit.hasNext();++amfit ) {
    while(X.hasNext()) {
        i++;
        (++X);
        AMFunction a = (*X);
        cout  << "first \t "<<i << " \t " << a.toString() << endl;
    }
    
    //for( AMFInterval::GeneralIterator& amfit = (*(amf.getIterator()));amfit.hasNext();++amfit ) {
    while(Y.hasNext()) {
        i++;
        (++Y);
        AMFunction a = (*Y);
        cout << "second \t " <<i << " \t " << a.toString() << endl;
    }
}


void test_split() {
    
}

void test_graph() {
    
}


//int main() {


    
  //  const clock_t begin_time = clock();
    // do something
    //test_iterator();
   // test_fastiterator();
    //test_bestSubSet();
   // std::cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    //test_bestSubSet();
    //cout << SmallBasicSet(3).size();
   // return 0;
//}

