########################################
#
########################################

DOSBOX_VERSION = 0.74-3
DOSBOX_SITE = https://sourceforge.net/projects/dosbox/files/dosbox/${DOSBOX_VERSION}
DOSBOX_INSTALL_STAGING = NO
DOSBOX_INSTALL_TARGET = YES
DOSBOX_DEPENDENCIES = sdl
DOSBOX_CONF_OPTS = --host=aarch64 --prefix=${TARGET_DIR}/usr --exec-prefix=${TARGET_DIR}/usr --with-sdl-exec-prefix=${STAGING_DIR}/usr


$(eval $(autotools-package))

