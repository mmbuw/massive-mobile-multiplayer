//Automatically created on every build

#ifndef EVENT_DICTIONARY_HPP
#define EVENT_DICTIONARY_HPP

#include <linux/input.h>
#include <map>
#include <string>

std::map<std::string, int> eventDictionary;

void fillDictionary()
{
    eventDictionary["EV_SYN"] = EV_SYN;
    eventDictionary["EV_REL"] = EV_REL;
    eventDictionary["EV_ABS"] = EV_ABS;
    eventDictionary["EV_MSC"] = EV_MSC;
    eventDictionary["EV_KEY"] = EV_KEY;

    eventDictionary["ABS_X"] = ABS_X; 
    eventDictionary["ABS_Y"] = ABS_Y;
    eventDictionary["BTN_A"] = BTN_A;
    eventDictionary["MSC_MAX"] = MSC_MAX;
 
}

#endif //EVENT_DICTIONARY_HPP