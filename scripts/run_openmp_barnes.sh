# Benchmark the approximation error between the serial Barnes Hut algorithm and the brute force algorithm

echo "Compiling... Please wait."
g++ -std=c++11 -fopenmp -O3 ../src/barnes_openmp.cpp ../src/barnes_hut.cpp ../src/utils.cpp
echo "Done."


echo "Executing with 100 simulation steps with N = 1000 particles and 1 thread"
time ./a.out 1000 0.0001 100 0.5 1 1

echo "Executing with 100 simulation steps with N = 1000 particles and 2 thread"
time ./a.out 1000 0.0001 100 0.5 2 2

echo "Executing with 100 simulation steps with N = 1000 particles and 3 thread"
time ./a.out 1000 0.0001 100 0.5 3 3

echo "Executing with 100 simulation steps with N = 1000 particles and 4 thread"
time ./a.out 1000 0.0001 100 0.5 4 4

echo "Executing with 100 simulation steps with N = 1000 particles and 5 thread"
time ./a.out 1000 0.0001 100 0.5 5 5

rm -f a.out
