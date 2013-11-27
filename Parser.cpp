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
