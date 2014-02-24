/*
 * Solver.cpp
 *
 *  Created on: 3-dec.-2013
 *      Author: Daan, Max
 */

#include "Solver.h"

Solver::Solver() {
	// TODO Auto-generated constructor stub
}

Solver::~Solver() {
	// TODO Auto-generated destructor stub
}

/**
 * Increase the coefficient of A in M by i
 */
void mapstore(map<AMFunction, long> M, AMFunction A, long i) {
	map<AMFunction, long>::iterator it = M.find(A);
	if (it != M.end()) {
		(*it).second += i;
	} else {
		M.insert(make_pair(A,0L));
	}
}

/**
 * Increase the coefficient of A in M by 1
 */
void mapstore(map<AMFunction, long> M, AMFunction A) {
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
long long Solver::pc2_dedekind(int m) {
	int n = m - 2;
	// generate
	vector<map<AMFunction,long>> classes = algorithm9(m);
	map<AMFunction,long> functions;

	// collect
	for (int i = 0; i < (int) classes.capacity() ; i++ ) {
		long coeff = combinations(n, i);
		for( pair<AMFunction,long> p : classes.at(i)) {
			mapstore(functions, p.first, p.second*coeff);
		}
	}

	AMFunction e = AMFunction::emptyFunction();
	AMFunction u = AMFunction::universeFunction(n);
	map<AMFunction, bigint> left_interval_size;
	map<AMFunction, bigint> right_interval_size;
	for( pair<AMFunction,long> fpair : functions ) {
		AMFunction &f = fpair.first;
		AMFInterval left = AMFInterval(e,f);
		AMFInterval right = AMFInterval(f,u);
		left_interval_size.insert(make_pair(f,left.lattice_size()));
		right_interval_size.insert(make_pair(f,right.lattice_size()));
	}

	bigint sum = 0L;
	long evaluations = 0;
	long possibilities = 0;

	AMFInterval::iterator it2 = AMFInterval(e,u).getIterator();
	while(it2.hasNext()) {
		AMFunction r2 = *it2;
		bigint r2size = right_interval_size.at(r2.standard());
		bigint sumP = 0L;
		for (pair<AMFunction,long> r1pair : functions ) {
			possibilities++;
			AMFunction &r1 = r1pair.first;
			if (r1.leq(r2)) {
				sumP = sumP	+ ( (r1pair.second)
							 	* (left_interval_size.at(r1))
							 	* PatricksCoefficient(r1, r2)
							  );
				evaluations++;
			}
		}
		sum = sum + (sumP * r2size);
	}

	return sum;

}

/**
 * Main algorithm for equivalence class generation
 */
vector<map<AMFunction,long>> Solver::algorithm9(int till) {
	vector<map<AMFunction,long>> res(till);
	map<AMFunction, long> res0;
	mapstore(res0,AMFunction::emptyFunction());
	mapstore(res0,AMFunction::emptySetFunction());
	res.push_back(res0);
	for (int n = 0; n < till; n++ ) {
		// calculate AMF(n+1) and add it to res.
		res.push_back(algorithm7(n,res.at(n)));
	}
	return res;
}

map<AMFunction,long> Solver::algorithm7(int n, map<AMFunction,long> S) {
	map<AMFunction,long> S1;
	AMFunction alfa = AMFunction::universeFunction(n);
	AMFunction u = AMFunction::universeFunction(n+1);
	AMFunction l = AMFunction::singletonFunction(n+1);
	for( pair<AMFunction,long> tpair : S ) {
		AMFunction t = tpair.first;
		perm_t rtsymm = (t.join(l)).symmetry_group();
		map<AMFunction, long> St;
		AMFInterval delta(t.join(l),u.omicron(t,alfa));
		for( AMFInterval::iterator amfit = delta.getIterator() ; amfit.hasNext() ; ++amfit ) {
			AMFunction a = (*amfit);
			mapstore(St,a.standard(rtsymm));
		}
		for ( pair<AMFunction,long> xpair : St ) {
			AMFunction x = xpair.first;
			mapstore(S1,x,(xpair.second)*(tpair.second));
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

long long Solver::PatricksCoefficient(AMFunction r1, AMFunction r2) {
    // trivial case, no solutions unless r1 <= r2
    if (!r1.leq(r2)) return 0;
    // trivial case, one solution if r1 == r2
    if (r1.equals(r2)) return 1;
    long long rest = 0;
    // treat the case of empty functions separately (most function in AMFunction and AMFinterval do not apply)
    if (r1.isEmpty()) {
        if (r2.isEmpty()) return 1; // (empty, empty)
        return 2; // (empty, r2), (r2,empty)
    }
    // return (1<<(CountConnected(graph(r1,r2.minus(r1)))));
    return 0;
}



/*
void verynaivededekind() {
	int const n = 4; // works instant up to 4... and 5 takes a while.
	int const sbsamount = pow(2,n);
	SmallBasicSet sbs[sbsamount];
	for (int i = 0 ; i < sbsamount ; i++) {
		sbs[i] = SmallBasicSet(i);
		cout << sbs[i].toString() << endl;
	}
	cout << "----" << endl;
	int accdede = 1;
	cout << "lege AMF - 1" << endl;
	list<AMFunction> amfs;
	amfs.push_front(AMFunction());
	int itno = 0;
	bool set_added = true;
	while (set_added) {
		set_added = false;
		cout << "chains of length " << (itno++)+1 << endl;
		list<AMFunction> amfs_new;
		for (AMFunction a : amfs) {
			for (SmallBasicSet s : sbs) {
				AMFunction a_new = a.shallowclone();
				if (!a_new.contains(s)) {
					a_new.addSet(s);
					if (a_new.isAntiMonotonic() && !contains(amfs_new,a_new)) {
						accdede++;
						// cout << a_new.toString() << " - " << accdede << endl;
						if (accdede % 1000 == 0) {cout << "count:" << accdede << endl;}
						amfs_new.push_front(a_new);
						set_added = true;
					}
				}
			}
		}
		amfs = amfs_new;
	}
	cout << "Dedekind number for n = " << n << ": " << accdede;
}

int main() {
    verynaivededekind();
}

*/
