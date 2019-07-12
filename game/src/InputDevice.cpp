#include "InputDevice.hpp"

InputDevice::InputDevice(int id, std::string const& name, std::string const& fileAddress, Player* playerFigure) : 
	id_(id), name_(name), fileAddress_(fileAddress), playerFigure_(playerFigure), inputValueX_(0), inputValueY_(0), inputButtonA_(false)
{
	deviceFileHandle_ = open(fileAddress_.c_str(), O_RDWR | O_NONBLOCK);

	if (deviceFileHandle_ == -1)
	{
		std::cout << "Cannot open input device " << fileAddress_ << std::endl;
	}

	//send team information back to controller
	writeMSCTeamToDevice();
}

int InputDevice::getDeviceId() const
{
	return id_;
}

Player* InputDevice::getPlayerInstance() const
{
	return playerFigure_;
}

int InputDevice::getDeviceFileHandle() const
{
	return deviceFileHandle_;
}

void InputDevice::writeMSCTeamToDevice()
{
	//Writes the team information back to the device (Event MSC_MAX)
	//Numbers 1 to 99 are red team numbers, 101 to 199 blue team numbers

	struct input_event eventHandle;
	memset(&eventHandle, 0, sizeof(eventHandle));

	int teamOffset;
	sf::Color teamColor = playerFigure_->getTeamColor();
	int playerNumber = playerFigure_->getShirtNumber();

	if (teamColor == sf::Color(255,0,0))
		teamOffset = 0;
	else if (teamColor == sf::Color(0,0,255))
		teamOffset = 100;

	int eventValue = playerNumber + teamOffset;

	eventHandle.type = EV_MSC;
	eventHandle.code = MSC_MAX;
	eventHandle.value = eventValue;

	write(deviceFileHandle_, &eventHandle, sizeof(eventHandle));

	struct input_event syncEventHandle;
	memset(&syncEventHandle, 0, sizeof(syncEventHandle));

	syncEventHandle.type = EV_SYN;
	syncEventHandle.code = SYN_REPORT;
	syncEventHandle.value = 1;

	write(deviceFileHandle_, &syncEventHandle, sizeof(syncEventHandle));
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
		double xPercentage = inputValueX_ / 1.0;
		double yPercentage = inputValueY_ / 1.0;
		double percentToVelFactor = 12.0;

		playerFigure_->addVelocityOffset(xPercentage * percentToVelFactor, yPercentage * percentToVelFactor);
	}

	if (inputButtonA_)
	{
		playerFigure_->shoot();
		inputButtonA_ = false;
	}
}
