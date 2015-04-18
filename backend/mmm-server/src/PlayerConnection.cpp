#include "PlayerConnection.hpp"

PlayerConnection::PlayerConnection(int id, sf::IPAddress const& ip, sf::SocketTCP const& socket) : 
	id_(id), ip_(ip), socket_(socket), checkAliveRunning_(true), clientResponding_(true),
	checkAliveThread_(&PlayerConnection::checkPingThreadTask, this)
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
}

PlayerConnection::~PlayerConnection()
{
	unregisterEventDevice();
	checkAliveRunning_ = false;
	checkAliveThread_.join();
}

bool PlayerConnection::isResponding() const
{
	return clientResponding_;
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
}

void PlayerConnection::unregisterEventDevice()
{
	ioctl(uinputHandle_, UI_DEV_DESTROY);
}

void PlayerConnection::checkPingThreadTask()
{
	while (checkAliveRunning_)
	{
		//perform a ping to the client every 5 seconds
		std::stringstream commandStream;
		commandStream << "ping -c 1 -W 1 " << ip_ << " > /dev/null";
		int ret = system(commandStream.str().c_str());

		if (!ret == 0)
		{
			//client did not respond, remove event device
			std::cout << "[Timeout] " << ip_ << " (Client ID " << id_ << ") is not responding, remove event device." << std::endl;
			clientResponding_ = false;
			unregisterEventDevice();
			return;
		}

		//sleep for 5 seconds
		usleep(5000000);
	}
}