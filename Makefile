all: sequential omp mpi
COMMON = DotGen.h Graph.h InitializeGraph.h Arguments.h Print.h

sequential: prims_sequential.cpp $(COMMON)
	g++ -O -o sequential ./prims_sequential.cpp

omp: prims_omp.cpp $(COMMON)
	g++ -O -fopenmp -o omp ./prims_omp.cpp

mpi: prims_mpi.cpp $(COMMON)
	mpiCC -O -o mpi ./prims_mpi.cpp

dots:
	dot -Tpng -O results/*.dot

run:
	docker run -v `pwd`:/data hamroctopus/graphviz:2.38.0

terminal:
	docker run -it -v `pwd`:/data hamroctopus/graphviz:2.38.0 /bin/bash

clean:
	rm sequential
