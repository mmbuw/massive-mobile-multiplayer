#ifndef INPUT_DEVICE_HPP
#define INPUT_DEVICE_HPP

#include <string>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

class InputDevice
{
	public:
		InputDevice(int id, std::string const& fileAddress);

		int getDeviceId() const;
		void readValuesAndReact();

	private:
	    int id_;
	    std::string fileAddress_;

	    int deviceFileHandle_;
};

#endif //INPUT_DEVICE_HPP