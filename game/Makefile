all: 
	g++ -std=c++0x -c *.cpp src/*.cpp -Iinclude/ -pthread
	g++ -std=c++0x -o main *.o -lsfml-graphics -lsfml-window -lsfml-system -pthread
	./main

clean:
	rm -f *.o
	rm -f main