#include QMK_KEYBOARD_H
#include <print.h>

#define FN1_SPC     LT(1, KC_SPC)
#define FN2_BSPC    LT(2, KC_BSPC)
#define FN3_C       LT(3, KC_C)
#define FN4_V       LT(4, KC_V)
#define FN5_B       LT(5, KC_B)
#define CTL_Z       CTL_T(KC_Z)
#define ALT_X       ALT_T(KC_X)
#define ALT_N       ALGR_T(KC_N)
#define CTL_M       RCTL_T(KC_M)
#define SFT_ENT     RSFT_T(KC_ENT)

#define HSV_OFF  0, 0, 0
#define HSV_ACTIVE_COL  36, 255, 1
#define HSV_ACTIVE_COL2  191, 255, 1
#define HSV_DIM_WHITE  0,0,1

#define ESPHOME_LED_OFFSET 0

#define SERIAL_COMMS_TIMEOUT  10000
#define SERIAL_HEARTBEAT_TIMEOUT 50000
#define SERIAL_SYNC_RETRIES 10
#define SER_OUT_RESET 0

/*
 * QMK <-> ESPHome Protocol
 * 
 * Direction QMK -> ESPHome
 * Packet size: 1 byte
 * 
 *  Packet | Action
 * --------+-------
 *    0    |  Reset comms. ESPHome will reply with 2x packets: 0,0,0,0 and 0,0,0,1.
 *    1    |  Trigger switch down 1
 *    ...  | 
 *    32   |  Trigger switch down 32
 *    33   |  Trigger switch up 1
 *    ...  |
 *    64   |  Trigger switch up 32
 * 
 * 
 * Direction ESPHome -> QMK
 * Packet size: 4 byte
 * 
 *  Packet  | Action
 * ---------+--------
 *  0,0,0,0 | Reset Sequence 1. If received by QMK, read single bytes until "1" is received.
 *  0,0,0,1 | Reset Sequence 2
 *  0,0,0,2 | Heartbeat. If QMK doesn't receive a heartbeat every 10000 iterations, trigger a comms reset.
 *          |
 *  1,r,g,b | Set LED 1 to RGB value
 *  ...     |
 *  6,r,g,b | Set LED 6 to RGB value
 */


enum custom_keycodes {
  WI_TASK = SAFE_RANGE,  // Task View. Win+Tab
  WI_DSKL,  // Virtual Desktop switch to left. Win+Ctrl+Left
  WI_DSKR,  // Virtual Desktop switch to right Win+Ctrl+Right
  WI_DSKN,  // New Virtual desktop. Win+Ctrl+D
  WI_DSKC,  // Close Virtual desktop Win+Ctrl+F4
  WI_ACTN,  // Windows Action Centre. Win+A
  WI_SETT,  // Windows Settings. Win+I
  WI_CORT,  // Cortana/Search. Win+S
  WI_LOCK,  // Lock Machine. Win+L
  WI_SNIP,  // Window Snipping. Win+Shift+S
  WI_HIDD,  // Windows Hidden/Advanced menu. Win+X
  WI_DEVS,  // Windows Devices menu. Win+K
  WI_EMOJ,  // Windows Emoji selector. Win+.
  WI_TDSK,  // Temporary Desktop View. Win+,
  WI_TMAN,  // Open Task Manager. Ctrl+Shift+Esc

  HA_SW1,
  HA_SW2,
  HA_SW3,
  HA_SW4,
  HA_SW5,
  HA_SW6,
  HA_SW7,
  HA_SW8,
  HA_SW9,
  HA_SW10,
  HA_SW11,
  HA_SW12,
  HA_SW13,
  HA_SW14,
  HA_SW15,
  HA_SW16,
  HA_SW17,
  HA_SW18,
  HA_SW19,
  HA_SW20,
  HA_SW21,
  HA_SW22,
  HA_SW23,
  HA_SW24,
  HA_SW25,
  HA_SW26,
  HA_SW27,
  HA_SW28,
  HA_SW29,
  HA_SW30,
  HA_SW31,
  HA_SW32
};

