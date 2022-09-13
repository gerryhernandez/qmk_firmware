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

#include <stdio.h>
#include <stdint.h>

#include "action.h"
#include "action_code.h"
#include "action_layer.h"
#include "action_tapping.h"
#include "action_util.h"
#include "crkbd.h"
#include "crkbd/rev1/rev1.h"
#include "keycode.h"
#include "keycode_legacy.h"
#include "progmem.h"
#include "quantum.h"
#include "quantum_keycodes.h"

// Func Prototypes:
bool is_on_gaming_layer(void);
void go_to_home_layer(void);

// State:
typedef union {
    uint32_t bits;
    struct {
        bool is_qwerty : 1;
        bool is_gaming : 1;
        bool is_alpha_auto_shift : 1;
    };
} custom_state_t;
custom_state_t custom_state;


// ######################################################################
// Custom Keycodes
// ######################################################################

enum custom_keycodes {
  CKC_LBRACE = SAFE_RANGE,
  CKC_RBRACE,
  CKC_LPAREN,
  CKC_RPAREN,
  CKC_HOMEL, // Home Layer
  CKC_S_CLMK, // Setting: Colemak
  CKC_S_QWRT, // Setting: Qwerty
  CKC_S_NORM, // Setting: Normal Mode
  CKC_S_GAME, // Setting: Game Mode
  CKC_S_AAS1, // Setting: Alpha Autoshift On
  CKC_S_AAS0, // Setting: Alpha Autoshift Off
};


// ######################################################################
// Layers
// ######################################################################

#define L_COLEMAK 0
#define L_QWERTY  1
#define L_GAME    2
#define L_NAVNUM  3
#define L_COMMA   4
#define L_PERIOD  5
#define L_MOUSEFN 6
#define L_SYMS2   7
#define L_GAMEALT 8
#define L_SETTING 9

// LK = "layer key"
#define LK_NAVNUM  LT(L_NAVNUM, KC_BSPC)
#define LK_MOUSEFN LT(L_MOUSEFN, KC_DEL)
#define LK_COMMA   LT(L_COMMA, KC_COMM)
#define LK_PERIOD  LT(L_PERIOD, KC_DOT)
#define LK_SYMS2   LT(L_SYMS2, KC_ESC)
#define LK_GAMEALT LT(L_GAMEALT, KC_EQUAL)

