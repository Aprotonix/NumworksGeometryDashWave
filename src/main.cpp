// #include "eadkpp.h"

// extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Wave";

// extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

// int main(int argc, char * argv[]) {
//     EADK::Display::pushRectUniform(EADK::Screen::Rect, 0x000000);
//     EADK::Display::drawString("test", EADK::Point(0, 0), false, 0xffffff, 0x000000);
//     return 0;
// }


#include "display.h"
#include "eadkpp.h"
#include "palette.h"
#include "game.hpp"


extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Wave";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;





int main(int argc, char * argv[]) {
  //EADK::Display::pushRectUniform(EADK::Screen::Rect, Black);
  
  Game game;
  while (true) {
    
  

   game.update();
  // if (game.quit) break;
   
  }

 
}
