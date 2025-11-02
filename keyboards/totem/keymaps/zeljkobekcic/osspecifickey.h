#pragma once

#include QMK_KEYBOARD_H

bool process_os_specific_key(
    keyrecord_t *record,
    uint16_t macos_key,
    uint16_t linux_key
);

