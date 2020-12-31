#include "kb.h"
#include "./layer_light.h"

// ALT_TAB
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

// KC_LEAD
// bool did_leader_succeed;

enum planck_keycodes {
  MARCO_VIM_NEXT = SAFE_RANGE,
  MARCO_VIM_PREVIOUS,
  MARCO_VIM_YANK,
  MARCO_VIM_PASTE,
  MARCO_VIM_FIND,
  ALT_TAB
};

enum planck_layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
  _FN
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define FN MO(_FN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = KEYMAP(
    KC_ESCAPE,      KC_Q,     KC_W,    KC_E,    KC_R,  KC_T,     KC_Y, KC_U,  KC_I,     KC_O,    KC_P,      KC_BSPACE,     \
    LCTL_T(KC_TAB), KC_A,     KC_S,    KC_D,    KC_F,  KC_G,     KC_H, KC_J,  KC_K,     KC_L,    KC_SCOLON, KC_QUOTE,     \
    KC_LSHIFT,      KC_Z,     KC_X,    KC_C,    KC_V,  KC_B,     KC_N, KC_M,  KC_COMMA, KC_DOT,  KC_SLASH,  RSFT_T(KC_ENTER), \
    FN,             KC_LCTRL, KC_LGUI, KC_LALT, LOWER, KC_SPACE,       RAISE, KC_LEFT,  KC_DOWN, KC_UP,     KC_RIGHT
  ),

  [_LOWER] = KEYMAP(
    RSFT(KC_GRAVE), RSFT(KC_1),     RSFT(KC_2),     RSFT(KC_3),     RSFT(KC_4),     RSFT(KC_5),  LSFT(KC_6), KC_7,           KC_8, KC_9,   LSFT(KC_9),        LSFT(KC_0), \
    KC_TRANSPARENT, KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,       KC_F6,      KC_4,           KC_5, KC_6,   KC_LBRACKET,       KC_RBRACKET, \
    KC_TRANSPARENT, KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,      KC_F12,     KC_1,           KC_2, KC_3,   LSFT(KC_LBRACKET), LSFT(KC_RBRACKET), \
    KC_NO,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, ALT_TAB,                 KC_TRANSPARENT, KC_0, KC_DOT, KC_PLUS,           KC_MINUS
  ),

  [_RAISE] = KEYMAP(
    KC_GRAVE,       KC_NO,           KC_NO,           KC_INSERT,      KC_HOME,        KC_PGUP,    KC_PAUSE,   LSFT(KC_7),     LSFT(KC_8),          LSFT(KC_MINUS),       LSFT(KC_EQUAL),      LSFT(KC_BSLASH), \
    KC_TRANSPARENT, DYN_MACRO_PLAY1, DYN_MACRO_PLAY2, KC_DELETE,      KC_END,         KC_PGDOWN,  KC_LEFT,    KC_DOWN,        KC_UP,               KC_RIGHT,             KC_EQUAL,            KC_BSLASH, \
    KC_TRANSPARENT, KC_MS_LEFT,      KC_MS_UP,        KC_MS_DOWN,     KC_MS_RIGHT,    KC_MS_BTN1, KC_MS_BTN2, KC_MINUS,       KC_PSCREEN,          KC_MEDIA_PREV_TRACK,  KC_MEDIA_NEXT_TRACK, KC_AUDIO_VOL_UP, \
    KC_NO,          KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT, KC_TRANSPARENT, KC_CAPSLOCK,            KC_TRANSPARENT, LCTL(LGUI(KC_LEFT)), LCTL(LGUI(KC_RIGHT)), KC_MEDIA_PLAY_PAUSE, KC_AUDIO_VOL_DOWN
  ),

  [_ADJUST] = KEYMAP(
    LALT(LCTL(KC_DELETE)), DYN_REC_START1, DYN_REC_START2, KC_NO,          KC_NO,          KC_NO, KC_NO, MARCO_VIM_YANK, KC_NO,    KC_NO,   KC_NO,   KC_NO, \
    KC_TRANSPARENT,        KC_NO,          KC_NO,          KC_NO,          MARCO_VIM_FIND, KC_NO, KC_NO, MARCO_VIM_PASTE,RGB_TOG,  RGB_VAI, RGB_VAD, MAGIC_TOGGLE_NKRO, \
    KC_TRANSPARENT,        KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO, KC_NO, KC_NO,          RGB_MOD,  RGB_HUI, RGB_HUD, RESET, \
    DYN_REC_STOP,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_NO,        KC_TRANSPARENT, RGB_RMOD, RGB_SAI, RGB_SAD, KC_NO
  ),

  [_FN] = KEYMAP(
  KC_DOT,        KC_1,    KC_2,  KC_3,   KC_4,   KC_5,    KC_PLUS,           KC_MINUS,KC_ASTERISK,KC_SLASH,      KC_LPRN,KC_RPRN,  \
  KC_COMMA,      KC_6,    KC_7,  KC_8,   KC_9,   KC_0,    MARCO_VIM_PREVIOUS,KC_NO,   KC_NO,      MARCO_VIM_NEXT,KC_NO,  KC_NO,  \
  KC_SCOLON,     KC_QUOTE,KC_DQT,KC_LABK,KC_RABK,KC_NO,   KC_NO,             KC_NO,   KC_NO,      KC_NO,         KC_NO,  KC_NO,  \
  KC_TRANSPARENT,KC_NO,   KC_NO, KC_NO,  KC_NO,  KC_SPACE,                   KC_NO,   KC_NO,      KC_NO,         KC_NO,  KC_NO
  ),
};

