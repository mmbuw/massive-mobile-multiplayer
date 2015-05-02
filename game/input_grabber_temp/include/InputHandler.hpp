#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <linux/input.h>

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <set>

#include "InputDevice.hpp"

class InputHandler
{
	public:
		InputHandler();

		void updateDeviceList();
		void processDeviceInputs();
		void addToDevicesIfNeeded(int deviceID, std::string const& eventString);
		std::map<int, InputDevice*>::iterator removeDevice(int deviceID);

	private:
		std::map<int, InputDevice*> currentInputDevices_;


};

#endif //INPUT_HANDLER_HPP