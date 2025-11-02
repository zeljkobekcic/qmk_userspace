.SILENT:


MAKEFLAGS += --no-print-directory

UNAME_S := $(shell uname -s)
QMKFMT_VERSION := v0.2.0

ifeq ($(UNAME_S),Darwin)
	QMKFMT_RELEASE := https://github.com/rcorre/qmkfmt/releases/download/$(QMKFMT_VERSION)/qmkfmt-$(QMKFMT_VERSION)-macos.tar.xz
endif

ifeq ($(UNAME_S),Linux)
	QMKFMT_RELEASE := https://github.com/rcorre/qmkfmt/releases/download/$(QMKFMT_VERSION)/qmkfmt-$(QMKFMT_VERSION)-linux.tar.xz
endif

QMK_USERSPACE := $(patsubst %/,%,$(dir $(shell realpath "$(lastword $(MAKEFILE_LIST))")))
ifeq ($(QMK_USERSPACE),)
    QMK_USERSPACE := $(shell pwd)
endif

QMK_FIRMWARE_ROOT = $(shell qmk config -ro user.qmk_home | cut -d= -f2 | sed -e 's@^None$$@@g')
ifeq ($(QMK_FIRMWARE_ROOT),)
    $(error Cannot determine qmk_firmware location. `qmk config -ro user.qmk_home` is not set)
endif

%:
	+$(MAKE) -C $(QMK_FIRMWARE_ROOT) $(MAKECMDGOALS) QMK_USERSPACE=$(QMK_USERSPACE)


.PHONY: setup
setup:
	qmk setup -H "$PWD/qmk_firmware" -y

.PHONY: setup-totem
setup-totem:
	git clone git@github.com:zeljkobekcic/qmk-config-totem.git qmk_firmware/keyboards/totem


.PHONY: mytotem
mytotem:
	qmk compile -kb totem -km zeljkobekcic

.PHONY: setup_hack
setup_hack:
	wget "$(QMKFMT_RELEASE)" --output-document ./hack/qmkfmt.tar.xz
	tar -xf ./hack/qmkfmt.tar.xz --directory hack
	rm ./hack/qmkfmt.tar.xz
	chmod +x ./hack/qmkfmt

.PHONY:
format:
	./hack/qmkfmt ./keyboards/totem/keymaps/zeljkobekcic/keymap.c
