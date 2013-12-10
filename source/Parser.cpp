/*
 * Parser.cpp
 *
 *  Created on: 21-nov.-2013
 *      Author: Daan
 */

#include "Parser.h"
#include <iostream>
using namespace std;

Parser::Parser() {

}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}

SmallBasicSet Parser::parse(string a, int size) {
	int b[size];
	for (int i = 0 ; i < size ; i++ ) {
		b[i] = a[i+1] - '0';
	}
	SmallBasicSet s(b, size);
	return s;
}

AMFunction Parser::parse_amf(string a) {
	int size = a.size();
	AMFunction amf;
	bool sbs = false;
	SmallBasicSet s;
	for (int i = 0 ; i < size ; i++) {
		if (sbs && a[i] != ']' && a[i] != ',') {
			s.quickadd(a[i] - '0');
		} else if ( a[i] == '[' ) {
			sbs = true;
			s = SmallBasicSet();
		} else if ( a[i] == ']' ) {
			sbs = false;
			amf.addSet(s);
		}
	}
	return amf;
}
