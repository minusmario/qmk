 /* Copyright 2021 MinusMario
  *
  * sofle rgb layout
  */
  // SOFLE RGB
#include <stdio.h>

#include QMK_KEYBOARD_H

#define INDICATOR_BRIGHTNESS 30

#define HSV_OVERRIDE_HELP(h, s, v, Override) h, s , Override

// Light combinations
#define SET_INDICATORS(hsv) \
    {0, 1, HSV_OVERRIDE_HELP(hsv, INDICATOR_BRIGHTNESS)}

enum sofle_layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
    _NUMPAD,
};

enum custom_keycodes {
    KC_LOWER = SAFE_RANGE,
    KC_RAISE,
    KC_ADJUST,
    MARCO_VIM_YANK,
    MARCO_VIM_PASTE,
    MARCO_VIM_FIND,
};

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |ESC   | Q  |   W    |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |TAB/Ctrl|   A  |   S  |   D  |   F  |  G   |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+--------|  MUTE   |  | LOCK  |------+------+------+------+------+------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |Shift/Enter|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |NumPad| WIN  |LOWER | LALT | /Space  /       \Enter \  | RALT |RAISE | RightClick | RCTR |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_QWERTY] = LAYOUT(
  //,------------------------------------------------.                   ,---------------------------------------------------.
  KC_ESC,   KC_1, KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,   KC_8,    KC_9,    KC_0,    KC_BSPC,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_TAB,   KC_Q, KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_LCTRL, KC_A, KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  KC_LSFT, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  KC_MUTE,  LGUI(KC_L),KC_N,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                  KC_LGUI, KC_LALT, KC_LOWER ,KC_SPC, MO(_NUMPAD),  KC_ENT, KC_SPC , KC_RAISE, KC_RALT, KC_RCTRL
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),

/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  ~   |   !  |   @  |   #  |   $  |   %  |                    |      |      |      |      |  (   |   )  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | trans|      |      |      |      |      |-------.    ,-------| left | down |  up  | right|   [  |   ]  |
 * |------+------+------+------+------+------| trans |    | trans |------+------+------+------+------+------|
 * | trans|      |      |      |      |      |-------|    |-------|      |      |      |      |   {  |   }  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | trans| trans|trans | trans| /trans  /       \trans \  |trans |trans | trans | trans |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_LOWER] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  KC_F1,     KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_TILD,  KC_EXLM, KC_AT,  KC_HASH,  KC_DLR, KC_PERC,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LPRN, KC_RPRN,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT,KC_LBRACKET, KC_RBRACKET,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,_______,   _______,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LCBR,  KC_RCBR,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                 _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
