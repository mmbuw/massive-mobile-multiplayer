#ifndef CONFIGURATION_FILE_PARSER
#define CONFIGURATION_FILE_PARSER

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <linux/uinput.h>

struct RegisteredInputs
{
	std::vector<int> registeredTypes;
	std::vector<int> registeredEvents;
	std::vector<int> eventBitsToSet;
};

RegisteredInputs const parseConfigurationFile(std::string filename, std::map<std::string, int>& eventDictionary);

#endif //CONFIGURATION_FILE_PARSER