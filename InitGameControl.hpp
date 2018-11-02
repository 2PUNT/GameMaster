#ifndef GAMEMASTER_INITGAMECONTROL_HPP
#define GAMEMASTER_INITGAMECONTROL_HPP

#include "rtos.hpp"

#include "IKeyboardListener.hpp"
// De Message ADT is niet beschreven in ADTs.hpp maar deze file gebruikt wel de Message ADT...
// De juiste file moet dus nog #include worden.

class InitGameControl: public rtos::task<>, public IKeyboardListener{
public:
	void KeyboardKeyPressed(char Key);
};

#endif
