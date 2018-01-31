JSONCPP_LOCAL_PATH:= .
LOCAL_PATH:= $(JSONCPP_LOCAL_PATH)/../..
JSONCPP_ROOT:= $(LOCAL_PATH)

rwildcard=$(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

JSONCPP_SOURCES := $(patsubst $(JSONCPP_ROOT)/%, %, $(call rwildcard, $(JSONCPP_ROOT)/source, *.cpp))

common_CFLAGS := 
  
#########################
# Build the libjsoncpp library


include $(JSONCPP_ROOT)/../jsoncpp_config/linux/makefile-config.inc    

LOCAL_SRC_FILES := $(JSONCPP_SOURCES)

LOCAL_C_INCLUDES := $(JSONCPP_ROOT)/include $(JSONCPP_ROOT)/source

LOCAL_EXPORT_C_INCLUDES += $(JSONCPP_ROOT)/include $(JSONCPP_ROOT)/source

LOCAL_CFLAGS := $(common_CFLAGS)
LOCAL_CFLAGS += $(JSONCPP_CFLAG)

LOCAL_MODULE:= libjsoncpp

#########################
#c++ static lib configuration

PROJECT = jsoncpp
# final target
TARGET = $(LOCAL_MODULE).a
# SRCDIR will be replaced by OBJDIR to generate object files directories
# if SRCDIR is empty, object files are prefixed by OBJDIR
SRCDIR := 
# src file extension
SRCEXT := cpp
# list of platforms, each platform is a target
PLATFORMS := linux64
# list of variants, each variant is a target
VARIANTS := release debug
#include common rules
include makefile-common.inc 
# build lib targets
$(foreach target,$(TARGETS),$(eval $(call LIB_TARGET,$(target))))




