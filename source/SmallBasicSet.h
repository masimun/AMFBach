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
    static const int MAXINT = 10000; // sum(1 .. 4096) = 8191
    //constexpr wordt op compile time berekend en niet op runtime. constexpr static const
    int bits[13] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096};
	int numberofelem;
	uint_fast16_t set;
public:
	SmallBasicSet();
	SmallBasicSet(int a[], int asize);
    SmallBasicSet(uint_fast16_t a);
	virtual ~SmallBasicSet();

	// query
	int maxelement();
    int maxintvalue();
	int numberofelements();
	int maximum(); // TODO: implement
	int minimum(); // TODO: implement
	bool isemptyset();
	string toString();
	string toBitString();
	uint_fast16_t getSet() const;

	// class
	static SmallBasicSet universe();
	static SmallBasicSet universe(int n);

	// operation
	SmallBasicSet setunion(SmallBasicSet s) const;
	SmallBasicSet setintersect(SmallBasicSet s) const;
	SmallBasicSet setdifference(SmallBasicSet s) const;
	SmallBasicSet map(int table[]) const;

	// comparison
	bool equals(SmallBasicSet s) const;
	bool hasAsSubset(SmallBasicSet s) const;

	// template args
	bool operator<(const SmallBasicSet& other) const;

	bool operator==(const SmallBasicSet& other) const;
    
	// algo tools
	void quickadd(int bit); // add an integer a to the set
	int getNextInSet(int i); // TODO: implement, returns i's successor

    //getters
    int getBit(int positie);
    
    //setters

private:
    void setSet(bool elements[]);
    void setSet(int setvalues[],int asize);
};

#endif
