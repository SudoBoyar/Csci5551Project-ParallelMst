all: sequential dots

sequential: DotGen.h Graph.h InitializeGraph.h prims_sequential.cpp
	g++ -O3 -o sequential ./prims_sequential.cpp

dots: results/*.dot
	dot -Tpng -O results/*.dot

run:
	docker run -v `pwd`:/data hamroctopus/graphviz:2.38.0

terminal:
	docker run -it -v `pwd`:/data hamroctopus/graphviz:2.38.0 /bin/bash

clean:
	rm sequential
