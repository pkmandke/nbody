N-Body Problems

The source code involves 3 implementations: the brute force serial algorithm, the serial Barnet Hut algorithm and the parallel Barnes Hut algorithm.
To run 1 iteration of each of the 3 implementations run: sh run_all_benchmarks.sh from inside ./scripts/

Clone: git clone https://github.com/pkmandke/nbody.git && cd nbody

* The brute force algorithm

To benchmark the time of the serial algorithm with N = 100, 500, 1000, 2000, 4000 and 500 particles,
Execute: cd ./scripts/ && sh serial_time_benchmark.sh

* The serial Barnes Hut algorithm

To benchmark the approximation error between the brute force and the serial Barnes Hut algorithm, 
Execute: cd ./scripts/ && sh run_serial_barnes.sh

* Parallel Barnes Hut using OpenMP

To run the OpenMP based Barnes Hut algorithm with different number of threads,
Execute: cd ./scripts && sh run_openmp_barnes.sh



* Build and Run

To build and run each individual parts, use make as follows

(First clone using: git clone https://github.com/pkmandke/nbody.git && cd nbody)

make serial # Build the serial brute force algorithm
./obj_/serial num_particles delta steps # Run the program

make barnes_serial   # Build the serial BH algorithm
./obj_/barnes_serial num_particles delta steps theta   # Run the program

make barnes_parallel   # Build the parallel version of BH
./obj_/barnes_parallel num_particles delta steps theta num_threads num_chunks   # Run the program

Finally execute
make clean   # Clean object files from ./obj_


Author: Prathamesh Mandke
