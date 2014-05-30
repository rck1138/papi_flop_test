#include<iostream>
#include<boost/format.hpp>
#include<papi.h>
#include "immintrin.h"

using namespace std;

int main(int argc, char **argv)
{
	typedef float rtype;
	typedef __m256 vtype;
	const int vlen = 32/sizeof(rtype);
	const int offset = 4*vlen;
	long long N = 8000000;          // number of trips through the loop
	rtype check;

	int *evts = new int[1];
	evts[0] = PAPI_EVENT ;

	long long cvals;
	rtype accum = 0.0;
   rtype *a1 = (rtype *) _mm_malloc(N*sizeof(rtype),32);
	rtype *a2 = (rtype *) _mm_malloc(N*sizeof(rtype),32);
	rtype *a3 = (rtype *) _mm_malloc(N*sizeof(rtype),32);

   vtype *a1v = (vtype *) _mm_malloc((N/vlen)*sizeof(vtype),32);
   vtype *a2v = (vtype *) _mm_malloc((N/vlen)*sizeof(vtype),32);
   vtype *a3v = (vtype *) _mm_malloc((N/vlen)*sizeof(vtype),32);
   vtype phi = _mm256_set1_ps(1.618033);

   // initialize values as per serial version
	for(int i=0; i<N; i++){
		a1[i] = rtype(N) - rtype(i);
		a2[i] = rtype(i) + 1.0;
	}

   // copy initial vals to AVX vars
   for(int i=0; i<N/vlen; i++){
      a1v[i] = _mm256_load_ps(&(a1[vlen*i]));
      a2v[i] = _mm256_load_ps(&(a2[vlen*i]));
   }

	// start counting
	PAPI_start_counters(evts, 1);

	// axpy test loop should perform 2N flops
	for(int i=0; i<N/vlen; i++){
#ifdef PREFETCH
      _mm_prefetch((char *)&(a1v[i+offset]),_MM_HINT_T0);
      _mm_prefetch((char *)&(a2v[i+offset]),_MM_HINT_T0);
#endif
      a3v[i] = _mm256_add_ps(a1v[i], _mm256_mul_ps(phi,a2v[i]));
	} 

	// stop counting
	PAPI_stop_counters(&cvals, 1);

	// print event counts
   cout << "Counted:    Expected:      Ratio:" << endl;
   cout << boost::format("%8d") % cvals << "    " << boost::format("%9d") % (2*N) << "   " << boost::format("%9.5f") % (float(cvals)/float(2*N)) << endl;
	
	// use the result so it doesn't get optimized away, check last value
   for(int i=0; i<N/vlen; i++) _mm256_store_ps (&a3[vlen*i], a3v[i]);
	for(int i=0; i<N; i++) accum += a3[i];
	check = a3[N-1] - (1.0+ N*1.618033);
	if (check != 0.0) cout << "Validation check failed" << endl;

	return 0;
}
