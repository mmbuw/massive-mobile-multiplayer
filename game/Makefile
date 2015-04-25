all: 
	g++ -std=c++11 -c *.cpp src/*.cpp -Iinclude/
	g++ -std=c++11 -o main *.o -lsfml-graphics -lsfml-window -lsfml-system
	./main

clean:
	rm -f *.o
	rm -f main