all:
	rm -rf build/
	mkdir build
	g++ -std=c++0x *.cpp src/*.cpp -Iinclude/ -pthread -o build/application-template -lsfml-graphics -lsfml-window -lsfml-system -pthread

clean:
	rm -rf build/
