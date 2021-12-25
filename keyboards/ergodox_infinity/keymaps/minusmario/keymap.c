#include QMK_KEYBOARD_H
#include "version.h"
#include "layers.h"
#include "animation.c"
#include "stdio.h"

enum custom_keycodes {
    VIM_NEXT = SAFE_RANGE,
    VIM_PREVIOUS,
    VIM_YANK,
    VIM_PASTE,
    VIM_FIND,
};
// Tap-Dance
enum tap_dance { COMMA_DOT, QUOTE_DQT, MINUS_PLUS };

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {[COMMA_DOT] = ACTION_TAP_DANCE_DOUBLE(KC_COMMA, KC_DOT), [QUOTE_DQT] = ACTION_TAP_DANCE_DOUBLE(KC_QUOTE, KC_DQT), [MINUS_PLUS] = ACTION_TAP_DANCE_DOUBLE(KC_MINUS, KC_PLUS)};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define NUMP MO(_NUMPAD)

/********animation related start********/
char wpm_str[13];
// WPM-responsive animation stuff here
#define IDLE_SPEED 40  // below this wpm value your animation will idle

// #define PREP_FRAMES 1 // uncomment if >1

#define TAP_SPEED 60  // above this wpm value typing animation to triggere

#define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing

uint32_t anim_timer         = 0;
uint32_t anim_sleep         = 0;
uint8_t  current_idle_frame = 0;
// uint8_t current_prep_frame = 0; // uncomment if PREP_FRAMES >1
uint8_t current_tap_frame = 0;
/********animation related end********/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE]   = LAYOUT_ergodox(
        KC_ESCAPE, KC_1, KC_2, KC_3, KC_4, KC_5, KC_MINUS,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_LPRN,
        LCTL_T(KC_TAB), KC_A, KC_S, KC_D, KC_F, KC_G,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LBRC,
        NUMP, KC_LCTL, KC_LGUI, KC_LALT, LOWER,
        VIM_PREVIOUS, VIM_NEXT, KC_PGUP, KC_SPC, KC_BSPC, KC_PGDN,

        KC_EQL, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_RPRN, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS,
        KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_RBRC, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
        RAISE, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,
        KC_INS, KC_DEL, KC_HOME, KC_END, KC_TAB, KC_ENT),
    [_LOWER]  = LAYOUT_ergodox(
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F11,
        KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_TRNS,
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_TRNS, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_TRNS,
        KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

        KC_F12, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_TRNS,
        KC_TRNS, KC_CIRC, KC_7, KC_8, KC_9, KC_LPRN, KC_RPRN,
        KC_F6, KC_4, KC_5, KC_6, KC_LBRC, KC_RBRC,
        KC_TRNS, KC_F12, KC_1, KC_2, KC_3, KC_LCBR, KC_RCBR,
        KC_TRNS, KC_0, TD(COMMA_DOT), TD(QUOTE_DQT), TD(MINUS_PLUS),
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    [_RAISE]  = LAYOUT_ergodox(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_GRAVE, KC_PAUSE, KC_PSCR, KC_INS, KC_HOME, KC_PGUP, KC_TRNS,
        KC_TRNS, LCTL(LGUI(KC_LEFT)), LCTL(LGUI(KC_RIGHT)), KC_DELETE, KC_END, KC_PGDOWN,
        KC_TRNS, KC_NLCK, KC_CAPS, KC_SLCK, KC_NO, KC_NO, KC_TRNS,
        KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, LGUI(KC_D), KC_AMPR, KC_ASTR, KC_UNDS, KC_PLUS, KC_PIPE,
        KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_EQUAL, KC_BSLS,
        KC_TRNS, KC_NO, KC_MINUS, KC_NO, KC_MEDIA_PLAY_PAUSE, KC_MUTE, KC_NO,
        KC_TRNS, KC_MEDIA_PREV_TRACK, KC_AUDIO_VOL_DOWN, KC_AUDIO_VOL_UP, KC_MEDIA_NEXT_TRACK,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    [_ADJUST] = LAYOUT_ergodox(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        LALT(LCTL(KC_DELETE)), DYN_REC_START1, DYN_REC_START2, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_TRNS, DYN_MACRO_PLAY1, DYN_MACRO_PLAY2, KC_NO, VIM_FIND, KC_NO,
        KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        DYN_REC_STOP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, VIM_YANK, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, VIM_PASTE, BL_TOGG, BL_INC, BL_DEC, MAGIC_TOGGLE_NKRO,
        KC_NO, KC_NO, KC_NO, BL_STEP, KC_NO, KC_NO, RESET,
        KC_TRNS, KC_NO, KC_NO, KC_NO, EEPROM_RESET,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    [_NUMPAD] = LAYOUT_ergodox(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_TRNS, KC_7, KC_8, KC_9, KC_LPRN, KC_RPRN, KC_NO,
        KC_TRNS, KC_4, KC_5, KC_6, KC_QUOTE, KC_DQT,
        KC_TRNS, KC_1, KC_2, KC_3, KC_COMMA, KC_DOT, KC_NO,
        KC_TRNS, KC_SPACE, KC_0, KC_ENTER, KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

        KC_PLUS, KC_MINUS, KC_ASTR, KC_SLASH, KC_NO, KC_NO, KC_TRNS,
        KC_NO, VIM_YANK, KC_NO, KC_NO, KC_NO, VIM_PASTE, KC_NO,
        VIM_PREVIOUS, KC_NO, KC_NO, VIM_NEXT, KC_NO, KC_NO,
        KC_NO, KC_LT, KC_GT, KC_NO, KC_NO, KC_NO, KC_TRNS,
        KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};

/********animation related start********/
// Images credit j-inc(/James Incandenza) and pixelbenny. Credit to obosob for initial animation approach.
static void render_anim(uint8_t wpm) {
    // assumes 1 frame prep stage
    void animation_phase(void) {
        if (wpm <= IDLE_SPEED) {
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            st7565_write_raw_P(idle[abs((IDLE_FRAMES - 1) - current_idle_frame)], ANIM_SIZE);
        }
        if (wpm > IDLE_SPEED && wpm < TAP_SPEED) {
            // oled_write_raw_P(prep[abs((PREP_FRAMES-1)-current_prep_frame)], ANIM_SIZE); // uncomment if IDLE_FRAMES >1
            st7565_write_raw_P(prep[0], ANIM_SIZE);  // remove if IDLE_FRAMES >1
        }
        if (wpm >= TAP_SPEED) {
            current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
            st7565_write_raw_P(tap[abs((TAP_FRAMES - 1) - current_tap_frame)], ANIM_SIZE);
        }
    }
    if (wpm != 000) {
        // st7565_on(); // not essential but turns on animation OLED with any alpha keypress
        if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
        anim_sleep = timer_read32();
    } else {
        if (timer_elapsed32(anim_sleep) > ST7565_TIMEOUT) {
            // st7565_off();
        } else {
            if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}
/********animation related end********/
uint32_t layer_state_set_user(uint32_t state) { return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST); }

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
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
        case VIM_FIND:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_V) SS_DELAY(100) SS_TAP(X_T) SS_DELAY(100) SS_TAP(X_SCOLON));
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
void st7565_task_user(void) {
    if (!st7565_is_on()) {
        return;
    }
    int     higestLayer = get_highest_layer(layer_state);
    led_t   leds        = host_keyboard_led_state();
    uint8_t n           = get_current_wpm();
    switch (higestLayer) {
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
            ergodox_infinity_lcd_color(UINT16_MAX / 2, UINT16_MAX / 2, UINT16_MAX / 2);
    }
    if (is_keyboard_master()) {
        switch (higestLayer) {
            case _BASE:
                st7565_write_P(PSTR("BASE LAYER\n"), false);
                break;
            case _LOWER:
                st7565_write_P(PSTR("LOWER LAYER\n"), false);
                break;
            case _RAISE:
                st7565_write_P(PSTR("RAISE LAYER\n"), false);
                break;
            case _ADJUST:
                st7565_write_P(PSTR("ADJUST LAYER\n"), false);
                break;
            case _NUMPAD:
                st7565_write_P(PSTR("NUMBERPAD LAYER\n"), false);
                break;
            default:
                st7565_write_ln_P(PSTR("Oops Undefined\n"), false);
        }
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
        st7565_write("\n", false);
        sprintf(wpm_str, "    WPM: %03d", n);
        st7565_write(wpm_str, false);
    } else {
        render_anim(n);
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
