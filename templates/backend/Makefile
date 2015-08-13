all:

ifeq ($(CONFIG),)
	$(error Please set a valid CONFIG file. Example: make CONFIG=configs/mmm-ball)
endif

	rm -rf build/
	mkdir build
	rm -rf include/EventDictionary.hpp
	python3 dictionary_builder/create_dictionary.py $(CONFIG) include/EventDictionary.hpp
	g++ -Wno-format-security -std=c++0x *.cpp src/*.cpp -I include/ -o build/mmm-server -lsfml-network -lsfml-system

clean:
	rm -rf build/
	rm -rf include/EventDictionary.hpp
