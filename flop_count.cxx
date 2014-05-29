#include<iostream>
#include<papi.h>

using namespace std;

int main(int argc, char **argv)
{
	long long N = 1000000;          // number of trips through the loop

	int *evts = new int[1];
	evts[0] = PAPI_DP_OPS;
	typedef double ftype;

	long long cvals;
	ftype accum = 0.0;
	ftype *a1 = new ftype[N];
	ftype *a2 = new ftype[N];
	ftype *a3 = new ftype[N];
	ftype phi = 1.618033;

	for(int i=0; i<N; i++){
		a1[i] = ftype(N) - ftype(i);
		a2[i] = ftype(i) + 1.0;
	}

	// start counting
	PAPI_start_counters(evts, 1);

	// axpy test loop should perform 2N flops
	for(int i=0; i<N; i++){
		a3[i] = a1[i] + phi*a2[i];
	} 

	// stop counting
	PAPI_stop_counters(&cvals, 1);

	// print event counts
	cout << "Counted:    Expected:    Ratio:" << endl;
	cout << cvals << "     " << 2*N << "     " << float(cvals)/float(2*N) << endl;
	
	// use the result so it doesn't get optimized away
	for(int i=0; i<N; i++) accum += a3[i];
	if(accum < 0.0) cout << "This shouldn't happen" << endl;

	return 0;
}