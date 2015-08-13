#include "InputDevice.hpp"

InputDevice::InputDevice(int id, std::string const& name, std::string const& fileAddress) : 
	id_(id), name_(name), fileAddress_(fileAddress), inputValueX_(0), inputValueY_(0), inputButtonA_(false)
{
	deviceFileHandle_ = open(fileAddress_.c_str(), O_RDWR | O_NONBLOCK);

	if (deviceFileHandle_ == -1)
	{
		std::cout << "Cannot open input device " << fileAddress_ << std::endl;
	}
}

int InputDevice::getDeviceId() const
{
	return id_;
}

int InputDevice::getDeviceFileHandle() const
{
	return deviceFileHandle_;
}

void InputDevice::setValueX(int value)
{
	inputValueX_ = value;
}

void InputDevice::setValueY(int value)
{
	inputValueY_ = value;
}

void InputDevice::setButtonA(bool value)
{
	inputButtonA_ = value;
}

void InputDevice::mapValuesAndApply()
{
	if (inputValueX_ != 0 || inputValueY_ != 0)
	{
		/* TODO for application developer: implement proper reaction to relative events */
		std::cout << "Current relative values: " << inputValueX_ << ", " << inputValueY_ << std::endl;
	}

	if (inputButtonA_)
	{
		/* TODO for application developer: implement proper reaction to button event */
		std::cout << "Button A was pressed" << std::endl;

		inputButtonA_ = false;
	}
}
