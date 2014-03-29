//
//  Solver_Multi.h
//  AMFBach
//
//  Created by Max Dekoninck on 27/03/14.
//  Copyright (c) 2014 Daan en Max. All rights reserved.
//

#ifndef __AMFBach__Solver_Multi__
#define __AMFBach__Solver_Multi__

#include <iostream>

#include <set>
#include <list>
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include "AMFInterval.h"
#include "AMFGraph.h"
#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
using namespace std;

class Solver_Multi {
public:
	Solver_Multi();
	virtual ~Solver_Multi();
    
    
	// ALGORITHMS
	static long long * pc2_dedekind_multi(int* pid);
    
    
};
#endif /* defined(__AMFBach__Solver_Multi__) */
