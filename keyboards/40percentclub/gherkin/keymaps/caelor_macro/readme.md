# Gherkin

![Gherkin](https://4.bp.blogspot.com/-sQ18-lNZXOc/WCzlTde-4PI/AAAAAAAB_JQ/qQrehAMG6DMKf3i4oj4mkmLGOfTUvb3KgCLcB/s640/IMG_20161116_122926.jpg)
===

30 key macro board based on the Gherkin PCB. The following changes have been made:

* PWM backlight replaced with WS2812 to indicate CapsLock and NumLock status
* A serial connection to an ESPHome board implemented, to allow macro keys to trigger Home Assistant automations and allow Home Assistant to trigger LEDs on the board.
* Default keymap covers media keys and productivity macros.

===

* [The original TMK firmware](https://github.com/di0ib/tmk_keyboard/tree/master/keyboard/gherkin)

Keyboard Maintainer: QMK Community  
Hardware Supported: Gherkin PCB  
Hardware Availability: [Gherkin project on 40% Keyboards](http://www.40percent.club/2016/11/gherkin.html)

Make example for this keyboard (after setting up your build environment):

    make 40percentclub/gherkin_macro:default

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

First pass at adding support for the gherkin keyboard. Compiles but completely
untested. Intended to kick-start development.
