#include "InputDevice.hpp"

InputDevice::InputDevice(int id, std::string const& fileAddress) : id_(id), fileAddress_(fileAddress) 
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

void InputDevice::readValuesAndReact()
{
	/* Read values from a /dev/input/event* source */
	/* Source: http://stackoverflow.com/questions/20943322/accessing-keys-from-linux-input-device */

	struct input_event ev;

	size_t read_result = read(deviceFileHandle_, &ev, sizeof(ev));

	if (read_result == (size_t)-1 || read_result != sizeof(ev))
	{
		return;
	}

	// interpret and react to input events (ToDo: forward events to player)
	if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2 && (int) ev.code == 304)
	{
		std::cout << "[Device " << id_ << "] KEY EVENT: " << evval[ev.value] << " BTN_A" << std::endl;
	}
	else if (ev.type == EV_REL && ev.value >= 0 && ev.value <= 1024)
	{
		if ((int) ev.code == 0)
			std::cout << "[Device " << id_ << "] REL EVENT X: " << ev.value << std::endl;
		else if ((int) ev.code == 1)
			std::cout << "[Device " << id_ << "] REL EVENT Y: " << ev.value << std::endl;
	}
}

void InputDevice::writeToDevice()
{
	struct input_event eventHandle;
	memset(&eventHandle, 0, sizeof(eventHandle));

	eventHandle.type = EV_KEY;
	eventHandle.code = BTN_A;
	eventHandle.value = 1;

	write(deviceFileHandle_, &eventHandle, sizeof(eventHandle));

	eventHandle.type = EV_KEY;
	eventHandle.code = BTN_A;
	eventHandle.value = 0;

	write(deviceFileHandle_, &eventHandle, sizeof(eventHandle));

	eventHandle.type = EV_SYN;
	eventHandle.code = SYN_REPORT;
	eventHandle.value = 1;

	write(deviceFileHandle_, &eventHandle, sizeof(eventHandle));
}