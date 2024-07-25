// Copyright 2023 Danny Nguyen (@nooges)
// SPDX-License-Identifier: GPL-2.0-or-later

#define LAYERLOCK MT(MOD_LCTL,KC_INS)
#define BRACES KC_LBRC
#include QMK_KEYBOARD_H
#include "features/layer_lock.h"

enum custom_layer {
    _MAIN,
    _FN1,
    _FN2,
    _FN3
};

/*
			┌──────────────────────────────┐
			│      ADVANCED FONCTIONS      │
			└──────────────────────────────┘

Layer Lock Key (https://github.com/getreuer)
Braces macro (https://github.com/getreuer)
*/

bool process_record_user(uint16_t keycode, keyrecord_t* record) {

	const uint8_t mods = get_mods();
	const uint8_t oneshot_mods = get_oneshot_mods();
		
	if (!process_layer_lock(keycode, record, KC_NO)) { return false; }

	switch (keycode) {
		case LAYERLOCK:  // Control on hold, Layer Lock on tap.
		if (record->tap.count) {
			if (record->event.pressed) {
			// Toggle the lock on the highest layer.
			layer_lock_invert(get_highest_layer(layer_state));
			}
			return false;  // Skip default handling on tap.
		}
		return true;  // Continue default handling on hold.

		case MT(MOD_LCTL,KC_NO):
		if (record->tap.count && record->event.pressed) {
			rgb_matrix_toggle(); // Intercept tap to send RGB toggle function
			return false;        // Return false to ignore further processing of key
			}
			break;

		case BRACES:  // Types [], {}, «», or <> and puts cursor between braces.
		if (record->event.pressed) {
			clear_oneshot_mods();  // Temporarily disable mods.
			unregister_mods(MOD_MASK_CSAG);
			if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
				SEND_STRING("{}");
			} else if ((mods | oneshot_mods) & MOD_MASK_ALT) {
				SEND_STRING(SS_RALT("[]"));		// Send «»
			} else if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
				SEND_STRING("<>");
			} else {
				SEND_STRING("[]");
			}
			tap_code(KC_LEFT);  // Move cursor between braces.
			register_mods(mods);  // Restore mods.
			}
			return false;
    // Other macros...
	}
	return true;
};

/*
			┌──────────────────────────────┐
			│         KEY LIGHTNING        │
			└──────────────────────────────┘

Key lighting depending on keycode and active layer (https://github.com/fxkuehl/qmk_firmware/keyboards/mantis/mantis.c)

		After the variable "hsv.h =" then indicate a HUE value ranging from 0 to 255 or matrix_hsv.h to match matrix HUE
		After the variable "hsv.s =" then indicate a Saturation value ranging from 0 to 255 or matrix_hsv.s to match matrix Saturation
		After the variable "hsv.v =" then indicate a Brightness value ranging from 0 to 255 or matrix_hsv.v to match matrix Brightness

To use different colors on different layers for the same keycode 
		case KC_xyz:
			switch(layer) {
			case 1: hsv.h = 80; break;			// green
			case 2: hsv.h = 252; break;			// red
			default: continue;					// no light
			}
			break;
*/

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

	const bool caps_lock = host_keyboard_led_state().caps_lock;			// Disable if not used
//	const bool num_lock = host_keyboard_led_state().num_lock;			// Disable if not used
	const uint8_t layer = get_highest_layer(layer_state);
    HSV matrix_hsv = rgb_matrix_get_hsv();

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            const uint8_t index = g_led_config.matrix_co[row][col];

            if (index < led_min || index >= led_max)
                continue;

            HSV hsv;
            hsv.s = 255;							// If not defined Saturation is set to
            hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS;	// If not defined Brightness is set to

//	/*1*/ uint16_t kc = layer ?														// Disable line 1 & 2 and enable line 3 to 
//	/*2*/         keymap_key_to_keycode(layer, (keypos_t){col,row}) : 0;			// light up keys even on the default layer
	/*3*/ uint16_t kc = keymap_key_to_keycode(layer, (keypos_t){col,row});			//