/* RAISE
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |   `  |      |      |insert| home |pageup|                    |   ^  |   &  |  *   |  _   |   +  |  |   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |trans |marco1|marco2|delete| end  |pagedown|------.    ,------| Left | Down | Up   | Rigth|   =  |  \   |
 * |------+------+------+------+------+------| trans |    | trans |------+------+------+------+------+------|
 * |trans |      |      |      |      |      |-------|    |-------|pause|-|printscreen | next |previous|    |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | trans| trans|trans | trans| /trans  /       \trans \  |trans |trans | trans | trans |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_RAISE] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  KC_F1,     KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_GRAVE, XXXXXXX, XXXXXXX, KC_INSERT,KC_HOME, KC_PGUP,                 KC_CIRC,  KC_AMPR, KC_ASTR, KC_UNDS, KC_PLUS, KC_PIPE,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______, DYN_MACRO_PLAY1,DYN_MACRO_PLAY2,KC_DELETE,KC_END,KC_PGDOWN,     KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT, KC_EQUAL,KC_BSLASH,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  _______, XXXXXXX,  XXXXXXX, XXXXXXX,XXXXXXX,XXXXXXX,_______,     _______,KC_PAUSE, KC_MINUS,KC_PSCREEN,KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK,XXXXXXX,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                 _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |^+alt+del|marco1|marco2|   |      |      |                    |      | "+y  |      |speed+|speed-|      |
 * |------+------+------+- ----+------+------|                    |------+------+------+------+------+------|
 * |trans |nlock |capslock|scroll_lock|vt;|  |-------.    ,-------|      | "+p  |rgb_tog|val+ | val- | NKRO |
 * |------+------+------+------+------+------| trans |    | trans |------+------+------+------+------+------|
 * |trans |      |      |      |      |      |-------|    |-------|      |      |rgb_mod|hue+ | hue- |reset |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |marcostop|trans|trans|trans| /trans  /       \trans \  |trans |trans | trans | trans |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_ADJUST] = LAYOUT(
  //,------------------------------------------------.                        ,---------------------------------------------------.
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                        |--------+-------+--------+--------+--------+---------|
  LALT(LCTL(KC_DELETE)),DYN_REC_START1,DYN_REC_START2,XXXXXXX,XXXXXXX,XXXXXXX, XXXXXXX, MARCO_VIM_YANK, XXXXXXX, RGB_SPI,RGB_SPD,XXXXXXX,
  //|------+-------+--------+--------+--------+------|                        |--------+-------+--------+--------+--------+---------|
  _______,  KC_NLCK, KC_CAPS,KC_SLCK, MARCO_VIM_FIND,XXXXXXX,                  XXXXXXX,   MARCO_VIM_PASTE,RGB_TOG, RGB_VAI,RGB_VAD,MAGIC_TOGGLE_NKRO,
  //|------+-------+--------+--------+--------+------|  ===  |        |  ===  |--------+-------+--------+--------+--------+---------|
  _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,_______,       _______,XXXXXXX, XXXXXXX, RGB_MOD, RGB_HUI,RGB_HUD,RESET,
  //|------+-------+--------+--------+--------+------|  ===  |        |  ===  |--------+-------+--------+--------+--------+---------|
                   DYN_REC_STOP, _______, _______, _______, _______,    _______, _______, _______,  RGB_SAI, RGB_SAD
    //            \--------+--------+--------+---------+-------|      |--------+---------+--------+---------+-------/
),
/* NUMPAD
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | trans|      |      |      |      |      |                    |  +   |  ,   |  .   |  '   |  "   | trans  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | trans|      |      |      |      |      |                    |  -   |   7  |   8  |   9  |   (  |  )   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|  *   |   4  |   5  |   6  |   [  |  ]   |
 * |------+------+------+------+------+------| trans |    | trans |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|  /   |   1  |   2  |   3  |   {  |  }   |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | trans| trans|trans | trans| /trans  /       \trans \  |trans |trans |  0   | trans |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_NUMPAD] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_PLUS, KC_COMMA, KC_DOT, KC_QUOTE, KC_DQT, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_MINS,  KC_7,   KC_8,    KC_9,   KC_LPRN, KC_RPRN,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_ASTR,  KC_4,   KC_5,    KC_6,   KC_LBRACKET, KC_RBRACKET,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,_______,   _______,KC_SLSH,  KC_1,   KC_2,    KC_3,   KC_LCBR,  KC_RCBR,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                    _______, _______, _______, _______, _______,   _______, _______, _______, KC_0, _______
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
)
};

#ifdef RGBLIGHT_ENABLE
char layer_state_str[70];
// Now define the array of layers. Later layers take precedence
const rgblight_segment_t PROGMEM layer_base_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_CYAN)
);
const rgblight_segment_t PROGMEM layer_lower_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_RED)
);
const rgblight_segment_t PROGMEM layer_raise_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_YELLOW)
);
const rgblight_segment_t PROGMEM layer_adjust_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_GREEN)
);
const rgblight_segment_t PROGMEM layer_numpad_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_PURPLE)
);

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    layer_base_lights,
    layer_lower_lights,
    layer_raise_lights,
    layer_adjust_lights,
    layer_numpad_lights
);
layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state,_QWERTY));
    rgblight_set_layer_state(1, layer_state_cmp(state,_LOWER));
    rgblight_set_layer_state(2, layer_state_cmp(state,_RAISE));
    rgblight_set_layer_state(3, layer_state_cmp(state,_ADJUST));
    rgblight_set_layer_state(4, layer_state_cmp(state,_NUMPAD));
    return state;
}
void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;

    rgblight_mode(10);// haven't found a way to set this in a more useful way

}
#endif

#ifdef OLED_DRIVER_ENABLE

char wpm_str[10];

static void print_status_narrow(void) {
    oled_write_P(PSTR("\n\n"), false);
    oled_write_ln_P(PSTR("Minus\nMario"), false);
    // Print current layer
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("BASE"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("RAIS"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("LOWE"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("ADJ"), false);
            break;
        case _NUMPAD:
            oled_write_ln_P(PSTR("NUMP"), false);
            break;
        default:
            oled_write_ln_P(PSTR("UNDEF"), false);
    }
    oled_write_P(PSTR("\n"), false);
    led_t led_state = host_keyboard_led_state();
    if (led_state.num_lock) {
        oled_write_ln_P(PSTR("NUM"), false);
    } else {
        oled_write_P(PSTR("\n"), false);
    }
    if (led_state.caps_lock) {
        oled_write_ln_P(PSTR("CAPS"), false);
    } else {
        oled_write_P(PSTR("\n"), false);
    }
    if (led_state.scroll_lock) {
        oled_write_ln_P(PSTR("SCRL"), false);
    } else {
        oled_write_P(PSTR("\n"), false);
    }
}

static void render_logo(void) {
    static const char PROGMEM raw_logo[] = {
        255,255,255,255,255,255,255,255,255,127, 63, 63, 15,  7,135,195,225,241,241,241,241,193,129,195,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255, 47,  7,  3,131,199,255,255,255, 63, 31, 11,129,193,225,243,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255, 63,  7,  1,  0, 48, 12, 
        3,129,224,248,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  7,  1,  0,240,252,254,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255, 31, 15, 15, 31,207,135,131,193,241,241,241,241,113,  1,  1,  1,  3,  3,  1,224,248,254,255,255, 63,  7,  3,193,240,248,254,255,255,255,255,255,255, 63, 15,  7,131,225,241,193,129,  3, 15,  7,  7,  7,195,241,240,248,248,248,248,248,  0,  0,  1,  1,  0, 
        0,192,224,248,252,255,255,255,255,255,255, 31, 15, 15,143,143,199,199,199,199,199,199,199,199,143, 15, 31, 63, 63,127,255,255,255,255,255,255,255,255,192,128,128,143, 15, 31, 31, 31, 31, 15,143,143,143,143,135,199,195,227,227,227,195,227,255,255,255,255,255,224,192,128,143,143,143,143,135,131,193,241,240,248,252,158,  0,  0,  0, 31,191,255, 15,  1,  0, 96,252,254,255,255,255,255,255,255,255,255,255,131,  0,  0, 24, 15,143,143,135,193,192,128,128,  0, 
        0,143,143,143,135,199,195,227,241,241,248,192,128,128,143, 15, 31, 63, 63, 63,127,255,255,255,  7,  0,  0,248,255,255,255,255,255,255,127,127,127, 63, 31, 31, 
        
        5,255,255,255,255,255,
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
    }
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case KC_RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case KC_ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
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

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (get_highest_layer(layer_state)) {
            case _RAISE:
                if (clockwise) {
                    SEND_STRING("gt");
                } else {
                    SEND_STRING("gT");
                }
                break;
            default:
                if (clockwise) {
                    tap_code(KC_WH_R);
                } else {
                    tap_code(KC_WH_L);
                }
                break;
        }
    } else if (index == 1) {
        switch (get_highest_layer(layer_state)) {
            case _LOWER:
                if (clockwise) {
                    tap_code(KC_PGDOWN);
                } else {
                    tap_code(KC_PGUP);
                }
                break;
            default:
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                    tap_code(KC_TAB);
                }
                alt_tab_timer = timer_read();
                if (clockwise) {
                    tap_code(KC_RIGHT);
                } else {
                    tap_code(KC_LEFT);
                }
                break;
        }
    }
    return true;
}

void matrix_scan_user(void) { // The very important timer.
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1000) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }
}

#endif
