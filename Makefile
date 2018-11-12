#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := Entities.cpp EncodeDecodeMSG.cpp InitGameControl.cpp DisplayControl.cpp

#Full list: Entities.cpp Dummies.cpp Display.cpp MSGDecoderControl.cpp ProcessHitControl.cpp RegisterGameParamsControl.cpp ShootControl.cpp UpdateGametimeControl.cpp DigitLedDisplay.cpp hwlib_TM1637.cpp EncodeDecodeMSG.cpp IrReceiver.cpp SoundDefinitions.cpp Speaker.cpp SpeakerControl.cpp

# header files in this project
HEADERS := ADTs.hpp Entities.hpp EncodeDecodeMSG.hpp IKeyboardListener.hpp InitGameControl.hpp IrTransmitter.hpp Keyboard_4x4.hpp SendIrMessageControl.hpp DisplayControl.hpp

# other places to look for files for this project
SEARCH  := ../Button/lib ../DigitLedDisplay/lib ../DisplayOLED/lib ../EncodeDecodeMSG/lib ../GameMaster ../IRReceiver/lib ../IRSend/lib ../Keyboard_4x4/lib ../Speaker/lib ../Player ../DisplayOLED/lib

# set REATIVE to the next higher directory 
# and defer to the Makefile.native there
RELATIVE := $(RELATIVE)../
include $(RELATIVE)Makefile.due
