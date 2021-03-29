#include <WiFi.h>
#include "time.h"
#include "config.h"
#include "main.h"
#include "appWifiTime.h"
#include "utils.h"

using namespace main;

void appwifi::appWifiTime() {
  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = -18000;
  const int   daylightOffset_sec = 3600;

  utils::wifiConnect();

  configTime(gmtOffset_sec, daylightOffset_sec , ntpServer, "time.nis.gov");
  delay(3000);

  // struct tm timeinfo;
  // if (!getLocalTime(&timeinfo)) {
  //   ttgo->tft->drawString("Failed",  5, 30, 1);
  // } else {
  //   ttgo->tft->setCursor(0, 130);
  //   ttgo->tft->print(&timeinfo, "%A, %B %d %Y\n%H:%M:%S");
  //   ttgo->rtc->setDateTime(timeinfo.tm_year, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  // }
  // delay(5000);
  
  utils::wifiDisconnect();
}
