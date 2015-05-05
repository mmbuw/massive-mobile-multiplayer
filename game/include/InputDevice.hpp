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

#include "Player.hpp"

static const std::vector<std::string> evval = {"RELEASED", "PRESSED", "REPEATED"};

class InputDevice
{
	public:
		InputDevice(int id, std::string const& name, std::string const& fileAddress, Player* playerFigure);

		int getDeviceId() const;
		void readValuesAndReact();
		void writeLEDToDevice(int code);

	private:
	    int id_;
	    std::string name_;
	    std::string fileAddress_;
	    int deviceFileHandle_;

	    Player* playerFigure_;
};

#endif //INPUT_DEVICE_HPP