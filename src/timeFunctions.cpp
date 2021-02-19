// Script for determining the time and returning
// it to the main framework Ko_Watch
// based on Dan Geiger's framework displayTime
#include <Arduino.h>
#include <stdio.h>
#include <time.h>
#include "config.h"
#include "main.h"
#include "timeFunctions.h"
#include "alarmFunctions.h"

LV_FONT_DECLARE(anonymous_b_84);
LV_FONT_DECLARE(anonymous_32);
LV_FONT_DECLARE(nunito_semib_24);


// first item in this arr is empty because it's a filler for 0 index
String months[] = {
  "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

String daysOfTheWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

static lv_style_t dateStyle;
static lv_style_t clockStyle;
static lv_style_t secClockStyle;

lv_obj_t *timefuncs::date_txt;
lv_obj_t *timefuncs::hr_txt;
lv_obj_t *timefuncs::min_txt;
lv_obj_t *timefuncs::sec_txt;
lv_obj_t *timefuncs::batt_lvl;

void timefuncs::timeInit() {
  /* STYLING */
  lv_style_init(&dateStyle);
  lv_style_set_text_color(&dateStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_text_font(&dateStyle, LV_STATE_DEFAULT, &nunito_semib_24);

  lv_style_init(&clockStyle);
  lv_style_set_text_color(&clockStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_text_font(&clockStyle, LV_STATE_DEFAULT, &anonymous_b_84);

  lv_style_init(&secClockStyle);
  lv_style_set_text_color(&secClockStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_text_font(&secClockStyle, LV_STATE_DEFAULT, &anonymous_32);
  
  timefuncs::date_txt = lv_label_create(main::screen, NULL);
  lv_obj_add_style(timefuncs::date_txt, LV_LABEL_PART_MAIN, &dateStyle);
  lv_obj_align(timefuncs::date_txt, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -15, -76);
  lv_label_set_align(timefuncs::date_txt, LV_LABEL_ALIGN_RIGHT);

  timefuncs::hr_txt = lv_label_create(main::screen, NULL);
  lv_obj_add_style(timefuncs::hr_txt, LV_LABEL_PART_MAIN, &clockStyle);
  lv_obj_align(timefuncs::hr_txt, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 5, -72);
  lv_label_set_align(timefuncs::hr_txt, LV_LABEL_ALIGN_CENTER);

  timefuncs::min_txt = lv_label_create(main::screen, NULL);
  lv_obj_add_style(timefuncs::min_txt, LV_LABEL_PART_MAIN, &clockStyle);
  lv_obj_align(timefuncs::min_txt, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 5, -10);
  lv_label_set_align(timefuncs::min_txt, LV_LABEL_ALIGN_CENTER);

  timefuncs::sec_txt = lv_label_create(main::screen, NULL);
  lv_obj_add_style(timefuncs::sec_txt, LV_LABEL_PART_MAIN, &secClockStyle);
  lv_obj_align(timefuncs::sec_txt, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 100, -10);
  lv_label_set_align(timefuncs::sec_txt, LV_LABEL_ALIGN_CENTER);

  timefuncs::batt_lvl = lv_label_create(main::screen, NULL);
  lv_obj_add_style(timefuncs::batt_lvl, LV_LABEL_PART_MAIN, &dateStyle);
  lv_obj_align(timefuncs::batt_lvl, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 10);
  lv_label_set_align(timefuncs::batt_lvl, LV_LABEL_ALIGN_RIGHT);
}

void timefuncs::setTime() {
  // Get the current data
  RTC_Date tnow = main::ttgo->rtc->getDateTime();

  main::hh = tnow.hour;
  main::mm = tnow.minute;
  main::ss = tnow.second;
  main::dday = tnow.day;
  main::mmonth = tnow.month;
  main::yyear = tnow.year;

  if ( main::hh == 0 && main::mm == 0 ) { // only update the day of week at midnight
    main::day_of_week = main::ttgo->rtc->getDayOfWeek(main::dday, main::mmonth, main::yyear);
  }
  
  lv_label_set_text_fmt(date_txt, "%s\n%s\n%d\n%d", daysOfTheWeek[main::day_of_week], months[main::mmonth], main::dday, main::yyear);

  if (main::ss == 0) {
    alarmfuncs::alarmCheck(main::hh, main::mm, main::day_of_week); // needs to be called before we start changing the time #s for display
  }

  if (main::hh > 12) {
    main::hh = main::hh - 12;
  }

  if (main::hh < 10) {
    lv_label_set_text_fmt(hr_txt, "0%d", main::hh);
  } else {
    lv_label_set_text_fmt(hr_txt, "%d", main::hh);
  }
  
  if (main::mm < 10) {
    lv_label_set_text_fmt(min_txt, "0%d", main::mm);
  } else {
    lv_label_set_text_fmt(min_txt, "%d", main::mm);
  }

  if (main::ss < 10) {
    lv_label_set_text_fmt(sec_txt, "0%d", main::ss);
  } else {
    lv_label_set_text_fmt(sec_txt, "%d", main::ss);
  }
}
