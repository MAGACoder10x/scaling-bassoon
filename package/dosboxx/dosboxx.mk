########################################
#
########################################

DOSBOXX_VERSION = dosbox-x-v2024.10.01
DOSBOXX_SITE = https://github.com/joncampbell123/dosbox-x.git
DOSBOXX_SITE_METHOD = git
DOSBOXX_INSTALL_STAGING = NO
#DOSBOXX_INSTALL_TARGET = YES
DOSBOXX_AUTORECONF = YES
DOSBOXX_DEPENDENCIES = sdl2 sdl2_net ffmpeg
DOSBOXX_CONF_ENV =  CFLAGS="-DLINUX $(TARGET_CFLAGS) -I${BUILD_DIR}/ffmpeg-6.1.2/" CXXFLAGS="-DLINUX $(TARGET_CXXFLAGS) -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -I${BUILD_DIR}/ffmpeg-6.1.2/" LIBS="-lavcodec -lavutil -lavformat -lswscale"
DOSBOXX_CONF_OPTS = --host=aarch64 --prefix=${TARGET_DIR}/usr --exec-prefix=${TARGET_DIR}/usr --sysconfdir=${TARGET_DIR}/etc --localstatedir=${TARGET_DIR}/var --disable-x11 --enable-sdl2 --with-sdl2-exec-prefix=${STAGING_DIR}/usr

define DOSBOXX_POST_CONF_PATCH_FIXUP
	patch ${BUILD_DIR}/dosboxx-dosbox-x-v2024.10.01/src/Makefile ${DOSBOXX_PKGDIR}/fixup
endef

DOSBOXX_POST_CONFIGURE_HOOKS += DOSBOXX_POST_CONF_PATCH_FIXUP

define DOSBOXX_INSTALL_TARGET_CMDS
	$(INSTALL) -m 755 $(@D)/src/dosbox-x ${TARGET_DIR}/usr/bin
	mkdir -p ${TARGET_DIR}/usr/share/dosbox-x/drivez
	mkdir -p ${TARGET_DIR}/usr/share/dosbox-x/glshaders
	mkdir -p ${TARGET_DIR}/usr/share/dosbox-x/languages
	$(INSTALL) -m 644 $(@D)/CHANGELOG ${TARGET_DIR}/usr/share/dosbox-x
	$(INSTALL) -m 644 $(@D)/contrib/fonts/FREECG98.BMP ${TARGET_DIR}/usr/share/dosbox-x
	$(INSTALL) -m 644 $(@D)/dosbox-x.reference.conf ${TARGET_DIR}/usr/share/dosbox-x
	$(INSTALL) -m 644 $(@D)/dosbox-x.reference.full.conf ${TARGET_DIR}/usr/share/dosbox-x
	$(INSTALL) -m 644 $(@D)/contrib/fonts/wqy_1?pt.bdf ${TARGET_DIR}/usr/share/dosbox-x
	$(INSTALL) -m 644 $(@D)/contrib/fonts/Nouveau_IBM.ttf ${TARGET_DIR}/usr/share/dosbox-x
	$(INSTALL) -m 644 $(@D)/contrib/fonts/SarasaGothicFixed.ttf ${TARGET_DIR}/usr/share/dosbox-x
	$(INSTALL) -m 644 $(@D)/contrib/windows/installer/drivez_readme.txt ${TARGET_DIR}/usr/share/dosbox-x/drivez/readme.txt
	$(INSTALL) -m 644 $(@D)/contrib/glshaders/*.glsl ${TARGET_DIR}/usr/share/dosbox-x/glshaders
	$(INSTALL) -m 644 $(@D)/contrib/translations/*/*.lng ${TARGET_DIR}/usr/share/dosbox-x/languages
	mkdir -p ${TARGET_DIR}/usr/share/icons/hicolor/scalable/apps
	$(INSTALL) -m 644 $(@D)/contrib/icons/dosbox-x.svg ${TARGET_DIR}/usr/share/icons/hicolor/scalable/apps/dosbox-x.svg
	mkdir -p ${TARGET_DIR}/usr/share/applications
	$(INSTALL) -m 644 $(@D)/contrib/linux/com.dosbox_x.DOSBox-X.desktop ${TARGET_DIR}/usr/share/applications
	mkdir -p ${TARGET_DIR}/usr/share/metainfo
	$(INSTALL) -m 644 $(@D)/contrib/linux/com.dosbox_x.DOSBox-X.metainfo.xml ${TARGET_DIR}/usr/share/metainfo
	mkdir -p ${TARGET_DIR}/usr/share/man/man1
	$(INSTALL) -m 644 $(@D)/contrib/linux/dosbox-x.1 ${TARGET_DIR}/usr/share/man/man1
	mkdir -p ${TARGET_DIR}/usr/share/bash-completion/completions
	$(INSTALL) -m 644 $(@D)/contrib/linux/dosbox-x ${TARGET_DIR}/usr/share/bash-completion/completions
	#command -v setcap >/dev/null && setcap cap_net_raw=ep ${TARGET_DIR}/usr/bin/dosbox-x
endef

$(eval $(autotools-package))