static bool host_is_asleep;
static uint32_t ticks_since_last_heartbeat;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // basic macro pad usage. Encoder 1=volume up/down, Encoder 2=mouse scroll
  [0] = LAYOUT_ortho_3x10_rev(
    TO(0)  , TO(1)  , TO(2)  , TO(3)  , HA_SW23, HA_SW17, HA_SW18, HA_SW19, HA_SW20, KC_VOLU,
    HA_SW13, HA_SW14, HA_SW15, HA_SW16, HA_SW22, WI_SNIP, WI_DSKL, WI_DSKN, WI_DSKR, KC_VOLD,
    MO(4)  , KC_MPRV, KC_MPLY, KC_MNXT, HA_SW21, KC_WSCH, KC_MYCM, KC_CALC, WI_EMOJ, KC_MUTE 
  ),

  // VB Audio remote control/triggers. Encoder 1, C/S F15, Encoder 2, C/S F16
  [1] = LAYOUT_ortho_3x10_rev(
    _______, _______, _______, _______, KC_F13,    KC_F14,    KC_F15,    KC_F16,    KC_F17,    _______,
    _______, _______, _______, _______, C(KC_F13), C(KC_F14), C(KC_F15), C(KC_F16), C(KC_F17), _______,
    _______, _______, _______, _______, S(KC_F13), S(KC_F14), S(KC_F15), S(KC_F16), S(KC_F17), _______ 
  ),

  // cursor keys. Encoder 1=cursor up/down, Encoder 2=page up/down
  [2] = LAYOUT_ortho_3x10_rev(
    _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_UP  , KC_ESC , XXXXXXX, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, _______
  ),

  [3] = LAYOUT_ortho_3x10_rev(
    _______, _______, _______, _______, _______, HA_SW9 , HA_SW10, HA_SW11, HA_SW12, _______,
    _______, _______, _______, _______, _______, HA_SW5 , HA_SW6 , HA_SW7 , HA_SW8 , _______,
    _______, _______, _______, _______, _______, HA_SW1 , HA_SW2 , HA_SW3 , HA_SW4 , _______
  ),

  [4] = LAYOUT_ortho_3x10_rev(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RESET  , XXXXXXX, XXXXXXX, XXXXXXX, KC_WAKE,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_SLEP,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PWR 
  )
};

/* RGB Lighting layers */
const rgblight_segment_t PROGMEM my_layer0_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {0, 1, HSV_DIM_WHITE}
);
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {0, 1, HSV_PURPLE}
);
const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {0, 1, HSV_CORAL}
);
const rgblight_segment_t PROGMEM my_layer3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {0, 1, HSV_GREEN}
);
const rgblight_segment_t PROGMEM my_layer4_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {0, 1, HSV_RED},
  {5, 1, HSV_RED} /* column 6 */
);


const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {4, 1, HSV_RED}
);
const rgblight_segment_t PROGMEM my_numlock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
  {9, 1, HSV_GREEN}
);


const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
  my_layer0_layer,
  my_layer1_layer,
  my_layer2_layer,
  my_layer3_layer,
  my_layer4_layer,
  my_capslock_layer,
  my_numlock_layer
);


void serial_tx(uint8_t data) {
  uint32_t timeout = 0;
  while (!SERIAL_UART_TXD_READY) {
    timeout++;
    if (timeout > SERIAL_COMMS_TIMEOUT) {
      print("Serial comms tx timeout\n");
      break;
    }
  }
  SERIAL_UART_DATA = data;
}

uint8_t serial_rx(void) {
  uint32_t timeout = 0;
  while (!SERIAL_UART_RXD_PRESENT) {
    timeout++;
    if (timeout > SERIAL_COMMS_TIMEOUT) {
      print("Serial comms rx timeout\n");
      return 0;
    }
  }
  return SERIAL_UART_DATA;
}


void keyboard_pre_init_user(void) {
  // Call the keyboard pre init code.
  rgblight_set_layer_state(0, true);

  // Set our LED pins as output
  setPinOutput(D5);
  setPinOutput(B0);
}

void keyboard_post_init_user(void) {
  rgblight_layers = my_rgb_layers;

  rgblight_enable_noeeprom(); // Enables RGB, without saving settings
  rgblight_sethsv_noeeprom(HSV_OFF);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

  print("Initialising serial comms with ESPHome\n");
  SERIAL_UART_INIT();
  ticks_since_last_heartbeat = 0;
}


void suspend_power_down_user(void) {
  host_is_asleep = true;
}

void suspend_wakeup_init_user(void) {
  host_is_asleep = false;
  layer_move(0); // default back to layer 0
}


