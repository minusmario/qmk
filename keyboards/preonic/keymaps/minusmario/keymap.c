/* Copyright 2015-2017 Jack Humbert
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
#include "muse.h"

enum preonic_layers {
  _DEFAULT,
  _LOWER,
  _RAISE,
  _ADJUST,
  _GAME
};

enum preonic_keycodes {
  MARCO_VIM_NEXT = SAFE_RANGE,
  MARCO_VIM_PREVIOUS,
  MARCO_VIM_YANK,
  MARCO_VIM_PASTE,
  MARCO_VIM_FIND,
  CLOSE_GAME
};

uint16_t double_tap_timer = 0; // timer

#define FN MO(_FN)

#ifdef AUDIO_ENABLE
float audio_caps_on[][2] = SONG(VIOLIN_SOUND);
float audio_caps_off[][2] = SONG(GUITAR_SOUND);
float audio_game_on[][2] = SONG(GAME_ON);
float audio_game_off[][2] = SONG(GAME_OFF);
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_DEFAULT] = LAYOUT_preonic_2x2u(
KC_ESCAPE,     KC_1,    KC_2,   KC_3,   KC_4,  KC_5,        KC_6, KC_7,         KC_8,    KC_9,   KC_0,     KC_BSPACE,        \
KC_ESCAPE,     KC_Q,    KC_W,   KC_E,   KC_R,  KC_T,        KC_Y, KC_U,         KC_I,    KC_O,   KC_P,     KC_BSPACE,        \
LCTL_T(KC_TAB),KC_A,    KC_S,   KC_D,   KC_F,  KC_G,        KC_H, KC_J,         KC_K,    KC_L,   KC_SCOLON,KC_QUOTE,         \
KC_LSHIFT,     KC_Z,    KC_X,   KC_C,   KC_V,  KC_B,        KC_N, KC_M,         KC_COMMA,KC_DOT, KC_SLASH, RSFT_T(KC_ENTER), \
TG(_GAME),     KC_LCTRL,KC_LGUI,KC_LALT,LT(_LOWER,KC_SPACE),LT(_RAISE,KC_SPACE),KC_LEFT, KC_DOWN,KC_UP,    KC_RIGHT
),
[_LOWER] = LAYOUT_preonic_2x2u(
KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,KC_F9,KC_F10,KC_F11,     KC_F12,     \
KC_TILD,KC_EXLM,KC_AT,  KC_HASH,KC_DLR, KC_PERC,KC_CIRC,KC_7, KC_8, KC_9,  KC_LPRN,    KC_RPRN,    \
KC_TRNS,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_4, KC_5, KC_6,  KC_LBRACKET,KC_RBRACKET,\
KC_TRNS,KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12, KC_1, KC_2, KC_3,  KC_LCBR,    KC_RCBR,    \
KC_NO,  KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,        KC_TRNS,      KC_0, KC_DOT,KC_PLUS,    KC_MINUS
),
[_RAISE] = LAYOUT_preonic_2x2u(
KC_F1,   KC_F2,          KC_F3,          KC_F4,    KC_F5,             KC_F6,         KC_F7,   KC_F8,   KC_F9,              KC_F10,              KC_F11,             KC_F12,          \
KC_GRAVE,KC_NO,          KC_NO,          KC_INSERT,KC_HOME,           KC_PGUP,       KC_PAUSE,KC_AMPR, KC_ASTR,            KC_UNDS,             KC_PLUS,            KC_PIPE,         \
KC_TRNS, DYN_MACRO_PLAY1,DYN_MACRO_PLAY2,KC_DELETE,KC_END,            KC_PGDOWN,     KC_LEFT, KC_DOWN, KC_UP,              KC_RIGHT,            KC_EQUAL,           KC_BSLASH,       \
KC_TRNS, KC_NLCK,        KC_CAPS,        KC_SLCK,  MARCO_VIM_PREVIOUS,MARCO_VIM_NEXT,KC_NO,   KC_MINUS,KC_PSCREEN,         KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK,KC_AUDIO_VOL_UP, \
KC_NO,   KC_TRNS,        KC_TRNS,        KC_TRNS,  KC_TRNS,                          KC_TRNS,          LCTL(LGUI(KC_LEFT)),LCTL(LGUI(KC_RIGHT)),KC_MEDIA_PLAY_PAUSE,KC_AUDIO_VOL_DOWN
),
[_ADJUST] = LAYOUT_preonic_2x2u(
KC_NO,                KC_NO,         KC_NO,         KC_NO,         KC_NO,         KC_NO,KC_NO,  KC_NO,          KC_NO,   KC_NO,  KC_NO,  KC_NO,            \
LALT(LCTL(KC_DELETE)),DYN_REC_START1,DYN_REC_START2,KC_NO,         KC_NO,         KC_NO,KC_NO,  MARCO_VIM_YANK, KC_NO,   KC_NO,  KC_NO,  KC_NO,            \
KC_TRNS,              AU_ON,         AU_OFF,        AU_TOG,        MARCO_VIM_FIND,KC_NO,KC_NO,  MARCO_VIM_PASTE,RGB_TOG, RGB_VAI,RGB_VAD,MAGIC_TOGGLE_NKRO,\
KC_TRNS,              MU_ON,         MU_OFF,        MU_TOG,        KC_NO,         KC_NO,KC_NO,  KC_NO,          RGB_MOD, RGB_HUI,RGB_HUD,RESET,            \
KC_TRNS,              KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,             KC_TRNS,                RGB_RMOD,RGB_SAI,RGB_SAD,DYN_REC_STOP
),
[_GAME] = LAYOUT_preonic_2x2u(
KC_ESCAPE, KC_1,    KC_2, KC_3,   KC_4,KC_5,KC_6, KC_7,KC_8,    KC_9,   KC_0,      KC_F9,            \
KC_TAB,    KC_Q,    KC_W, KC_E,   KC_R,KC_T,KC_Y, KC_U,KC_I,    KC_O,   KC_P,      KC_BSPACE,        \
KC_LCTRL,  KC_A,    KC_S, KC_D,   KC_F,KC_G,KC_H, KC_J,KC_K,    KC_L,   KC_SCOLON, KC_QUOTE,         \
KC_LSHIFT, KC_Z,    KC_X, KC_C,   KC_V,KC_B,KC_N, KC_M,KC_COMMA,KC_DOT, KC_SLASH,  RSFT_T(KC_ENTER), \
CLOSE_GAME,KC_LCTRL,KC_NO,KC_LALT,KC_SPACE, KC_SPACE,  KC_LEFT, KC_DOWN,KC_UP,     KC_RIGHT
)
};

layer_state_t layer_state_set_user(layer_state_t state) {  // This runs code every time that the layers get changed
#ifdef AUDIO_ENABLE
    switch (get_highest_layer(state)) {
        case _GAME:
            PLAY_SONG(audio_game_on);
            break;
        default:
            break;
    }
#endif
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);;
}

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
        case CLOSE_GAME:
            if (record->event.pressed) {
                if (double_tap_timer == 0) {
                    double_tap_timer = timer_read();
                } else {
                    layer_off(_GAME);
                    layer_on(_DEFAULT);
                    double_tap_timer = 0;
#ifdef AUDIO_ENABLE
                    PLAY_SONG(audio_game_off);
#endif
                }
            }
            return false;
            /*case LOWER:
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
                break;*/
    }
    return true;
};

bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

void encoder_update_user(uint8_t index, bool clockwise) {
    if (muse_mode) {
        if (IS_LAYER_ON(_RAISE)) {
            if (clockwise) {
                muse_offset++;
            } else {
                muse_offset--;
            }
        } else {
            if (clockwise) {
                muse_tempo += 1;
            } else {
                muse_tempo -= 1;
            }
        }
    } else {
        if (clockwise) {
            register_code(KC_PGDN);
            unregister_code(KC_PGDN);
        } else {
            register_code(KC_PGUP);
            unregister_code(KC_PGUP);
        }
    }
}

/* void dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if (active) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            break;
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
} */

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
    if (timer_elapsed(double_tap_timer) > 100) {
        double_tap_timer = 0;
    }
}

/* bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
        case RAISE:
        case LOWER:
            return false;
        default:
            return true;
    }
} */

bool led_update_user(led_t led_state) {
    #ifdef AUDIO_ENABLE
    static uint8_t caps_state = 0;
    if (caps_state != led_state.caps_lock) {
        led_state.caps_lock ? PLAY_SONG(audio_caps_on) : PLAY_SONG(audio_caps_off);
        caps_state = led_state.caps_lock;
    }
    #endif
    return true;
}