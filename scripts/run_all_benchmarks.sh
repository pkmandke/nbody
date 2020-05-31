# Benchmark the 3 N-Body implementations

echo "Compiling the serial brute force algorithm... Please wait."
g++ -std=c++11 -O3 ../src/serial.cpp ../src/utils.cpp
echo "Done."

echo "Executing with 100 simulation steps with N = 1000 particles."
time ./a.out 1000 0.0001 100

rm -f a.out

echo "Compiling the serial barnes Hut code.. Please wait"
g++ -std=c++11 -O3 ../src/approx_error.cpp ../src/barnes_hut.cpp ../src/utils.cpp
echo "Done"

echo "Approximation error for 100 simulation steps with N = 1000 particles."
time ./a.out 1000 0.0001 100 0.5

rm -f a.out

echo "Compiling OpenMP based Barnes Hut algorithm... Please wait."
g++ -std=c++11 -fopenmp -O3 ../src/barnes_openmp.cpp ../src/barnes_hut.cpp ../src/utils.cpp
echo "Done"

echo "Executing the OpenMP based barnes Hut algorithm for 100 simulation steps with N = 1000 particles and 4 threads."
time ./a.out 1000 0.0001 100 0.5 4 4

rm -f a.out

