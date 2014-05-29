CXX = icpc
CFLAGS = -O0
PAPIROOT = /glade/apps/opt/papi/5.3.0/intel/12.1.5
INC = -I$(PAPIROOT)/include
LIB = -L$(PAPIROOT)/lib64 -lpapi -Wl,-rpath,$(PAPIROOT)/lib64

all: flop_count_ser flop_count_ser_pf \
     flop_count_sse_sp flop_count_sse_sp_pf flop_count_sse_dp flop_count_sse_dp_pf \
     flop_count_avx_sp flop_count_avx_sp_pf flop_count_avx_dp flop_count_avx_dp_pf

flop_count_ser: flop_count_ser.cxx
	$(CXX) -o flop_count_ser $(INC) $(CFLAGS) $(LIB) $<

flop_count_ser_pf: flop_count_ser.cxx
	$(CXX) -o flop_count_ser_pf -DPREFETCH $(INC) $(CFLAGS) $(LIB) $<

flop_count_sse_sp: flop_count_sse_sp.cxx
	$(CXX) -o flop_count_sse_sp $(INC) $(CFLAGS) $(LIB) $<

flop_count_sse_sp_pf: flop_count_sse_sp.cxx
	$(CXX) -o flop_count_sse_sp_pf -DPREFETCH $(INC) $(CFLAGS) $(LIB) $<

flop_count_sse_dp: flop_count_sse_dp.cxx
	$(CXX) -o flop_count_sse_dp $(INC) $(CFLAGS) $(LIB) $<

flop_count_sse_dp_pf: flop_count_sse_dp.cxx
	$(CXX) -o flop_count_sse_dp_pf -DPREFETCH $(INC) $(CFLAGS) $(LIB) $<

flop_count_avx_sp: flop_count_avx_sp.cxx
	$(CXX) -o flop_count_avx_sp $(INC) $(CFLAGS) $(LIB) $<

flop_count_avx_sp_pf: flop_count_avx_sp.cxx
	$(CXX) -o flop_count_avx_sp_pf -DPREFETCH $(INC) $(CFLAGS) $(LIB) $<

flop_count_avx_dp: flop_count_avx_dp.cxx
	$(CXX) -o flop_count_avx_dp $(INC) $(CFLAGS) $(LIB) $<

flop_count_avx_dp_pf: flop_count_avx_dp.cxx
	$(CXX) -o flop_count_avx_dp_pf -DPREFETCH $(INC) $(CFLAGS) $(LIB) $<

clean:
	rm -f *.o *.s flop_count_ser flop_count_ser_pf \
                 flop_count_sse_sp flop_count_sse_sp_pf flop_count_sse_dp flop_count_sse_dp_pf \
                 flop_count_avx_sp flop_count_avx_sp_pf flop_count_avx_dp flop_count_avx_dp_pf
