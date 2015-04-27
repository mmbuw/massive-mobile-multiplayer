all: 
	g++ -std=c++0x -c *.cpp src/*.cpp -Iinclude/
	g++ -std=c++0x -o main *.o -lsfml-graphics -lsfml-window -lsfml-system
	./main

clean:
	rm -f *.o
	rm -f main