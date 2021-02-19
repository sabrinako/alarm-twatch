#include <Arduino.h>
#include <time.h>
#include "config.h"
#include "main.h"
#include "appAlarm.h"

using namespace main;

void _appNewAlarm();
void _editAlarms();
static void new_alarm_btn_handler(lv_obj_t *obj, lv_event_t event);
static void edit_alarm_btn_handler(lv_obj_t *obj, lv_event_t event);

lv_obj_t *top_alarm_win;

static void new_alarm_btn_handler(lv_obj_t *obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    _appNewAlarm();
  } 
}

static void edit_alarm_btn_handler(lv_obj_t *obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    _editAlarms();
  } 
}

//private function for the new alarm screen
void _appNewAlarm() {
  lv_obj_t *new_alarm_win = lv_win_create(screen, NULL);
  /*Add control button to the header*/
  lv_obj_t * close_btn = lv_win_add_btn(new_alarm_win, LV_SYMBOL_CLOSE);
  /*Add close button and use built-in close action*/
  lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
  lv_win_set_title(new_alarm_win, "New Alarm");
  lv_obj_set_click(new_alarm_win, true);

  // lv_obj_t *hour_roller = lv_roller_create(new_alarm_win, NULL);
  // lv_roller_set_options(hour_roller,
  //   "00\n"
  //   "01\n"
  //   "02\n"
  //   "03\n"
  //   "04\n"
  //   "05\n"
  //   "06\n"
  //   "07\n"
  //   "08\n"
  //   "09\n"
  //   "10\n"
  //   "11\n"
  //   "12\n"
  //   "13\n"
  //   "14\n"
  //   "15\n"
  //   "16\n"
  //   "17\n"
  //   "18\n"
  //   "19\n"
  //   "20\n"
  //   "21\n"
  //   "22\n"
  //   "23",
  //   LV_ROLLER_MODE_INFINITE);
  // lv_roller_set_visible_row_count(hour_roller, 2);
  // lv_obj_align(hour_roller, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);
//  lv_obj_set_event_cb(hour_roller, menu_handler);

  // lv_obj_t *min_roller = lv_roller_create(new_alarm_win, NULL);
  // lv_roller_set_options(min_roller,
  //   "00\n"
  //   "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n"
  //   "11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n"
  //   "21\n22\n23\n24\n25\n26\n27\n28\n\29\n30\n"
  //   "31\n32\n33\n34\n35\n36\n37\n38\n\39\n40\n"
  //   "41\n42\n43\n44\n45\n46\n47\n48\n\49\n50\n"
  //   "51\n52\n53\n54\n55\n56\n57\n58\n\59",
  //   LV_ROLLER_MODE_INFINITE);
  // lv_roller_set_visible_row_count(min_roller, 2);
  // lv_obj_align(min_roller, NULL, LV_ALIGN_IN_TOP_LEFT, 120, 10);

  lv_obj_t *new_alarm_btn = lv_btn_create(top_alarm_win, NULL);
  lv_obj_set_event_cb(new_alarm_btn, new_alarm_btn_handler);
  lv_obj_align(new_alarm_btn, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_t *new_label = lv_label_create(new_alarm_btn, NULL);
  lv_label_set_text(new_label, "Save");
}

void _editAlarms() {
  Serial.println("edit alarm");
}

//enter the new alarm app through this function
void appalarm::appAlarm() {
  top_alarm_win = lv_win_create(screen, NULL);
  /*Add control button to the header*/
  lv_obj_t * close_btn = lv_win_add_btn(top_alarm_win, LV_SYMBOL_CLOSE);
  /*Add close button and use built-in close action*/
  lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
  lv_win_set_title(top_alarm_win, "Alarms");
  lv_obj_set_click(top_alarm_win, true);

  lv_obj_t *new_alarm_btn = lv_btn_create(top_alarm_win, NULL);
  lv_obj_set_event_cb(new_alarm_btn, new_alarm_btn_handler);
  lv_obj_align(new_alarm_btn, NULL, LV_ALIGN_CENTER, 0, -40);
  lv_obj_t *new_label = lv_label_create(new_alarm_btn, NULL);
  lv_label_set_text(new_label, "New");

  lv_obj_t *edit_alarm_btn = lv_btn_create(top_alarm_win, NULL);
  lv_obj_set_event_cb(edit_alarm_btn, edit_alarm_btn_handler);
  lv_obj_align(edit_alarm_btn, NULL, LV_ALIGN_CENTER, 0, 40);
  lv_obj_t *edit_label = lv_label_create(edit_alarm_btn, NULL);
  lv_label_set_text(edit_label, "Edit");
}
