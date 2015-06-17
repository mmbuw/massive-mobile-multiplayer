all:
	@echo "Please choose a valid target. Targets differ in the configuration file they use."

mmm-server:
	rm -rf build/
	mkdir build
	rm -rf include/EventDictionary.hpp
	python3 dictionary_builder/create_dictionary.py configs/mmm-ball include/EventDictionary.hpp
	g++ -std=c++0x *.cpp src/*.cpp -I include/ -o build/mmm-server -lsfml-network -lsfml-system 

clean:
	rm -rf build/
