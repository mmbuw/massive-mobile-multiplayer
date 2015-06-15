#include "ConfigurationFileParser.hpp"

RegisteredInputs const parseConfigurationFile(std::string filename, std::map<std::string, int>& eventDictionary)
{
	RegisteredInputs output;
	std::ifstream inputFile(filename);

	std::string line;
	int currentType = 0;
	while (std::getline(inputFile, line))
	{
		std::stringstream trimmer(line);
		std::string classification;
		std::string name;
		std::string minimum;
		std::string maximum;

		trimmer >> classification;
		trimmer >> name;
		trimmer >> minimum;
		trimmer >> maximum;

		if (classification == "type")
		{
			output.registeredTypes.push_back(eventDictionary[name]);
			currentType = eventDictionary[name];
		}
		else if (classification == "event")
		{
			output.registeredEvents.push_back(eventDictionary[name]);

			switch (currentType)
			{
				case 1: output.eventBitsToSet.push_back(UI_SET_KEYBIT); break;
				case 2: output.eventBitsToSet.push_back(UI_SET_RELBIT); break;
				case 3: output.eventBitsToSet.push_back(UI_SET_ABSBIT); break;
				case 4: output.eventBitsToSet.push_back(UI_SET_MSCBIT); break;
				case 5: output.eventBitsToSet.push_back(UI_SET_SWBIT); break;
				case 17: output.eventBitsToSet.push_back(UI_SET_LEDBIT); break;
				case 18: output.eventBitsToSet.push_back(UI_SET_SNDBIT); break;
				case 21: output.eventBitsToSet.push_back(UI_SET_FFBIT); break;
				default: output.eventBitsToSet.push_back(-1);
			}

		}
		else
		{
			continue;
		}
	}

	return output;
} 