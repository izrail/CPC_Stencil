LD = mpicc #mpiicc 
CC = sw5cc
OPT =
#CFLAGS = -Wall -std=c99 $(OPT) -OPT:IEEE_arith=1  #-qopenmp
CFLAGS = -O3 -host  -I/usr/sw-mpp/mpi2/include/ -lm -Wall -std=c99  $(OPT) -OPT:IEEE_arith=1 
SFLAGS = -O3 -slave -LNO:simd=1 -lm_slave
LDFLAGS = -Wall
# $(LINK_SPC) #-qopenmp
LDLIBS = $(LDFLAGS)

targets = benchmark-optimized
objects = check.o benchmark.o stencil-naive.o stencil-optimized.o slave.o

.PHONY : default
default : all

.PHONY : all
all : clean $(targets)


benchmark-optimized : check.o benchmark.o stencil-optimized.o slave.o
	$(LD) -o $@ $^ $(LDLIBS)

stencil-optimized.o:stencil-optimized.c
	$(CC) $(CFLAGS) -c $<
slave.o:slave.c
	$(CC) $(SFLAGS) -c $<
check.o:check.c
	$(CC) $(CFLAGS) -c $<
benchmark.o:benchmark.c
	$(CC) $(CFLAGS) -c $<

#%.o : %.c common.h
#	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(targets) $(objects)
