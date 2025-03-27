driver_inc_dirs += \
    $(drivers_dir)/lpc_puf/inc \
    $(drivers_dir)/lpc_puf/inc/mbedtls/include/mbedtls \
    $(drivers_dir)/lpc_puf/inc/mbedtls/include/psa \
    $(drivers_dir)/lpc_puf/inc/mbedtls/include \
    $(drivers_dir)/lpc_puf/inc/mbedtls/library \
    $(drivers_dir)/lpc_puf/inc/mbedtls/port/ksdk \
    $(drivers_dir)/lpc_puf/inc/drivers \
    $(drivers_dir)/lpc_puf/inc/device \
    $(drivers_dir)/lpc_puf/inc/utilities \
    $(drivers_dir)/lpc_puf/inc/component/serial_manager \
    $(drivers_dir)/lpc_puf/inc/board \
    $(drivers_dir)/lpc_puf/inc/CMSIS


driver_s_srcs+=

driver_c_srcs += \
    lpc_puf/lpc_puf.c \
    lpc_puf/src/source/* \
    lpc_puf/src/device/* \
    lpc_puf/src/drivers/* \
    lpc_puf/src/mbedtls/library/* \
    lpc_puf/src/mbedtls/port/ksdk/* \

CFLAGS += -include $(drivers_dir)/lpc_puf/inc/lpc_puf.h -DMBEDTLS_CONFIG_FILE='"ksdk_mbedtls_config.h"'
