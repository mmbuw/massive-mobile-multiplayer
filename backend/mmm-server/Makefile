all:
	g++ -c mmm-server.cpp src/*.cpp -Iinclude/ -std=c++0x 
	g++ -o mmm-server *.o -lsfml-network -lsfml-system -std=c++0x 

clean:
	rm *.o
	rm mmm-server