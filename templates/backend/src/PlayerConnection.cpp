#include "PlayerConnection.hpp"

int PlayerConnection::instance_count = 0;
RegisteredInputs PlayerConnection::registeredInputs = RegisteredInputs();

PlayerConnection::PlayerConnection(sf::TcpSocket* socket) : 
  socket_(socket), uinputHandle_(-1)
{
	++PlayerConnection::instance_count;
	id_ = PlayerConnection::instance_count;
	lastInputTime_ = Clock::now();
	createEventDevice();
}

void PlayerConnection::createEventDevice()
{
	/* set up input device handling */
	uinputHandle_ = open("/dev/uinput", O_RDWR );

	if (uinputHandle_ < 0)
	{
		std::cout << "[Server] Error while creating uinput device." << std::endl;
	}

	//define allowed event types
	for (int i = 0; i < PlayerConnection::registeredInputs.registeredTypes.size(); ++i)
	{
		ioctl(uinputHandle_, UI_SET_EVBIT, PlayerConnection::registeredInputs.registeredTypes[i]);
	}

	//define allowed events
	for (int i = 0; i < PlayerConnection::registeredInputs.registeredEvents.size(); ++i)
	{
		ioctl(uinputHandle_, PlayerConnection::registeredInputs.eventBitsToSet[i], PlayerConnection::registeredInputs.registeredEvents[i]);
	}

	//create event device
	std::stringstream namingStream;
	namingStream << "MMM Controller " << id_ << std::endl;
	std::string deviceName = namingStream.str();

	memset(&eventDevice_, 0, sizeof(eventDevice_));
	snprintf(eventDevice_.name, UINPUT_MAX_NAME_SIZE, deviceName.c_str());

	/* set event device properly */
	eventDevice_.id.bustype = BUS_VIRTUAL;
	eventDevice_.id.version = 1;

	write(uinputHandle_, &eventDevice_, sizeof(eventDevice_));
	ioctl(uinputHandle_, UI_DEV_CREATE);

	lastInputTime_ = Clock::now();
}

PlayerConnection::~PlayerConnection()
{
	delete socket_;
	unregisterEventDevice();
}

void PlayerConnection::injectSingleEvent(int type, int code, int value) const
{
	if (uinputHandle_ != -1)
	{
		struct input_event eventHandle;
		memset(&eventHandle, 0, sizeof(eventHandle));

		eventHandle.type = type;
		eventHandle.code = code;
		eventHandle.value = value;

		write(uinputHandle_, &eventHandle, sizeof(eventHandle));

		eventHandle.type = EV_SYN;
		eventHandle.code = SYN_REPORT;
		eventHandle.value = 1;

		write(uinputHandle_, &eventHandle, sizeof(eventHandle));

		lastInputTime_ = Clock::now();
	}
}

void PlayerConnection::injectMultiEvent(int type, std::vector<int> const& codes, std::vector<int> const& values) const
{
	if (uinputHandle_ != -1)
	{
		int numValues = values.size();

		struct input_event eventHandle[numValues];
		memset(&eventHandle, 0, sizeof(eventHandle));

		for (int i = 0; i < values.size(); ++i)
		{
			eventHandle[i].type = type;
			eventHandle[i].code = codes[i];
			eventHandle[i].value = values[i];
		}

		write(uinputHandle_, &eventHandle, sizeof(eventHandle));

		struct input_event syncEventHandle;
		memset(&syncEventHandle, 0, sizeof(syncEventHandle));

		syncEventHandle.type = EV_SYN;
		syncEventHandle.code = SYN_REPORT;
		syncEventHandle.value = 1;

		write(uinputHandle_, &syncEventHandle, sizeof(syncEventHandle));

		lastInputTime_ = Clock::now();
	}

}

void PlayerConnection::unregisterEventDevice()
{
	if (uinputHandle_ != -1)
		ioctl(uinputHandle_, UI_DEV_DESTROY);
}

bool PlayerConnection::checkAlive() const
{
	Clock::time_point nowTime = Clock::now();
	int elapsedSeconds = (std::chrono::duration_cast<seconds>(nowTime - lastInputTime_)).count();

	if (elapsedSeconds > CONNECTION_TIMEOUT_SECONDS)
	{
		return false;
	}

	return true;
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