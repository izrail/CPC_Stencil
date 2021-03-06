LD = mpicc  
CC = sw5cc
OPT =
#CFLAGS = -Wall -std=c99 $(OPT) -OPT:IEEE_arith=1  #-qopenmp
CFLAGS = -O3  -host -I/usr/sw-mpp/mpi2/include/ -lm -std=c99  -LNO:prefetch=2:fusion=2:opt=1 -OPT:alias=typed:Ofast:IEEE_arith=1 
#CFLAGS = -Ofast -host -I/usr/sw-mpp/mpi2/include/ -lm -std=c99  -LNO:fusion=2:opt=1 -OPT:alias=typed:IEEE_arith=1 
SFLAGS = -O3 -slave  -lm_slave -msimd 
LDFLAGS = -Wall
# $(LINK_SPC) #-qopenmp
LDLIBS = $(LDFLAGS)

objects = check.o benchmark.o stencil-naive.o stencil-optimized.o slave_27.o slave_7.o benchmark-optimized

.PHONY : default
default : all

.PHONY : all
all : clean benchmark-optimized 

benchmark-optimized : check.o benchmark.o stencil-optimized.o slave_7.o 
	$(LD) -o $@ $^ $(LDLIBS)

stencil-optimized.o:stencil-optimized.c
	$(CC) $(CFLAGS) -c $<
slave_27.o:slave_27.c
	$(CC) $(SFLAGS) -c $<
slave_7.o:slave_7.c
	$(CC) $(SFLAGS) -c $<
check.o:check.c
	$(CC) $(CFLAGS) -c $<
benchmark.o:benchmark.c
	$(CC) $(CFLAGS) -c $<

#%.o : %.c common.h
#	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf  $(objects)
