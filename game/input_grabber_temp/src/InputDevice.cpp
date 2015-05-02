#include "InputDevice.hpp"

InputDevice::InputDevice(int id, std::string const& fileAddress) : id_(id), fileAddress_(fileAddress) {}

int InputDevice::getDeviceId() const
{
	return id_;
}