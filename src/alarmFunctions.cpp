#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <iostream>
#include <bitset>
#include <string>
#include <cstring>
#include <vector>
#include <SPIFFS.h>
#include "main.h"
#include "alarmFunctions.h"
#include "utils.h"

using namespace main;

HTTPClient https;

char dbURL[40] = "https://sheetdb.io/api/v1/your_api_here";

const char* rootCert = \
"-----BEGIN CERTIFICATE-----\n" \
"-----END CERTIFICATE-----\n";

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
};
typedef struct Alarm Alarm;

/* the array of alarms we've set */
Alarm alarms_arr[] = {
  {"Example", 9, 30, 9},
  {"Take Meds", 10, 0, 8}
};

struct NewAlarm {
  char title[12];
  uint8_t hour;
  uint8_t minute;
  std::bitset<8> day_of_week;
};
typedef struct NewAlarm NewAlarm;

std::vector<NewAlarm> alarms_vector;

/*
  Function that tells us if a bit at nth spot is set to 1 (true) or 0 (false)
 */
bool is_bit_set(std::bitset<8> eight_bit, int n) {
	//checking bit status, goes from right to left
  for (int i=0; i<8; i++) {
    Serial.println(eight_bit[i]);
  }
	return (eight_bit.test(n));
}

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

    for ( int i = 0; i < 4; i = i + 1 ) {
      utils::vibrate_watch();
    }
  }
}

void alarmfuncs::alarmCheckNew(uint8_t hh, uint8_t mm, uint32_t dow) {
  auto results = std::find_if(std::begin(alarms_vector), std::end(alarms_vector),
    [hh, mm, dow] (NewAlarm item) {
      return item.hour == hh && item.minute == mm && is_bit_set(item.day_of_week, dow);
    });

  if (results != std::end(alarms_vector)) {
    if (!ttgo->bl->isOn()) {
      utils::wakeup();
    }

    createAlarmPopup(results->title);

    for ( int i = 0; i < 4; i = i + 1 ) {
      utils::vibrate_watch();
    }
  }
}

// from https://github.com/paulmartinetti/twatch-take8
String callHttps(String myUri) {
  utils::wifiConnect();
  WiFiClientSecure *client = new WiFiClientSecure;

  if(client && WiFi.status() == WL_CONNECTED) {
    client -> setCACert(rootCert);

    if (https.begin(*client, myUri)) {
      int httpCode = https.GET();
      if (httpCode > 399) {
        Serial.print("http error");
        https.end();
      } else if (httpCode > 0) {
        // no longer need uri value, so swap w incoming json
        // (saves memory creating another String)
        myUri = https.getString();
      }  else {
        myUri = "[]";
      }
    } else {
      myUri = "[]";
    }
  }

  https.end();
  utils::wifiDisconnect();
  return myUri;
}

void saveAlarmsToSpiff(String alarm_json) {
  File file = SPIFFS.open("/alarms.json", "w");
  file.println(alarm_json);
  file.close();

  File testReadFile = SPIFFS.open("/alarms.json", "r");
  Serial.println(testReadFile.readString());
  testReadFile.close();
}

void readAlarmJson(String alarm_json) {
  alarms_vector.clear();

  DynamicJsonDocument doc(3072);
  DeserializationError err = deserializeJson(doc, alarm_json);

  if (err) {
    Serial.println("Error with deserializing json. Try again");
  }

  for (JsonObject elem : doc.as<JsonArray>()) {
    std::bitset<8> day_of_week;
    const char* title = elem["title"];
    // Serial.println(title);
    const char* dow = elem["day_of_week"];
    for (int i=0; i<8; i++) {
      // Serial.println(dow[i]);
      if(dow[i] == '0') {
          day_of_week.set(i, false);
      }
      else if(dow[i] == '1') {
          day_of_week.set(i, true);
      }
    }

    NewAlarm current_alarm{};
    std::strcpy(current_alarm.title, elem["title"]);
    current_alarm.hour = std::atoi(elem["hour"]);
    current_alarm.minute = std::atoi(elem["minute"]);
    current_alarm.day_of_week = day_of_week;

    alarms_vector.push_back(current_alarm);
  }
}

/*
* Function that makes HTTP call for alarm info, then saves to SPIFFS +
* saves to current working memory
*/
void alarmfuncs::getAlarmsFromSheets() {
  String alarm_json = callHttps(dbURL);
  saveAlarmsToSpiff(alarm_json);
  readAlarmJson(alarm_json);
}
