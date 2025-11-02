#include QMK_KEYBOARD_H
#include "oneshot.h"
#include "osspecifickey.h"

#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)
#define LA_NUM MO(NUM)
#define LA_FUN MO(FUN)
#define TAB_LEFT_LINUX LCTL(KC_PGDN)
#define TAB_RIGHT_LINUX LCTL(KC_PGUP)
#define TAB_LEFT_MACOS LALT(LGUI(KC_LEFT))
#define TAB_RIGHT_MACOS LALT(LGUI(KC_RIGHT))

enum layers { BASE, NAV, SYM, NUM, FUN };

enum keycodes {
    _OS_SHFT = SAFE_RANGE,
    _OS_CTRL,
    _OS_ALT,
    _OS_RALT,
    _OS_GUI,
    _OS_GAC, // GUI + ALT + CTRL
    _OS_TAB_LEFT,
    _OS_TAB_RIGHT,
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
               KC_TAB,   _OS_TAB_LEFT, _OS_TAB_RIGHT, KC_NO,   KC_NO,   KC_NO,   KC_HOME, KC_END,  KC_BSPC, KC_DEL,
               _OS_SHFT, _OS_CTRL,     _OS_ALT,       _OS_GUI, _OS_GAC, KC_NO,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
        KC_NO, KC_NO,    KC_COPY,      KC_PASTE,      KC_CUT,  KC_NO,   KC_NO,   KC_NO,   KC_PGDN, KC_PGUP, KC_NO,   KC_NO,
                                       KC_TRNS,       KC_TRNS, KC_TRNS, KC_TRNS, LA_NUM,  KC_TRNS
    ),

    [SYM] = LAYOUT(
               KC_ESC,  KC_DLR,  KC_PERC, KC_LPRN, KC_RPRN, KC_CIRC, KC_TILD, KC_GRV,  KC_PIPE,  KC_BSLS,
               KC_MINS, KC_PLUS, KC_EQL,  KC_LCBR, KC_RCBR, KC_HASH, _OS_GUI, _OS_ALT, _OS_CTRL, _OS_SHFT,
        KC_NO, KC_UNDS, KC_ASTR, KC_AT,   KC_LBRC, KC_RBRC, KC_NO,   KC_AMPR, KC_SCLN, KC_COLN,  KC_EXLM,  KC_NO,
                                 KC_TRNS, LA_FUN,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [NUM] = LAYOUT(
               KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,     KC_9,     KC_0,
               _OS_SHFT, _OS_CTRL, _OS_ALT, _OS_GUI, _OS_GAC, _OS_GAC, _OS_GUI, _OS_RALT, _OS_CTRL, _OS_SHFT,
        KC_NO, KC_NO,    KC_NO,    KC_NO,   KC_NO,   KC_VOLD, KC_MPRV, KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
                                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [FUN] = LAYOUT(
               KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,    KC_F9,    KC_F10,
               _OS_SHFT, _OS_CTRL, _OS_ALT, _OS_GUI, _OS_GAC, _OS_GAC, _OS_GUI, _OS_RALT, _OS_CTRL, _OS_SHFT,
        KC_NO, KC_NO,    KC_NO,    KC_NO,   QK_BOOT, KC_F11,  KC_F12,  KC_MPLY, KC_NO,    KC_NO,    KC_TRNS,  KC_NO,
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
        case LA_FUN:
        case LA_NUM:
        case KC_LSFT:
        case _OS_SHFT:
        case _OS_CTRL:
        case _OS_ALT:
        case _OS_RALT:
        case _OS_GUI:
        case _OS_GAC:
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
    update_oneshot(&os_shft_state, KC_LSFT, _OS_SHFT, keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, _OS_CTRL, keycode, record);
    update_oneshot(&os_alt_state, KC_LALT, _OS_ALT, keycode, record);
    update_oneshot(&os_ralt_state, KC_RALT, _OS_RALT, keycode, record);
    update_oneshot(&os_gui_state, KC_LGUI, _OS_GUI, keycode, record);

    switch (keycode) {
        case _OS_TAB_LEFT:
            return process_os_specific_key(record, TAB_LEFT_MACOS, TAB_LEFT_LINUX);

        case _OS_TAB_RIGHT:
            return process_os_specific_key(record, TAB_RIGHT_MACOS, TAB_RIGHT_LINUX);

        case _OS_GAC:
            update_oneshot(&os_ctrl_state, KC_LCTL, _OS_CTRL, _OS_CTRL, record);
            update_oneshot(&os_alt_state, KC_LALT, _OS_ALT, _OS_ALT, record);
            update_oneshot(&os_gui_state, KC_LGUI, _OS_GUI, _OS_GUI, record);
    }

    return true;
}

