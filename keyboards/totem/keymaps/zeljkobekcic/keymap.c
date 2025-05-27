#include QMK_KEYBOARD_H
#include "totem.h"
#include "oneshot.h"

#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

enum layers { BASE = 0, NAV, SYM, NUM, ADJ };

enum keycodes {
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_RALT,
    OS_GUI,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    [BASE] = LAYOUT(
               KC_Q, KC_W, KC_F,   KC_P,   KC_B,   KC_J,    KC_L,   KC_U,    KC_Y,   KC_QUOT,
               KC_A, KC_R, KC_S,   KC_T,   KC_G,   KC_M,    KC_N,   KC_E,    KC_I,   KC_O,
        KC_NO, KC_Z, KC_X, KC_C,   KC_D,   KC_V,   KC_K,    KC_H,   KC_COMM, KC_DOT, KC_SLSH, KC_NO,
                           KC_ENT, LA_NAV, KC_SPC, KC_LSFT, LA_SYM, KC_LCTL
    ),

    [NAV] = LAYOUT(
               KC_TAB,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_HOME, KC_END,  KC_BSPC, KC_DEL,
               OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_NO,   KC_NO,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
        KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_PGDN, KC_PGUP, KC_NO,   KC_NO,
                                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [SYM] = LAYOUT(
               KC_ESC,  KC_DLR,  KC_PERC, KC_LPRN, KC_RPRN, KC_CIRC, KC_TILD, KC_GRV,  KC_PIPE, KC_BSLS,
               KC_MINS, KC_PLUS, KC_EQL,  KC_LCBR, KC_RCBR, KC_HASH, OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_NO, KC_UNDS, KC_ASTR, KC_AT,   KC_LBRC, KC_RBRC, KC_NO,   KC_AMPR, KC_SCLN, KC_COLN, KC_EXLM, KC_NO,
                                 KC_TRNS, KC_TRNS, MO(ADJ), KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [NUM] = LAYOUT(
               KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
               OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_F11,  KC_F12,  OS_GUI,  OS_RALT, OS_CTRL, OS_SHFT,
        KC_NO, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_NO,
                                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [ADJ] = LAYOUT(
               KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_VOLU, KC_MNXT, KC_NO,  KC_NO, KC_NO,
               KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_VOLD, KC_MPRV, KC_NO,  KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,   QK_BOOT, KC_NO,   KC_MUTE, KC_MPLY, KC_NO,  KC_NO, KC_TRNS, KC_NO,
                             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )

    // clang-format on
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        case LA_SYM:
        case LA_NAV:
            return true;
        default:
            return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case LA_SYM:
        case LA_NAV:
        case KC_LSFT:
        case OS_SHFT:
        case OS_CTRL:
        case OS_ALT:
        case OS_RALT:
        case OS_GUI:
            return true;
        default:
            return false;
    }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state  = os_up_unqueued;
oneshot_state os_ralt_state = os_up_unqueued;
oneshot_state os_gui_state  = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
    update_oneshot(&os_alt_state, KC_LALT, OS_ALT, keycode, record);
    update_oneshot(&os_ralt_state, KC_RALT, OS_RALT, keycode, record);
    update_oneshot(&os_gui_state, KC_LGUI, OS_GUI, keycode, record);

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}

