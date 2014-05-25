/*
 * Solver.cpp
 *
 *  Created on: 3-dec.-2013
 *      Author: Daan, Max
 */

#include "Solver.h"
#define MASTER 0

Solver::Solver() {
	// TODO Auto-generated constructor stub
}

Solver::~Solver() {
	// TODO Auto-generated destructor stub
}

/**
 * Increase the coefficient of A in M by i
 */
void Solver::mapstore(map<AMFunction, long>& M, AMFunction A, long i) {
	map<AMFunction, long>::iterator it = M.find(A);
	if (it != M.end()) {
		(*it).second += i;
	} else {
		M.insert(make_pair(A,i));
	}
}

void Solver::mapstore(map<AMFunction, bignum>& M, AMFunction A, bignum i) {
	map<AMFunction, bignum>::iterator it = M.find(A);
	if (it != M.end()) {
		(*it).second += i;
	} else {
		M.insert(make_pair(A,i));
	}
}

/**
 * Increase the coefficient of A in M by 1
 */
void Solver::mapstore(map<AMFunction, long>& M, AMFunction A) {
	mapstore(M,A,1L);
}

/**
 * ( n | i ) = n! / i!(n-i)!
 */
long Solver::combinations(int n, int i) {
	if ( n < i ) { return 0; }
	long res = 1;
	int t = n - i;
	while (n > t) { res *= n; n--; }
	while (i > 0) { res /= i; i--; }
	return res;
}


/**
 * Algorithm for calculating the m'th dedekind number
 * using 2nd degree p-coefficients
 */
