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
my $n;      # number of test iterations
my $r;      # ratio - output of running a test
my $avg_r;  # average ratio of the $n tests
my $min_r;  # min ratio over $n tests
my $max_r;  # max ratio over $n tests

foreach $e ("PAPI_FP_OPS"){
	#system("make clean; make PAPI_EVENT=$e");
	foreach $t (@tests){
		$_ = `./$t`;
		s/\n//g;
		/([0-9]+\.[0-9]+)$/;
		$r = $1;
		print "r = $r\n"; 
	}
} 
