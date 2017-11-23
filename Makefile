all: sequential dots

sequential: DotGen.h Graph.h InitializeGraph.h sequential_array.cpp
	g++ -O3 -o sequential ./sequential_array.cpp

dots: sparse_100_input.dot sparse_100_result.dot
	dot -Tpng -O sparse_100_input.dot
	dot -Tpng -O sparse_100_result.dot
	dot -Tpng -O sparse_100_mst_overlay.dot

run:
	docker run -v `pwd`:/data hamroctopus/graphviz:2.38.0

terminal:
	docker run -it -v `pwd`:/data hamroctopus/graphviz:2.38.0 /bin/bash

clean:
	rm sequential
