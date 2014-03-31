/*
 * AMFunction.h
 *
 *  Created on: 25-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 */
#ifndef AMFUNCTION_H_
#define AMFUNCTION_H_

using namespace std;
#include <set>
#include <list>
#include "SmallBasicSet.h"
#include <iostream>
#include <utility>
#include <unordered_set>
#include <vector>
#include "MappingPermutator.h"

/**
 * This class represents an anti-monotonic function.
 *
 * Invariant: isAntiMonotonic()
 */
class AMFunction {
private:
	SmallBasicSet universe; // set of integers on which the function operates
	set<SmallBasicSet> sets;
public:
	struct AMFHasher {
		size_t operator() (const AMFunction & amf) const;
	};
	typedef AMFHasher hasher;

    struct lex_compare {
    	bool operator() (const vector<int>& lhs, const vector<int>& rhs) const {
    		stringstream s1,s2;
    		for ( int a : lhs ) { s1 << a; }
    		for ( int b : rhs ) { s2 << b; }
    		return s1.str() < s2.str();
    	}
    };
    typedef set<vector<int>,lex_compare> perm_t;

    struct pt_less {
    	bool operator() (const SmallBasicSet* const lhs, const SmallBasicSet* const rhs) const {
    		return (*lhs) < (*rhs);
    	}
    };

	AMFunction();
	AMFunction(SmallBasicSet N);
	AMFunction(SmallBasicSet s[], int size);
	virtual ~AMFunction();

	// query
	SmallBasicSet span() const;
	bool isAntiMonotonic() const;
	bool isEmpty() const;
	set<SmallBasicSet> getSets() const;
	string toString();
	bool contains(const SmallBasicSet & s) const;
    long size() const;
    SmallBasicSet getUniverse();

	// alter
	void setSets(set<SmallBasicSet> ss);
	void addSet(const SmallBasicSet & s);
	void removeAll(AMFunction amf);
    void removeSets(list<SmallBasicSet> rs);
	void addSetConditional(const SmallBasicSet & s);
	void makeAntiMonotonic();

	// compare
	bool leq(const AMFunction & other) const;
	bool gt(const AMFunction & other) const;
	bool ge(const SmallBasicSet & s) const;
	bool equals(const AMFunction & other) const;
	bool operator<(const AMFunction& other) const;
	bool operator==(const AMFunction& other) const;

	// operations
	AMFunction shallowclone();
	AMFunction join(const AMFunction & other) const;
	AMFunction meet(const AMFunction & other) const;
	AMFunction times(const AMFunction & other) const;
	AMFunction operator^(const AMFunction & other) const; // meet
	AMFunction operator+(const AMFunction & other) const; // join
	AMFunction map(int inverse[]) const;
	AMFunction omicron(const AMFunction & tau, const AMFunction & alfa) const;
    AMFunction project(const SmallBasicSet & sbs) const;
    vector<AMFunction> reduce(const SmallBasicSet & sbs) const;
    AMFunction minus(const AMFunction & f) const;

	// standard
	perm_t symmetry_group() const;
	AMFunction standard(const perm_t & permutations) const;
	AMFunction standard() const;
	AMFunction lexi_standard() const;
	AMFunction lexi_standard2() const;
    
	// class
	static AMFunction empty_function();
	static AMFunction empty_set_function();
	static AMFunction universe_function(const SmallBasicSet & N);
	static AMFunction universe_function(int n);
	static AMFunction singleton_function(int l);
	static AMFunction immediate_subsets(const SmallBasicSet & s);

};

#endif /* AMFUNCTION_H_ */
