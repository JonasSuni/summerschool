
main: main.c
	nvc -o main main.c dev_sum.c -fopenmp -mp=gpu -gpu=cc70 -Minfo=all

clean:
	rm -f main

