#include QMK_KEYBOARD_H

enum layers {
  _BASE,
  _FN
};
enum planck_keycodes {
  MARCO_VIM_NEXT = SAFE_RANGE,
  MARCO_VIM_PREVIOUS,
  MARCO_VIM_YANK,
  MARCO_VIM_PASTE
};
#define FN TG(_FN)
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_ansi(
        KC_GRAVE,   KC_ESC,        KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,           KC_7,KC_8,KC_9,KC_0,KC_MINS,KC_EQL,              KC_BSPC,KC_NO,KC_HOME,
        KC_PGUP,    KC_TAB,         KC_Q,KC_W,KC_E,KC_R,KC_T,            KC_Y,KC_U,KC_I,   KC_O,KC_P,   KC_LBRC,KC_RBRC,     KC_BSLS,      KC_END,
        KC_PGDOWN,  LCTL_T(KC_TAB),  KC_A,KC_S,KC_D,KC_F,KC_G,            KC_H,KC_J,KC_K,   KC_L,KC_SCLN,KC_QUOT,             KC_ENT,
        KC_INSERT,  KC_LSFT,           KC_Z,KC_X,KC_C,KC_V,KC_B,            KC_N,KC_M,KC_COMM,KC_DOT,KC_SLSH,               KC_RSFT,KC_UP,
        KC_DELETE,  KC_LCTL, KC_LGUI, KC_LALT,   KC_SPC,     FN,           KC_SPC,     KC_RALT,     KC_RGUI,         KC_RCTL,  KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_FN] = LAYOUT_ansi(
        LALT(LCTL(KC_DELETE)),    KC_TRNS,       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,                KC_TRNS,KC_TRNS,KC_TRNS,   KC_TRNS,  KC_TRNS,KC_TRNS, KC_TRNS,KC_TRNS,     KC_TRNS,
        KC_TRNS,                  KC_TRNS,        DYN_MACRO_PLAY1,DYN_MACRO_PLAY2,KC_TRNS,KC_TRNS,KC_TRNS,    MARCO_VIM_YANK,KC_TRNS,KC_TRNS,   KC_TRNS,  MARCO_VIM_PASTE,   KC_TRNS,KC_TRNS,KC_TRNS,      KC_TRNS,
        DYN_REC_START1,           KC_TRNS,         KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,                    MARCO_VIM_PREVIOUS,KC_TRNS,KC_TRNS,   MARCO_VIM_NEXT,  KC_TRNS,KC_TRNS,KC_TRNS,
        DYN_REC_START2,           KC_TRNS,           KC_NLCK,KC_CAPS,KC_SLCK,KC_TRNS,KC_TRNS,                    KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
        DYN_REC_STOP,             KC_TRNS, KC_TRNS,   KC_TRNS,       KC_TRNS,           FN,                    KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
    case MARCO_VIM_PASTE:
        if (record->event.pressed) {
            SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_DELAY(100) SS_LSFT(SS_TAP(X_EQUAL)) SS_DELAY(100) SS_TAP(X_P));
        }
        return false;
}
return true;
}