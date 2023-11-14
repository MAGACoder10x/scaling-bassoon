###############################################################################
# External buildroot mk file - This is where all of your custom logic may be 
# placed
###############################################################################
include $(sort $(wildcard $(BR2_EXTERNAL_RPI_CUSTOM_PATH)/package/*/*.mk))