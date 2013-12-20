/*
 * Toolkit.h
 *
 *  Created on: 20-dec.-2013
 *      Author: Daan
 */

#ifndef TOOLKIT_HPP_
#define TOOLKIT_HPP_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct lex_compare {
	bool operator() (const vector<int>& lhs, const vector<int>& rhs) const {
		stringstream s1,s2;
		for ( int a : lhs ) { s1 << a; }
		for ( int b : rhs ) { s2 << b; }
		return s1.str() < s2.str();
	}
};

/* set of permutations (int vectors)*/
typedef set<vector<int>,lex_compare> perm_t;

#endif /* TOOLKIT_H_ */
