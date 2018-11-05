#ifndef GAMEMASTER_INITGAMECONTROL_HPP
#define GAMEMASTER_INITGAMECONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "IKeyboardListener.hpp"
#include "message.hpp"
#include "Display.hpp"
#include "EncodeDecodeMSG.hpp"
#include "SendIrMessageControl.hpp"

class InitGameControl: public rtos::task<>, public IKeyboardListener{
private:
	rtos::channel<char, 10> KeyPressedInitQueue;
	
	enum InitGameControlStates = { WaitForKeyC, WaitForNumber, WaitForHash, StartCommand };
	InitGameControlStates currentState;
	
	DisplayControl& displayControl;
	EncodeDecodeMSG& encodeDecoder;
	SendIrMessageControl& sendIrMessageControl;
	
	char pressedKey;
	uint8_t command;
	uint16_t encodedMessage;
	
public:
	InitGameControl(const unsigned int priority, const char* taskName, DisplayControl& _displayControl, EncodeDecodeMSG& _encodeDecoder, SendIrMessageControl& _sendIrMessageControl):
		task(priority, taskName), KeyPressedInitQueue(this, "KeyPressedInitQueue"), displayControl(_displayControl), encodeDecoder(_encodeDecoder),
		sendIrMessageControl(_sendIrMessageControl){currentState = WaitForKeyC;}
	
	void KeyboardKeyPressed(char Key);
	void main();
};

#endif
