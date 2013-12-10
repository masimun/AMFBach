/*
 * Parser.h
 *
 *  Created on: 21-nov.-2013
 *      Author: Daan
 */

#ifndef PARSER_H_
#define PARSER_H_
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include "AMFInterval.h"

class Parser {
public:
	Parser();
	virtual ~Parser();

	SmallBasicSet parse(string a, int size);
	AMFunction parse_amf(string a);
	AMFInterval parse_interval(string a, string b);
};

#endif /* PARSER_H_ */
