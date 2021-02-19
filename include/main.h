#include "config.h"

namespace main {
    extern TTGOClass *ttgo;
    extern TFT_eSPI *tft;
    extern AXP20X_Class *power;
    extern lv_obj_t *screen;

    extern uint8_t hh, mm, ss, mmonth, dday; // H, M, S variables
    extern uint32_t day_of_week; //0 = sun -> 7 = sat
    extern uint16_t yyear; // Year is 16 bit int

    extern bool screenOn;
}