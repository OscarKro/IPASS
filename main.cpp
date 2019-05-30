#include "hwlib.hpp"

int main( void ){	  
   namespace target = hwlib::target;
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
   auto display = hwlib::glcd_oled( i2c_bus, 0x3c );  
     
   display.clear();
   for( uint16_t i = 10; i < display.size.x - 10; i++ ){
       display.write( hwlib::location( i, 10 ), hwlib::black );
   }
}
