#!/usr/bin/env bash

make sequential
make omp

echo "Seq 64"
./sequential 64 1
./sequential 64 1
./sequential 64 1

echo "Seq 640"
./sequential 640 1
./sequential 640 1
./sequential 640 1

echo "Seq 6400"
./sequential 6400 1
./sequential 6400 1
./sequential 6400 1

echo "Seq 64000"
./sequential 64000 1
./sequential 64000 1
./sequential 64000 1

echo "OMP 64"
./omp 64 1
./omp 64 1
./omp 64 1

echo "OMP 640"
./omp 640 1
./omp 640 1
./omp 640 1

echo "OMP 6400"
./omp 6400 1
./omp 6400 1
./omp 6400 1

echo "OMP 64000"
./omp 64000 1
./omp 64000 1
./omp 64000 1