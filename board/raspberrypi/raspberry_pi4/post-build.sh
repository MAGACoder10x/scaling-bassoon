#!/bin/sh

set -u
set -e

# Add a console on tty1
if [ -e ${TARGET_DIR}/etc/inittab ]; then
    grep -qE '^tty1::' ${TARGET_DIR}/etc/inittab || \
	sed -i '/GENERIC_SERIAL/a\
tty1::respawn:/sbin/getty -L  tty1 0 vt100 # HDMI console' ${TARGET_DIR}/etc/inittab
# systemd doesn't use /etc/inittab, enable getty.tty1.service instead
elif [ -d ${TARGET_DIR}/etc/systemd ]; then
    mkdir -p "${TARGET_DIR}/etc/systemd/system/getty.target.wants"
    ln -sf /lib/systemd/system/getty@.service \
       "${TARGET_DIR}/etc/systemd/system/getty.target.wants/getty@tty1.service"
fi

#copy over waveshare dtbo
if [ -e ${BINARIES_DIR}/rpi-firmware ]; then
    cp ${BR2_EXTERNAL_RPI_CUSTOM_PATH}/board/raspberrypi/raspberry_pi4/dts/waveshare-touch-35dpi.dtbo ${BINARIES_DIR}/rpi-firmware/
    cp ${BR2_EXTERNAL_RPI_CUSTOM_PATH}/board/raspberrypi/raspberry_pi4/dts/vc4-kms-DPI-35inch.dtbo ${BINARIES_DIR}/rpi-firmware/
    cp ${BR2_EXTERNAL_RPI_CUSTOM_PATH}/board/raspberrypi/raspberry_pi4/dts/waveshare-35dpi-3b-4b.dtbo ${BINARIES_DIR}/rpi-firmware/
    cp ${BR2_EXTERNAL_RPI_CUSTOM_PATH}/board/raspberrypi/raspberry_pi4/dts/waveshare-35dpi-3b.dtbo ${BINARIES_DIR}/rpi-firmware/    
    cp ${BR2_EXTERNAL_RPI_CUSTOM_PATH}/board/raspberrypi/raspberry_pi4/dts/waveshare-35dpi-4b.dtbo ${BINARIES_DIR}/rpi-firmware/
fi