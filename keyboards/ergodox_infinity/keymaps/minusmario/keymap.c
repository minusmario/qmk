#include QMK_KEYBOARD_H
#include "version.h"
#include "layers.h"

enum custom_keycodes {
  MARCO_VIM_NEXT = SAFE_RANGE,
  MARCO_VIM_PREVIOUS,
  MARCO_VIM_YANK,
  MARCO_VIM_PASTE,
  MARCO_VIM_FIND,
};
// Tap-Dance
enum tap_dance {
    COMMA_DOT,
    QUOTE_DQT,
    MINUS_PLUS
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [COMMA_DOT] = ACTION_TAP_DANCE_DOUBLE(KC_COMMA, KC_DOT),
    [QUOTE_DQT] = ACTION_TAP_DANCE_DOUBLE(KC_QUOTE, KC_DQT),
    [MINUS_PLUS] = ACTION_TAP_DANCE_DOUBLE(KC_MINUS, KC_PLUS)
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define NUMP MO(_NUMPAD)

/********animation related start********/
char wpm_str[10];
 // WPM-responsive animation stuff here
#define IDLE_FRAMES 5
#define IDLE_SPEED 40 // below this wpm value your animation will idle

// #define PREP_FRAMES 1 // uncomment if >1

#define TAP_FRAMES 2
#define TAP_SPEED 60 // above this wpm value typing animation to triggere

#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
#define ANIM_SIZE 512 // number of bytes in array, minimize for adequate firmware size, max is 1024

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;
// uint8_t current_prep_frame = 0; // uncomment if PREP_FRAMES >1
uint8_t current_tap_frame = 0;
/********animation related end********/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BASE] = LAYOUT_ergodox(
  KC_ESCAPE,     KC_1,   KC_2,   KC_3,   KC_4, KC_5,KC_MINUS,
  KC_TAB,        KC_Q,   KC_W,   KC_E,   KC_R, KC_T,KC_LPRN,
  LCTL_T(KC_TAB),KC_A,   KC_S,   KC_D,   KC_F, KC_G,
  KC_LSFT,       KC_Z,   KC_X,   KC_C,   KC_V, KC_B,KC_LBRC,
  NUMP,          KC_LCTL,KC_LGUI,KC_LALT,LOWER,
                                                           MARCO_VIM_PREVIOUS,MARCO_VIM_NEXT,
                                                                              KC_PGUP,
                                                    KC_SPC,KC_BSPC,           KC_PGDN,

                 KC_EQL, KC_6, KC_7, KC_8,   KC_9,   KC_0,   KC_BSPC,
                 KC_RPRN,KC_Y, KC_U, KC_I,   KC_O,   KC_P,   KC_BSLS,
                         KC_H, KC_J, KC_K,   KC_L,   KC_SCLN,KC_QUOT,
                 KC_RBRC,KC_N, KC_M, KC_COMM,KC_DOT, KC_SLSH,KC_RSFT,
                               RAISE,KC_LEFT,KC_DOWN,KC_UP,  KC_RIGHT,
  KC_INS, KC_DEL,
  KC_HOME,
  KC_END, KC_TAB, KC_ENT
  ),
