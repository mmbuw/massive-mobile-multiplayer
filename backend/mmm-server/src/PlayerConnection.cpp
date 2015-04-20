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
	ioctl(uinputHandle_, UI_SET_EVBIT, EV_REL);
	ioctl(uinputHandle_, UI_SET_EVBIT, EV_KEY);
	ioctl(uinputHandle_, UI_SET_EVBIT, EV_SYN);

	//define allowed events
	ioctl(uinputHandle_, UI_SET_RELBIT, REL_X);
	ioctl(uinputHandle_, UI_SET_RELBIT, REL_Y);
	ioctl(uinputHandle_, UI_SET_KEYBIT, BTN_A);

	//create event device
	std::stringstream nameStream;
	nameStream << "MMM-Controller-" << id;

	memset(&eventDevice_, 0, sizeof(eventDevice_));
	snprintf(eventDevice_.name, UINPUT_MAX_NAME_SIZE, nameStream.str().c_str());

	eventDevice_.absmin[REL_X] = 0;
	eventDevice_.absmax[REL_X] = 1023;
	eventDevice_.absmin[REL_Y] = 0;
	eventDevice_.absmax[REL_Y] = 1023;

	/* set event device properties */
	//eventDevice_.id.bustype = BUS_USB;
	//eventDevice_.id.vendor = 0x1234;
	//eventDevice_.id.product = 0xfedc;
	//eventDevice_.id.version = 1;

	write(uinputHandle_, &eventDevice_, sizeof(eventDevice_));
	ioctl(uinputHandle_, UI_DEV_CREATE);

	lastInputTime_ = std::chrono::system_clock::now();
}

PlayerConnection::~PlayerConnection()
{
	unregisterEventDevice();
}

void PlayerConnection::injectKeyEvent(int eventCode) const
{
	struct input_event eventHandle;
	memset(&eventHandle, 0, sizeof(eventHandle));

	eventHandle.type = EV_KEY;
	eventHandle.code = eventCode;
	eventHandle.value = 1;

	write(uinputHandle_, &eventHandle, sizeof(eventHandle));

	eventHandle.type = EV_KEY;
	eventHandle.code = eventCode;
	eventHandle.value = 0;

	write(uinputHandle_, &eventHandle, sizeof(eventHandle));

	eventHandle.type = EV_SYN;
	eventHandle.code = SYN_REPORT;
	eventHandle.value = 1;

	write(uinputHandle_, &eventHandle, sizeof(eventHandle));

	lastInputTime_ = std::chrono::system_clock::now();
}

void PlayerConnection::injectRelEvent(int xCoord, int yCoord) const
{
	struct input_event eventHandle[2];
	memset(&eventHandle, 0, sizeof(eventHandle));

	eventHandle[0].type = EV_REL;
	eventHandle[0].code = REL_X;
	eventHandle[0].value = xCoord;

	eventHandle[1].type = EV_REL;
	eventHandle[1].code = REL_Y;
	eventHandle[1].value = yCoord;

	write(uinputHandle_, &eventHandle, sizeof(eventHandle));

	struct input_event syncEventHandle;
	memset(&syncEventHandle, 0, sizeof(syncEventHandle));

	syncEventHandle.type = EV_SYN;
	syncEventHandle.code = SYN_REPORT;
	syncEventHandle.value = 1;

	write(uinputHandle_, &syncEventHandle, sizeof(syncEventHandle));

	lastInputTime_ = std::chrono::system_clock::now();
}

void PlayerConnection::unregisterEventDevice()
{
	ioctl(uinputHandle_, UI_DEV_DESTROY);
}

bool PlayerConnection::checkAlive() const
{
	std::chrono::time_point<std::chrono::system_clock> nowTime = std::chrono::system_clock::now();
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - lastInputTime_);

	if (ms.count() > CONNECTION_TIMEOUT * 1000)
	{
		return false;
	}

	return true;
}