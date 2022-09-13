/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

#pragma once

//#define USE_MATRIX_I2C

/* Select hand configuration */

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

//#define TAPPING_FORCE_HOLD
//#define TAPPING_TERM 100

#ifdef RGBLIGHT_ENABLE
    #define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_RAINBOW_MOOD
    #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
    #define RGBLIGHT_EFFECT_SNAKE
    #define RGBLIGHT_EFFECT_KNIGHT
    #define RGBLIGHT_EFFECT_CHRISTMAS
    #define RGBLIGHT_EFFECT_STATIC_GRADIENT
    #define RGBLIGHT_EFFECT_RGB_TEST
    #define RGBLIGHT_EFFECT_ALTERNATING
    #define RGBLIGHT_EFFECT_TWINKLE
    #define RGBLIGHT_LIMIT_VAL 120
    #define RGBLIGHT_HUE_STEP 10
    #define RGBLIGHT_SAT_STEP 17
    #define RGBLIGHT_VAL_STEP 17
#endif

// #define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"

#define TAPPING_TERM_PER_KEY
#define IGNORE_MOD_TAP_INTERRUPT_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define PERMISSIVE_HOLD_PER_KEY
#define COMBO_ONLY_FROM_LAYER 0
#define COMBO_SHOULD_TRIGGER
#define COMBO_TERM_PER_COMBO
#define AUTO_SHIFT_TIMEOUT_PER_KEY
#define COMBO_MUST_HOLD_PER_COMBO

#define MK_KINETIC_SPEED                          // Enable Kinetic mousekey movement
#define MOUSEKEY_DELAY 	                      5 	// Delay between pressing a movement key and cursor movement
#define MOUSEKEY_INTERVAL 	                  10  // Time between cursor movements in milliseconds
#define MOUSEKEY_MOVE_DELTA 	                1   // Step size for accelerating from initial to base speed
#define MOUSEKEY_INITIAL_SPEED 	              1  	// Initial speed of the cursor in pixel per second
#define MOUSEKEY_BASE_SPEED 	                1 	// Maximum cursor speed at which acceleration stops
#define MOUSEKEY_DECELERATED_SPEED 	          1  	// Decelerated cursor speed
#define MOUSEKEY_ACCELERATED_SPEED 	          1 	// Accelerated cursor speed
#define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS 	    16 	// Initial number of movements of the mouse wheel
#define MOUSEKEY_WHEEL_BASE_MOVEMENTS 	      32 	// Maximum number of movements at which acceleration stops
#define MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS 	8 	// Accelerated wheel movements
#define MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS 	8 	// Decelerated wheel movements


