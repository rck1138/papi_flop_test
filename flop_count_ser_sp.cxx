#include<iostream>
#include<papi.h>
#include "immintrin.h"

using namespace std;

int main(int argc, char **argv)
{
	long long N = 8000000;          // number of trips through the loop

	int *evts = new int[1];
	evts[0] = PAPI_DP_OPS;
	typedef double ftype;

	long long cvals;
	ftype accum = 0.0;
	ftype *a1 = new ftype[N];
	ftype *a2 = new ftype[N];
	ftype *a3 = new ftype[N];
	ftype phi = 1.618033;
	ftype check;

	for(int i=0; i<N; i++){
		a1[i] = ftype(N) - ftype(i);
		a2[i] = ftype(i) + 1.0;
	}

	// start counting
	PAPI_start_counters(evts, 1);

	// axpy test loop should perform 2N flops
	for(int i=0; i<N; i++){
#ifdef PREFETCH
		_mm_prefetch((char *)&a1[i+128], _MM_HINT_T0);
		_mm_prefetch((char *)&a2[i+128], _MM_HINT_T0);
#endif
		a3[i] = a1[i] + phi*a2[i];
	} 

	// stop counting
	PAPI_stop_counters(&cvals, 1);

	// print event counts
	cout << "Counted:    Expected:    Ratio:" << endl;
	cout << cvals << "     " << 2*N << "     " << float(cvals)/float(2*N) << endl;
	
	// use the result so it doesn't get optimized away
	for(int i=0; i<N; i++) accum += a3[i];
	check = a3[N-1] - (1.0+ N*1.618033);
	if (check != 0.0) cout << "Validation check failed" << endl;

	return 0;
}
