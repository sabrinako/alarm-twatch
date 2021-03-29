/*
   T-Watch project with a focus on robust alarm functionality
*/
#include <Arduino.h> 
#include <WiFi.h> 
#include <EEPROM.h>
#include <soc/rtc.h>
#include <SPIFFS.h>
#include "config.h"
#include "main.h"
#include "alarmFunctions.h"
#include "appWifiTime.h"
#include "appAlarm.h"
#include "appSettings.h"
#include "menu.h"
#include "timeFunctions.h"
#include "utils.h"

LV_IMG_DECLARE(wallpaper);

//uint8_t screen_timeout = 5000;

TTGOClass *main::ttgo;
TFT_eSPI *main::tft;
AXP20X_Class *main::power;
lv_obj_t *main::screen;

uint8_t main::hh, main::mm, main::ss, main::mmonth, main::dday; // H, M, S variables
uint32_t main::day_of_week; //0 = sun -> 7 = sat
uint16_t main::yyear; // Year is 16 bit int

uint32_t targetTime = 0; // for next 1 second display update

//wake up variables
bool irq = false;
bool main::screenOn = true;

//screen drag variables for menu
int16_t x, y = 240;
bool dragUpwards = false;

static void clock_screen_handler(lv_obj_t *obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSING) {
    int16_t current_x, current_y;
    main::ttgo->getTouch(current_x, current_y);

    if (current_y < y && abs(current_x - x) < 10) {
      dragUpwards = true;
    } else {
      dragUpwards = false;
    }
    y = current_y;
    x = current_x;
  }

  if (event == LV_EVENT_RELEASED) {
    x = 240;
    y = 240;
    if (dragUpwards) {
      menu::appMenu();
      dragUpwards = false;
    }
  }
}

void setup() {
  main::ttgo = TTGOClass::getWatch();
  main::ttgo->begin();

  EEPROM.begin(EEPROM_SIZE);

  pinMode(4, OUTPUT); // vibration motor turn on

  // Receive objects for easy writing
  main::tft = main::ttgo->tft;
  main::power = main::ttgo->power;

  // Turn off unused power
  main::power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
  main::power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF); //NO USE
  main::power->setPowerOutPut(AXP202_LDO3, AXP202_OFF); //Backplane power supply
  main::power->setPowerOutPut(AXP202_LDO4, AXP202_OFF); //S76/78G Backplane only

  // Initialize lvgl
  main::ttgo->lvgl_begin();

  main::screen = lv_scr_act();

  lv_obj_t *img1 = lv_img_create(main::screen, NULL);
  lv_img_set_src(img1, &wallpaper);
  lv_obj_set_event_cb(main::screen, clock_screen_handler);

  //Check if the RTC clock matches, if not, use compile time
  main::ttgo->rtc->check();

  //Synchronize time to system time
  main::ttgo->rtc->syncToSystem();

  Serial.begin(115200);
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  timefuncs::timeInit();
  timefuncs::setTime();

  main::ttgo->setBrightness(EEPROM.read(10));
  main::ttgo->openBL(); // Turn on the backlight

  // setting up the interrupt to detect if the power button is pressed
  pinMode(AXP202_INT, INPUT_PULLUP);
  attachInterrupt(AXP202_INT, [] {
    irq = true;
  }, FALLING);

  // Clear IRQ unprocessed first
  main::power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
  main::power->clearIRQ();

  main::day_of_week = main::ttgo->rtc->getDayOfWeek(main::dday, main::mmonth, main::yyear);
}

void loop() {
  timefuncs::setTime();
  lv_task_handler();

  utils::updateBatteryLabel();
  
  if (irq) {
      irq = false;
      main::power->readIRQ();
      if (main::power->isPEKShortPressIRQ()) {
          if (main::screenOn) {
            utils::light_sleep();
          } else {
            utils::wakeup();
          }
      }
      main::power->clearIRQ();
  }

  delay(200);
}