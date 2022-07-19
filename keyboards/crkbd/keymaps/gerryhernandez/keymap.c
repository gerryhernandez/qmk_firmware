/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define TAPPING_TERM_PER_KEY
#define IGNORE_MOD_TAP_INTERRUPT_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define PERMISSIVE_HOLD_PER_KEY
#define COMBO_ONLY_FROM_LAYER 0
#define COMBO_TERM_PER_COMBO
#define AUTO_SHIFT_TIMEOUT_PER_KEY

#include "action_util.h"
#include "quantum.h"
#include "action_code.h"
#include "action_layer.h"
#include "crkbd.h"
#include "crkbd/rev1/rev1.h"
#include "action.h"
#include "action_tapping.h"
#include "keycode.h"
#include "quantum_keycodes.h"

#include <stdio.h>
#include <stdint.h>

// ######################################################################
// Tap Dance
// ######################################################################

typedef struct {
  bool is_press_action;
  int state;
} tap_state_t;

//Tap Dance Declarations
enum {
  TD_LALT = 0
};

// Tap Dance Funcs/handlers

void td_lalt_finished (qk_tap_dance_state_t *state, void *user_data) {
    bool is_hold = state->pressed && !state->interrupted;
    switch(state->count) {
        case 1:
            register_code(is_hold ? KC_LALT : KC_HOME);
            break;
        case 2:
            register_code(KC_END);
            break;
    }
}

void td_lalt_reset (qk_tap_dance_state_t *state, void *user_data) {
    bool is_hold = state->pressed && !state->interrupted;
    switch(state->count) {
        case 1:
            unregister_code(is_hold ? KC_LALT : KC_HOME);
            break;
        case 2:
            unregister_code(KC_END);
            break;
    }
}

//Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_LALT]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_lalt_finished, td_lalt_reset)
};


// ######################################################################
// Layers
// ######################################################################

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------------------.                                     ,-------------------------------------------------------------------------------.
       KC_TAB,          KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                                KC_J,        KC_L,       KC_U,          KC_Y,        KC_SCLN,       KC_MINS,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
   LT(6, KC_ESCAPE),    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                                KC_M,        KC_N,       KC_E,          KC_I,           KC_O,       KC_QUOT,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
     TD(TD_LALT),       KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                                                KC_K,        KC_H,   LT(3, KC_COMM),  LT(4, KC_DOT), KC_SLSH, ALT_T(KC_DELETE),
  //|--------------+--------+--------+--------+--------+--------------+--------|               |------------+---------------+--------+--------------+---------------+--------+--------------|
                                                _______, SFT_T(KC_SPC), KC_LGUI,               CTL_T(KC_ENT), LT(2, KC_BSPC),   MO(5)
                                              /* ^^ sustain key */
                                            //`--------------------------------'               `-------------------------------------'
  ),

  // Gaming
  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------------------.                                     ,-------------------------------------------------------------------------------.
       KC_TAB,          KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                                KC_J,        KC_L,       KC_U,          KC_Y,        KC_SCLN,       KC_MINS,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
       KC_ESC,          KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                                KC_M,        KC_N,       KC_E,          KC_I,           KC_O,       KC_QUOT,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
     KC_LEFT_ALT,       KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                                                KC_K,        KC_H,   LT(3, KC_COMM),  LT(4, KC_DOT), KC_SLSH, ALT_T(KC_END),
  //|--------------+--------+--------+--------+--------+--------------+-------------|          |------------+---------------+--------+--------------+---------------+--------+--------------|
                                           KC_LEFT_CTRL,  KC_SPC,      KC_LEFT_SHIFT,          CTL_T(KC_ENT), LT(2, KC_BSPC),   MO(5)
                                        //`----------------------------------------'           `-------------------------------------'
  ),

  // Navigation, Numbers/Symbols
  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______,   KC_UP, _______, _______,                      _______,    KC_4,    KC_5,    KC_6, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        DF(0), _______, KC_LEFT, KC_DOWN, KC_RGHT, _______,                       KC_GRV,    KC_1,    KC_2,    KC_3,   KC_P0,  KC_EQL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______,    KC_7,    KC_8,   KC_P9, KC_BSLS, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          DF(2), G(KC_SPC), _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

  // Window Management (comma key)
  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        DF(0), _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            DF(3), _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

  // TBD - (period key)
  [4] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        DF(0), _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            DF(4), _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

  // Mouse and F1-F12
  [5] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, KC_BTN2, KC_MS_U, KC_BTN1, _______,                      _______,   KC_F4,   KC_F5,   KC_F6, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        DF(0), _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                       KC_F12,   KC_F1,   KC_F2,   KC_F3,  KC_F10,  KC_F11,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, KC_WH_D, KC_WH_U, _______,                      _______,   KC_F7,   KC_F8,   KC_F9, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            DF(5), _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

  // Syms 2
  [6] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        DF(0), _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            DF(6), _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

  // Settings
  [7] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        DF(0), _______, _______, _______, _______, _______,                      _______,   TO(1), _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

