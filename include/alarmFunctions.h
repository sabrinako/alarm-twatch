#include "config.h"

namespace alarmfuncs {
    void createAlarmPopup(char *alarm_text);
    bool checkDay(uint32_t day_of_week, byte alarm_setting);
    extern void alarmCheck(uint8_t hh, uint8_t mm, uint32_t day_of_week);
    extern void alarmCheckNew(uint8_t hh, uint8_t mm, uint32_t dow);
    extern void getAlarmsFromSheets();
}