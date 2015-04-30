#include "PlayerConnection.hpp"

int PlayerConnection::instance_count = 0;

PlayerConnection::PlayerConnection(sf::TcpSocket* socket) : 
  socket_(socket) 
{
	++PlayerConnection::instance_count;
	id_ = PlayerConnection::instance_count;

	/* set up input device handling */
	uinputHandle_ = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if (uinputHandle_ < 0)
	{
		std::cout << "[Server] Error while creating uinput device." << std::endl;
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
	std::stringstream namingStream;
	namingStream << "MMM Controller " << id_;
	std::string deviceName = namingStream.str();

	memset(&eventDevice_, 0, sizeof(eventDevice_));
	snprintf(eventDevice_.name, UINPUT_MAX_NAME_SIZE, deviceName.c_str());

	eventDevice_.absmin[REL_X] = 0;
	eventDevice_.absmax[REL_X] = 1023;
	eventDevice_.absmin[REL_Y] = 0;
	eventDevice_.absmax[REL_Y] = 1023;

	/* set event device properly */
	//eventDevice_.id.bustype = BUS_USB;
	//eventDevice_.id.vendor = 0x1234;
	//eventDevice_.id.product = 0xfedc;
	//eventDevice_.id.version = 1;

	write(uinputHandle_, &eventDevice_, sizeof(eventDevice_));
	ioctl(uinputHandle_, UI_DEV_CREATE);
}

PlayerConnection::~PlayerConnection()
{
	delete socket_;
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

bool PlayerConnection::checkAlive() const
{
	/*std::chrono::time_point<std::chrono::system_clock> nowTime = std::chrono::system_clock::now();
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - lastInputTime_);

	if (ms.count() > CONNECTION_TIMEOUT * 1000)
	{
		return false;
	}

	return true;*/
}

void PlayerConnection::sendViaSocket(std::string const& message)
{
	/* "Encoding" the message using the WebSocket protocol */
    /* (http://stackoverflow.com/questions/8125507/how-can-i-send-and-receive-websocket-messages-on-the-server-side) */

	//Maximum message length is 125 (enough for our implementation)
	if (message.size() > 125)
	{
		std::cout << "[Error] Maximum socket message send size is 125." << std::endl;
		return;
	}

	int messageLength(message.size());
	char bytesFormatted[message.size() + 2];

	bytesFormatted[0] = 129;
	bytesFormatted[1] = (char) messageLength;

	for (int i = 2; i < messageLength + 2; ++i)
	{
		bytesFormatted[i] = message[i-2];
	}

	if (socket_->send(bytesFormatted, sizeof(bytesFormatted)) != sf::Socket::Done)
	{
		std::cout << "[Error] Sending message to client failed." << std::endl;
	}
}

int PlayerConnection::getID() const
{
	return id_;
}

sf::IpAddress const PlayerConnection::getIP()
{
	return socket_->getRemoteAddress();
}

sf::TcpSocket* PlayerConnection::getSocket()
{
	return socket_;
}