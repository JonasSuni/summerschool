
IFLAGS = -I /proj/group/spacephysics/hip/4.5/hipamd/build/install/include

main: main.cpp
	hipcc $(HIP_IFLAGS) -x cu -o $@ $^
	#nvcc -x cu -o $@ $^

clean:
	rm -f main

