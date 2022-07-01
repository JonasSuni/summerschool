
main: main.c
	nvc -o $@ $^ -fopenmp -mp=gpu -gpu=cc70 -Minfo=all

cpu: main.c
	nvc -o main main.c -mp -Minfo=all

clean:
	rm -f main

