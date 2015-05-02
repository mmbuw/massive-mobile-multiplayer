#ifndef INPUT_DEVICE_HPP
#define INPUT_DEVICE_HPP

#include <string>

class InputDevice
{
	public:
		InputDevice(int id, std::string const& fileAddress);

		int getDeviceId() const;

	private:
	    int id_;
	    std::string fileAddress_;
};

#endif //INPUT_DEVICE_HPP