/*
 * AMFGraph.h
 *
 *  Created on: 21-dec.-2013
 *      Author: Daan, Max
 */

#ifndef AMFGRAPH_H_
#define AMFGRAPH_H_

using namespace std;
#include <set>
#include <list>
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include <iostream>
#include <utility>
#include <unordered_set>
#include <vector>
#include "Toolkit.hpp"

class AMFGraph {
private:
	edges_t edges;
public:
    AMFGraph();
	//AMFGraph(AMFunction r1,AMFunction r2);
    AMFGraph(edges_t);
    
	virtual ~AMFGraph();

    
	long count_connected();
    static long countConnected(edges_t g);
    AMFGraph transitiveClosure();
    static void doNode(edges_t* g, SmallBasicSet* n, set<SmallBasicSet>* had);
	edges_t* getEdges();
};

#endif /* AMFGRAPH_H_ */
