################### makefile ########################
############## common for make app ##################
#####################################################

include ${SOFT_WORKDIR}/build/buildenv.mk


LOCAL_NAME := demo/demo_ethereum

LOCAL_API_DEPENDS :=  ${API_PLATFORM_DEPENDS} \
                        demo/demo_ethereum/include

LOCAL_MODULE_DEPENDS :=

LOCAL_EXPORT_FLAG += \
                    MBTK_TEST_FLAG


C_SRC := ${wildcard *.c}
#####################################################
include ${SOFT_WORKDIR}/build/makerules.mk

