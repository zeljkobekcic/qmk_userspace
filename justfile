QMKFMT_VERSION := "v0.2.0"
UNAME_S := `uname -s`
PLATFORM := if UNAME_S == "Darwin" { "macos" } else { UNAME_S }

QMKFMT_RELEASE := "https://github.com/rcorre/qmkfmt/releases/download/" + QMKFMT_VERSION + "/qmkfmt-" + QMKFMT_VERSION + "-" + PLATFORM + ".tar.xz"

QMK_FW:= "$PWD/qmk_firmware"
QMK_FW_KEYBOARDS := "$PWD/qmk_firmware/keyboards/"

TOTEM_KEYBOARD := "totem"
TOTEM_KEYMAP := "zeljkobekcic"
TOTEM_FIRMWARE := "./" + TOTEM_KEYBOARD +"_" + TOTEM_KEYMAP + ".uf2"
TOTEM_KEYMAP_FILE := "./keyboards/" + TOTEM_KEYBOARD + "/keymaps/" + TOTEM_KEYMAP + "/keymap.c"
TOTEM_COMPILE_ARGS := "--clean --keyboard " + TOTEM_KEYBOARD + " --keymap " + TOTEM_KEYMAP


@setup:
    @qmk setup -y -H {{ QMK_FW }}

@setup-totem:
    @git clone git@github.com:zeljkobekcic/qmk-config-totem.git "{{ QMK_FW_KEYBOARDS }}/totem"

@compile:
    @qmk compile {{ TOTEM_COMPILE_ARGS }}

@flash:
    @qmk flash {{ TOTEM_COMPILE_ARGS }}

@setup_hack:
    @wget '{{QMKFMT_RELEASE}}' --output-document ./hack/qmkfmt.tar.xz
    @tar -xf ./hack/qmkfmt.tar.xz --directory hack
    @rm ./hack/qmkfmt.tar.xz
    @chmod +x ./hack/qmkfmt

@format:
  @./hack/qmkfmt '{{ TOTEM_KEYMAP_FILE }}'
