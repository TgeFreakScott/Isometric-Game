
#include "XBOX360CONTROLLER.h"

int Xbox360Controller::s_noOfControllers = 1;

Xbox360Controller::Xbox360Controller()
{
	sf_Joystick_index = sf::Joystick::Count;
}

Xbox360Controller::~Xbox360Controller()
{
}

void Xbox360Controller::update()
{
	// x&y is left stick, R&U is right stick while PovX&PovY is the dpad. V&Z are the triggers

	// set previous to current state before current state gets changed
	m_previousState = m_currentState;

	//Button input check.
	m_currentState.A = sf::Joystick::isButtonPressed(0, 0); //A
	m_currentState.B = sf::Joystick::isButtonPressed(0, 1); //B
	m_currentState.X = sf::Joystick::isButtonPressed(0, 2); //X
	m_currentState.Y = sf::Joystick::isButtonPressed(0, 3); //Y

	m_currentState.Start = sf::Joystick::isButtonPressed(0, 7);	//Start
	m_currentState.Back = sf::Joystick::isButtonPressed(0, 6); //Back

	m_currentState.LB = sf::Joystick::isButtonPressed(0, 4);  //LB
	m_currentState.RB = sf::Joystick::isButtonPressed(0, 5);  //RB

	m_currentState.LeftThumbStickClick = sf::Joystick::isButtonPressed(0, 8); //Left Thumb Stick
	m_currentState.RightThumbStickClick = sf::Joystick::isButtonPressed(0, 9);//Right Thumb Stick
	// check dpad presses
	// check for right
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) >= dpadThreshhold)
	{
		m_currentState.DpadRight = true;
	}
	else
	{
		m_currentState.DpadRight = false;
	}
	// check for left
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) <= -1 * (dpadThreshhold))
	{
		m_currentState.DpadLeft = true;
	}
	else
	{
		m_currentState.DpadLeft = false;
	}

	// check for up
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) >= dpadThreshhold)
	{
		m_currentState.DpadUp = true;
	}
	else
	{
		m_currentState.DpadUp = false;
	}

	// check down
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) <= -1 * (dpadThreshhold))
	{
		m_currentState.DpadDown = true;
	}
	else
	{
		m_currentState.DpadDown = false;
	}

	// left thumbstick uses X&Y
	m_currentState.LeftThumbStick = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y));
	// while right thumbstick uses U&R
	m_currentState.RightThumbStick = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U), sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R));

	//get values of the triggers (v/z axis)
	m_currentState.LTrigger = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
	m_currentState.RTrigger = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V);
}

bool Xbox360Controller::isConnected() {
	if (sf_Joystick_index < 0 || sf_Joystick_index > sf::Joystick::Count)
		return false;
	else
		return sf::Joystick::isConnected(sf_Joystick_index);
}

bool Xbox360Controller::connect() {
	for (int j = s_noOfControllers; j < sf::Joystick::Count; j++) {
		if (sf::Joystick::isConnected(j)) 
		{
			sf_Joystick_index = j; s_noOfControllers++;
#ifdef _DEBUG
			std::cout << "connected controller id:" << j;
			sf::Joystick::Identification id = sf::Joystick::getIdentification(0);
			std::cout << "\nVendor ID: " << id.vendorId << "\nProduct ID: " << id.productId << std::endl;
#endif // _DEBUG
			return true;
		}
	}
	return false;
}