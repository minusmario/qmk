#pragma once

#ifdef AUDIO_ENABLE
    #define STARTUP_SONG SONG(STARTUP_SOUND)
#endif

/*
 * MIDI options
 */

/* Prevent use of disabled MIDI features in the keymap */
//#define MIDI_ENABLE_STRICT 1

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 2

// Most tactile encoders have detents every 4 stages
#define ENCODER_RESOLUTION 4



#undef DEBOUNCE
#define DEBOUNCE 10

#undef TAPPING_TERM
#define TAPPING_TERM 150
#define PERMISSIVE_HOLD

#undef ONESHOT_TIMEOUT
#define ONESHOT_TIMEOUT 3000
#define LEADER_TIMEOUT 500 
#define LEADER_PER_KEY_TIMING
#define USB_POLLING_INTERVAL_MS 1
/* fix no startup song according to: https://www.reddit.com/r/olkb/comments/r6cfvn/help_startup_planck_song_gone_after_qmk_flash/ */
#define AUDIO_INIT_DELAY
// #define USB_SUSPEND_WAKEUP_DELAY 200
