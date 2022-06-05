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


// ALT_TAB
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

// KC_LEAD
bool did_leader_succeed;

enum planck_layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
  _FN
};

enum planck_keycodes {
  MARCO_VIM_NEXT = SAFE_RANGE,
  MARCO_VIM_PREVIOUS,
  MARCO_VIM_YANK,
  MARCO_VIM_PASTE,
  MARCO_VIM_FIND,
  ALT_TAB
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

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define FN MO(_FN)

#ifdef AUDIO_ENABLE
float audio_led_on[][2] = SONG(GAME_ON);
float audio_led_off[][2] = SONG(GAME_OFF);
#endif

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
    KC_NO,  KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,        KC_TRNS,KC_0,TD(COMMA_DOT),TD(QUOTE_DQT),TD(MINUS_PLUS)
  ),

  [_RAISE] = LAYOUT_planck_mit(
    KC_GRAVE,KC_PAUSE,           KC_PSCREEN,          KC_INSERT,KC_HOME,KC_PGUP,  LGUI(KC_D),KC_AMPR, KC_ASTR,            KC_UNDS,             KC_PLUS,        KC_PIPE, \
    KC_TRNS, LCTL(LGUI(KC_LEFT)),LCTL(LGUI(KC_RIGHT)),KC_DELETE,KC_END, KC_PGDOWN,KC_LEFT,   KC_DOWN, KC_UP,              KC_RIGHT,            KC_EQUAL,       KC_BSLASH, \
    KC_TRNS, KC_NLCK,            KC_CAPS,             KC_SLCK,  KC_NO,  KC_NO,    KC_NO,     KC_MINUS,KC_NO,              KC_MEDIA_PLAY_PAUSE, KC_MUTE,        KC_NO, \
    KC_NO,   KC_TRNS,            KC_TRNS,             KC_TRNS,  KC_TRNS,    KC_TRNS,         KC_TRNS, KC_MEDIA_PREV_TRACK,KC_AUDIO_VOL_DOWN,   KC_AUDIO_VOL_UP,KC_MEDIA_NEXT_TRACK
  ),

  [_ADJUST] = LAYOUT_planck_mit(
    LALT(LCTL(KC_DELETE)),DYN_REC_START1, DYN_REC_START2, KC_NO,  KC_NO,         KC_NO,ALT_TAB,KC_NO,  KC_NO, MU_ON,MU_OFF,KC_NO,             \
    KC_TRNS,              DYN_MACRO_PLAY1,DYN_MACRO_PLAY2,KC_NO,  MARCO_VIM_FIND,KC_NO,KC_NO,  KC_NO,  MU_MOD,AU_ON,AU_OFF,MAGIC_TOGGLE_NKRO, \
    KC_TRNS,              KC_NO,          KC_NO,          KC_NO,  KC_NO,         KC_NO,KC_NO,  KC_NO,  KC_NO, KC_NO,KC_NO, RESET, \
    DYN_REC_STOP,         KC_TRNS,        KC_TRNS,        KC_TRNS,KC_TRNS,          KC_NO,     KC_TRNS,KC_NO, KC_NO,KC_NO, EEPROM_RESET
  ),

  [_FN] = LAYOUT_planck_mit(
    KC_7,   KC_8,KC_9, KC_PLUS, KC_COMMA,      KC_LPRN,MARCO_VIM_YANK,    KC_NO,KC_NO,  KC_NO,         MARCO_VIM_PASTE,KC_BSPACE,\
    KC_4,   KC_5,KC_6, KC_MINUS,KC_DOT,        KC_RPRN,MARCO_VIM_PREVIOUS,KC_NO,KC_NO,  MARCO_VIM_NEXT,KC_NO,          KC_NO,  \
    KC_1,   KC_2,KC_3, KC_ASTR, KC_QUOTE,      KC_NO,  KC_NO,             KC_NO,KC_NO,  KC_NO,         KC_NO,          KC_ENTER,\
    KC_TRNS,KC_0,KC_NO,KC_SLASH,LSFT(KC_QUOTE),   KC_SPACE,               KC_NO,KC_LEFT,KC_DOWN,       KC_UP,          KC_RIGHT
  ),
};


layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
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
        return false;
}
return true;
}

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

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
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 500) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    case LOWER:
      return false;
    default:
      return true;
  }
}

bool led_update_user(led_t led_state) {
    #ifdef AUDIO_ENABLE
    static uint8_t num_state = 0;
    static uint8_t caps_state = 0;
    static uint8_t scroll_state = 0;
    if (num_state != led_state.num_lock) {
        led_state.num_lock ? PLAY_SONG(audio_led_on) : PLAY_SONG(audio_led_off);
        num_state = led_state.num_lock;
    }
    if (caps_state != led_state.caps_lock) {
        led_state.caps_lock ? PLAY_SONG(audio_led_on) : PLAY_SONG(audio_led_off);
        caps_state = led_state.caps_lock;
    }
    if (scroll_state != led_state.scroll_lock) {
        led_state.scroll_lock ? PLAY_SONG(audio_led_on) : PLAY_SONG(audio_led_off);
        scroll_state = led_state.scroll_lock;
    }
    #endif
    return true;
}
