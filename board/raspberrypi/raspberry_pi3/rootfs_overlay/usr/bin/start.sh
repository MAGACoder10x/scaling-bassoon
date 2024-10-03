#!/bin/sh

modprobe spi-bcm2835
modprobe fb_st7789v

export QTDIR=/usr/lib/qt
export QT_QPA_FONTDIR=/usr/share/fonts/dejavu/
export QT_QPA_PLATFORM_PLUGIN_PATH=${QTDIR}/plugins
export QT_QPA_PLATFORM=linuxfb:fb=/dev/fb1
export LD_LIBRARY_PATH=${QTDIR}/lib:$LD_LIBRARY_PATH
export PATH=${QTDIR}/bin:$PATH
export PKG_CONFIG_PATH=${QTDIR}/lib/pkgconfig/${PKG_CONFIG_PATH}
export QT_QPA_EVDEV_KEYBOARD_PARAMETERS=/dev/input/event0

./HawkingFrontPanel
