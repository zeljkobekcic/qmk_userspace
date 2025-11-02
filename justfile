QMKFMT_VERSION := "v0.2.0"
UNAME_S := `uname -s`
PLATFORM := if UNAME_S == "Darwin" { "macos" } else { UNAME_S }

QMKFMT_RELEASE := "https://github.com/rcorre/qmkfmt/releases/download/" + QMKFMT_VERSION + "/qmkfmt-" + QMKFMT_VERSION + "-" + PLATFORM + ".tar.xz"

QMK_FW:= "$PWD/qmk_firmware"
QMK_FW_KEYBOARDS := "$PWD/qmk_firmware/keyboards/"

@setup:
  @qmk setup -y -H {{ QMK_FW }}

@setup-totem:
  @git clone git@github.com:zeljkobekcic/qmk-config-totem.git {{ QMK_FW_KEYBOARDS }}

@totem:
  @qmk compile -kb totem -km zeljkobekcic

@setup_hack:
  @wget "{{QMKFMT_RELEASE}}" --output-document ./hack/qmkfmt.tar.xz
  @tar -xf ./hack/qmkfmt.tar.xz --directory hack
  @rm ./hack/qmkfmt.tar.xz
  @chmod +x ./hack/qmkfmt

@format:
  @./hack/qmkfmt ./keyboards/totem/keymaps/zeljkobekcic/keymap.c
