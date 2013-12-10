/*
 * Solver.cpp
 *
 *  Created on: 3-dec.-2013
 *      Author: Daan
 */

#include "Solver.h"

Solver::Solver() {
	// TODO Auto-generated constructor stub
}

Solver::~Solver() {
	// TODO Auto-generated destructor stub
}

void mapstore(map<AMFunction, long> M, AMFunction A, long i) {
	M.insert(make_pair(A,i));
}
void mapstore(map<AMFunction, long> M, AMFunction A) {
	M.insert(make_pair(A,0L));
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

vector<map<AMFunction,long>> Solver::algorithm9(int till) {
	vector<map<AMFunction,long>> res(till);
	map<AMFunction, long> res0;
	mapstore(res0,AMFunction::emptyFunction());
	mapstore(res0,AMFunction::emptySetFunction());
	res.push_back(res0);
	for (int n = 0; n < till; n++ ) {
		// calculate AMF(n+1) and add it to res.
		// res.push_back(algorithm7(n,res.at(n)));
	}
	return res;
}

map<AMFunction,long> algorithm7(int n, map<AMFunction,long> S) {
	map<AMFunction,long> S1;
	AMFunction alfa = AMFunction::universeFunction(n);
	AMFunction u = AMFunction::universeFunction(n+1);
	AMFunction l = AMFunction::singletonFunction(n+1);
	for( pair<AMFunction,long> tpair : S ) {
		AMFunction t = tpair.first;
		tr1::unordered_set<vector<int>> rtsymm = (t.join(l)).symmetry_group();
		map<AMFunction, long> St;
		AMFInterval delta(t.join(l),u.omicron(t,alfa));
		/* TODO: iterate over delta
		 * for (AMFunction x : delta) {
		 * 		mapstore(St,x.standard());
		 * }
		 */
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
