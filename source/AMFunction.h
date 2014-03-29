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
    class AMFiterator : public iterator<forward_iterator_tag, AMFunction> {
		friend class AMFunction;
	public:
		SmallBasicSet* sbs;
		int current;
		SmallBasicSet* currentSet;

        
		AMFiterator(AMFunction* set);
		//const reference operator*() { return prev; }
		AMFiterator operator++();
		bool hasNext();
	};

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
	bool isAntiMonotonic();
	bool isEmpty() const;
	set<SmallBasicSet> getSets() const;
	string toString();
	bool contains(SmallBasicSet s);
	bool ge(SmallBasicSet s) const;
    long size();
    SmallBasicSet getUniverse();

	// alter
	void setSets(set<SmallBasicSet> ss);
	void addSet(SmallBasicSet s);
	void removeAll(AMFunction amf);
    void removeSets(list<SmallBasicSet> rs);
	void addSetConditional(SmallBasicSet s);
	void makeAntiMonotonic();
    

	// compare
	bool leq(AMFunction other) const;
	bool gt(AMFunction other) const;
	bool equals(AMFunction other) const;
	bool operator<(const AMFunction& other) const;
	bool operator==(const AMFunction& other) const;

	// operations
	AMFunction shallowclone();
	AMFunction join(AMFunction other) const;
	AMFunction meet(AMFunction other) const;
	AMFunction times(AMFunction other) const;
	AMFunction operator^(AMFunction other); // meet
	AMFunction operator+(AMFunction other); // join
	AMFunction map(int inverse[]);
	AMFunction omicron(AMFunction tau, AMFunction alfa);
    AMFunction project(SmallBasicSet sbs);
    vector<AMFunction> reduce(SmallBasicSet sbs);
    AMFunction minus(AMFunction f);

	// standard
	perm_t symmetry_group();
	AMFunction standard(perm_t permutations);
	AMFunction standard();
	AMFunction lexi_standard();
    
	// class
	static AMFunction emptyFunction();
	static AMFunction emptySetFunction();
	static AMFunction universeFunction(SmallBasicSet N);
	static AMFunction universeFunction(int n);
	static AMFunction singletonFunction(int l);
	static AMFunction immediate_subsets(SmallBasicSet s);

	// debug
	string bugstr;
    
};

#endif /* AMFUNCTION_H_ */
