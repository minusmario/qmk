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
};

enum planck_keycodes {
  ST_MACRO_0 = SAFE_RANGE,
  ST_MACRO_1,
  ST_MACRO_2,
  ST_MACRO_3,
  ALT_TAB
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BASE] = LAYOUT_planck_mit(
    KC_ESCAPE,      KC_Q,     KC_W,    KC_E,    KC_R,  KC_T,     KC_Y, KC_U,  KC_I,     KC_O,    KC_P,      KC_BSPACE,     \
    LCTL_T(KC_TAB), KC_A,     KC_S,    KC_D,    KC_F,  KC_G,     KC_H, KC_J,  KC_K,     KC_L,    KC_SCOLON, KC_QUOTE,     \
    KC_LSHIFT,      KC_Z,     KC_X,    KC_C,    KC_V,  KC_B,     KC_N, KC_M,  KC_COMMA, KC_DOT,  KC_SLASH,  RSFT_T(KC_ENTER), \
    KC_LEAD,        KC_LCTRL, KC_LGUI, KC_LALT, LOWER, KC_SPACE,       RAISE, KC_LEFT,  KC_DOWN, KC_UP,     KC_RIGHT
),
[_LOWER] = LAYOUT_planck_mit(
    RSFT(KC_GRAVE), RSFT(KC_1),     RSFT(KC_2),     RSFT(KC_3),     RSFT(KC_4),     RSFT(KC_5),  LSFT(KC_6), KC_7,           KC_8, KC_9,   LSFT(KC_9),        LSFT(KC_0), \
    KC_TRANSPARENT, KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,       KC_F6,      KC_4,           KC_5, KC_6,   KC_LBRACKET,       KC_RBRACKET, \
    KC_TRANSPARENT, KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,      KC_F12,     KC_1,           KC_2, KC_3,   LSFT(KC_LBRACKET), LSFT(KC_RBRACKET), \
    KC_NO,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, ALT_TAB,                 KC_TRANSPARENT, KC_0, KC_DOT, KC_PLUS,           KC_MINUS
),
[_RAISE] = LAYOUT_planck_mit(
    KC_GRAVE,       DYN_REC_START1,  DYN_REC_START2,  KC_INSERT,      KC_HOME,        KC_PGUP,    KC_PAUSE,   LSFT(KC_7),     LSFT(KC_8),          LSFT(KC_MINUS),       LSFT(KC_EQUAL),      LSFT(KC_BSLASH), \
    KC_TRANSPARENT, DYN_MACRO_PLAY1, DYN_MACRO_PLAY2, KC_DELETE,      KC_END,         KC_PGDOWN,  KC_LEFT,    KC_DOWN,        KC_UP,               KC_RIGHT,             KC_EQUAL,            KC_BSLASH, \
    KC_TRANSPARENT, KC_MS_LEFT,      KC_MS_UP,        KC_MS_DOWN,     KC_MS_RIGHT,    KC_MS_BTN1, KC_MS_BTN2, KC_MINUS,       KC_PSCREEN,          KC_MEDIA_PREV_TRACK,  KC_MEDIA_NEXT_TRACK, KC_AUDIO_VOL_UP, \
    DYN_REC_STOP,   KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT, KC_TRANSPARENT, KC_CAPSLOCK,            KC_TRANSPARENT, LCTL(LGUI(KC_LEFT)), LCTL(LGUI(KC_RIGHT)), KC_MEDIA_PLAY_PAUSE, KC_AUDIO_VOL_DOWN
),
[_ADJUST] = LAYOUT_planck_mit(
    LALT(LCTL(KC_DELETE)), KC_NO,          KC_NO,          KC_NO,          KC_NO,         KC_NO, KC_NO, ST_MACRO_1,     KC_NO, KC_NO, ST_MACRO_0, KC_NO, \
    KC_TRANSPARENT,        AU_ON,          AU_OFF,         AU_TOG,         ST_MACRO_2,    KC_NO, KC_NO, ST_MACRO_3,     KC_NO, KC_NO, KC_NO,      KC_NO, \
    KC_TRANSPARENT,        MU_ON,          MU_OFF,         MU_TOG,         KC_NO,         KC_NO, KC_NO, KC_NO,          KC_NO, KC_NO, KC_NO,      RESET, \
    KC_NO,                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,KC_NO,        KC_TRANSPARENT, KC_NO, KC_NO, KC_NO,      KC_NO
),
};


layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
switch (keycode) {
    case ST_MACRO_0:
    if (record->event.pressed) {
      SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_P))));

    }
    break;
    case ST_MACRO_1:
    if (record->event.pressed) {
      SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_DELAY(100) SS_LSFT(SS_TAP(X_EQUAL)) SS_DELAY(100) SS_TAP(X_Y));

    }
    break;
    case ST_MACRO_2:
    if (record->event.pressed) {
      SEND_STRING(SS_TAP(X_V) SS_DELAY(100) SS_TAP(X_T) SS_DELAY(100) SS_TAP(X_SCOLON));

    }
    break;
    case ST_MACRO_3:
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
}
return true;
}

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update(bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_DOWN);
      #else
        tap_code(KC_PGDN);
      #endif
    } else {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_UP);
      #else
        tap_code(KC_PGUP);
      #endif
    }
  }
}

#ifdef AUDIO_ENABLE
float lead_start[][2] = SONG(MARIO_THEME);
float lead_succeed[][2] = SONG(LEAD_SUCCESS);
float lead_fail[][2] = SONG(LEAD_FAIL);
#endif
LEADER_EXTERNS();
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
  LEADER_DICTIONARY() {
    did_leader_succeed = leading = false;

    SEQ_ONE_KEY(KC_H) {
      SEND_STRING("gT");
      did_leader_succeed = true;
    }
    SEQ_ONE_KEY(KC_L) {
      SEND_STRING("gt");
      did_leader_succeed = true;
    }
    SEQ_ONE_KEY(KC_BSPACE) {
      register_code(KC_NUMLOCK);
      unregister_code(KC_NUMLOCK);
      did_leader_succeed = true;
    }
    SEQ_TWO_KEYS(KC_BSPACE, KC_BSPACE) {
      register_code(KC_CAPSLOCK);
      unregister_code(KC_CAPSLOCK);
      did_leader_succeed = true;
    }
    SEQ_THREE_KEYS(KC_BSPACE, KC_BSPACE, KC_BSPACE) {
      register_code(KC_SCROLLLOCK);
      unregister_code(KC_SCROLLLOCK);
      did_leader_succeed = true;
    }
    leader_end();
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

void leader_start(void) {
#ifdef AUDIO_ENABLE
    PLAY_SONG(lead_start);
#endif
}

void leader_end(void) {
if (did_leader_succeed) {
#ifdef AUDIO_ENABLE
    PLAY_SONG(lead_succeed);
#endif
  } else {
#ifdef AUDIO_ENABLE
    PLAY_SONG(lead_fail);
#endif
  }
}