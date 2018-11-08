#include "rtos.hpp"
#include "ADTs.hpp"
#include "EncodeDecodeMSG.hpp"
#include "IKeyboardListener.hpp"
#include "InitGameControl.hpp"
#include "IrTransmitter.hpp"
#include "Keyboard_4x4.hpp"
#include "SendIrMessageControl.hpp"


int main(){
// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	hwlib::wait_ms(500);
	
	namespace target = hwlib::target;
	auto out0 = target::pin_oc( target::pins::a0 );
	auto out1 = target::pin_oc( target::pins::a1 );
	auto out2 = target::pin_oc( target::pins::a2 );
	auto out3 = target::pin_oc( target::pins::a3 );
	
	auto in0  = target::pin_in( target::pins::a4 );
	auto in1  = target::pin_in( target::pins::a5 );
	auto in2  = target::pin_in( target::pins::a6 );
	auto in3  = target::pin_in( target::pins::a7 );
	
	auto out_port = hwlib::port_oc_from_pins( out0, out1, out2, out3 );
	auto in_port  = hwlib::port_in_from_pins( in0,  in1,  in2,  in3  );
	auto matrix   = hwlib::matrix_of_switches( out_port, in_port );
	auto keypad   = hwlib::keypad< 16 >( matrix, "123A456B789C*0#D" );
	
	auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
	auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
	auto oled = hwlib::glcd_oled_buffered( i2c_bus, 0x3c );
	
	auto keyboard = Keyboard_4x4<1>(keypad, 1, "TheKeyBoard");
	auto EncoderDecoder = EncodeDecodeMSG();
	auto IR = hwlib::target::d2_36kHz();
	auto IR_LED = IrTransmitter(IR);
	auto SendControl = SendIrMessageControl(IR_LED, 800, 1600,0); //werkt mis niet
	auto display = DisplayControl(oled);
	auto InitControl = InitGameControl(2, "InitControl", display, EncoderDecoder, SendControl);
	
	
	
	IKeyboardListener* keyBoardDummyBase = &InitControl;
	keyboard.setKeyboardListener(keyBoardDummyBase);
	
	
	
	
	rtos::run();
	
	return 0;
}