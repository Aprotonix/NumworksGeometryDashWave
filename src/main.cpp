

#include "display.h"
#include "eadkpp.h"
#include "palette.h"
#include "game.hpp"
#include "string.hpp" //DRAW STRING



extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Wave";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;





int main(int argc, char * argv[]) {
  //EADK::Display::pushRectUniform(EADK::Screen::Rect, Black);
  
  Game game;
  uint64_t last_time;
  uint64_t end_time;
   uint64_t frame_time;
  
  while (true) {
    last_time = eadk_timing_millis();
    game.update();
    if (game.quit) break;

   end_time = eadk_timing_millis();
    frame_time = end_time - last_time;


    if (frame_time > game.MSPT) {
      // We're running behind, no time to sleep.
      continue;
    }

    #if DEBUG
    renderNumber(10, 5, frame_time, White);
    #endif
    EADK::Timing::msleep(game.MSPT - frame_time);

  // eadk_timing_msleep(game.MSPT);
  }
  EADK::Timing::msleep(100);//To avoid go directly on Calculation App when quit game

 
}