uint32_t layer_state_set_user(uint32_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _LOWER));
    rgblight_set_layer_state(1, layer_state_cmp(state, _RAISE));
    rgblight_set_layer_state(2, layer_state_cmp(state, _ADJUST));
    rgblight_set_layer_state(3, layer_state_cmp(state, _FN));
    return state;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(4, led_state.caps_lock);
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case MARCO_VIM_NEXT:
            if (record->event.pressed) {
                SEND_STRING("gt");
            }
            break;
        case MARCO_VIM_PREVIOUS:
            if (record->event.pressed) {
                SEND_STRING("gT");
            }
            break;
        case MARCO_VIM_YANK:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_DELAY(100) SS_LSFT(SS_TAP(X_EQUAL)) SS_DELAY(100) SS_TAP(X_Y));
            }
            break;
        case MARCO_VIM_FIND:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_V) SS_DELAY(100) SS_TAP(X_T) SS_DELAY(100) SS_TAP(X_SCOLON));
            }
            break;
        case MARCO_VIM_PASTE:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_DELAY(100) SS_LSFT(SS_TAP(X_EQUAL)) SS_DELAY(100) SS_TAP(X_P));
            }
            break;
        case ALT_TAB:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
    }
    return true;
}

// layer indicator
const rgblight_segment_t PROGMEM layer_indi_lower[] = RGBLIGHT_LAYER_SEGMENTS(LOWER_LIGHTING);
const rgblight_segment_t PROGMEM layer_indi_raise[] = RGBLIGHT_LAYER_SEGMENTS(RAISW_LIGHTING);
const rgblight_segment_t PROGMEM layer_indi_adjust[] = RGBLIGHT_LAYER_SEGMENTS(ADJUST_LIGHTING);
const rgblight_segment_t PROGMEM layer_indi_fn[] = RGBLIGHT_LAYER_SEGMENTS(FN_LIGHTING);
const rgblight_segment_t PROGMEM layer_indi_caps[] = RGBLIGHT_LAYER_SEGMENTS({12, 1, HSV_RED});
const rgblight_segment_t* const PROGMEM rgb_layers[]    = RGBLIGHT_LAYERS_LIST(layer_indi_lower, layer_indi_raise, layer_indi_adjust, layer_indi_fn, layer_indi_caps);

void keyboard_post_init_user(void) { rgblight_layers = rgb_layers; }

// LEADER_EXTERNS();
void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 500) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }
    /*   LEADER_DICTIONARY() {
        did_leader_succeed = leading = false;

        SEQ_ONE_KEY(KC_M) {
          SEND_STRING("1gt");
          did_leader_succeed = true;
        }
        leader_end();
      } */
}

/* void leader_start(void) {
  rgblight_blink_layer(2, 100);
}

void leader_end(void) {
  if (did_leader_succeed) {
    rgblight_blink_layer(3, 100);
  }
} */