/*
 * Parser.h
 *
 *  Created on: 21-nov.-2013
 *      Author: Daan
 */

#ifndef PARSER_H_
#define PARSER_H_
#include "SmallBasicSet.h"

class Parser {
public:
	Parser();
	virtual ~Parser();

	SmallBasicSet parse(string a, int size);
};

#endif /* PARSER_H_ */
