# Benchmark the brute force serial algorithm

echo "Compiling... Please wait."
g++ -std=c++11 -O3 ../src/serial.cpp ../src/utils.cpp
echo "Done."


echo "Executing with 100 simulation steps with N = 100 particles."
time ./a.out 100 0.0001 100

echo "Executing with 100 simulation steps with N = 500 particles."
time ./a.out 500 0.0001 100

echo "Executing with 100 simulation steps with N = 1000 particles."
time ./a.out 1000 0.0001 100

echo "Executing with 100 simulation steps with N = 2000 particles."
time ./a.out 2000 0.0001 100

echo "Executing with 100 simulation steps with N = 4000 particles."
time ./a.out 4000 0.0001 100

echo "Executing with 100 simulation steps with N = 5000 particles."
time ./a.out 5000 0.0001 100

rm -f a.out
