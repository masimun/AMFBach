/*
 * SmallBasicSet.h
 *
 *  Created on: 20-nov.-2013
 *      Author: Daan Seynaeve, Max Dekonick
 */
#ifndef SMALLBASICSET_H_
#define SMALLBASICSET_H_

using namespace std;
#include <string>
#include <sstream>
#include <cstdint>
#include <vector>

/**
 * Deze klasse is bedoeld om sets van integers voor te stellen.
 */
class SmallBasicSet {
	static const int MAXELEMENT = 13;
    static const int MAXSET = 10000; // sum(1 .. 4096) = 8191
    static const int bits[13];
	uint_fast16_t set;
	friend std::ostream& operator<<(std::ostream &strm, const SmallBasicSet &s);

public:

	SmallBasicSet();
	SmallBasicSet(int a[], int asize);
    SmallBasicSet(uint_fast16_t a);
	virtual ~SmallBasicSet();

	// iterator
	class SBSIterator {
		friend class SmallBasicSet;
	public:
		const SmallBasicSet* sbs;
		int current;
		int prev;
		long bit;

		SBSIterator(const SmallBasicSet* const set);
		const int operator*() { return prev; }
		SBSIterator operator++();
		bool hasNext();
	};
	typedef SBSIterator iterator;
	iterator getIterator() const { return SBSIterator(this); };

	// hasher
	struct SBSHasher {
		size_t operator() ( const SmallBasicSet & sbs ) const;
	};
	typedef SBSHasher hasher;

	// query
	int maxelement() const;
    int maxintvalue() const;
	int numberofelements() const;
	int maximum() const;
	int minimum() const;
	bool isemptyset() const;
	string toString() const;
	string toBitString() const;
	uint_fast16_t getSet() const;
    int size() const;

	// class
	static SmallBasicSet universe();
	static SmallBasicSet universe(int n);

	// operation
	SmallBasicSet setunion(const SmallBasicSet & s) const;
	SmallBasicSet setintersect(const SmallBasicSet & s) const;
	SmallBasicSet setdifference(const SmallBasicSet & s) const;
    SmallBasicSet difference(int other) const;
	SmallBasicSet operator/(const SmallBasicSet& other) const;
	SmallBasicSet map(int table[]) const;
	SmallBasicSet minmap(vector<int> & map, int* & next) const;

	// comparison
	bool equals(SmallBasicSet s) const;
	bool hasAsSubset(SmallBasicSet s) const;
	bool operator<(const SmallBasicSet& other) const;
	bool operator==(const SmallBasicSet& other) const;
    
	// miscellaneous
	void quickadd(int bit); // add a number a to the set
	int getNextInSet(int i); // TODO: implement, returns i's successor (still needed?)
    static int get_bit(int pos);

private:
    void setSet(bool elements[]);
    void setSet(int setvalues[],int asize);
};

#endif
