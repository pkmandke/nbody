CC=g++
CPPFLAGS=-std=c++11 -fopenmp -O3

ODIR=./obj_

OPTS=serial barnes_serial barnes_parallel
OPTS_CLEAN=$(patsubst %,$(ODIR)/%,$(OPTS))

FILES_SERIAL=$(ODIR)/utils.o ./src/serial.cpp
FILES_BARNES_SERIAL=$(ODIR)/utils.o ./src/approx_error.cpp ./src/barnes_hut.cpp
FILES_BARNES_PARALLEL=$(ODIR)/utils.o ./src/barnes_openmp.cpp ./src/barnes_hut.cpp

INCL=./src/nbody.h

all: $(OPTS)

utils_: $(INCL)
	$(CC) $(CPPFLAGS) -c -o $(ODIR)/utils.o ./src/utils.cpp

serial: utils_ $(INCL)
	$(CC) $(CPPFLAGS) -o $(ODIR)/serial $(FILES_SERIAL)

barnes_serial: utils_ $(INCL)
	$(CC) $(CPPFLAGS) -o $(ODIR)/barnes_serial $(FILES_BARNES_SERIAL)

barnes_parallel: utils_ $(INCL)
	$(CC) $(CPPFLAGS) -o $(ODIR)/barnes_parallel $(FILES_BARNES_PARALLEL)

.PHONY: clean

clean:
	rm -f $(OPTS_CLEAN)
	rm -f $(ODIR)/utils.o