void Solver::pc2_dedekind(int m) {
	int n = m - 2;
	long REPORT = 20000;
    
    int size;
    int rank;
    char name[BUFSIZ];
    int res;
    cout.precision(25);
    
    clock_t begin = clock();
    /**
     * Initialize MPI
     */
    MPI_Init(NULL, NULL);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    MPI_Get_processor_name(name, &res);
    
	
	
    
    cout << name << " " << rank <<": has started generating equivalence classes" << endl;
    
    
	// generate
	vector<map<AMFunction,long>*>* classes = algorithm9(n);
	map<AMFunction,long> functions;
    
    
	clock_t end_classes = clock();
	cout << name << " " << rank << ": is finished generating equivalence classes" << endl;
	cout << name << " " << rank << ": @ " << (long double) (end_classes - begin) / (CLOCKS_PER_SEC) << " sec" << endl;
    
    
	// collect
	for (int i = 0; i < (int) classes->capacity() ; i++ ) {
		long coeff = combinations(n, i);
		for( pair<AMFunction,long> p : *classes->at(i)) {
			mapstore(functions, p.first, p.second*coeff);
		}
		delete classes->at(i);
	}
	delete classes;
    
	// transfer to vector for division purposes
	vector<pair<AMFunction,bignum> > functions_vector;
	functions_vector.reserve(functions.size());
	for ( pair<AMFunction,long> fpair : functions ) {
		functions_vector.push_back(make_pair(fpair.first,fpair.second));
	}
    
    
	clock_t end_collect = clock();
	cout << name << " " << rank << ": is finished collecting equivalence classes" << endl;
	cout << name << " " << rank << ": @ " << (long double) (end_collect - begin) / (CLOCKS_PER_SEC ) << " sec" << endl;
	cout << name << " " << rank << ": Amount of representatives:" << functions.size() << endl;
    
    
	// generate interval sizes
	AMFunction e = AMFunction::empty_function();
	AMFunction u = AMFunction::universe_function(n);
	map<AMFunction, bignum> left_interval_size;
	map<AMFunction, bignum> right_interval_size;
	for( pair<AMFunction,bignum> fpair : functions_vector ) {
		AMFunction &f = fpair.first;
		AMFInterval left = AMFInterval(e,f);
		AMFInterval right = AMFInterval(f,u);
		left_interval_size.insert(make_pair(f,left.lattice_size()));
		right_interval_size.insert(make_pair(f,right.lattice_size()));
	}
    
    
	clock_t end_isizes = clock();
	cout << name << " " << rank << ": is finished generating interval sizes" << endl;
	cout <<name << " " << rank << ": @ " << (long double) (end_isizes - begin) / (CLOCKS_PER_SEC) << " sec" << endl;
    
    
	bignum sum = 0L;
	long long evaluations = 0;
    AMFInterval::GeneralFastIterator& it2 = *(AMFInterval(e,u).getFastIterator());
    while(it2.hasNext()) {
        ++it2;
        AMFunction & r2 = *it2;
        bignum r2size = right_interval_size.at(r2.standard());
        bignum sumP = 0L;
        // vector<pair<AMFunction,bignum> >::iterator iter = functions_vector.begin();
        int i = rank;
        while (i < functions_vector.size()) {
            pair<AMFunction, bignum> r1pair = functions_vector[i];
            AMFunction & r1 = r1pair.first;
            if (r1.leq(r2)) {
                sumP = sumP + ((r1pair.second) * (left_interval_size.at(r1)) * PatricksCoefficient(r1, r2));
                
                 ++evaluations;
                 if ( evaluations % REPORT == 0 ) {
                 cout << name << " " << rank << " partial sum: " << sum << " (" << evaluations << " evaluations)" << endl;
                 
                 }
                
            }
            i += size;
        }
        sum += (sumP * r2size);
    }
    
    //cout << evaluations << endl;
    if (rank == MASTER) {
        
        bignum rec;
        int i =1;
        while (i < size) {
            
            MPI_Recv(&rec, 1, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            sum = sum + rec;
            i++;
        }
        clock_t end_algo = clock();
        //cout <<(long double) (end_algo - begin) / (CLOCKS_PER_SEC) << endl;
        
        
        cout << name << " " << rank << "finished: " << evaluations << " evaluations" << endl;
        cout << name << " " << rank << "@ " << (long double) (end_algo - begin) / (CLOCKS_PER_SEC) << " sec" << endl;
        cout <<"AMF("<< m <<") =" <<sum << endl;
        
    } else if (rank != MASTER) {
        
        MPI_Send(&sum, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    
}

/**
 * Main algorithm for equivalence class generation
 */
vector<map<AMFunction,long>*>* Solver::algorithm9(int till) {
	vector<map<AMFunction,long>*>* res = new vector<map<AMFunction,long>*>(till+1);
	map<AMFunction, long>* res0 = new map<AMFunction, long>();
	mapstore(*res0,AMFunction::empty_function());
	mapstore(*res0,AMFunction::empty_set_function());
	(*res)[0] = res0;
	for (int n = 0; n < till; n++ ) {
		// calculate AMF(n+1) and add it to res.
		(*res)[n+1] = algorithm7(n,(*res)[n]);
	}
	return res;
}

map<AMFunction,long>* Solver::algorithm7(int n, const map<AMFunction,long>* const S) {
	map<AMFunction,long>* S1 = new map<AMFunction,long>();
	AMFunction alfa = AMFunction::universe_function(n);
	AMFunction u = AMFunction::universe_function(n+1);
	AMFunction l = AMFunction::singleton_function(n+1);
	for( pair<AMFunction,long> tpair : *S ) {
		AMFunction t = tpair.first;
		AMFunction::perm_t rtsymm = (t.join(l)).symmetry_group();
		map<AMFunction, long> St;
		AMFInterval delta(t.join(l),u.omicron(t,alfa));
		AMFInterval::GeneralFastIterator& amfit = *(delta.getFastIterator());
		while( amfit.hasNext() ) {
			++amfit;
			AMFunction a = (*amfit);
			mapstore(St,a.standard(rtsymm));
		}
		for ( pair<AMFunction,long> xpair : St ) {
			AMFunction x = xpair.first;
			mapstore(*S1,x.standard(),(xpair.second)*(tpair.second));
		}
	}
	return S1;
}


bool contains(list<AMFunction> as, AMFunction a) {
	for ( AMFunction b : as ) {
		if (b.equals(a)) {
			return true;
		}
	}
	return false;
}

bignum Solver::PatricksCoefficient(const AMFunction & r1, const AMFunction & r2) {
    // trivial case, no solutions unless r1 <= r2
    if (!r1.leq(r2)) {
        return 0;
    }
    // trivial case, one solution if r1 == r2
    if (r1.equals(r2)){
        return 1;
    }
    // treat the case of empty functions separately (most function in AMFunction and AMFinterval do not apply)
    if (r1.isEmpty()) {
        if (r2.isEmpty()) return 1; // (empty, empty)
        return 2; // (empty, r2), (r2,empty)
    }
    return (1<<(AMFGraph(r1,r2.minus(r1)).count_connected()));
}

