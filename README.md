# alarm-twatch
A TTGO TWatch library focused on adding alarm functionality.

## Purpose
I've rotated through various time management tools to keep me on track throughout the day, each with it pros and cons. To try to take the benefits from each tool I've used, I've decided to combine what I've found useful, or wished had existed in other tools, into this watch library.

## Credits
I've drawn inspiration from various sources. This project would not exist without the extensive open source projects and information out there for the TWatch. Here's my best attempt at documenting and crediting these brilliant people.
- [LilyGO's Official Library](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library)
    - [Referenced this code specifically to add wake up/sleep through the external button](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/tree/6cb6fbd89fc9242811b36bccb3888cbb04e86b22/examples/BasicUnit/WakeupFormPEKKey)
- [Dan Geiger's T-Watch Arduino Framework](https://www.instructables.com/Lilygo-T-Watch-2020-Arduino-Framework/)
    - Utilized as the jumping point for this repo and made this seem much more approachable of a project.I've gotten rid of most of the code from this, but still am utilizing the wifi sync for clock
- [Paul Martinetti's T-Watch Project](https://github.com/paulmartinetti/twatch-take8)
    - While I didn't end up using any specific code from this project, I learned a lot about the hardware through attempting to mimic the BMA423 wake up + the links he's provided
    - Also hoping to utilize SheetsDB in a future feature, thank you for introducing me to such a cool service! (As a way to manage alarms remotely)
- [William Dudley's T-Watch Project](https://github.com/wfdudley/T-watch-2020)
    - Once again, no specific code but alerted me to utilizing SPIFFS/EEPROM, custom fonts, general C++ knowledge
- Stack Overflow, my savior and there's no way I can reference everyone/everything I've learned from there for this project (Mainly covered my lacking C++ skills)

## Features
- Custom watch face (font/background)
- Wifi sync of RTC
- Multiple alarms!
    - NOTE: Only work when in light sleep/awake
- Light sleep/wake up through external power button
- Ability to change brightness on watch
    - Brightness value stored in EEPROM

## Known Issues
- Even though the screen is put to sleep during light sleep, it still detects touch
- Alarm screen does not actually do anything yet
- Wifi time sync automatically runs when menu option chosen
- RARE: vibrate alarm goes off endlessly, stops when deep sleeping (hold external button 6 secs)

## How To Build
This project is built using PlatformIO. [Here](https://www.youtube.com/watch?v=7vOaqnmAkcg) is a good example of loading projects through PlatformIO/VSCode.

## TODO
- [ ] Fall asleep from screen inactivity
- [ ] Add battery icon to home screen
- [ ] New alarms + edit alarms through watch OR SheetsDB sync for alarms
- [ ] Save alarm data to SPIFFS
- [ ] Create more utility functions/clean code
- [ ] Utilize memory more efficiently
- [ ] Unit tests
