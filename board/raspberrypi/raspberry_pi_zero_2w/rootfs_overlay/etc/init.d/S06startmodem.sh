#!/bin/sh
#
# Starts wifi networking on rpi zero 2w
#
start() {
  printf 'Loading Modem Drivers'
  
  modprobe uio_pdrv_genirq
  modprobe spi-bcm2835
  modprobe spidev

  status=$?
  if [ "$status" -eq 0 ]; then
    echo "OK"
  else
    echo "FAIL"
  fi

  status=$?
  if [ "$status" -eq 0 ]; then
    echo "OK"
  else
    echo "FAIL"
  fi
  return "$status"
}

stop() {
  printf 'Stopping Modem Drivers'
  rmmod spidev
  rmmod spi-bcm2835
  rmmod uio_pdrv_genirq
  status=$?
  if [ "$status" -eq 0 ]; then
    echo "OK"
  else
    echo "FAIL"
  fi
  return "$status"
}

restart() {
  stop
  sleep 1
  start
}

case "$1" in
  start|stop|restart)
    "$1";;
  reload)
    # Restart, since there is no true "reload" feature.
    restart;;
  *)
    echo "Usage: $0 {start|stop|restart|reload}"
    exit 1
esac