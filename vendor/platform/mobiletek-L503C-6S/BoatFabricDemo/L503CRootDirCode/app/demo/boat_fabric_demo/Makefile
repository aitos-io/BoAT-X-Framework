################### makefile ########################
############## common for make app ##################
#####################################################

include ${SOFT_WORKDIR}/build/buildenv.mk


LOCAL_NAME := demo/boat_fabric_demo

LOCAL_API_DEPENDS :=  ${API_PLATFORM_DEPENDS} \
                        demo/boat_fabric_demo/include

LOCAL_MODULE_DEPENDS :=

LOCAL_EXPORT_FLAG += \
                    MBTK_TEST_FLAG


C_SRC := ${wildcard *.c}
#####################################################
include ${SOFT_WORKDIR}/build/makerules.mk

