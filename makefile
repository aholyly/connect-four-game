All: 
	g++ connect_four.cpp -c -std=c++11 -Wall
	g++ connect_four.o -o exe
clean:
	rm *.o exe