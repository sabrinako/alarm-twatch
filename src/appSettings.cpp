#include <EEPROM.h>
#include "config.h"
#include "main.h"
#include "appSettings.h"

using namespace main;

lv_obj_t *settings_win;
static lv_obj_t * slider_label;

char* convert_byte_to_string(byte val) {
  static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
  snprintf(buf, 4, "%u", val);
  return buf;
}

static void slider_event_cb(lv_obj_t * slider, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
      byte value = lv_slider_get_value(slider);
      static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
      snprintf(buf, 4, "%u", lv_slider_get_value(slider));
      lv_label_set_text(slider_label, convert_byte_to_string(value));
      ttgo->setBrightness(value); 
    } else if (event == LV_EVENT_RELEASED) {
      byte value = lv_slider_get_value(slider);
      EEPROM.write(BRIGHTNESS_EEPROM_ADD, value);
      EEPROM.commit();
    }
}

void appsettings::appSettings() {
  byte brightness = EEPROM.read(BRIGHTNESS_EEPROM_ADD);
  
  settings_win = lv_win_create(screen, NULL);
  /*Add control button to the header*/
  lv_obj_t * close_btn = lv_win_add_btn(settings_win, LV_SYMBOL_CLOSE);
  /*Add close button and use built-in close action*/
  lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
  lv_win_set_title(settings_win, "Settings");

  lv_obj_set_click(settings_win, true);

  lv_obj_t *slider = lv_slider_create(settings_win, NULL);
  lv_obj_set_width(slider, 150);
  lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_slider_set_value(slider, brightness, NULL);
  lv_obj_set_event_cb(slider, slider_event_cb);
  lv_slider_set_range(slider, 0, 255);  

  slider_label = lv_label_create(settings_win, NULL);
  lv_label_set_text(slider_label, convert_byte_to_string(brightness));
  lv_obj_set_auto_realign(slider_label, true);
  lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}
