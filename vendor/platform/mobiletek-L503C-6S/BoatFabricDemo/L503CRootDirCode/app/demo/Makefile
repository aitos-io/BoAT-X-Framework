################### makefile ########################
############## common for make app ##################
#####################################################

include ${SOFT_WORKDIR}/build/buildenv.mk


LOCAL_NAME := demo

LOCAL_API_DEPENDS :=  ${API_PLATFORM_DEPENDS} \
                      demo/boat_fabric_demo/include 

LOCAL_MODULE_DEPENDS := demo/hello_test
LOCAL_MODULE_DEPENDS += demo/visual_at_demo
LOCAL_MODULE_DEPENDS += demo/uart_demo
LOCAL_MODULE_DEPENDS += demo/fs_demo
LOCAL_MODULE_DEPENDS += demo/http_demo
LOCAL_MODULE_DEPENDS += demo/mqtt_demo
LOCAL_MODULE_DEPENDS += demo/device_demo
LOCAL_MODULE_DEPENDS += demo/audio_demo
LOCAL_MODULE_DEPENDS += demo/nw_demo
LOCAL_MODULE_DEPENDS += demo/os_demo
LOCAL_MODULE_DEPENDS += demo/sms_demo
LOCAL_MODULE_DEPENDS += demo/gps_demo
LOCAL_MODULE_DEPENDS += demo/fota_demo
LOCAL_MODULE_DEPENDS += demo/ftp_demo
LOCAL_MODULE_DEPENDS += demo/lcd_demo
LOCAL_MODULE_DEPENDS += demo/cam_demo
LOCAL_MODULE_DEPENDS += demo/ntp_demo
LOCAL_MODULE_DEPENDS += demo/lwm2m_demo
LOCAL_MODULE_DEPENDS += demo/boat_fabric_demo

LOCAL_EXPORT_FLAG += \
                    MBTK_TEST_FLAG


C_SRC := ${wildcard *.c}
C_SRC += 

#####################################################
include ${SOFT_WORKDIR}/build/makerules.mk

