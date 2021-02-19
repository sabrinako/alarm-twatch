// Menu based on Dan Geiger's framework
//
// Make the following updates:
// 1) Update maxApp to the total number of apps.
// 2) Update appName to add the title of the app.
// 3) in the main routine in TWatch_framework, add a case to the switch statement to call your app routine.
#include <Arduino.h>
#include <stdio.h>
#include "config.h"
#include "main.h"
#include "appAlarm.h"
#include "appSettings.h"
#include "appWifiTime.h"
#include "menu.h"

void menu_handler(lv_obj_t *obj, lv_event_t event);
void appMenu();

lv_obj_t *menu_win;
lv_obj_t *menu_list;

void menu_handler(lv_obj_t *obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    int32_t clicked_index = lv_list_get_btn_index(menu_list, obj);
    switch (clicked_index) {
      case 0:
          appwifi::appWifiTime();
          break;
      case 1:
          appalarm::appAlarm();
          break;
      case 2:
          appsettings::appSettings();
          break;
    }
  }
}


void menu::appMenu() {  
  menu_win = lv_win_create(main::screen, NULL);
  /*Add control button to the header*/
  lv_obj_t * close_btn = lv_win_add_btn(menu_win, LV_SYMBOL_CLOSE);
  /*Add close button and use built-in close action*/
  lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
  lv_win_set_title(menu_win, "Menu");

  lv_obj_set_click(menu_win, true);

  menu_list = lv_list_create(menu_win, NULL);
  lv_obj_set_size(menu_list, 200, 140);
  lv_obj_align(menu_list, NULL, LV_ALIGN_CENTER, 0, 0);

  /*Add buttons to the list*/
  lv_obj_t * list_btn;

  list_btn = lv_list_add_btn(menu_list, LV_SYMBOL_WIFI, "Wifi Time");
  lv_obj_set_event_cb(list_btn, menu_handler);

  list_btn = lv_list_add_btn(menu_list, LV_SYMBOL_PLUS, "Alarm");
  lv_obj_set_event_cb(list_btn, menu_handler);

  list_btn = lv_list_add_btn(menu_list, LV_SYMBOL_SETTINGS, "Settings");
  lv_obj_set_event_cb(list_btn, menu_handler);
}
