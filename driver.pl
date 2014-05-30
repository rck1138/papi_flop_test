#!/usr/bin/perl -w
# driver script to run through the various PAPI
# flop counting test combinations
use strict;

my @events = ("PAPI_FP_INS","PAPI_FP_OPS","PAPI_SP_OPS",
              "PAPI_DP_OPS","PAPI_VEC_SP","PAPI_VEC_DP");
my @tests = ("flop_count_ser_sp","flop_count_ser_sp_pf",
             "flop_count_ser_dp","flop_count_ser_dp_pf",
             "flop_count_sse_sp","flop_count_sse_sp_pf",
             "flop_count_sse_dp","flop_count_sse_dp_pf",
             "flop_count_avx_sp","flop_count_avx_sp_pf",
             "flop_count_avx_dp","flop_count_avx_dp_pf");

my $e;      # event idx
my $t;      # test idx
my $n=20;   # number of test iterations
my $i;      # loop dummy
my $r;      # ratio - output of running a test
my $avg_r;  # average ratio of the $n tests
my $min_r;  # min ratio over $n tests
my $max_r;  # max ratio over $n tests

foreach $e (@events){
	open(FILE, ">$e.results");
	system("make clean; make PAPI_EVENT=$e");
	foreach $t (@tests){
		$min_r = 1000;
		$max_r = 0.0;
		$avg_r = 0.0; 
		for($i=0; $i<$n; $i++){
			$_ = `./$t`;
			s/\n//g;
			/([0-9]+\.[0-9]+)$/;
			$r = $1;
			$avg_r += $r;
			$min_r = $r < $min_r ? $r : $min_r;
			$max_r = $r > $max_r ? $r : $max_r;
		}
		$avg_r /= $n;
		printf FILE "%-20s: avg: %7.5f, range: (%7.5f - %7.5f)\n", $t, $avg_r, $min_r, $max_r;
	}
	close FILE;
} 