[_LOWER] = LAYOUT_ergodox(
  KC_TRNS,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F11,
  KC_TILD,KC_EXLM,KC_AT,  KC_HASH,KC_DLR, KC_PERC,KC_NO,
  KC_TRNS,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,
  KC_TRNS,KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_NO,
  KC_NO,  KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                                        KC_TRNS,KC_TRNS,
                                                                KC_TRNS,
                                                KC_TRNS,KC_TRNS,KC_TRNS,

                  KC_F12, KC_F6,  KC_F7,  KC_F8,KC_F9,        KC_F10,       KC_TRNS,
                  KC_TRNS,KC_CIRC,KC_7,   KC_8, KC_9,         KC_LPRN,      KC_RPRN,
                          KC_F6,  KC_4,   KC_5, KC_6,         KC_LBRC,      KC_RBRC,
                  KC_TRNS,KC_F12, KC_1,   KC_2, KC_3,         KC_LCBR,      KC_RCBR,
                                  KC_TRNS,KC_0, TD(COMMA_DOT),TD(QUOTE_DQT),TD(MINUS_PLUS),
  KC_TRNS,KC_TRNS,
  KC_TRNS,
  KC_TRNS,KC_TRNS,KC_TRNS
),
[_RAISE] = LAYOUT_ergodox(
  KC_TRNS, KC_TRNS,            KC_TRNS,             KC_TRNS,  KC_TRNS,KC_TRNS, KC_TRNS,
  KC_GRAVE,KC_PAUSE,           KC_PSCR,             KC_INS,   KC_HOME,KC_PGUP, KC_TRNS,
  KC_TRNS, LCTL(LGUI(KC_LEFT)),LCTL(LGUI(KC_RIGHT)),KC_DELETE,KC_END, KC_PGDOWN,
  KC_TRNS, KC_NLCK,            KC_CAPS,             KC_SLCK,  KC_NO,  KC_NO, KC_TRNS,
  KC_NO,   KC_TRNS,            KC_TRNS,             KC_TRNS,  KC_TRNS,
                                                                                      KC_TRNS,KC_TRNS,
                                                                                              KC_TRNS,
                                                                              KC_TRNS,KC_TRNS,KC_TRNS,

  KC_TRNS,KC_TRNS,   KC_TRNS, KC_TRNS,            KC_TRNS,            KC_TRNS,        KC_TRNS,
  KC_TRNS,LGUI(KC_D),KC_AMPR, KC_ASTR,            KC_UNDS,            KC_PLUS,        KC_PIPE,
          KC_LEFT,   KC_DOWN, KC_UP,              KC_RIGHT,           KC_EQUAL,       KC_BSLS,
  KC_TRNS,KC_NO,     KC_MINUS,KC_NO,              KC_MEDIA_PLAY_PAUSE,KC_MUTE,        KC_NO,
                     KC_TRNS, KC_MEDIA_PREV_TRACK,KC_AUDIO_VOL_DOWN,  KC_AUDIO_VOL_UP,KC_MEDIA_NEXT_TRACK,
  KC_TRNS,KC_TRNS,
  KC_TRNS,
  KC_TRNS,KC_TRNS,KC_TRNS
),
[_ADJUST] = LAYOUT_ergodox(
  KC_NO,                KC_NO,          KC_NO,          KC_NO,  KC_NO,         KC_NO, KC_NO,
  LALT(LCTL(KC_DELETE)),DYN_REC_START1, DYN_REC_START2, KC_NO,  KC_NO,         KC_NO, KC_NO,
  KC_TRNS,              DYN_MACRO_PLAY1,DYN_MACRO_PLAY2,KC_NO,  MARCO_VIM_FIND,KC_NO,
  KC_TRNS,              KC_NO,          KC_NO,          KC_NO,  KC_NO,         KC_NO, KC_NO,
  DYN_REC_STOP,         KC_TRNS,        KC_TRNS,        KC_TRNS,KC_TRNS,
                                                                                      KC_TRNS,KC_TRNS,
                                                                                              KC_TRNS,
                                                                              KC_TRNS,KC_TRNS,KC_TRNS,

  KC_NO,KC_NO,KC_NO,          KC_NO,  KC_NO, KC_NO, KC_NO,
  KC_NO,KC_NO,MARCO_VIM_YANK, KC_NO,  KC_NO, KC_NO, KC_NO,
        KC_NO,MARCO_VIM_PASTE,BL_TOGG,BL_INC,BL_DEC,MAGIC_TOGGLE_NKRO,
  KC_NO,KC_NO,KC_NO,          BL_STEP,KC_NO, KC_NO, RESET,
              KC_TRNS,        KC_NO  ,KC_NO, KC_NO, EEPROM_RESET,
  KC_TRNS,KC_TRNS,
  KC_TRNS,
  KC_TRNS,KC_TRNS,KC_TRNS
),
[_NUMPAD] = LAYOUT_ergodox(
  KC_NO,  KC_NO,   KC_NO,KC_NO,   KC_NO,   KC_NO,  KC_NO,
  KC_TRNS,KC_7,    KC_8, KC_9,    KC_LPRN, KC_RPRN,KC_NO,
  KC_TRNS,KC_4,    KC_5, KC_6,    KC_QUOTE,KC_DQT,
  KC_TRNS,KC_1,    KC_2, KC_3,    KC_COMMA,KC_DOT, KC_NO,
  KC_TRNS,KC_SPACE,KC_0, KC_ENTER,KC_NO,
                                                        KC_TRNS,KC_TRNS,
                                                                KC_TRNS,
                                                KC_TRNS,KC_TRNS,KC_TRNS,

  KC_NO,KC_NO,             KC_NO,   KC_NO,  KC_NO,         KC_NO,  KC_NO,
  KC_NO,KC_PLUS,           KC_MINUS,KC_ASTR,KC_SLASH,      KC_NO,  KC_TRNS,
        MARCO_VIM_PREVIOUS,KC_NO,   KC_NO,  MARCO_VIM_NEXT,KC_NO,  KC_NO,
  KC_NO,KC_LT,             KC_GT,   KC_NO,  KC_NO,         KC_NO,  KC_TRNS,
                           KC_NO,   KC_TRNS,KC_TRNS,       KC_TRNS,KC_TRNS,
  KC_TRNS,KC_TRNS,
  KC_TRNS,
  KC_TRNS,KC_TRNS,KC_TRNS
),
};