// Layer masks
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Colemak:
  [L_COLEMAK] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------------------.                                     ,-------------------------------------------------------------------------------.
             KC_TAB,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                               KC_J,        KC_L,       KC_U,          KC_Y,        KC_SCLN,     KC_MINUS,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
           LK_SYMS2,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                                KC_M,        KC_N,       KC_E,          KC_I,           KC_O,     KC_QUOTE,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
     ALT_T(KC_HOME),    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                                                KC_K,        KC_H,      LK_COMMA,     LK_PERIOD,     KC_SLSH, ALT_T(KC_END),
  //|--------------+--------+--------+--------+--------+--------------+--------|               |------------+---------------+--------+--------------+---------------+--------+--------------|
                                                KC_CAPS, SFT_T(KC_SPC), KC_LGUI,               CTL_T(KC_ENT),    LK_NAVNUM, LK_MOUSEFN
                                              /* ^^ sustain key */
                                            //`--------------------------------'               `-------------------------------------'
  ),

  // QWERTY:
  [L_QWERTY] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------------------.                                     ,-------------------------------------------------------------------------------.
            _______,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                                KC_Y,        KC_U,       KC_I,          KC_O,           KC_P,      _______,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
            _______,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                                KC_H,        KC_J,       KC_K,          KC_L,   KC_SEMICOLON,      _______,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
            _______,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                                KC_N,        KC_M,          _______,        _______, _______,      _______,
  //|--------------+--------+--------+--------+--------+--------------+--------|               |------------+---------------+--------+--------------+---------------+--------+--------------|
                                                _______,       _______, _______,                     _______,        _______, _______
                                              /* ^^ sustain key */
                                            //`--------------------------------'               `-------------------------------------'
  ),

  // Gaming
  [L_GAME] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------------------.                                     ,-------------------------------------------------------------------------------.
      KC_TAB,        _______, _______, _______, _______, _______,                                               _______,      _______,     _______,       _______,    _______,    _______,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
       KC_HOME,      _______, _______, _______, _______, _______,                                               _______,      _______,     _______,       _______,    _______,    _______,
  //|--------------+--------+--------+--------+--------+--------------|                                     |---------------+--------+--------------+---------------+--------+--------------|
      LK_GAMEALT,    _______, _______, _______, _______, _______,                                               _______,      _______,     _______,       _______,    _______,    _______,
  //|--------------+--------+--------+--------+--------+--------------+-------------|          |------------+---------------+--------+--------------+---------------+--------+--------------|
                                      KC_LCTRL,  KC_SPC, KC_LEFT_SHIFT,                              _______,     _______,    _______
                                        //`----------------------------------------'           `-------------------------------------'
  ),

  // Navigation, Numbers/Symbols
  [L_NAVNUM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                        ,-----------------------------------------------------.
      _______, _______, _______,   KC_UP, _______, _______,                          _______,    KC_4,    KC_5,    KC_6, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                        |--------+--------+--------+--------+--------+--------|
    CKC_HOMEL, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______,                         KC_EQUAL,    KC_1,    KC_2,    KC_3,    KC_0, _______,
  //|--------+--------+--------+--------+--------+--------|                        |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                          _______,    KC_7,    KC_8,    KC_9, KC_BSLS, _______,
  //|--------+--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
                                     DF(L_NAVNUM), G(KC_SPC), _______,        _______, _______, _______
                                      //`--------------------------'      `--------------------------'
  ),

  // Brackets (comma key)
  [L_COMMA] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------------------.                           ,-----------------------------------------------------.
      _______, _______,    _______,    _______,    _______,    _______,                             _______, _______, _______, _______, _______, _______,
  //|--------+--------+-----------+-----------+-----------+-----------|                           |--------+--------+--------+--------+--------+--------|
    CKC_HOMEL, _______, CKC_LBRACE, CKC_LPAREN, CKC_RPAREN, CKC_RBRACE,                             _______, _______, _______, _______, _______, _______,
  //|--------+--------+-----------+-----------+-----------+-----------|                           |--------+--------+--------+--------+--------+--------|
      _______, _______,    _______,    _______,    _______,    _______,                             _______, _______, _______, _______, _______, _______,
  //|--------+--------+-----------+-----------+-----------+-----------+--------|         |--------+--------+--------+--------+--------+--------+--------|
                                               DF(L_COMMA),    _______, _______,           _______, _______, _______
                                            //`--------------------------------'         `--------------------------'
  ),

  // TBD - (period key)
  [L_PERIOD] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    CKC_HOMEL, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                              DF(L_PERIOD), _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

  // Mouse and F1-F12
  [L_MOUSEFN] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, KC_BTN2, KC_MS_U, KC_BTN1, _______,                      _______,   KC_F4,   KC_F5,   KC_F6,  KC_F11, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    CKC_HOMEL, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                      _______,   KC_F1,   KC_F2,   KC_F3,  KC_F10, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, KC_WH_D, KC_WH_U, _______,                      _______,   KC_F7,   KC_F8,   KC_F9,  KC_F12, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                    DF(L_MOUSEFN), _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

  // Window Management (comma key)
  [L_SYMS2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                              ,-----------------------------------------------------.
      _______, _______, _______, G(KC_K), _______, _______,                                _______, G(KC_4), G(KC_5), G(KC_6), _______, G(S(KC_BSLS)),
  //|--------+--------+--------+--------+--------+--------|                              |--------+--------+--------+--------+--------+--------|
    CKC_HOMEL, _______, G(KC_H), G(KC_J), G(KC_L), _______,                                _______, G(KC_1), G(KC_2), G(KC_3), _______, _______,
  //|--------+--------+--------+--------+--------+--------|                              |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                                _______, G(KC_7), G(KC_8), G(KC_9), _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|            |--------+--------+--------+--------+--------+--------+--------|
                                      DF(L_SYMS2), _______, _______,            G(KC_ENT), _______, _______
                                      //`--------------------------'            `--------------------------'
  ),

  // Gaming Alt
  [L_GAMEALT] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_ESC,  KC_END,    KC_6,    KC_5,    KC_4,   KC_UP,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LEFT,    KC_0,    KC_3,    KC_2,    KC_1, KC_PGUP,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_DOWN,    KC_9,    KC_8,    KC_7, KC_PGDN,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

  // Settings
  [L_SETTING] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                         _______,    _______,    _______,    _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |-----------+-----------+-----------+-----------+--------+--------|
    CKC_HOMEL, _______, _______, _______, _______, _______,                      CKC_S_AAS1, CKC_S_CLMK, CKC_S_QWRT,    _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |-----------+-----------+-----------+-----------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      CKC_S_AAS0, CKC_S_NORM, CKC_S_GAME,    _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+-----------+-----------+-----------+-----------+--------+--------|
                                          _______, _______, _______,    _______,    _______,    _______
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

