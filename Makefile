CXX = icpc
#CFLAGS = -O3 -xAVX -vec-report3 -S
CFLAGS = -O0
#PAPIROOT = /glade/apps/opt/papi/4.2.1/gnu/4.7.2/
#PAPIROOT = /glade/apps/opt/papi/5.1.0.2/gnu/4.7.2/
PAPIROOT = /glade/apps/opt/papi/5.3.0/intel/12.1.5
INC = -I$(PAPIROOT)/include
LIB = -L$(PAPIROOT)/lib64 -lpapi -Wl,-rpath,$(PAPIROOT)/lib64

all: flop_count_avx flop_count_avx_pf

flop_count_avx: flop_count_avx.cxx
	$(CXX) -o flop_count_avx $(INC) $(CFLAGS) $(LIB) $<

flop_count_avx_pf: flop_count_avx.cxx
	$(CXX) -o flop_count_avx_pf -DPREFETCH $(INC) $(CFLAGS) $(LIB) $<

clean:
	rm -f *.o *.s flop_count_avx flop_count_avx_pf
