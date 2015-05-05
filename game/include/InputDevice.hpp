#ifndef INPUT_DEVICE_HPP
#define INPUT_DEVICE_HPP

#include <string>
#include <iostream>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <cstring>

static const std::vector<std::string> evval = {"RELEASED", "PRESSED", "REPEATED"};

class InputDevice
{
	public:
		InputDevice(int id, std::string const& name, std::string const& fileAddress);

		int getDeviceId() const;
		void readValuesAndReact();
		void writeLEDToDevice(int code);

	private:
	    int id_;
	    std::string name_;
	    std::string fileAddress_;
	    int deviceFileHandle_;

	    //ToDo: add player figure in game
};

#endif //INPUT_DEVICE_HPP