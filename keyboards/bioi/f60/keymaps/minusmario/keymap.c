/* Copyright 2021 kb-elmo<mail@elmo.space>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

enum f60_layers {
    _BASE,
    _FN_1,
    _FN_2
};
enum custom_keycodes {
  VIM_NEXT = SAFE_RANGE,
  VIM_PREVIOUS,
  VIM_YANK,
  VIM_PASTE
};
// Tap-Dance
enum {
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
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_60_ansi(
        KC_ESC,KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,KC_7,KC_8,KC_9,KC_0,KC_MINS,KC_EQL,    KC_BSPC,
        KC_TAB,  KC_Q,KC_W,KC_E,KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_LBRC,KC_RBRC,  KC_BSLS,
        KC_LCTL,   KC_A,KC_S,KC_D,KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCLN,KC_QUOT,     KC_ENT,
        KC_LSFT,     KC_Z,KC_X,KC_C,KC_V,KC_B,KC_N,KC_M,KC_COMM,KC_DOT,KC_SLSH,       KC_RSFT,
        MO(_FN_2), KC_LCTL, KC_LALT, LT(_FN_1,KC_SPC),                     KC_RGUI, OSL(_FN_1), LCTL(LGUI(KC_LEFT)), LCTL(LGUI(KC_RIGHT))
    ),
    [_FN_1] = LAYOUT_60_ansi(
        KC_GRV,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,KC_F12,    KC_TRNS,
        KC_TRNS,  DYN_REC_START1,DYN_REC_START2,KC_INSERT,KC_HOME,KC_PGUP,VIM_YANK,KC_NO,KC_NO,KC_NO,VIM_PASTE,KC_NO,KC_NO,  KC_NO,
        KC_TRNS,   DYN_MACRO_PLAY1,DYN_MACRO_PLAY2,KC_DELETE,KC_END,KC_PGDOWN,KC_LEFT,KC_DOWN,KC_UP,KC_RIGHT,KC_NO,KC_NO,     KC_TRNS,
        KC_TRNS,     KC_NLCK,KC_CAPS,KC_SLCK,KC_PAUSE,KC_PSCREEN,KC_NO,KC_NO,KC_AUDIO_VOL_DOWN,KC_AUDIO_VOL_UP,KC_MUTE,       KC_UP,
        DYN_REC_STOP, KC_TRNS, KC_TRNS, KC_TRNS,                     KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT
    ),
    [_FN_2] = LAYOUT_60_ansi(
        LALT(LCTL(KC_DELETE)),KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_LPRN,KC_RPRN,KC_NO,RESET,    KC_TRNS,
        KC_TRNS,                KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_7,KC_8,KC_9,TD(COMMA_DOT),TD(QUOTE_DQT),TD(MINUS_PLUS),  KC_NO,
        KC_TILD,                 KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_4,KC_5,KC_6,KC_NO,KC_NO,     KC_TRNS,
        KC_TRNS,                   KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_0,KC_1,KC_2,KC_3,KC_NO,       KC_UP,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_SPC,                      EEP_RST, KC_LEFT, KC_DOWN, KC_RIGHT
    )
};
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case VIM_NEXT:
            if (record->event.pressed) {
                SEND_STRING("gt");
            }
            return false;
        case VIM_PREVIOUS:
            if (record->event.pressed) {
                SEND_STRING("gT");
            }
            return false;
        case VIM_YANK:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_DELAY(100) SS_LSFT(SS_TAP(X_EQUAL)) SS_DELAY(100) SS_TAP(X_Y));
            }
            return false;
        case VIM_PASTE:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_DELAY(100) SS_LSFT(SS_TAP(X_EQUAL)) SS_DELAY(100) SS_TAP(X_P));
            }
            return false;
    }
    return true;
}
