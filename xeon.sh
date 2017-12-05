#!/usr/bin/env bash

make sequential
make omp

echo "Seq 32"
./sequential 32 1
./sequential 32 1
./sequential 32 1

echo "Seq 320"
./sequential 320 1
./sequential 320 1
./sequential 320 1

echo "Seq 3200"
./sequential 3200 1
./sequential 3200 1
./sequential 3200 1

echo "Seq 32000"
./sequential 32000 1
./sequential 32000 1
./sequential 32000 1

echo "OMP 32"
./omp 32 1
./omp 32 1
./omp 32 1

echo "OMP 320"
./omp 320 1
./omp 320 1
./omp 320 1

echo "OMP 3200"
./omp 3200 1
./omp 3200 1
./omp 3200 1

echo "OMP 32000"
./omp 32000 1
./omp 32000 1
./omp 32000 1