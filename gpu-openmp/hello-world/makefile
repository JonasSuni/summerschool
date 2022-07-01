
main: main.c
	nvc -o $@ $^ -fopenmp -mp=gpu -gpu=cc70

cpu: main.c
	nvc -o main main.c -mp

clean:
	rm -f main

