#include <WiFi.h>
#include "time.h"
#include "config.h"
#include "main.h"
#include "appWifiTime.h"

using namespace main;

lv_obj_t *top_wifi_win;

void appwifi::appWifiTime() {
  top_wifi_win = lv_win_create(screen, NULL);
  /*Add control button to the header*/
  lv_obj_t * close_btn = lv_win_add_btn(top_wifi_win, LV_SYMBOL_CLOSE);
  /*Add close button and use built-in close action*/
  lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
  lv_win_set_title(top_wifi_win, "Wifi Time Sync");
  lv_obj_set_click(top_wifi_win, true);
  lv_obj_t *preload = lv_spinner_create(top_wifi_win, NULL);
  lv_obj_set_size(preload, 100, 100);
  lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 0);

  // WiFi settings ******* Use your network values **********
  const char* ssid     = "your network";
  const char* password = "your password";

  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = -18000;
  const int   daylightOffset_sec = 3600;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {}

  configTime(gmtOffset_sec, daylightOffset_sec , ntpServer, "time.nis.gov");
  delay(3000);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    ttgo->tft->drawString("Failed",  5, 30, 1);
  } else {
    ttgo->tft->setCursor(0, 130);
    ttgo->tft->print(&timeinfo, "%A, %B %d %Y\n%H:%M:%S");
    ttgo->rtc->setDateTime(timeinfo.tm_year, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }

  delay(5000);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  lv_obj_del(top_wifi_win);
//  ttgo->tft->fillScreen(TFT_BLACK);
}
