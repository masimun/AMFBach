/*
 * AMFGraph.h
 *
 *  Created on: 21-dec.-2013
 *      Author: Daan
 */

#ifndef AMFGRAPH_H_
#define AMFGRAPH_H_

using namespace std;
#include <set>
#include <list>
#include "SmallBasicSet.h"
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
	virtual ~AMFGraph();

	long count_connected();
	edges_t* getEdges();
};

#endif /* AMFGRAPH_H_ */
