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

/**
 * Deze klasse is bedoeld om sets van integers voor te stellen.
 */
class SmallBasicSet {
	static const int MAXELEMENT = 13;
    static const int MAXSET = 10000; // sum(1 .. 4096) = 8191
    constexpr static int bits[13] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096};
	int numberofelem;
	uint_fast16_t set;
	friend std::ostream& operator<<(std::ostream &strm, const SmallBasicSet &s);

public:

	SmallBasicSet();
	SmallBasicSet(int a[], int asize);
    SmallBasicSet(uint_fast16_t a);
	virtual ~SmallBasicSet();

	// iterator
	class SBSIterator : public iterator<forward_iterator_tag, int> {
		friend class SmallBasicSet;
	public:
		SmallBasicSet* sbs;
		int current;
		int prev;
		long bit;

		SBSIterator(SmallBasicSet* set);
		const reference operator*() { return prev; }
		SBSIterator operator++();
		bool hasNext();
	};
	typedef SBSIterator iterator;
	iterator getIterator() { return iterator(this); };

	// hasher
	struct SBSHasher {
		size_t operator() ( const SmallBasicSet & sbs ) const;
	};
	typedef SBSHasher hasher;

	// query
	int maxelement();
    int maxintvalue();
	int numberofelements() const;
	int maximum();
	int minimum();
	bool isemptyset();
	string toString() const;
	string toBitString();
	uint_fast16_t getSet() const;
    int size() const;

	// class
	static SmallBasicSet universe();
	static SmallBasicSet universe(int n);

	// operation
	SmallBasicSet setunion(SmallBasicSet s) const;
	SmallBasicSet setintersect(SmallBasicSet s) const;
	SmallBasicSet setdifference(SmallBasicSet s) const;
    SmallBasicSet difference(int other) const;
	SmallBasicSet operator/(const SmallBasicSet& other) const;
	SmallBasicSet map(int table[]) const;

	// comparison
	bool equals(SmallBasicSet s) const;
	bool hasAsSubset(SmallBasicSet s) const;
	bool operator<(const SmallBasicSet& other) const;
	bool operator==(const SmallBasicSet& other) const;
    
	// miscellaneous
	void quickadd(int bit); // add a number a to the set
	int getNextInSet(int i); // TODO: implement, returns i's successor (still needed?)
    int getBit(int positie) const;

private:
    void setSet(bool elements[]);
    void setSet(int setvalues[],int asize);
};

#endif
