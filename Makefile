run:
	docker run -v `pwd`:/data hamroctopus/graphviz:2.38.0

terminal:
	docker run -it -v `pwd`:/data hamroctopus/graphviz:2.38.0 /bin/bash