#include QMK_KEYBOARD_H
#include "raw_hid.h"
//#include "print.h"

#define JM_NOTIFY_DATA_LEN 8

const uint8_t PROGMEM layer0_notify_data[8] = {
	'J', 'M', 'L', '0', 0, 0, 0, 0
};
const uint8_t PROGMEM layer1_notify_data[8] = {
	'J', 'M', 'L', '1', 0, 0, 0, 0
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_80_with_macro(
    KC_MUTE,          KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_INS,
    KC_F1,   KC_F2,   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC, KC_HOME,
    KC_F3,   KC_F4,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_END,
    KC_F5,   KC_F6,   KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    KC_F7,   KC_F8,   KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN,
    KC_F9,   KC_F10,  KC_LCTL, KC_LALT, KC_LGUI, MO(1),   KC_SPC,  KC_SPC,           MO(1),   KC_SPC,  KC_RALT, KC_RCTL, KC_RGUI, KC_LEFT, KC_DOWN, KC_RGHT
  ),
  [1] = LAYOUT_80_with_macro(
    _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    RGB_HUI, RGB_HUD, KC_GESC, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______,
    RGB_SAI, RGB_SAD, RGB_TOG, RGB_MOD, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    RGB_VAI, RGB_VAD, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______
  ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 1) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    } else if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
    }
    return true;
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
	if (length >= 5 && data[0] == 2) {
		if (data[1] == 'J' && data[2] == 'M') {
			if (data[3] == 'L') {
                if (data[4] == 'R') {
                    if (IS_LAYER_ON(1)) {
                        data[4] = 0xF - 1;
                    }
                    else {
                        data[4] = 0xF;
                    }
                }
                else if (data[4] == 'S')
                {
                    if (data[5] == '0')
                    {
                        layer_off(1);
                    }
                    else if (data[5] == '1')
                    {
                        layer_on(1);
                    }
                }
			}
		}
	}

    //print("received kb handler: ");
    //for(int i = 0; i < length; i++)
    //{
        //uprintf("%u", data[i]);
    //}
    //print("\n");
}

uint32_t layer_state_set_user(uint32_t state) {
	switch (biton32(state)) {
		case 0:
			raw_hid_send((uint8_t*)layer0_notify_data, JM_NOTIFY_DATA_LEN);
			//print("JML0");
			break;
		case 1:
			//print("JML1");
			raw_hid_send((uint8_t*)layer1_notify_data, JM_NOTIFY_DATA_LEN);
			break;
	}

	return state;
}
