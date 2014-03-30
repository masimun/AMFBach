//
//  SolverMulti_tests.cpp
//  AMFBach
//
//  Created by Max Dekoninck on 27/03/14.
//  Copyright (c) 2014 Daan en Max. All rights reserved.
//


using namespace std;
#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <set>
#include <list>
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include "AMFInterval.h"
#include "AMFGraph.h"
#include "Solver.h"
#include <map>
#include <vector>
#include <unordered_set>

#define NUM_THREADS     2

struct thread_data{
    
    int thread_id;
    int amount = NUM_THREADS;
    AMFunction r2;
    map<AMFunction,long> functions;
    map<AMFunction, long long> right_interval_size;
    map<AMFunction, long long> left_interval_size;
    clock_t begin;
};


void* loop (void * threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    
    long REPORT = 20000;
    
    long long sum = 0L;
	long long evaluations = 0;
	long long possibilities = 0;
    
    AMFunction r2 = my_data->r2;
    long long r2size = my_data->right_interval_size.at(r2.standard());
    long long sumP = 0L;
    
    //here threading
    for (pair<AMFunction,long> r1pair : my_data->functions ) {
        possibilities++;
        AMFunction &r1 = r1pair.first;
        if (r1.leq(r2)) {
            sumP = sumP	+ ((r1pair.second) * (my_data->left_interval_size.at(r1)) * Solver::PatricksCoefficient(r1, r2));
            evaluations++;
            if ( evaluations % REPORT == 0 ) {
                cout << "partial sum: " << sum << " (" << evaluations << " evaluations)" << endl;
            }
        }
    }
    sum = sum + (sumP * r2size);
    
    clock_t end_algo = clock();
	cout << "finished: " << evaluations << " evaluations" << endl;
	cout << "@ " << (double) (end_algo - my_data->begin) / (CLOCKS_PER_SEC / 1000) << " msec" << endl;
    
	cout << "Result = "  << sum << endl;
    
    pthread_exit(NULL);
}


/**
 * Algorithm for calculating the m'th dedekind number
 * using 2nd degree p-coefficients
 */
void pc2_dedekind_multi(int m) {
    
    
    int n = m -2;
	
    
	clock_t begin = clock();
	cout << "started generating equivalence classes" << endl;
    
	// generate
	vector<map<AMFunction,long>*>* classes = Solver::algorithm9(n);
    map<AMFunction,long> functions;
    map<AMFunction,long> temp;
    
	clock_t end_classes = clock();
	cout << "finished generating equivalence classes" << endl;
	cout << "@ " << (double) (end_classes - begin) / (CLOCKS_PER_SEC / 1000) << " msec" << endl;
    
    
    int rest = ((int)classes->capacity()) % NUM_THREADS;
    int split = ( (int) classes->capacity())/NUM_THREADS;
    int t = 0;
    vector<map<AMFunction,long>> store (NUM_THREADS);

	// collect
    for (int i = 0; i < (int) classes->capacity() ; i++ ) {
 		long coeff = Solver::combinations(n, i);
		for( pair<AMFunction,long> p : *classes->at(i)) {
            if(!(i<split+rest)){
                store[t] = temp;
                t = t+1;
                temp.clear();
            }
            Solver::mapstore(temp, p.first, p.second*coeff);
			Solver::mapstore(functions, p.first, p.second*coeff);
		}
	}
    //	clock_t end_collect = clock();
    //	cout << "finished collecting equivalence classes" << endl;
    //	cout << "@ " << (double) (end_collect - begin) / (CLOCKS_PER_SEC / 1000) << " msec" << endl;
	cout << "Amount of representatives:" << functions.size() << endl;
    
	AMFunction e = AMFunction::empty_function();
	AMFunction u = AMFunction::universe_function(n);
	map<AMFunction, long long> left_interval_size;
	map<AMFunction, long long> right_interval_size;
	for( pair<AMFunction,long> fpair : functions ) {
		AMFunction &f = fpair.first;
		AMFInterval left = AMFInterval(e,f);
		AMFInterval right = AMFInterval(f,u);
		left_interval_size.insert(make_pair(f,left.lattice_size()));
		right_interval_size.insert(make_pair(f,right.lattice_size()));
	}
    
	clock_t end_isizes = clock();
	cout << "finished generating interval sizes" << endl;
	cout << "@ " << (double) (end_isizes - begin) / (CLOCKS_PER_SEC / 1000) << " msec" << endl;
    
    //	cout << "Test: interval sizes for n = " << n << endl;
    //	cout << "---------------------------------------------" << endl;
    //	for ( pair<AMFunction,long> fpair : functions ) {
    //		AMFunction& a = fpair.first;
    //		long l = left_interval_size.find(a)->second;
    //		long r = right_interval_size.find(a)->second;
    //		cout << a.toString() << "\t\tN:" << fpair.second << "\t\tL:" << l << "\t\tR:" << r << endl;
    //	}
    //	cout << "---------------------------------------------" << endl;
    
	//return 0; //STOP
    


    
    
	AMFInterval::GeneralFastIterator& it2 = *(AMFInterval(e,u).getFastIterator());
	while(it2.hasNext()) {
		++it2;

        pthread_t threads[NUM_THREADS];
        struct thread_data td[NUM_THREADS];
        int rc;
        int i;
        for(i=1; i <= NUM_THREADS; i++ ){
            cout << "main() : creating thread, " << i << endl;
            
            td[i].thread_id = i;
            td[i].functions = store[i-1];
            td[i].begin = begin;
            td[i].left_interval_size = left_interval_size;
            td[i].right_interval_size = right_interval_size;
            td[i].r2 = *it2;
            td[i].amount = NUM_THREADS;
            rc = pthread_create(&threads[i], NULL, loop, (void *)&td[i]);
            //rc = pthread_create(&threads[i], NULL, (long long *) pc2_dedekind_multi, (void *)&td[i]);
            if (rc){
                cout << "Error:unable to create thread," << rc << endl;
                exit(-1);
            }
        }
        pthread_exit(NULL);
        
        
	}
    

    
    
    
}


//int main(){
//    pc2_dedekind_multi(4+2);
//	return 0;
//}

