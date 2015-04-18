all:
	g++ -c mmm-server.cpp src/*.cpp -Iinclude/ -lpthread -std=c++0x 
	g++ -o mmm-server *.o -lsfml-network -lsfml-system -lpthread -std=c++0x 

clean:
	rm *.o
	rm mmm-server