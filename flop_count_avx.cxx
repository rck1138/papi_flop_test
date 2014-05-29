#include<iostream>
#include<papi.h>
#include "immintrin.h"

using namespace std;

int main(int argc, char **argv)
{
	long long N = 8000000;          // number of trips through the loop

	int *evts = new int[1];
	evts[0] = PAPI_DP_OPS;

	long long cvals;
	double accum = 0.0;
	//double *a1 = new double[N];
   double *a1 = (double *) _mm_malloc(N*sizeof(double),32);
	double *a2 = (double *) _mm_malloc(N*sizeof(double),32);
	double *a3 = (double *) _mm_malloc(N*sizeof(double),32);

   __m256d *a1v = (__m256d *) _mm_malloc((N/4)*sizeof(__m256d),32);
   __m256d *a2v = (__m256d *) _mm_malloc((N/4)*sizeof(__m256d),32);
   __m256d *a3v = (__m256d *) _mm_malloc((N/4)*sizeof(__m256d),32);
   __m256d phi = _mm256_set1_pd(1.618033);

   // initialize values as normal
	for(int i=0; i<N; i++){
		a1[i] = double(N) - double(i);
		a2[i] = double(i) + 1.0;
	}

   // copy initial vals to AVX vars
   for(int i=0; i<N/4; i++){
      a1v[i] = _mm256_load_pd(&(a1[4*i]));
      a2v[i] = _mm256_load_pd(&(a2[4*i]));
   }

	// start counting
	PAPI_start_counters(evts, 1);

	// axpy test loop should perform 2N flops
	for(int i=0; i<N/4; i++){
#ifdef PREFETCH
      _mm_prefetch((char *)&(a1v[i+8]),_MM_HINT_T0);
      _mm_prefetch((char *)&(a2v[i+8]),_MM_HINT_T0);
#endif
      a3v[i] = _mm256_add_pd(a1v[i], _mm256_mul_pd(phi,a2v[i]));
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
