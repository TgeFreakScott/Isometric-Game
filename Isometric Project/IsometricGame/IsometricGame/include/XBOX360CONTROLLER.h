
#ifndef XBOX360CONTROLLER
#define XBOX360CONTROLLER

#include <SFML\Graphics.hpp>
#include <iostream>

/// <summary>
/// data to store the current state of the controller
/// </summary>
struct GamePadState
{
	bool A{ false };
	bool B{ false };
	bool X{ false };
	bool Y{ false };
	bool LB{ false };
	bool RB{ false };
	bool LeftThumbStickClick{ false };
	bool RightThumbStickClick{ false };
	bool DpadUp{ false };
	bool DpadDown{ false };
	bool DpadLeft{ false };
	bool DpadRight{ false };
	bool Start{ false };
	bool Back{ false };
	bool Xbox{ false };
	float RTrigger{ 0 };
	float LTrigger{ 0 };
	sf::Vector2f RightThumbStick{ 0.0f,0.0f };
	sf::Vector2f LeftThumbStick{ 0.0f,0.0f };

	float leftAngleRAD;
	float rightAngleRAD;

	float leftAngleDEG;
	float rightAngleDEG;
};
	
class Xbox360Controller
{
private:
	// dead zone for the dpad? (works like an another joystick)
	const int dpadThreshhold = 50;

public:
	static int s_noOfControllers;
	// index count if multiple controllers connected
	int sf_Joystick_index;
	// current state of all the buttons
	GamePadState m_currentState;
	// previous state of used to check for the moment button is pressed
	GamePadState m_previousState;

	Xbox360Controller();
	~Xbox360Controller();
	void update();
	bool isConnected();
	bool connect();
};


#endif // !XBOX360CONTROLLER