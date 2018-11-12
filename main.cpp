#include "hwlib.hpp"
#include "rtos.hpp"

#include "ADTs.hpp"
#include "EncodeDecodeMSG.hpp"
#include "IKeyboardListener.hpp"
#include "InitGameControl.hpp"
#include "IrTransmitter.hpp"
#include "Keyboard_4x4.hpp"
#include "SendIrMessageControl.hpp"
#include "DisplayControl.hpp"


int main(){
// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	hwlib::wait_ms(500);
	
	// <<<<<<<<<< Initialization of Data >>>>>>>>>>//
	// <<<<<<<<<< All pins used >>>>>>>>>>//
	hwlib::target::pins playerDisplaySCLPinID = hwlib::target::pins::scl;
	hwlib::target::pins playerDisplaySDAPinID = hwlib::target::pins::sda;
	auto IR = hwlib::target::d2_36kHz();
	// Keyboard:
	hwlib::target::pins out0ID 	= hwlib::target::pins::a0;
	hwlib::target::pins out1ID 	= hwlib::target::pins::a1;
	hwlib::target::pins out2ID 	= hwlib::target::pins::a2;
	hwlib::target::pins out3ID 	= hwlib::target::pins::a3;
	hwlib::target::pins in0ID 	= hwlib::target::pins::a4;
	hwlib::target::pins in1ID 	= hwlib::target::pins::a5;
	hwlib::target::pins in2ID 	= hwlib::target::pins::a6;
	hwlib::target::pins in3ID 	= hwlib::target::pins::a7;
	
	// <<<<<<<<<< All priorities >>>>>>>>>>//
	const unsigned int PriorityInitGameControl = 2;
	const unsigned int PrioritySendIrMessageControl = 0;
	const unsigned int Prioritykeyboard = 1;
	
	//<<<<<<<<<< Creation and Initialization of classes >>>>>>>>>>//
	//<<<<<<<<<< Keyboard_4x4 >>>>>>>>>>//
	auto out0 = hwlib::target::pin_oc( out0ID );
	auto out1 = hwlib::target::pin_oc( out1ID );
	auto out2 = hwlib::target::pin_oc( out2ID );
	auto out3 = hwlib::target::pin_oc( out3ID );
	
	auto in0  = hwlib::target::pin_in( in0ID );
	auto in1  = hwlib::target::pin_in( in1ID );
	auto in2  = hwlib::target::pin_in( in2ID );
	auto in3  = hwlib::target::pin_in( in3ID );
	
	auto out_port = hwlib::port_oc_from_pins( out0, out1, out2, out3 );
	auto in_port  = hwlib::port_in_from_pins( in0,  in1,  in2,  in3  );
	auto matrix   = hwlib::matrix_of_switches( out_port, in_port );
	auto keypad   = hwlib::keypad< 16 >( matrix, "123A456B789C*0#D" );
	auto keyboard = Keyboard_4x4<1>(keypad, Prioritykeyboard, "keyboard");
	
	//<<<<<<<<<< DisplayOLED >>>>>>>>>>//
	auto scl = hwlib::target::pin_oc( playerDisplaySCLPinID );
	auto sda = hwlib::target::pin_oc( playerDisplaySDAPinID );
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
	auto oled = hwlib::glcd_oled_buffered( i2c_bus, 0x3c );
		
	auto w1 = hwlib::window_part(
		oled,
		hwlib::location( 0, 0 ),
		hwlib::location( 128, 32));
		
	auto w2 = hwlib::window_part(
		oled,
		hwlib::location( 0, 32 ),
		hwlib::location( 128, 32));
		
	auto f1 = hwlib::font_default_8x8();
	auto d1 = hwlib::window_ostream( w1, f1 );

	auto f2 = hwlib::font_default_8x8();
	auto d2 = hwlib::window_ostream( w2, f2 );

	auto ose = hwlib::window_part(
		oled, hwlib::location( 0, 0 ),
	hwlib::location( 128, 32));
	
	auto display = DisplayControl(oled,w1, w2, d1, d2, 100);
	
	//<<<<<<<<<< EncodeDecodeMSG >>>>>>>>>>//
	auto encodeDecodeMSG = EncodeDecodeMSG();
	
	//<<<<<<<<<< IRSend >>>>>>>>>>//
	auto IR_LED = IrTransmitter(IR);
	auto SendControl = SendIrMessageControl(IR_LED, 800, 1600,PrioritySendIrMessageControl); //werkt mis niet
	
	//<<<<<<<<<< InitGameControl >>>>>>>>>>//
	auto InitControl = InitGameControl(PriorityInitGameControl, "InitControl", display, encodeDecodeMSG, SendControl);
	IKeyboardListener* InitControlListenerBase = &InitControl;
	keyboard.setKeyboardListener(InitControlListenerBase);
	
	
	rtos::run();
	
	return 0;
}