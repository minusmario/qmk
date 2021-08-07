#include QMK_KEYBOARD_H

enum planck_keycodes {
  MARCO_VIM_NEXT = SAFE_RANGE,
  MARCO_VIM_PREVIOUS,
  MARCO_VIM_YANK,
  MARCO_VIM_PASTE,
  MARCO_VIM_FIND,
};

enum planck_layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
  _FN
};

// Tap-Dance
enum {
    COMMA_DOT,
    QUOTE_DQT
};
// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [COMMA_DOT] = ACTION_TAP_DANCE_DOUBLE(KC_COMMA, KC_DOT),
    [QUOTE_DQT] = ACTION_TAP_DANCE_DOUBLE(KC_QUOTE, KC_DQT),
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define FN MO(_FN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT_planck_mit(
    KC_ESCAPE,     KC_Q,    KC_W,   KC_E,   KC_R, KC_T,    KC_Y,KC_U, KC_I,    KC_O,   KC_P,     KC_BSPACE,     \
    LCTL_T(KC_TAB),KC_A,    KC_S,   KC_D,   KC_F, KC_G,    KC_H,KC_J, KC_K,    KC_L,   KC_SCOLON,KC_QUOTE,     \
    KC_LSHIFT,     KC_Z,    KC_X,   KC_C,   KC_V, KC_B,    KC_N,KC_M, KC_COMMA,KC_DOT, KC_SLASH, RSFT_T(KC_ENTER), \
    FN,            KC_LCTRL,KC_LGUI,KC_LALT,LOWER,KC_SPACE,     RAISE,KC_LEFT, KC_DOWN,KC_UP,    KC_RIGHT
  ),

  [_LOWER] = LAYOUT_planck_mit(
    KC_TILD,KC_EXLM,KC_AT,  KC_HASH,KC_DLR, KC_PERC,KC_CIRC,KC_7,   KC_8,KC_9,  KC_LPRN,    KC_RPRN, \
    KC_TRNS,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_4,   KC_5,KC_6,  KC_LBRACKET,KC_RBRACKET, \
    KC_TRNS,KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12, KC_1,   KC_2,KC_3,  KC_LCBR,    KC_RCBR, \
    KC_NO,  KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,        KC_TRNS,KC_0,TD(COMMA_DOT),TD(QUOTE_DQT),KC_MINUS
  ),

  [_RAISE] = LAYOUT_planck_mit(
    KC_GRAVE,KC_NO,          KC_NO,          KC_INSERT,KC_HOME,KC_PGUP,  KC_PAUSE,KC_AMPR, KC_ASTR,            KC_UNDS,             KC_PLUS,            KC_PIPE, \
    KC_TRNS, DYN_MACRO_PLAY1,DYN_MACRO_PLAY2,KC_DELETE,KC_END, KC_PGDOWN,KC_LEFT, KC_DOWN, KC_UP,              KC_RIGHT,            KC_EQUAL,           KC_BSLASH, \
    KC_TRNS, KC_NLCK,        KC_CAPS,        KC_SLCK,    KC_NO,  KC_NO,    KC_NO,   KC_MINUS,KC_PSCREEN,         KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK,KC_AUDIO_VOL_UP, \
    KC_NO,   KC_TRNS,        KC_TRNS,        KC_TRNS,  KC_TRNS,KC_TRNS,           KC_TRNS, LCTL(LGUI(KC_LEFT)),LCTL(LGUI(KC_RIGHT)),KC_MEDIA_PLAY_PAUSE,KC_AUDIO_VOL_DOWN
  ),

  [_ADJUST] = LAYOUT_planck_mit(
    LALT(LCTL(KC_DELETE)),DYN_REC_START1,DYN_REC_START2,KC_NO,  KC_NO,         KC_NO,KC_NO,MARCO_VIM_YANK, KC_NO,   RGB_SPI,RGB_SPD,KC_NO,             \
    KC_TRNS,              KC_NLCK,       KC_SLCK,       KC_NO,  MARCO_VIM_FIND,KC_NO,KC_NO,MARCO_VIM_PASTE,RGB_TOG, RGB_VAI,RGB_VAD,MAGIC_TOGGLE_NKRO, \
    KC_TRNS,              KC_NO,         KC_NO,         KC_NO,  KC_NO,         KC_NO,KC_NO,KC_NO,          RGB_MOD, RGB_HUI,RGB_HUD,RESET, \
    DYN_REC_STOP,         KC_TRNS,       KC_TRNS,       KC_TRNS,KC_TRNS,       KC_NO,      KC_TRNS,        RGB_RMOD,RGB_SAI,RGB_SAD,KC_NO
  ),

  [_FN] = LAYOUT_planck_mit(
  KC_TRNS,KC_7,   KC_8, KC_9,   KC_LPRN, KC_RPRN,KC_PLUS,           KC_MINUS,KC_ASTR,KC_SLASH,      KC_NO,  KC_TRNS,\
  KC_TRNS,KC_4,   KC_5, KC_6,   KC_QUOTE,KC_DQT, MARCO_VIM_PREVIOUS,KC_NO,   KC_NO,  MARCO_VIM_NEXT,KC_NO,  KC_NO,  \
  KC_TRNS,KC_1,   KC_2, KC_3,   KC_COMMA,KC_DOT, KC_LT,             KC_GT,   KC_NO,  KC_NO,         KC_NO,  KC_TRNS,\
  KC_TRNS,KC_TRNS,KC_0, KC_TRNS,KC_NO,   KC_TRNS,                   KC_NO,   KC_TRNS,KC_TRNS,       KC_TRNS,KC_TRNS
  ),
};