bool is_on_gaming_layer(void) {
    return layer_state_is(L_GAME) || layer_state_is(L_GAMEALT);
}

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
  CE_LHYPER,
  CE_RHYPER,
  CE_LMEH,
  CE_RMEH,
  CE_SETTINGS,
  COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH; // QMK requires COMBO_LEN to be set

const uint16_t PROGMEM lhyper_combo[] = {KC_T, KC_D, COMBO_END};
const uint16_t PROGMEM rhyper_combo[] = {KC_N, KC_H, COMBO_END};
const uint16_t PROGMEM lmeh_combo[] = {KC_T, KC_P, COMBO_END};
const uint16_t PROGMEM rmeh_combo[] = {KC_N, KC_L, COMBO_END};
const uint16_t PROGMEM settings_combo[] = {KC_L, KC_U, KC_Y, KC_SCLN, COMBO_END};

combo_t key_combos[] = {
  [CE_LHYPER] = COMBO_ACTION(lhyper_combo),
  [CE_RHYPER] = COMBO_ACTION(rhyper_combo),
  [CE_LMEH] = COMBO_ACTION(lmeh_combo),
  [CE_RMEH] = COMBO_ACTION(rmeh_combo),
  [CE_SETTINGS] = COMBO_ACTION(settings_combo),
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
        case CE_LHYPER:
        case CE_RHYPER:
            do_key(KC_HYPR);
            break;

        case CE_LMEH:
        case CE_RMEH:
            do_key(KC_MEH);
            break;

        case CE_SETTINGS:
            // do_key(OSL(L_SETTING));
            if(pressed) {
                set_oneshot_layer(L_SETTING, ONESHOT_START);
            } else {
                set_oneshot_layer(L_SETTING, ONESHOT_PRESSED);
            }
            break;
    }
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    switch(combo_index) {
        case CE_SETTINGS:
            return true;
    }

    return layer_state_is(custom_state.is_qwerty);
}

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    return 40;
}

bool get_combo_must_hold(uint16_t index, combo_t *combo) {
    return false;
}



// ######################################################################
// Auto Shift
// ######################################################################

uint16_t get_autoshift_timeout(uint16_t keycode, keyrecord_t *record) {
    return 175;
}

bool get_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    if(is_on_gaming_layer()) {
        return false;
    }
    switch (keycode) {
        case CKC_LBRACE ... CKC_RPAREN:
        case KC_A ... KC_Z:
          return custom_state.is_alpha_auto_shift;
        case KC_1 ... KC_0:
        case KC_MINUS ... KC_SLASH:
        case KC_NONUS_BACKSLASH:
            return true;
    }
    return false;
}

bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case CKC_LBRACE ... CKC_RPAREN:
        case KC_0:
        case KC_9:
            return true;
    }
    return false;
}

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    switch(keycode) {
        case CKC_LBRACE:
            register_code16(shifted ? KC_LEFT_BRACKET : S(KC_LEFT_BRACKET));
            break;

        case CKC_RBRACE:
            register_code16(shifted ? KC_RIGHT_BRACKET : S(KC_RIGHT_BRACKET));
            break;

        case CKC_LPAREN:
            register_code16(shifted ? S(KC_COMMA) : S(KC_9));
            break;

        case CKC_RPAREN:
            register_code16(shifted ? S(KC_DOT) : S(KC_0));
            break;

        case KC_0:
            register_code16(shifted ? KC_GRAVE : KC_0);
            break;

        case KC_9:
            register_code16(shifted ? S(KC_GRAVE) : KC_9);
            break;

        default:
            if (shifted) {
                add_weak_mods(MOD_BIT(KC_LSFT));
            }
            // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
            register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    switch(keycode) {
        case CKC_LBRACE:
            unregister_code16(shifted ? KC_LEFT_BRACKET : S(KC_LEFT_BRACKET));
            break;

        case CKC_RBRACE:
            unregister_code16(shifted ? KC_RIGHT_BRACKET : S(KC_RIGHT_BRACKET));
            break;

        case CKC_LPAREN:
            unregister_code16(shifted ? S(KC_COMMA) : S(KC_9));
            break;

        case CKC_RPAREN:
          unregister_code16(shifted ? S(KC_DOT) : S(KC_0));
          break;

        case KC_0:
            unregister_code16(shifted ? KC_GRAVE : KC_0);
            break;

        case KC_9:
            unregister_code16(shifted ? S(KC_GRAVE) : KC_9);
            break;

        default:
            // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
            // The IS_RETRO check isn't really necessary here, always using
            // keycode & 0xFF would be fine.
            unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

// ######################################################################
// Custom Behavior
// ######################################################################

// Util Funcs:

void go_to_home_layer(void) {
    layer_clear();
    layer_on(L_COLEMAK);
    if(custom_state.is_qwerty) {
        layer_on(L_QWERTY);
    }
    if(custom_state.is_gaming) {
        layer_on(L_GAME);
    }
}

// Hooks:

void keyboard_post_init_user(void) {
    custom_state.is_qwerty = false;
    custom_state.is_gaming = false;
    custom_state.is_alpha_auto_shift = true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    void do_key(uint16_t kc) {
        if (record->event.pressed) {
            register_code16(kc);
        } else {
            unregister_code16(kc);
        }
    }
    switch(keycode) {
        // Mac-style CMD+Left = HOME
        case G(KC_LEFT):
            do_key(KC_HOME);
            return false;

        // Mac-style CMD+Right = END
        case G(KC_RIGHT):
            do_key(KC_END);
            return false;

        case CKC_HOMEL:
            go_to_home_layer();
            return false;

        case CKC_S_CLMK:
            custom_state.is_qwerty = false;
            go_to_home_layer();
            return false;

        case CKC_S_QWRT:
            custom_state.is_qwerty = true;
            go_to_home_layer();
            return false;

        case CKC_S_NORM:
            custom_state.is_gaming = false;
            go_to_home_layer();
            return false;

        case CKC_S_GAME:
            custom_state.is_gaming = true;
            go_to_home_layer();
            return false;

        case CKC_S_AAS1:
            custom_state.is_alpha_auto_shift = true;
            go_to_home_layer();
            return false;

        case CKC_S_AAS0:
            custom_state.is_alpha_auto_shift = false;
            go_to_home_layer();
            return false;
    }
    return true; // Process all other keycodes normally
}
