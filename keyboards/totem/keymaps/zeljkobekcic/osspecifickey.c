#include "osspecifickey.h"

bool process_os_specific_key(
    keyrecord_t *record,
    uint16_t macos_key,
    uint16_t linux_key
) {
    if (record->event.pressed) {
        os_variant_t host_os = detected_host_os();
        switch (host_os) {
            case OS_MACOS:
            case OS_IOS:
                register_code16(macos_key);
                break;
            default: // Catches OS_LINUX, OS_WINDOWS, and unknown
                register_code16(linux_key);
                break;
        }
    } else {
        // Unregister both keycodes on release to be safe
        unregister_code16(macos_key);
        unregister_code16(linux_key);
    }
    return false; // Tell QMK we've handled this key
}