extern rgb_config_t rgb_matrix_config;

uint32_t layer_state_set_user(uint32_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
    [1] = {
        {14,255,255}, {105,255,255}, {105,255,255}, {105,255,255}, {249,228,255}, {249,228,255}, {249,228,255}, {35,255,255}, {35,255,255}, {35,255,255}, {134,255,213}, {134,255,213},
        {0,0,255}, {154,255,255}, {154,255,255}, {154,255,255}, {154,255,255}, {154,255,255}, {154,255,255}, {35,255,255}, {35,255,255}, {35,255,255}, {85,203,158}, {85,203,158},
        {0,0,255}, {154,255,255}, {154,255,255}, {154,255,255}, {154,255,255}, {154,255,255}, {154,255,255}, {35,255,255}, {35,255,255}, {35,255,255}, {243,222,234}, {243,222,234},
        {0,0,0}, {0,0,255}, {0,0,255}, {0,0,255}, {105,255,255}, {0,0,0}, {105,255,255}, {35,255,255}, {0,0,255}, {0,0,255}, {0,0,255}
        },

    [2] = {
        {85,203,158}, {0,0,0}, {0,0,0}, {32,255,234}, {32,255,234}, {31,255,255}, {85,203,158}, {0,204,255}, {0,204,255}, {0,204,255}, {0,204,255}, {0,204,255},
        {0,0,255}, {141,255,233}, {141,255,233}, {31,255,255}, {31,255,255}, {31,255,255}, {0,0,255}, {0,0,255}, {0,0,255}, {0,0,255}, {141,255,233}, {141,255,233},
        {0,0,255}, {0,255,255}, {60,255,255}, {240,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,204,255}, {31,255,255}, {205,255,255}, {205,255,255}, {14,222,242},
        {0,0,0}, {0,0,255}, {0,0,255}, {0,0,255}, {105,255,255}, {14,255,255}, {105,255,255}, {0,0,255}, {0,0,255}, {85,203,158}, {14,222,242}
        },

    [3] = {
        {0,183,238}, {134,255,213}, {134,255,213}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {105,255,255}, {0,0,0}, {233,218,217}, {233,218,217}, {0,0,0},
        {0,0,255}, {14,255,255}, {14,255,255}, {0,0,0}, {105,255,255}, {0,0,0}, {0,0,0}, {105,255,255}, {120,255,255}, {14,222,242}, {14,255,255}, {0,255,255},
        {0,0,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {60,255,255}, {154,86,255}, {154,86,255}, {243,222,234},
        {243,222,234}, {0,0,255}, {0,0,255}, {0,0,255}, {105,255,255}, {0,0,0}, {105,255,255}, {60,255,255}, {85,203,158}, {85,203,158}, {0,0,0}
        },

    [4] = {
        {0,0,255}, {141,255,233}, {141,255,233}, {141,255,233}, {33,255,255}, {33,255,255}, {50,153,244}, {50,153,244}, {50,153,244}, {50,153,244}, {0,0,0}, {0,0,255},
        {0,0,255}, {141,255,233}, {141,255,233}, {141,255,233}, {33,255,255}, {33,255,255}, {205,255,255}, {0,0,0}, {0,0,0}, {205,255,255}, {0,0,0}, {0,0,0},
        {0,0,255}, {141,255,233}, {141,255,233}, {141,255,233}, {33,255,255}, {33,255,255}, {33,255,255}, {33,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,255},
        {14,255,255}, {0,0,255}, {141,255,233}, {0,0,255}, {0,0,0}, {250,159,255}, {0,0,0}, {0,0,255}, {0,0,255}, {0,0,255}, {0,0,255}
        },
};

void set_layer_color(int layer) {
  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb( hsv );
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

void rgb_matrix_indicators_user(void) {
  switch (biton32(layer_state)) {
    case 1:
      set_layer_color(1);
      break;
    case 2:
      set_layer_color(2);
      break;
    case 3:
      set_layer_color(3);
      break;
    case 4:
      set_layer_color(4);
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
    if (host_keyboard_led_state().num_lock) {
        rgb_matrix_set_color(12, RGB_RED);
    }
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(13, RGB_YELLOW);
    }
    if (host_keyboard_led_state().scroll_lock) {
        rgb_matrix_set_color(14, RGB_GREEN);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MARCO_VIM_NEXT:
            if (record->event.pressed) {
                SEND_STRING("gt");
            }
            return false;
        case MARCO_VIM_PREVIOUS:
            if (record->event.pressed) {
                SEND_STRING("gT");
            }
            return false;
        case MARCO_VIM_YANK:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_DELAY(100) SS_LSFT(SS_TAP(X_EQUAL)) SS_DELAY(100) SS_TAP(X_Y));
            }
            return false;
        case MARCO_VIM_FIND:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_V) SS_DELAY(100) SS_TAP(X_T) SS_DELAY(100) SS_TAP(X_SCOLON));
            }
            return false;
        case MARCO_VIM_PASTE:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_DELAY(100) SS_LSFT(SS_TAP(X_EQUAL)) SS_DELAY(100) SS_TAP(X_P));
            }
            return false;
    }
    return true;
}