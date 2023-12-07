########################################
#
########################################

LCD_TEST_VERSION = 0.1
LCD_TEST_SITE = $(BR2_EXTERNAL_RPI_CUSTOM_PATH)/package/lcd_test
LCD_TEST_SITE_METHOD = local

LCD_TEST_INSTALL_STAGING = NO
LCD_TEST_INSTALL_TARGET = YES
LCD_TEST_READ_CONF_OPTS = -DBUILD_SHARED_LIBS=OFF  -DCMAKE_CXX_STANDARD=20

$(eval $(cmake-package))