bool led_update_user(led_t led_state) {
  rgblight_set_layer_state(5, led_state.caps_lock);
  rgblight_set_layer_state(6, led_state.num_lock);

  writePin(B0, !led_state.caps_lock);
  writePin(D5, !led_state.num_lock);

  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  if (!host_is_asleep) {
    rgblight_set_layer_state(0, layer_state_cmp(state, 0));
    rgblight_set_layer_state(1, layer_state_cmp(state, 1));
    rgblight_set_layer_state(2, layer_state_cmp(state, 2));
    rgblight_set_layer_state(3, layer_state_cmp(state, 3));
    rgblight_set_layer_state(4, layer_state_cmp(state, 4));
  }
 
  return state; 
}


void matrix_scan_user(void) {
  ticks_since_last_heartbeat++;
  if (ticks_since_last_heartbeat > SERIAL_HEARTBEAT_TIMEOUT) {
    print("Heartbeat timeout from ESPHome. Sending sync request\n");
    serial_tx(SER_OUT_RESET);
    ticks_since_last_heartbeat = 0; // give ESPHome some time to resync.
  }

  if (SERIAL_UART_RXD_PRESENT) {
    uint8_t pkt_command = serial_rx();
    uint8_t pkt_b2 = serial_rx();
    uint8_t pkt_b3 = serial_rx();
    uint8_t pkt_b4 = serial_rx();

    switch(pkt_command) {
      case 0: // command, sync & admin
        if (pkt_b2 == 0 && pkt_b3 == 0 && pkt_b4 == 0) {
          // sync packet.
          uint32_t sync_timeout = 0;
          while (serial_rx() != 1) {
            sync_timeout++;
            if (sync_timeout > SERIAL_SYNC_RETRIES) {
              print("Timeout looking for sync with ESPHome. Aborting, not synchronised.\n");
              break;
            }
          }
          print("ESPHome Comms Sync complete\n");
        }
        else if (pkt_b2 == 0 && pkt_b3 == 0 && pkt_b4 == 2) {
          // heartbeat
          ticks_since_last_heartbeat = 0;
          print("ESPHome heartbeat received\n");
        }
        break;

      case 1 ... RGBLED_NUM:  // led control. Next packets are h,s,v
        printf("ESPHome set LED %d to %d, %d, %d\n", pkt_command, pkt_b2, pkt_b3, pkt_b4);
        rgblight_setrgb_at(pkt_b2,pkt_b3,pkt_b4, ESPHOME_LED_OFFSET + pkt_command - 1); // adjust to zero offset led index
        break;
      

      default:
        printf("Unknown ESPHome command: %d %d %d %d\n", pkt_command, pkt_b2, pkt_b3, pkt_b4);
        break;
    }
  }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch(keycode) {
      case HA_SW1:
        serial_tx(1);
        break;
      case HA_SW2:
        serial_tx(2);
        break;
      case HA_SW3:
        serial_tx(3);
        break;
      case HA_SW4:
        serial_tx(4);
        break;
      case HA_SW5:
        serial_tx(5);
        break;
      case HA_SW6:
        serial_tx(6);
        break;
      case HA_SW7:
        serial_tx(7);
        break;
      case HA_SW8:
        serial_tx(8);
        break;
      case HA_SW9:
        serial_tx(9);
        break;
      case HA_SW10:
        serial_tx(10);
        break;
      case HA_SW11:
        serial_tx(11);
        break;
      case HA_SW12:
        serial_tx(12);
        break;
      case HA_SW13:
        serial_tx(13);
        break;
      case HA_SW14:
        serial_tx(14);
        break;
      case HA_SW15:
        serial_tx(15);
        break;
      case HA_SW16:
        serial_tx(16);
        break;
      case HA_SW17:
        serial_tx(17);
        break;
      case HA_SW18:
        serial_tx(18);
        break;
      case HA_SW19:
        serial_tx(19);
        break;
      case HA_SW20:
        serial_tx(20);
        break;
      case HA_SW21:
        serial_tx(21);
        break;
      case HA_SW22:
        serial_tx(22);
        break;
      case HA_SW23:
        serial_tx(23);
        break;
      case HA_SW24:
        serial_tx(24);
        break;
      case HA_SW25:
        serial_tx(25);
        break;
      case HA_SW26:
        serial_tx(26);
        break;
      case HA_SW27:
        serial_tx(27);
        break;
      case HA_SW28:
        serial_tx(28);
        break;
      case HA_SW29:
        serial_tx(29);
        break;
      case HA_SW30:
        serial_tx(30);
        break;
      case HA_SW31:
        serial_tx(31);
        break;
      case HA_SW32:
        serial_tx(32);
        break;
    }
  }
  else {
    switch(keycode) {
      case HA_SW1:
        serial_tx(33);
        break;
      case HA_SW2:
        serial_tx(34);
        break;
      case HA_SW3:
        serial_tx(35);
        break;
      case HA_SW4:
        serial_tx(36);
        break;
      case HA_SW5:
        serial_tx(37);
        break;
      case HA_SW6:
        serial_tx(38);
        break;
      case HA_SW7:
        serial_tx(39);
        break;
      case HA_SW8:
        serial_tx(40);
        break;
      case HA_SW9:
        serial_tx(41);
        break;
      case HA_SW10:
        serial_tx(42);
        break;
      case HA_SW11:
        serial_tx(43);
        break;
      case HA_SW12:
        serial_tx(44);
        break;
      case HA_SW13:
        serial_tx(45);
        break;
      case HA_SW14:
        serial_tx(46);
        break;
      case HA_SW15:
        serial_tx(47);
        break;
      case HA_SW16:
        serial_tx(48);
        break;
      case HA_SW17:
        serial_tx(49);
        break;
      case HA_SW18:
        serial_tx(50);
        break;
      case HA_SW19:
        serial_tx(51);
        break;
      case HA_SW20:
        serial_tx(52);
        break;
      case HA_SW21:
        serial_tx(53);
        break;
      case HA_SW22:
        serial_tx(54);
        break;
      case HA_SW23:
        serial_tx(55);
        break;
      case HA_SW24:
        serial_tx(56);
        break;
      case HA_SW25:
        serial_tx(57);
        break;
      case HA_SW26:
        serial_tx(58);
        break;
      case HA_SW27:
        serial_tx(59);
        break;
      case HA_SW28:
        serial_tx(60);
        break;
      case HA_SW29:
        serial_tx(61);
        break;
      case HA_SW30:
        serial_tx(62);
        break;
      case HA_SW31:
        serial_tx(63);
        break;
      case HA_SW32:
        serial_tx(64);
        break;
    }    
  }


  // we only handle the release of other keys
  if (!record->event.pressed) {
    return true;
  }

  switch(keycode) {
    case WI_TASK: // Win+Tab
      SEND_STRING( SS_LWIN( SS_TAP(X_TAB) ) );
      break;

    case WI_DSKL: // Win+Ctrl+Left
      SEND_STRING( SS_LWIN( SS_LCTL( SS_TAP(X_LEFT) )) );
      break;

    case WI_DSKR: // Win+Ctrl+Right
      SEND_STRING( SS_LWIN( SS_LCTL( SS_TAP(X_RIGHT) )) );
      break;

    case WI_DSKN: // Win+Ctrl+D
      SEND_STRING( SS_LWIN( SS_LCTL( SS_TAP(X_D) )) );
      break;

    case WI_DSKC: // Win+Ctrl+F4
      SEND_STRING( SS_LWIN( SS_LCTL( SS_TAP(X_F4) )) );
      break;

    case WI_ACTN: // Win+A
      SEND_STRING( SS_LWIN( SS_TAP(X_A) ) );
      break;

    case WI_SETT: // Win+I
      SEND_STRING( SS_LWIN( SS_TAP(X_I) ) );
      break;

    case WI_CORT: // Win+S
      SEND_STRING( SS_LWIN( SS_TAP(X_S) ) );
      break;

    case WI_LOCK: // Win+L
      SEND_STRING( SS_LWIN( SS_TAP(X_L) ) );
      break;

    case WI_SNIP: // Win+Shift+S
      SEND_STRING( SS_LWIN( SS_LSFT( SS_TAP(X_S) )) );
      break;

    case WI_HIDD: // Win+X
      SEND_STRING( SS_LWIN( SS_TAP(X_X) ) );
      break;

    case WI_DEVS: // Win+K
      SEND_STRING( SS_LWIN( SS_TAP(X_K) ) );
      break;

    case WI_EMOJ: // Win+.
      SEND_STRING( SS_LWIN( SS_TAP(X_DOT) ) );
      break;

    case WI_TDSK: // Win+,
      SEND_STRING( SS_LWIN( SS_TAP(X_COMMA) ) );
      break;

    case WI_TMAN: // Ctrl+Shift+Esc
      SEND_STRING( SS_LCTL( SS_LSFT( SS_TAP(X_ESC) )) );
      break;
  }

  return true;
}
