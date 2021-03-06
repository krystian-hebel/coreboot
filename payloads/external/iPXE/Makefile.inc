STABLE_COMMIT_ID=fd6d1f4660a37d75acba1c64e2e5f137307bbc31

TAG-$(CONFIG_IPXE_MASTER)=origin/master
TAG-$(CONFIG_IPXE_STABLE)=$(STABLE_COMMIT_ID)

project_name=iPXE
project_dir=ipxe
project_git_repo=https://git.ipxe.org/ipxe.git

unexport KCONFIG_AUTOHEADER
unexport KCONFIG_AUTOCONFIG
unexport KCONFIG_DEPENDENCIES
unexport KCONFIG_SPLITCONFIG
unexport KCONFIG_TRISTATE
unexport KCONFIG_NEGATIVES

all: build

$(project_dir):
	echo "    Cloning $(project_name) from Git"
	git clone $(project_git_repo) $(project_dir)

fetch: $(project_dir)
	cd $(project_dir); \
		git show $(TAG-y) >/dev/null 2>&1 ; \
		if [ $$? -ne 0 ] || [ "$(TAG-y)" = "origin/master" ]; then \
			echo "    Fetching new commits from the $(project_name) repo"; \
			git fetch; \
		fi

checkout: fetch
	echo "    Checking out $(project_name) revision $(TAG-y)"
	cd  $(project_dir); \
		git checkout master; \
		git branch -D coreboot 2>/dev/null; \
		git checkout -b coreboot $(TAG-y)

config: checkout
ifeq ($(CONSOLE_SERIAL),yy)
	cp "$(project_dir)/src/config/console.h" "$(project_dir)/src/config/console.h.cb"
	cp "$(project_dir)/src/config/serial.h" "$(project_dir)/src/config/serial.h.cb"
	sed 's|//#define\s*CONSOLE_SERIAL.*|#define CONSOLE_SERIAL|' "$(project_dir)/src/config/console.h" > "$(project_dir)/src/config/console.h.tmp"
	mv "$(project_dir)/src/config/console.h.tmp" "$(project_dir)/src/config/console.h"
	sed 's|#define\s*COMCONSOLE.*|#define COMCONSOLE $(IPXE_UART)|' "$(project_dir)/src/config/serial.h" > "$(project_dir)/src/config/serial.h.tmp"
	sed 's|#define\s*COMSPEED.*|#define COMSPEED $(CONFIG_TTYS0_BAUD)|'  "$(project_dir)/src/config/serial.h.tmp" > "$(project_dir)/src/config/serial.h"
endif
ifneq ($(CONFIG_PXE_CUSTOM_GENERAL_H),)
ifneq ("$(wildcard $(CONFIG_PXE_CUSTOM_GENERAL_H))","")
	cat $(CONFIG_PXE_CUSTOM_GENERAL_H) > $(project_dir)/src/config/local/general.h
else
	echo "Error: File $(CONFIG_PXE_CUSTOM_GENERAL_H) does not exist"
	false
endif
endif
ifneq ($(CONFIG_PXE_CUSTOM_BOOTMENU_FILE),)
ifneq ("$(wildcard $(CONFIG_PXE_CUSTOM_BOOTMENU_FILE))","")
	cat $(CONFIG_PXE_CUSTOM_BOOTMENU_FILE) > $(project_dir)/src/menu.ipxe
else
	echo "Error: File $(CONFIG_PXE_CUSTOM_BOOTMENU_FILE) does not exist"
	false
endif
endif

build: config
	echo "    MAKE       $(project_name) $(TAG-y)"
ifneq ($(CONFIG_PXE_CUSTOM_BOOTMENU_FILE),)
	$(MAKE) -C $(project_dir)/src bin/$(PXE_ROM_PCI_ID).rom EMBED=./menu.ipxe
else
	$(MAKE) -C $(project_dir)/src bin/$(PXE_ROM_PCI_ID).rom
endif
	cp $(project_dir)/src/bin/$(PXE_ROM_PCI_ID).rom $(project_dir)/ipxe.rom
ifeq ($(CONSOLE_SERIAL),yy)
	cp "$(project_dir)/src/config/console.h.cb" "$(project_dir)/src/config/console.h"
	cp "$(project_dir)/src/config/serial.h.cb" "$(project_dir)/src/config/serial.h"
endif

clean:
	test -d $(project_dir) && $(MAKE) -C $(project_dir)/src veryclean || exit 0
	rm -f $(project_dir)/ipxe.rom

distclean:
	rm -rf $(project_dir)

.PHONY: all fetch config build clean distclean
