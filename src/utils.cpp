#include <Arduino.h>
#include "config.h"
#include "main.h"
#include "timeFunctions.h"
#include "utils.h"

LV_FONT_DECLARE(nunito_semib_24);

void utils::vibrate_watch() {
  digitalWrite(4, HIGH);
  delay(300);
  digitalWrite(4, LOW);
  delay(300);
}

void utils::light_sleep() {
  main::screenOn = false;
  main::power->setPowerOutPut(AXP202_LDO2, false); // v1, battery lasted 6hrs killing display only
  main::ttgo->closeBL();
  main::ttgo->displaySleep();
  setCpuFrequencyMhz(10);
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
  main::ttgo->powerOff();
  esp_light_sleep_start();
}

void utils::wakeup() {
  lv_disp_trig_activity(NULL);
  main::screenOn = true;
  main::power->setPowerOutPut(AXP202_LDO2, true); // v1
  setCpuFrequencyMhz(160);
  main::ttgo->displayWakeup();
  main::ttgo->openBL();
  main::ttgo->rtc->syncToSystem();
}

int utils::getBatteryLvl() {
  // Turn on the battery adc to read the values
  main::ttgo->power->adc1Enable(AXP202_BATT_CUR_ADC1, true);
  int per = main::ttgo->power->getBattPercentage();
  return per;
}

void utils::updateBatteryLabel() {
  int batt_num = getBatteryLvl();
  if (batt_num > 75) {
    lv_label_set_text_fmt(timefuncs::batt_lvl, LV_SYMBOL_BATTERY_FULL "%d", batt_num);
  } else if (batt_num < 75 && batt_num > 50) {
    lv_label_set_text_fmt(timefuncs::batt_lvl, LV_SYMBOL_BATTERY_3 "%d", batt_num);
  } else if (batt_num < 50 && batt_num > 25) {
    lv_label_set_text_fmt(timefuncs::batt_lvl, LV_SYMBOL_BATTERY_2 "%d", batt_num);
  } else {
    lv_label_set_text_fmt(timefuncs::batt_lvl, LV_SYMBOL_BATTERY_1 "%d", batt_num);
  }
}
