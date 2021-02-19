#include <ArduinoJson.h>
#include <time.h>
#include <SPIFFS.h>
#include "main.h"
#include "alarmFunctions.h"
#include "utils.h"

using namespace main;

lv_obj_t * alarm_box;

/*
    struct Alarm
    ------------
    data structure created to simulate alarms
    (built in alarm functionality for rtc only allows 1)

    char title: the text that will display when the alarm goes off
    int hour: hour the alarm should go off, in conjunction with minute
    int minute: minute the alarm should go off, in conjunction with hour
    char dayOfWeek: the days of the week this alarm should go off

    dayOfWeek: 0 sunday 7 saturday, 8 everyday 9 weekdays only 10 weekends only 11 weekdays minus friday
    there's a clever way to do this with binary code for the byte but that's a later task
*/
struct Alarm {
  char title[12];
  uint8_t hour;
  uint8_t minute;
  byte day_of_week;
//  char dayOfWeek[8];
};
typedef struct Alarm Alarm;

/* the array of alarms we've set */
Alarm alarms_arr[] = {
  {"Example Alarm", 9, 30, 9},
  {"Take Meds", 10, 00, 8}
};

/*
   handler for button on alarm message pop up
*/
static void alarm_btn_handler(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_VALUE_CHANGED) {
    lv_msgbox_start_auto_close(alarm_box, 10);
  }
}


/*
   create the ui pop up, called if we're expecting an alarm to go off
*/
void alarmfuncs::createAlarmPopup(char *alarm_text) {
  //TODO: add snooze functionality
  static const char * btns[] = {"OK", ""};

  alarm_box = lv_msgbox_create(screen, NULL);
  lv_msgbox_set_text(alarm_box, alarm_text);
  lv_msgbox_add_btns(alarm_box, btns);
  lv_obj_set_width(alarm_box, 200);
  lv_obj_set_event_cb(alarm_box, alarm_btn_handler);
  lv_obj_align(alarm_box, NULL, LV_ALIGN_CENTER, 0, 0);
}


bool alarmfuncs::checkDay(uint32_t day_of_week, byte alarm_setting) {
  switch( alarm_setting) {
    case 8:
      return true;
      break;
    case 9:
      if (day_of_week > 0 && day_of_week < 7) {
        return true;
      }
      break;
    case 10:
      if (day_of_week == 0 || day_of_week == 7) {
        return true;
      }
      break;
    case 11:
      if (day_of_week > 0 && day_of_week < 6) {
        return true;
      }
      break;
    default:
      if (day_of_week == alarm_setting) {
        return true;
      }
      break;
  }
  return false;
}

/*
   function to check if an alarm should be going off at this time
*/
void alarmfuncs::alarmCheck(uint8_t hh, uint8_t mm, uint32_t day_of_week) {
  auto results = std::find_if(std::begin(alarms_arr), std::end(alarms_arr),
    [hh, mm, day_of_week] (Alarm item) {
      return item.hour == hh && item.minute == mm && checkDay(day_of_week, item.day_of_week);
    });
  if (results != std::end(alarms_arr)) {
    if (!ttgo->bl->isOn()) {
      utils::wakeup();
    }

    createAlarmPopup(results->title);

    for ( int i = 0; i < 3; i = i + 1 ) {
      utils::vibrate_watch();
    }
  }
}

/*
* CURRENTLY NOT FUNCTIONAL
*/
void alarmfuncs::getAlarmsFromSpiffs() {
  File file = SPIFFS.open("/alarms.txt", "w+");
  while (file.available()) {
    Serial.write(file.read());
  }
//  StaticJsonDocument<2048> doc;
//  DeserializationError err = deserializeJson(doc, file);
//
//  if (err) {
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(err.f_str());
//    return;
//  }
//  int counter = 0;
//  for (JsonObject elem : doc.as<JsonArray>()) {
//    char title_str[12] = {elem["title"]};
//    Serial.println(title_str);
//    alarms_vector[counter] = {elem["title"], elem["hour"], elem["minute"], elem["day_of_week"]};
//    counter += 1;
//  }
//  Serial.println("The size of vector: ");
//  Serial.println(alarms_vector[3].title);
}
