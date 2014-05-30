// flop_count_worst.cxx
// This code uses PAPI to count a known number of flops for an algorithm
// with an extremely poor cache hit rate.  PAPI is known to over-count
// flops due to instruction re-issue when waiting for values to arrive
// from memory.  This code is intended to demonstrate an upper bound on
// the amount of over counting that can occur in such situations.
#include<iostream>
#include<boost/format.hpp>
#include<papi.h>
#include <cstdlib>
#include "immintrin.h"

using namespace std;

// function to generate permutation of [1,N]
void gen_permutation(size_t *arr, size_t n);

int main(int argc, char **argv)
{
	typedef double rtype;
	typedef __m256d vtype;
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

	size_t *idx = (size_t *) malloc((N/vlen)*sizeof(size_t));
   vtype *a1v = (vtype *) _mm_malloc((N/vlen)*sizeof(vtype),32);
   vtype *a2v = (vtype *) _mm_malloc((N/vlen)*sizeof(vtype),32);
   vtype *a3v = (vtype *) _mm_malloc((N/vlen)*sizeof(vtype),32);
   vtype phi = _mm256_set1_pd(1.618033);

   // initialize values as per serial version
	for(int i=0; i<N; i++){
		a1[i] = rtype(N) - rtype(i);
		a2[i] = rtype(i) + 1.0;
	}

   // copy initial vals to AVX vars
   for(int i=0; i<N/vlen; i++){
      a1v[i] = _mm256_load_pd(&(a1[vlen*i]));
      a2v[i] = _mm256_load_pd(&(a2[vlen*i]));
   }

	// initialize permuted index array
	gen_permutation(idx, N/vlen);

	// start counting
	PAPI_start_counters(evts, 1);

	// axpy test loop should perform 2N flops
	// we are steeping through the array in random order
	// for explicitly poor cache performance
	for(int i=0; i<N/vlen; i++){
      a3v[i] = _mm256_add_pd(a1v[idx[i]], _mm256_mul_pd(phi,a2v[idx[i]]));
	} 

	// stop counting
	PAPI_stop_counters(&cvals, 1);

	// print event counts
   cout << "Counted:    Expected:      Ratio:" << endl;
   cout << boost::format("%8d") % cvals << "    " << boost::format("%9d") % (2*N) << "   " << boost::format("%9.5f") % (float(cvals)/float(2*N)) << endl;
	
	// use the result so it doesn't get optimized away, check last value
   for(int i=0; i<N/vlen; i++) _mm256_store_pd (&a3[vlen*i], a3v[i]);
	for(int i=0; i<N; i++) accum += a3[i];
	if(accum < 0) cout << "This can't happen" << endl;

	return 0;
}


 /* gen_permutation
 * This function fills an array of size n with a random
 * permutation of the integers {0, ... ,n-1} subject to
 * the constraint that the permutation can be expressed
 * as a one-cycle of length n
*/
void gen_permutation(size_t *arr, size_t n){
   size_t *tmp;  /* temporary array                            */
   size_t top;   /* elements tmp[j<top] have not been selected */
   size_t i,j;   /* array indices                              */
   double r;     /* random number used to set 0 <= j < top     */

   /* allocate space for tmp array and fill it */
   /* with the integers {1, ... ,n-1}          */
   tmp = (size_t *)malloc(n*sizeof(size_t));
   top=n-1;
   for(i=0; i<top; i++){
      tmp[i] = i+1;
   }

   /* shuffle the integers in tmp to get a suitable permutation */
   /* store the permutation in arr                              */
   i=0;
   while(top > 0){
      r = (double)rand()/(double)(RAND_MAX);
      j = (unsigned int)(r*(double)top);
      if (j == top) j--;
      arr[i] = tmp[j];
      top--;
      tmp[j] = tmp[top];
      i = arr[i];
   }
   arr[i] = 0;

   free(tmp);
}
