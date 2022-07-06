
main: main.cpp
	#hipcc -x cu -o $@ $^
	nvcc -x cu -o $@ $^

clean:
	rm -f main

