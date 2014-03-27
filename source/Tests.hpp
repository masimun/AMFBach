/*
 * Tests.hpp:
 *
 * Small lighweight testing framework
 *
 *  Created on: 9-dec.-2013
 *      Author: Daan Seyaeve
 */

#ifndef TESTS_HPP_
#define TESTS_HPP_

using namespace std;
#include <iostream>
#include <bitset>
#include <string>
#include "ctime"

#include "SmallBasicSet.h"
#include "AMFInterval.h"
#include "AMFunction.h"
#include "Parser.h"

namespace test {

	template<class T> inline void ASSERT_EQUAL(T,T);

	inline void ASSERT_EQUAL(string a, string b) {
		if (a != b) {
			cout << "TEST FAILED: EQUAL ASSERTION: (" << a << "," << b;
			cout << endl;
		} else {
			cout << "TEST SUCCESFUL" << endl;
		}
	}

	inline void ASSERT_EQUAL(AMFunction a, AMFunction b) {
		ASSERT_EQUAL(a.toString(), b.toString());
	}

}


#endif /* TESTS_HPP_ */
