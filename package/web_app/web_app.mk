################################################################################
#
# web_app
#
################################################################################
WEB_APP_VERSION = main
WEB_APP_SITE=git@github.com:MAGACoder10x/cunning-strudle.git
WEB_APP_SITE_METHOD=git
WEB_APP_DEPENDENCIES = \
	host-rustc

WEB_APP_CARGO_ENV = CARGO_HOME=$(HOST_DIR)/share/cargo
WEB_APP_LINKER_ENV = CARGO_TARGET_ARMV7_UNKNOWN_LINUX_GNUEABIHF_LINKER=$(TARGET_CC)
WEB_APP_SERVER_CARGO_MODE = $(if $(BR2_ENABLE_DEBUG),debug,release)

WEB_APP_SERVER_BIN_DIR = web_app/target/$(RUSTC_TARGET_NAME)/$(WEB_APP_SERVER_CARGO_MODE)

WEB_APP_CARGO_OPTS = \
	--$(WEB_APP_SERVER_CARGO_MODE) \
	--target=$(RUSTC_TARGET_NAME) \
	--manifest-path=$(@D)/web_app/Cargo.toml

define WEB_APP_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(WEB_APP_CARGO_ENV) $(WEB_APP_LINKER_ENV) \
		cargo build $(WEB_APP_CARGO_OPTS)
endef

define WEB_APP_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/$(WEB_APP_SERVER_BIN_DIR)/web_app \
	$(TARGET_DIR)/usr/local/bin/web_app
endef

$(eval $(generic-package))
