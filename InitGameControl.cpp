#include "hwlib.hpp"
#include "rtos.hpp"

#include "IKeyboardListener.hpp"
#include "message.hpp"
#include "Display.hpp"
#include "EncodeDecodeMSG.hpp"
#include "SendIrMessageControl.hpp"


void InitGameControl::KeyboardKeyPressed(char Key){
	KeyPressedInitQueue.write(Key);
}
void InitGameControl::main(){
	while(true){
		switch(currentState){
			case WaitForKeyC:
				wait(KeyPressedInitQueue);
				pressedKey = KeyPressedInitQueue.read();
				if(pressedKey == 'c'){
					command = 0;
					currentState = WaitForNumber;
				}
			case WaitForNumber:
				displayControl.DisplayString(command, ENTIRE_SCREEN);
				wait(KeyPressedInitQueue);
				pressedKey = KeyPressedInitQueue.read();
				if(pressedKey >= '0' && pressedKey <= '9'){
					command *= 10;
					command += (pressedKey - '0');
					if(command > 15) command = 0;
				}else if(pressedKey == '#' && command != 0){
					encodedMessage = encodeDecoder.EncodeMessage(Message(0, command));
					currentState = WaitForHash;
				}
			case WaitForHash:
				wait(KeyPressedInitQueue);
				pressedKey = KeyPressedInitQueue.read();
				if(pressedKey == '*'){
					encodedMessage = encodeDecoder.EncodeMessage(Message(0, 0));
					displayControl.DisplayString("StartCommand", ENTIRE_SCREEN);
					sendIrMessageControl.sendMessage(encodedMessage);
					currentState = StartCommand;
				}else if(pressedKey == '#'){
					sendIrMessageControl.sendMessage(encodedMessage);
				}
			case StartCommand:
				wait(KeyPressedInitQueue);
				pressedKey = KeyPressedInitQueue.read();
				if(pressedKey == 'c'){
					currentState = WaitForNumber;
				}else if(pressedKey = '*'){
					sendIrMessageControl.sendMessage(encodedMessage);
				}
		}
	}
}
