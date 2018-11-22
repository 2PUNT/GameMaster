#ifndef GAMEMASTER_INITGAMECONTROL_HPP
#define GAMEMASTER_INITGAMECONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "IKeyboardListener.hpp"
#include "ADTs.hpp"
#include "EncodeDecodeMSG.hpp"
#include "SendIrMessageControl.hpp"
#include "DisplayControl.hpp"

/// \brief Control object that manages the initialization of the game.
/// \details InitGameControl manages the gamemaster: register the gametime,
/// sending the gametime and sending the startcommand.
/// This class inherits from rtos::task<> and IKeyboardListener.
class InitGameControl: public rtos::task<>, public IKeyboardListener{
private:
	rtos::channel<char, 10> KeyPressedInitQueue;
	
	enum InitGameControlStates { WaitForKeyC, WaitForNumber, WaitForHash, StartCommand };
	InitGameControlStates currentState;
	
	DisplayControl& displayControl;
	EncodeDecodeMSG& encodeDecoder;
	SendIrMessageControl& sendIrMessageControl;
	
	char pressedKey;
	uint8_t command;
	uint16_t encodedMessage;
	void main();
public:
	/// \brief InitGameControl constructor
	/// \details Constructs an InitGameControl object with the specified parameters.
	/// @param priority The priority of the task.
	/// @param taskName The name of the task, used in debugging.
	/// @param _displayControl A reference to the DisplayControl object this class will use to display data.
	/// @param _encodeDecoder A reference to the EncodeDecodeMSG object this class will use to encode its ir-messages.
	/// @param _sendIrMessageControl A reference to the SendIrMessageControl object this class will use to send its ir-messages.
	InitGameControl(const unsigned int priority, const char* taskName, DisplayControl& _displayControl, EncodeDecodeMSG& _encodeDecoder, SendIrMessageControl& _sendIrMessageControl):
		task(priority, taskName), KeyPressedInitQueue(this, "KeyPressedInitQueue"), displayControl(_displayControl), encodeDecoder(_encodeDecoder),
		sendIrMessageControl(_sendIrMessageControl){currentState = WaitForKeyC;}
	
	/// \brief function to let this class know a key on its keyboard is pressed.
	/// @param Key Specifies which key is pressed.
	void KeyboardKeyPressed(char Key);
};

#endif
