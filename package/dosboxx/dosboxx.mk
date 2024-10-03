########################################
#
########################################

DOSBOXX_VERSION = dosbox-x-v2024.10.01
DOSBOXX_SITE = https://github.com/joncampbell123/dosbox-x.git
DOSBOXX_SITE_METHOD = git
DOSBOXX_INSTALL_STAGING = NO
DOSBOXX_INSTALL_TARGET = YES
DOSBOXX_AUTORECONF = YES
DOSBOXX_DEPENDENCIES = sdl2 sdl2_net ffmpeg
DOSBOXX_CONF_ENV =  CFLAGS="-DLINUX $(TARGET_CFLAGS) -I${BUILD_DIR}/ffmpeg-6.1.2/" CXXFLAGS="-DLINUX $(TARGET_CXXFLAGS) -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -I${BUILD_DIR}/ffmpeg-6.1.2/" LIBS="-lavcodec -lavutil -lavformat -lswscale"
DOSBOXX_CONF_OPTS = --host=aarch64 --enable-sdl2 --with-sdl2-exec-prefix=${STAGING_DIR}/usr

define DOSBOXX_POST_CONF_PATCH_FIXUP
	patch ${BUILD_DIR}/dosboxx-dosbox-x-v2024.10.01/src/Makefile ${DOSBOXX_PKGDIR}/fixup
endef

DOSBOXX_POST_CONFIGURE_HOOKS += DOSBOXX_POST_CONF_PATCH_FIXUP

$(eval $(autotools-package))