//			if (kc >= QK_MOD_TAP && kc <= QK_LAYER_TAP_MAX)			// Disable this two line to explicitly use 
//				kc &= 0xff;											// MT() and LT() keycodes in your switch-cases

            switch (kc) {
			case EE_CLR:
			case QK_BOOT:
				hsv.h = 3;
				break;
			case MT(MOD_LCTL,KC_INS):
				hsv.h = 3;
				break;
            case KC_RIGHT ... KC_UP:
                hsv.h = matrix_hsv.h;
                break;
            case KC_F1 ... KC_F12:
				hsv.h = matrix_hsv.h;
//				hsv.v >>= 2;			// Decrease brightness
                break;
			case KC_P1 ... KC_P0:
				hsv.h = matrix_hsv.h;
				break;
			case KC_PMNS:
				hsv.h = 172;
				break;
			case KC_PPLS:
				hsv.h = 255;
				break;
			case KC_PEQL:
				hsv.h = 85;
				break;

		   case MT(MOD_LSFT,KC_LEFT):
		   case MT(MOD_LSFT | MOD_RSFT,KC_RGHT):
                if (caps_lock)
 					hsv.h = matrix_hsv.h;
				else
				continue;
				break;
				
#ifdef MOUSEKEY_ENABLE
            case KC_MS_U ... KC_BTN2:
            case KC_WH_U ... KC_WH_D:
                hsv.h = 40;
                break;
#endif
            case RGB_TOG ... RGB_SPD:
                hsv.h = matrix_hsv.h + ((((kc + 3) >> 1) % 6) * 85 >> 1);
				hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
//				hsv.s = matrix_hsv.s;
                uint8_t inc = (kc + !!(get_mods() & MOD_LSFT)) & 1;
				hsv.v >>= 1 - inc;
                break;

            default:
/*			
                if (caps_lock && (row == 0 ||
                            (row == 3 && (col == 0 || col == 9))
                            )) // Light up the top row for caps-lock on Mantis keyboard 
                    hsv.h = matrix_hsv.h+128, hsv.s >>= 1,
                        hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
                else
*/	
                    continue;
            }

            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
        }
    }

    return false;
}

/*
			┌──────────────────────────────┐
			│            COMBOS            │
			└──────────────────────────────┘

*/

enum combos {
	LSFT_RSFT_CAPS
};

	const uint16_t PROGMEM lsft_rsft_combo[] = {MT(MOD_LSFT,KC_LEFT), MT(MOD_LSFT | MOD_RSFT,KC_RGHT), COMBO_END};
	
