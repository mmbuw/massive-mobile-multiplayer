#include "PlayerConnection.hpp"

PlayerConnection::PlayerConnection(int id, sf::IPAddress const& ip, sf::SocketTCP const& socket) : 
	id_(id), ip_(ip), socket_(socket) 
{
	//create handle to /dev/uinput
	uinputHandle_ = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if (uinputHandle_ < 0)
	{
		std::cout << "Error: creating uinput handle failed." << std::endl;
	}

	//define allowed event types
	ioctl(uinputHandle_, UI_SET_EVBIT, EV_KEY);
	ioctl(uinputHandle_, UI_SET_EVBIT, EV_SYN);

	//define allowed events
	ioctl(uinputHandle_, UI_SET_KEYBIT, KEY_D);

	//create event device
	std::stringstream nameStream;
	nameStream << "MMM-Controller-" << id;

	memset(&eventDevice_, 0, sizeof(eventDevice_));
	snprintf(eventDevice_.name, UINPUT_MAX_NAME_SIZE, nameStream.str().c_str());

	/* set event device properties */
	//eventDevice_.id.bustype = BUS_USB;
	//eventDevice_.id.vendor = 0x1234;
	//eventDevice_.id.product = 0xfedc;
	//eventDevice_.id.version = 1;

	write(uinputHandle_, &eventDevice_, sizeof(eventDevice_));
	ioctl(uinputHandle_, UI_DEV_CREATE);
}

PlayerConnection::~PlayerConnection()
{
	ioctl(uinputHandle_, UI_DEV_DESTROY);
}

void PlayerConnection::injectEvent(int eventType, int eventCode) const
{
	struct input_event eventHandle;
	memset(&eventHandle, 0, sizeof(eventHandle));

	eventHandle.type = eventType;
	eventHandle.code = eventCode;
	eventHandle.value = 1;

	write(uinputHandle_, &eventHandle, sizeof(eventHandle));

	eventHandle.type = eventType;
	eventHandle.code = eventCode;
	eventHandle.value = 0;

	write(uinputHandle_, &eventHandle, sizeof(eventHandle));

	eventHandle.type = EV_SYN;
	eventHandle.code = SYN_REPORT;
	eventHandle.value = 1;

	write(uinputHandle_, &eventHandle, sizeof(eventHandle));
}