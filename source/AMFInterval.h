/*
 * AMFInterval.h
 *
 *  Created on: 2-dec.-2013
 *      Author: Daan
 */

#ifndef AMFINTERVAL_H_
#define AMFINTERVAL_H_

using namespace std;
#include <string>
#include "SmallBasicSet.h"
#include "SetsPoset.h"
#include "AMFunction.h"
#include "AMFGraph.h"
#include <utility>
#include <stdlib.h>

/**
 * A (closed) interval of anti-monotonic functions.
 */
class AMFInterval {
public:
    typedef iterator<forward_iterator_tag, AMFunction> iterator_amf;
    
    class GeneralIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        
        bool virtual hasNext() = 0;
        virtual const reference operator*()= 0;
        virtual iterator<forward_iterator_tag, AMFunction> operator++() = 0;
    };
    
    class GeneralFastIterator : public iterator<forward_iterator_tag, AMFunction> {
        friend class AMFInterval;
    public:
        
        bool virtual hasNext() = 0;
        virtual const reference operator*()= 0;
        virtual iterator<forward_iterator_tag, AMFunction> operator++() = 0;
        virtual ~GeneralFastIterator();
    };
    
    class AMFClosedIterator : public GeneralIterator{
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        vector<SmallBasicSet> axes;
        GeneralIterator* X;
        GeneralIterator* Y;
        AMFInterval* Xaxis;
        AMFInterval* Yaxis;
        AMFunction currentX;
        AMFunction currentY;
        GeneralIterator* currentIterator;
        
        AMFClosedIterator(AMFInterval* intr);
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();

    };
    
    class AMFGeneralClosedIterator : public GeneralIterator {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        long spanSize;
        int pos;
        long minSizeBottom;
        vector<AMFunction> theList;
        SmallBasicSet span;
        
        AMFGeneralClosedIterator(AMFInterval* intr);
        AMFunction getCurrent() {return current;}
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();

    };
    
    class AMFIterator : public GeneralIterator {
        friend class AMFInterval;
    public:
        AMFInterval* intr;
        AMFunction current;
        bool thereIsNext;
        GeneralIterator* theIt;
        AMFunction nxt;
        
        AMFIterator(AMFInterval* intr);
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();


    };
    
    class AMFEmptyIterator : public GeneralIterator {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        
        AMFEmptyIterator(AMFInterval* intr);
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();

    };
    
    class AMFOneOrTwoIterator : public GeneralIterator {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        int pos, last;
        vector<AMFunction> theList;
        SmallBasicSet span;
        
        AMFOneOrTwoIterator(AMFInterval* intr);
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();

    };
    
    class AMFOneElementIterator : public GeneralIterator {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        bool given = false;
        
        AMFOneElementIterator(AMFInterval* intr);
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();

    };
    
    
    
    class AMFExceptionalClosedIterator : public GeneralIterator {
        friend class AMFInterval;
    public:
        AMFunction current;
        AMFunction bottom;
        bool virgin;
        AMFInterval* interval;
        GeneralIterator* normal;
        

        
        AMFExceptionalClosedIterator(AMFInterval* intr);
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();

    };
    
    
    class AMFFastNonEmptyIterator : public GeneralFastIterator {
        friend class AMFInterval;
    public:
        AMFInterval* interval;
        AMFunction current;
        SmallBasicSet span;
        AMFunction maxSpan;
        bool isFinished = false;
        AMFunction ret;
        GeneralFastIterator* itr0;
        GeneralFastIterator* itr1;
        AMFunction alfa0;
        AMFunction alfa1;
        
        vector<AMFunction> alfaBottom;
        vector<AMFunction> alfaTop;
        
        
        
        AMFFastNonEmptyIterator(AMFInterval* intr);
        const reference operator*() {return ret;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        AMFunction nextCurrent();
        bool hasNext();
        virtual ~AMFFastNonEmptyIterator();
        
    };
    
    class AMFFastEmptySpanIterator : public GeneralFastIterator {
        friend class AMFInterval;
    public:
        AMFunction current;
        AMFunction nxt;
        AMFInterval* interval;
        bool isFinished = false;
        
        AMFFastEmptySpanIterator(AMFInterval* intr);
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();
        virtual ~AMFFastEmptySpanIterator();
    };
    
    class AMFFastEmptyIterator : public GeneralFastIterator {
        friend class AMFInterval;
    public:
        AMFunction current;
        
        AMFFastEmptyIterator();
        const reference operator*() {return current;};
        iterator<forward_iterator_tag, AMFunction> operator++();
        bool hasNext();
        virtual ~AMFFastEmptyIterator();
    };


private:
	AMFunction from;
	AMFunction till;
    bool closedAtTop;
    bool closedAtBottom;

public:
	AMFInterval(AMFunction bottom, AMFunction top);
    AMFInterval(AMFunction bottom, AMFunction top, bool closedTop, bool closedBot);

	virtual ~AMFInterval();

    //iterator
    GeneralIterator *getIterator();
    GeneralIterator *getClosedIterator();
    GeneralFastIterator *getFastIterator();

    
    //methode
    vector<SmallBasicSet> bestSplit();
    SmallBasicSet bestSubset(SmallBasicSet span, long target,AMFunction bottom);
    
    //graph
    AMFGraph graph();
    long long lattice_size();
    
    //getters
    AMFunction getTop();
    AMFunction getBottom();
    SmallBasicSet getUniverse();
    bool isClosedAtBottom();
    bool isClosedAtTop();
    
	// query
	string toString();

	// class
	static AMFInterval fullspace(int n);
};

#endif /* AMFINTERVAL_H_ */
