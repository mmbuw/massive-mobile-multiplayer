#include "InputDevice.hpp"

InputDevice::InputDevice(int id, std::string const& name, std::string const& fileAddress, Player* playerFigure) : 
	id_(id), name_(name), fileAddress_(fileAddress), playerFigure_(playerFigure), inputValueX_(0), inputValueY_(0), inputButtonA_(false)
{
	deviceFileHandle_ = open(fileAddress_.c_str(), O_RDWR | O_NONBLOCK);

	if (deviceFileHandle_ == -1)
	{
		std::cout << "Cannot open input device " << fileAddress_ << std::endl;
	}

	//ToDo: register player figure in game and send corresponding LED code back
	sf::Color teamColor = playerFigure_->getTeamColor();

	if (teamColor == sf::Color(0, 0, 255))
	{
		writeLEDToDevice(LED_MISC);
	}
	else if (teamColor == sf::Color(255, 0, 0))
	{
		writeLEDToDevice(LED_COMPOSE);
	}

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

void InputDevice::writeLEDToDevice(int code)
{
	// allowed LED codes by the device are LED_MISC and LED_COMPOSE
	// LED_MISC is used to assign player to blue team, LED_COMPOSE for red team

	struct input_event eventHandle;
	memset(&eventHandle, 0, sizeof(eventHandle));

	eventHandle.type = EV_LED;
	eventHandle.code = code;
	eventHandle.value = 1;

	write(deviceFileHandle_, &eventHandle, sizeof(eventHandle));

	eventHandle.type = EV_LED;
	eventHandle.code = code;
	eventHandle.value = 0;

	write(deviceFileHandle_, &eventHandle, sizeof(eventHandle));

	eventHandle.type = EV_SYN;
	eventHandle.code = SYN_REPORT;
	eventHandle.value = 1;

	write(deviceFileHandle_, &eventHandle, sizeof(eventHandle));
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
		double xPercentage = inputValueX_ / 1000.0;
		double yPercentage = inputValueY_ / 1000.0;
		double percentToVelFactor = 4.0;

		//playerFigure_->addVelocityOffset(xPercentage * percentToVelFactor, yPercentage * percentToVelFactor);
		playerFigure_->setVelocity(xPercentage * percentToVelFactor, yPercentage * percentToVelFactor);
	}

	if (inputButtonA_)
	{
		playerFigure_->shoot();
		inputButtonA_ = false;
	}
}