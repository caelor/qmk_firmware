#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#undef PRODUCT
#define PRODUCT         Gherkin Macro

#undef DESCRIPTION
#define DESCRIPTION     A 30 key + 2 rotary macro pad with RGB lighting

/* Encoders */
// #define ENCODERS_PAD_A { D2, F5 }
// #define ENCODERS_PAD_B { D3, F4 }

#undef BACKLIGHT_PIN
#undef BACKLIGHT_LEVELS

/* LED lighting */
#define RGB_DI_PIN B5
#ifdef RGB_DI_PIN
#define RGBLED_NUM 10
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LIMIT_VAL 64

#define RGBLIGHT_SLEEP
// #define RGBLIGHT_EFFECT_BREATHING
// #define RGBLIGHT_EFFECT_BREATH_MAX 8
#endif

/* MIDI capability */
#define MIDI_ADVANCED

#define LAYOUT_ortho_3x10_rev( \
         K00,   K01,   K02,   K03,   K04,   K05,   K10,   K11,   K12,   K13 , \
         K14,   K15,   K20,   K21,   K22,   K23,   K24,   K25,   K30,   K31 , \
         K32,   K33,   K34,   K35,   K40,   K41,   K42,   K43,   K44,   K45   \
) { \
        { K45,   K44,   K43,   K42,   K41,   K40 }, \
        { K35,   K34,   K33,   K32,   K31,   K30 }, \
        { K25,   K24,   K23,   K22,   K21,   K20 }, \
        { K15,   K14,   K13,   K12,   K11,   K10 }, \
        { K05,   K04,   K03,   K02,   K01,   K00 }  \
}



//UART settings for communication with the ESPHome micro
// On the Gherkin:
//                 TX    RX
// Pro Micro Pin:  D3    D2
// Header Pin:     2     3
//
// Other ESPHome wiring: Header pin 1 (5v), Header pin 9 (gnd)
//
#define SERIAL_UART_BAUD 9600
#define SERIAL_UART_DATA UDR1
#define SERIAL_UART_UBRR (F_CPU / (16UL * SERIAL_UART_BAUD) - 1)
#define SERIAL_UART_TXD_READY (UCSR1A & _BV(UDRE1))
#define SERIAL_UART_RXD_PRESENT (UCSR1A & _BV(RXC1))
#define SERIAL_UART_INIT() do { \
  /* baud rate */ \
  UBRR1L = SERIAL_UART_UBRR; \
  /* baud rate */ \
  UBRR1H = SERIAL_UART_UBRR >> 8; \
  /* enable TX and RX */ \
  UCSR1B = _BV(TXEN1) | _BV(RXEN1); \
  /* 8-bit data */ \
  UCSR1C = _BV(UCSZ11) | _BV(UCSZ10); \
} while(0)
