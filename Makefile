build:
	gcc -o2 -o trapc src/*.c src/archs/*.c -Wall
install:
	mv trapc /usr/bin/trapc
