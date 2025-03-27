driver_inc_dirs += \
    $(drivers_dir)/lpc_puf/inc \
    $(drivers_dir)/lpc_puf/inc/device \
    $(drivers_dir)/lpc_puf/inc/drivers \
    $(drivers_dir)/lpc_puf/inc/utilities \
    $(drivers_dir)/lpc_puf/inc/component/serial_manager \
    $(drivers_dir)/lpc_puf/inc/board \
    $(drivers_dir)/lpc_puf/inc/CMSIS

driver_c_srcs+= \
    lpc_puf/lpc_puf.c \
    lpc_puf/src/drivers/fsl_puf.c \
    lpc_puf/src/drivers/fsl_reset.c \
    lpc_puf/src/drivers/fsl_clock.c \
    lpc_puf/src/drivers/fsl_common.c \
    lpc_puf/src/drivers/fsl_power.c \

driver_s_srcs+=

CFLAGS += -include $(drivers_dir)/lpc_puf/inc/lpc_puf.h