combo_t key_combos[] = {
	[LSFT_RSFT_CAPS] = COMBO(lsft_rsft_combo, KC_CAPS),
};
/*
			┌──────────────────────────────┐
			│            LAYOUT            │
			└──────────────────────────────┘

*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_MAIN] = LAYOUT(
/*
	┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                            ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
	│   ESC ` │   1 !	│	2 @	  │   3 #   │   4 $   │   5 %   │                            │   6 ^   │   7 &   │   8 *   │   9 (   │   0 )   │  BSPC   │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                            ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │   TAB   │    Q    │    W    │    E    │    R    │    T    │                            │    Y    │    U    │    I    │    O    │    P    │   DEL   │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                            ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │LSFT/LEFT│    A    │    S    │    D    │    F    │    G    │                            │    H    │    J    │    K    │    L    │   ; :   │RSFT/RGHT│
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐        ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │  LGUI   │    Z    │    X    │    C    │    V    │    B    │LALT/DOWN│        │ RALT/UP │    N    │    M    │   , <   │   . >   │   / ?   │  ENT    │
	└─────────┴─────────┴─────────┴────┬────┴────┬────┴────┬────┴────┬────┘        └────┬────┴────┬────┴────┬────┴────┬────┴─────────┴─────────┴─────────┘
                                       │   [ {   │LCTL/RGB │ LT1/SPC │                  │ LT2/SPC │RCTL/- _ │   ' "   │
                                       └─────────┴─────────┴─────────┘                  └─────────┴─────────┴─────────┘
*/
	QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5,														KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,

	KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T,														KC_Y, KC_U, KC_I, KC_O, KC_P, KC_DEL,

	MT(MOD_LSFT,KC_LEFT), KC_A,  KC_S, KC_D, KC_F, KC_G,										KC_H, KC_J, KC_K, KC_L, KC_SCLN, MT(MOD_LSFT | MOD_RSFT,KC_RGHT),

	KC_LGUI, KC_Z, KC_X, KC_C, KC_V, KC_B, MT(MOD_LALT,KC_DOWN),					MT(MOD_LALT | MOD_RALT,KC_UP), KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ENT,

			KC_LBRC, MT(MOD_LCTL,KC_NO), LT(1,KC_SPC),											LT(2,KC_SPC), MT(MOD_RCTL,KC_MINS), KC_QUOT

  ),

  [_FN1] = LAYOUT(
/*
	┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                            ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
	│         │    !    │    @    │    #    │    $    │    %    │                            │    ^    │    &    │    *    │    (    │    )    │         │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                            ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │         │         │         │   UP    │         │   PGUP  │                            │    -    │    7    │    8    │    9    │         │         │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                            ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │         │         │   LEFT  │  DOWN   │  RGHT   │   PGDN  │                            │    +    │    4    │    5    │    6    │         │         │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐        ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │         │         │         │         │         │         │         │        │         │    =    │    1    │    2    │    3    │         │         │
	└─────────┴─────────┴─────────┴────┬────┴────┬────┴────┬────┴────┬────┘        └────┬────┴────┬────┴────┬────┴────┬────┴─────────┴─────────┴─────────┘
                                       │         │LCTL/LOCK│         │                  │ LT3/SPC │         │    0    │
                                       └─────────┴─────────┴─────────┘                  └─────────┴─────────┴─────────┘
*/
	_______, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,											KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,

	_______, _______, _______, KC_UP, _______, KC_PGUP,											KC_PMNS, KC_P7, KC_P8, KC_P9, _______, _______,

	_______, _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,										KC_PPLS, KC_P4, KC_P5, KC_P6, _______, _______,

	_______, _______, _______, _______, _______, _______, _______,					_______, KC_PEQL, KC_P1, KC_P2, KC_P3, _______, _______,

			_______, MT(MOD_LCTL,KC_INS), _______, 												LT(3,KC_SPC), _______, KC_P0

  ),

  [_FN2] = LAYOUT(
/*
	┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                            ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
	│    F1   │    F2   │    F3   │    F4   │   F5    │   F6    │                            │    F7   │    F8   │    F9   │   F10   │   F11   │   F12   │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                            ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │         │         │         │         │         │         │                            │         │         │         │         │         │         │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                            ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │         │         │         │         │         │         │                            │         │         │         │         │         │         │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐        ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │         │         │         │         │         │         │         │        │         │         │         │         │         │  \ |    │         │
	└─────────┴─────────┴─────────┴────┬────┴────┬────┴────┬────┴────┬────┘        └────┬────┴────┬────┴────┬────┴────┬────┴─────────┴─────────┴─────────┘
                                       │         │LCTL/LOCK│ LT3/SPC │                  │         │         │         │
                                       └─────────┴─────────┴─────────┘                  └─────────┴─────────┴─────────┘
*/
	KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,													KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,

	_______, _______, _______, KC_HASH, KC_DLR, KC_EXLM,										KC_AT, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,

	_______, KC_MPRV, KC_MNXT, KC_VOLU, KC_PGUP, KC_UNDS,										KC_EQL, KC_HOME, _______, _______, _______, _______,

	KC_MUTE, KC_MSTP, KC_MPLY, KC_VOLD, KC_PGDN, KC_MINS, _______,					_______, KC_PLUS, KC_END, _______, _______, KC_BSLS, _______,

			_______, MT(MOD_LCTL,KC_INS), LT(3,KC_SPC),											_______,  _______,  _______

  ),

  [_FN3] = LAYOUT(
/*
	┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                            ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
	│         │         │         │         │         │         │                            │         │         │         │         │         │  BOOT   │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                            ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
	│         │         │         │         │         │         │                            │         │         │         │         │         │  EECLR  │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                            ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
	│         │         │         │         │         │         │                            │   MOD+  │   HUE+  │   SAT+  │   BRI+  │   SPD+  │         │
	├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┐        ┌─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
    │         │         │         │         │         │ RGBTOG  │         │        │         │   MOD-  │   HUE-  │   SAT-  │   BRI-  │   SPD-  │         │
	└─────────┴─────────┴─────────┴────┬────┴────┬────┴────┬────┴────┬────┘        └────┬────┴────┬────┴────┬────┴────┬────┴─────────┴─────────┴─────────┘
                                       │         │LCTL/LOCK│         │                  │         │         │         │
                                       └─────────┴─────────┴─────────┘                  └─────────┴─────────┴─────────┘
*/
	_______, _______, _______, _______, _______, _______,										_______,  _______,  _______,  _______,  _______,  QK_BOOT,

	_______, _______, _______, _______, _______, _______,										_______,  _______,  _______,  _______,  _______,  EE_CLR,

	_______, _______, _______, _______, _______, _______,										RGB_MOD,  RGB_HUI,  RGB_SAI,  RGB_VAI,  RGB_SPI,  _______,

	_______, _______, _______, _______, _______, RGB_TOG, _______,					_______, RGB_RMOD, RGB_HUD,  RGB_SAD, RGB_VAD, RGB_SPD, _______,

			_______, MT(MOD_LCTL,KC_INS), _______,												_______, _______, _______

  )
};
