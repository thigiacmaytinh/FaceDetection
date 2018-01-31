# lib prefix
LIBPREFIX	:= lib
# lib file extension
LIBEXT	:= a
#obj file extension
OBJEXT := o

# compiler
CC := gcc
# compiler flags
CFLAGS := $(LOCAL_CFLAGS) 
# preprocessor flags
CPPFLAGS := $(LOCAL_CPPFLAGS)

# targets are combination of platforms and variants
$(foreach variant, $(VARIANTS), $(eval PLATFORMS_$(variant) := $(addsuffix _$(variant), $(PLATFORMS))))
TARGETS := $(foreach variant, $(VARIANTS), $(PLATFORMS_$(variant)))

# C++ compiler flags per target
CFLAGS_linux32_release += -m32 -std=c++11
CFLAGS_linux64_release += -m64 -Wconversion -std=c++11
CFLAGS_linux32_debug += -m32 -g -std=c++11
CFLAGS_linux64_debug += -m64 -Wconversion -g -std=c++11

# project directories
ROOT := $(LOCAL_PATH)

# include directories
INCLUDES := $(addprefix -I, $(LOCAL_C_INCLUDES))

# lib directories 
$(foreach target, $(TARGETS), $(eval LIB_DIR_$(target) := $(addsuffix /$(subst _,/,$(target)), $(LOCAL_LIB_DIR))))
$(foreach target, $(TARGETS), $(eval LIB_DIR_$(target) += $(LOCAL_LIB_DIR_$(target))))

$(foreach target, $(TARGETS), $(eval LIB_DIR_$(target) := $(addprefix -L, $(LIB_DIR_$(target)))))

# static libs to link against
STATIC_LIBRARIES := $(addprefix -l, $(LOCAL_STATIC_LIBRARIES))

# binary file directory (relative to root)
BINDIR ?= bin
# binary file directory per target (relative to root)
$(foreach target, $(TARGETS), $(eval BINDIR_$(target) := $(BINDIR)/$(subst _,/,$(target))))

# output files (complete path)
$(foreach target, $(TARGETS), $(eval OUT_$(target) := $(ROOT)/$(BINDIR_$(target))/$(TARGET)))

# object file directory (relative to root)
OBJDIR ?= build
# object file directory per target (relative to root)
$(foreach target, $(TARGETS), $(eval OBJDIR_$(target) := $(OBJDIR)/$(subst _,/,$(target))))

# src files (relative to root)
SRC_FILES := $(LOCAL_SRC_FILES)
# source files (complete path)
SRC := $(addprefix $(ROOT)/, $(SRC_FILES))
# src dir (complete path)
ROOTED_SRCDIR := $(if $(SRCDIR), $(ROOT)/$(SRCDIR), $(ROOT))

# object files (relative to root)
$(foreach target, $(TARGETS), $(eval OBJ_FILES_$(target) := $(SRC_FILES:.$(SRCEXT)=.$(OBJEXT))))
# strip SRCDIR
$(foreach target, $(TARGETS), $(eval OBJ_FILES_$(target) := $(patsubst $(SRCDIR)/%, %, $(OBJ_FILES_$(target)))))
# prefix OBJDIR
$(foreach target, $(TARGETS), $(eval OBJ_FILES_$(target) := $(addprefix $(OBJDIR_$(target))/, $(OBJ_FILES_$(target)))))

# object files (complete path)
$(foreach target, $(TARGETS), $(eval OBJ_$(target) := $(addprefix $(ROOT)/, $(OBJ_FILES_$(target)))))


#########################
# common rules

all : $(TARGETS)

# build each target 
$(foreach target,$(TARGETS),$(eval $(target) : $(OUT_$(target))))

# for each platform, build variants
$(foreach platform,$(PLATFORMS),$(eval $(platform) : $(addprefix $(platform)_, $(VARIANTS))))

# for each variant, build platforms
$(foreach variant,$(VARIANTS),$(eval $(variant) : $(addsuffix _$(variant), $(PLATFORMS))))

# generate lib, create bindir first
define LIB_TARGET
$$(OUT_$(1)) : $$(OBJ_$(1))
	mkdir -p $$(dir $$@)
	ar rcs $$@ $$^
endef	

# generate exe, create bindir first
define EXE_TARGET
$$(OUT_$(1)) : $$(OBJ_$(1))
	mkdir -p $$(dir $$@)
	$(CC) $$(CFLAGS_$(1)) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $$^ $$(LIB_DIR_$(1)) $(STATIC_LIBRARIES) -o $$@
endef	

#generate .o files for a target	
define OBJ_TARGET
$$(OBJ_$(1)) : $$(ROOT)/$$(OBJDIR_$(1))/%.$$(OBJEXT) : $$(ROOTED_SRCDIR)/%.$$(SRCEXT)
	mkdir -p $$(dir $$@)
	$(CC) -c $$(CFLAGS_$(1)) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDES) -o $$@ $$<
endef	

$(foreach target,$(TARGETS),$(eval $(call OBJ_TARGET,$(target))))

# clean output files for a target
clean_% : 
	rm -f $(OBJ_$*) $(OUT_$*)

# clean output files for all targets
$(foreach target, $(TARGETS), $(eval clean : clean_$(target)))

print-%:
	@echo $* = $($*)
 
.PHONY : all clean clean_% print-%
