all:
	g++ -c mmm-server.cpp src/*.cpp -Iinclude/
	g++ -o mmm-server *.o -lsfml-network -lsfml-system

clean:
	rm *.o
	rm mmm-server