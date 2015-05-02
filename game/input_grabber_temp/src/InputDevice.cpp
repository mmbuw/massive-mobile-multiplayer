#include "InputDevice.hpp"

InputDevice::InputDevice(int id, std::string const& fileAddress) : id_(id), fileAddress_(fileAddress) 
{
	deviceFileHandle_ = open(fileAddress_.c_str(), O_RDONLY);

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

	ssize_t read_result = read(deviceFileHandle_, &ev, sizeof(ev));

	if (read_result == (ssize_t)-1)
	{
		if (errno == EINTR)
		{
			return;
		}
		else
		{
			return;
		}
	}
	else
	{
		if (read_result != sizeof(ev))
		{
			errno = EIO;
			return;
		}
	}

	// interpret and react to input events (ToDo: forward events to player)
	if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2 && (int) ev.code == 304)
	{
		std::cout << "KEY EVENT: " << evval[ev.value] << " BTN_A" << std::endl;
	}
	else if (ev.type == EV_REL && ev.value >= 0 && ev.value <= 1024)
	{
		if ((int) ev.code == 0)
			std::cout << "REL EVENT X: " << ev.value << std::endl;
		else if ((int) ev.code == 1)
			std::cout << "REL EVENT Y: " << ev.value << std::endl;
	}
}