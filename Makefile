build:
	gcc -o3 -o trapc src/*.c src/archs/*.c -Wall
install:
	mv trapc /usr/bin/trapc