/*
  [] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),
*/

};

// ######################################################################
// Per-key tap/hold configuration
// ######################################################################

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return 175;
    }
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case SFT_T(KC_SPC):
            // Do not force the mod-tap key press to be handled as a modifier
            // if any other key was pressed while the mod-tap key is held down.
            // return true;
        default:
            // Force the mod-tap key press to be handled as a modifier if any
            // other key was pressed while the mod-tap key is held down.
            return false;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case LT(2, KC_BSPC):
            // Do not select the hold action when another key is pressed.
            // return false;
        default:
            // Immediately select the hold action when another key is pressed.
            return true;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case LT(1, KC_BSPC):
            // Do not select the hold action when another key is tapped.
            // return false;
        default:
            // Immediately select the hold action when another key is tapped.
            return true;
    }
}


// ######################################################################
// Combos
// ######################################################################

enum combo_events {
  L_PAREN,
  R_PAREN,
  L_BRACE,
  R_BRACE,
  L_BRACK,
  R_BRACK,
  LESS_THN,
  GRTR_THN,
  SETTINGS,
  COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH; // QMK requires COMBO_LEN to be set

const uint16_t PROGMEM l_paren_combo[] = {KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM r_paren_combo[] = {KC_N, KC_E, COMBO_END};
const uint16_t PROGMEM l_brace_combo[] = {KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM r_brace_combo[] = {KC_H, LT(3, KC_COMMA), COMBO_END};
const uint16_t PROGMEM l_brack_combo[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM r_brack_combo[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM less_thn_combo[] = {KC_S, KC_G, COMBO_END};
const uint16_t PROGMEM greater_thn_combo[] = {KC_M, KC_E, COMBO_END};
const uint16_t PROGMEM settings_combo[] = {KC_L, KC_U, KC_Y, KC_SCLN, COMBO_END};

combo_t key_combos[] = {
  [L_PAREN] = COMBO_ACTION(l_paren_combo),
  [R_PAREN] = COMBO_ACTION(r_paren_combo),
  [L_BRACE] = COMBO_ACTION (l_brace_combo),
  [R_BRACE] = COMBO_ACTION (r_brace_combo),
  [L_BRACK] = COMBO_ACTION (l_brack_combo),
  [R_BRACK] = COMBO_ACTION (r_brack_combo),
  [LESS_THN] = COMBO_ACTION(less_thn_combo),
  [GRTR_THN] = COMBO_ACTION(greater_thn_combo),
  [SETTINGS] = COMBO_ACTION(settings_combo)
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    void do_key(uint16_t kc) {
        if (pressed) {
            register_code16(kc);
        } else {
            unregister_code16(kc);
        }
    }
    switch (combo_index) {
        case L_PAREN:
            do_key(S(KC_9));
            break;

        case R_PAREN:
            do_key(S(KC_0));
            break;

        case L_BRACE:
            do_key(S(KC_LBRC));
            break;

        case R_BRACE:
            do_key(S(KC_RBRC));
            break;

        case L_BRACK:
            do_key(KC_LBRC);
            break;

        case R_BRACK:
            do_key(KC_RBRC);
            break;

        case LESS_THN:
            do_key(S(KC_COMM));
            break;

        case GRTR_THN:
            do_key(S(KC_DOT));
            break;

        case SETTINGS:
            if(pressed) {
                set_oneshot_layer(7, ONESHOT_START);
            } else {
                set_oneshot_layer(7, ONESHOT_PRESSED);
            }
            break;
    }
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if(combo_index == SETTINGS) {
        return true;
    }
    return layer_state_is(0);
}

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch(index) {
        case L_PAREN:
        case R_PAREN:
            return 30;
    }
    return 40;
}



// ######################################################################
// Auto Shift
// ######################################################################

uint16_t get_autoshift_timeout(uint16_t keycode, keyrecord_t *record) {
    return 175;
}

bool get_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    if(layer_state_is(1)) {
        return false;
    }
    switch (keycode) {
        case KC_1 ... KC_0:
        case KC_MINUS ... KC_SLASH:
        case KC_NONUS_BACKSLASH:
            return true;
    }
    return false;
}
