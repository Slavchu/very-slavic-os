include $(MOD_DIR)/source.mk
LOCAL_INCLUDES += include

# Please, don't do it in this way, I am just lazy and braindead
MODULE_SNAKE ?= 1

ifeq ($(MODULE_SNAKE), 0)
GLOBAL_DEFINES += -DMODULE_SNAKE
SUB_MODULES += module/snake
endif