/********animation related start********/
// Images credit j-inc(/James Incandenza) and pixelbenny. Credit to obosob for initial animation approach.
static void render_anim(void) {
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
        {
         0,  0,252,252,  4,  4,  4,  4,  4,  0,252,  4,  4,  4,  4,  4,252,252,  0,252,252,  0,  0,  0,252,252,  0,  0,  0,252,252, 36, 36, 36,252,  0,252,252,  4,  4,136,112,  0,252,252,  4,  4,  4,  4,252,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  4,  4,  4,  8, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1, 25,100,130,  2,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  7, 31,  7, 31,  7, 28,  7, 31,  7, 31,  7,  2,  2,  0,  0,  0,  0
        },
        {
        0,  0,252,252,  4,  4,  4,  4,  4,  0,252,  4,  4,  4,  4,  4,252,252,  0,252,252,  0,  0,  0,252,252,  0,  0,  0,252,252, 36, 36, 36,252,  0,252,252,  4,  4,136,112,  0,252,252,  4,  4,  4,  4,252,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  4,  4,  4,  8, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1, 25,100,130,  2,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  7, 31,  7, 31,  7, 28,  7, 31,  7, 31,  7,  2,  2,  0,  0,  0,  0
        },
        {
        0,  0,252,252,  4,  4,  4,  4,  4,  0,252,  4,  4,  4,  4,  4,252,252,  0,252,252,  0,  0,  0,252,252,  0,  0,  0,252,252, 36, 36, 36,252,  0,252,252,  4,  4,136,112,  0,252,252,  4,  4,  4,  4,252,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16,  8,  4,  2,  2,  4, 24, 96,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1, 61,194,  1,  1,  2,  2,  4,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 96, 96,  0,129,130,130,132,  8, 16, 32, 64,128,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 25,  6,  0,  0,  0,  0,  0,  0,  0, 24, 24, 24, 27,  3,  0, 64,160, 34, 36, 20, 18, 18, 18, 11,  8,  8,  8,  8,  5,  5,  9,  9, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  7, 31,  7, 31,  7, 28,  7, 31,  7, 31,  7,  2,  2,  0,  0,  0,  0
        },
        {
        0,  0,252,252,  4,  4,  4,  4,  4,  0,252,  4,  4,  4,  4,  4,252,252,  0,252,252,  0,  0,  0,252,252,  0,  0,  0,252,252, 36, 36, 36,252,  0,252,252,  4,  4,136,112,  0,252,252,  4,  4,  4,  4,252,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1, 31,225,  0,  0,  1,  1,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  7, 31,  7, 31,  7, 28,  7, 31,  7, 31,  7,  2,  2,  0,  0,  0,  0
        },
        {
        0,  0,252,252,  4,  4,  4,  4,  4,  0,252,  4,  4,  4,  4,  4,252,252,  0,252,252,  0,  0,  0,252,252,  0,  0,  0,252,252, 36, 36, 36,252,  0,252,252,  4,  4,136,112,  0,252,252,  4,  4,  4,  4,252,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  4,  2,  2,  2,  4, 56, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1, 29,226,  1,  1,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  7, 31,  7, 31,  7, 28,  7, 31,  7, 31,  7,  2,  2,  0,  0,  0,  0
        }
    };
    static const char PROGMEM prep[][ANIM_SIZE] = {
        {
        0,  0,252,252,  4,  4,  4,  4,  4,  0,252,  4,  4,  4,  4,  4,252,252,  0,252,252,  0,  0,  0,252,252,  0,  0,  0,252,252, 36, 36, 36,252,  0,252,252,  4,  4,136,112,  0,252,252,  4,  4,  4,  4,252,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1, 31,225,  0,  0,  1,  1,  2,  2,129,128,128,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,225, 26,  6,  9, 49, 53,  1,138,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0, 24,  6,  5,152,153,132,195,124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  4,  4,  4,  4,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  7, 31,  7, 31,  7, 28,  7, 31,  7, 31,  7,  2,  2,  0,  0,  0,  0
        }
    };
    static const char PROGMEM tap[TAP_FRAMES][ANIM_SIZE] = {
        {
        0,  0,252,252,  4,  4,  4,  4,  4,  0,252,  4,  4,  4,  4,  4,252,252,  0,252,252,  0,  0,  0,252,252,  0,  0,  0,252,252, 36, 36, 36,252,  0,252,252,  4,  4,136,112,  0,252,252,  4,  4,  4,  4,252,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,248,248,248,248,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1, 31,225,  0,  0,  1,  1,  2,  2,129,128,128,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,  1,  2,  4,  8, 16, 32, 67,135,  7,  1,  0,184,188,190,159, 95, 95, 79, 76, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0, 24,  6,  5,152,153,132, 67,124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1, 61,124,252,252,252,252,252, 60, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  7, 31,  7, 31,  7, 28,  7, 31,  7, 31,  7,  2,  2,  0,  0,  0,  0
        },
        {
        0,  0,252,252,  4,  4,  4,  4,  4,  0,252,  4,  4,  4,  4,  4,252,252,  0,252,252,  0,  0,  0,252,252,  0,  0,  0,252,252, 36, 36, 36,252,  0,252,252,  4,  4,136,112,  0,252,252,  4,  4,  4,  4,252,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1, 31,225,  0,  0,  1,  1,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,225, 26,  6,  9, 49, 53,  1,138,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  4,  4,  4,  4,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,249,249,121,121, 57, 49,  2,  2,  4,  4,  8,  8,  8,232,232,199,128,  0,  6,  6, 15, 15, 31, 31, 63,127, 63, 31,  7,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  7, 31,  7, 31,  7, 28,  7, 31,  7, 31,  7,  2,  2,  0,  0,  0,  0
        },
    };

    //assumes 1 frame prep stage
    void animation_phase(void) {
        if(get_current_wpm() <=IDLE_SPEED){
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            st7565_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
         }
         if(get_current_wpm() >IDLE_SPEED && get_current_wpm() <TAP_SPEED){
             // oled_write_raw_P(prep[abs((PREP_FRAMES-1)-current_prep_frame)], ANIM_SIZE); // uncomment if IDLE_FRAMES >1
             st7565_write_raw_P(prep[0], ANIM_SIZE);  // remove if IDLE_FRAMES >1
         }
         if(get_current_wpm() >=TAP_SPEED){
             current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
             st7565_write_raw_P(tap[abs((TAP_FRAMES-1)-current_tap_frame)], ANIM_SIZE);
         }
    }
    if(get_current_wpm() != 000) {
        st7565_on(); // not essential but turns on animation OLED with any alpha keypress
        if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
        anim_sleep = timer_read32();
    } else {
        if(timer_elapsed32(anim_sleep) > ST7565_TIMEOUT) {
            st7565_off();
        } else {
            if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}
/********animation related end********/
uint32_t layer_state_set_user(uint32_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
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
 void st7565_task_user(void) {
     if (!st7565_is_on()) {
         return;
     }
     if (is_keyboard_master()) {
         switch (get_highest_layer(layer_state)) {
             case _BASE:
                 st7565_write_P(PSTR("BASE LAYER\n"), false);
                 ergodox_infinity_lcd_color(UINT16_MAX / 2, UINT16_MAX / 2, UINT16_MAX / 2);
                 break;
             case _LOWER:
                 st7565_write_P(PSTR("LOWER LAYER\n"), false);
                 ergodox_infinity_lcd_color(UINT16_MAX / 2, 0, 0);
                 break;
             case _RAISE:
                 st7565_write_P(PSTR("RAISE LAYER\n"), false);
                 ergodox_infinity_lcd_color(UINT16_MAX / 2, UINT16_MAX / 2, 0);
                 break;
             case _ADJUST:
                 st7565_write_P(PSTR("ADJUST LAYER\n"), false);
                 ergodox_infinity_lcd_color(0, 0, UINT16_MAX / 2);
                 break;
             case _NUMPAD:
                 st7565_write_P(PSTR("NUMBERPAD LAYER\n"), false);
                 ergodox_infinity_lcd_color(0, UINT16_MAX / 2, 0);
                 break;
             default:
                 st7565_write_ln_P(PSTR("Oops Undefined"), false);
                 ergodox_infinity_lcd_color(UINT16_MAX / 2, UINT16_MAX / 2, UINT16_MAX / 2);
         }
         led_t leds = host_keyboard_led_state();
         if (leds.num_lock) {
             st7565_write("Num ", false);
         }
         if (leds.caps_lock) {
             st7565_write("Caps ", false);
         }
         if (leds.scroll_lock) {
             st7565_write("Scroll ", false);
         }
         if (leds.compose) {
             st7565_write("Com ", false);
         }
         if (leds.kana) {
             st7565_write("Kana", false);
         }
         uint8_t n  = get_current_wpm();
         wpm_str[3] = '\0';
         wpm_str[2] = '0' + n % 10;
         wpm_str[1] = '0' + (n /= 10) % 10;
         wpm_str[0] = '0' + n / 10;
         st7565_write_P(PSTR("\n     WPM: "), false);
         st7565_write(wpm_str, false);
         st7565_advance_page(true);
     } else {
         // Draw logo
/*          static const char qmk_logo[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00};
         st7565_write(qmk_logo, false);
         st7565_write("  Infinity  Ergodox  ", false); */
         render_anim();
         switch (get_highest_layer(layer_state)) {
             case _BASE:
                 ergodox_infinity_lcd_color(UINT16_MAX / 2, UINT16_MAX / 2, UINT16_MAX / 2);
                 break;
             case _LOWER:
                 ergodox_infinity_lcd_color(UINT16_MAX / 2, 0, 0);
                 break;
             case _RAISE:
                 ergodox_infinity_lcd_color(UINT16_MAX / 2, UINT16_MAX / 2, 0);
                 break;
             case _ADJUST:
                 ergodox_infinity_lcd_color(0, 0, UINT16_MAX / 2);
                 break;
             case _NUMPAD:
                 ergodox_infinity_lcd_color(0, UINT16_MAX / 2, 0);
                 break;
             default:
                 st7565_write_ln_P(PSTR("Oops Undefined"), false);
                 ergodox_infinity_lcd_color(UINT16_MAX / 2, UINT16_MAX / 2, UINT16_MAX / 2);
         }
     }
 }
/* void matrix_scan_user(void) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    if (host_keyboard_led_state().num_lock) {
        ergodox_right_led_1_on();
    }
    if (host_keyboard_led_state().caps_lock) {
        ergodox_right_led_2_on();
    }
    if (host_keyboard_led_state().scroll_lock) {
        ergodox_right_led_3_on();
    }
} */
