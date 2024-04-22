test:
	gcc -c ../utils.c -I ..
	g++ utils_test.cpp -std=c++11 -stdlib=libc++  -I .. -I /Users/kevin/Projects/code/googletest/googletest/include/  -I /Users/kevin/Projects/code/googletest/googlemock/include -L /Users/kevin/Projects/code/googletest/lib/ -I /Users/kevin/Projects/Bot/pingpong/protocal/device utils.o -lgtest -lgmock
	./a.out