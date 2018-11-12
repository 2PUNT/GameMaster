#include "InitGameControl.hpp"
#include "hwlib.hpp"

void InitGameControl::KeyboardKeyPressed(char Key){
	KeyPressedInitQueue.write(Key);
	}
void InitGameControl::main(){
	while(true){
		switch(currentState){
			case WaitForKeyC:
				hwlib::cout << "InitGameControl: WaitForKeyC\n";
				displayControl.DisplayString("Press C to start!", StringType::ENTIRE_SCREEN);
				wait(KeyPressedInitQueue);
				pressedKey = KeyPressedInitQueue.read();
				if(pressedKey == 'C'){
					command = 0;
					currentState = WaitForNumber;
				}
				break;
			case WaitForNumber:
				hwlib::cout << "InitGameControl: WaitForNumber\n";
				displayControl.DisplayString(command, StringType::ENTIRE_SCREEN);
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
			break;
			case WaitForHash:
				hwlib::cout << "InitGameControl: WaitForHash\n";
				wait(KeyPressedInitQueue);
				pressedKey = KeyPressedInitQueue.read();
				if(pressedKey == '*'){
					encodedMessage = encodeDecoder.EncodeMessage(Message(0, 0));
					displayControl.DisplayString("StartCommand", StringType::ENTIRE_SCREEN);
					sendIrMessageControl.sendMessage(encodedMessage);
					currentState = StartCommand;
				}else if(pressedKey == '#'){
					hwlib::cout << hwlib::hex << encodedMessage << hwlib::endl;
					hwlib::cout << hwlib::hex << encodeDecoder.DecodeMessage(encodedMessage).data << hwlib::endl;
					sendIrMessageControl.sendMessage(encodedMessage);
				}
			break;
			case StartCommand:
				hwlib::cout << "InitGameControl: StartCommand\n";
				wait(KeyPressedInitQueue);
				pressedKey = KeyPressedInitQueue.read();
				if(pressedKey == 'C'){
					currentState = WaitForNumber;
				}else if(pressedKey == '*'){
					hwlib::cout << hwlib::bin << encodedMessage << hwlib::endl;
					sendIrMessageControl.sendMessage(encodedMessage);
				}
			break;
		}
	}
}
