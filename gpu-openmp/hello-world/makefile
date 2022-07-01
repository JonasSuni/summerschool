
main: main.c
	mpicxx -o $@ $^ -fopenmp
	nvc -o $@ $^ -mp=gpu -gpu=cc70

clean:
	rm -f main

