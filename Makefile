PROGRAM_NAME := sdvxclone

CC := gcc
CXX := g++

ifeq ($(VERBOSE),YES)
	V_CC = $(CC)
	V_CXX = $(CXX)
	AT := 
else
	V_CC = @echo "	Building $@..."; $(CC)
	V_CXX = @echo "	Building $@..."; $(CXX)
	AT := @
endif

MSYS64 := C:/msys64
FIND := $(MSYS64)/usr/bin/find
MKDIR := $(MSYS64)/usr/bin/mkdir
CP := $(MSYS64)/usr/bin/cp
PWD := $(MSYS64)/usr/bin/pwd

PROJECT_DIR := $(shell $(PWD))

SRC_DIR := $(PROJECT_DIR)/src
OUT_DIR := $(PROJECT_DIR)/bin
BUILD_DIR := $(PROJECT_DIR)/build
LIB_DIR := $(PROJECT_DIR)/lib

INCDIRS := $(shell $(FIND) $(SRC_DIR) -type d) \
		   $(LIB_DIR)/glew-2.2.0/include \
		   $(LIB_DIR)/SDL2-2.30.2/x86_64-w64-mingw32/include \
		   $(LIB_DIR)/glm-1.0.1-light/include \
		   $(LIB_DIR)/freetype-2.13.2/include \
		   $(LIB_DIR)/stb_image \
		   $(LIB_DIR)/json-3.11.3 \
		   $(LIB_DIR)/lua-5.4.6/src

INCFLAGS := $(addprefix -I,$(INCDIRS))

LINKDIRS := $(LIB_DIR)/glew-2.2.0/lib/Release/x64 \
			$(LIB_DIR)/SDL2-2.30.2/x86_64-w64-mingw32/lib \
			$(LIB_DIR)/freetype-2.13.2/builds/windows/mingw \
			$(LIB_DIR)/lua-5.4.6/lib

LINKLIBS := glew32 libSDL2 opengl32 freetype lua
LINKFLAGS := $(addprefix -L,$(LINKDIRS)) $(addprefix -l,$(LINKLIBS))

CFLAGS := -std=c23 -O2 -g3
CXXFLAGS := -std=c++23 -O2 -g3
CPPFLAGS := $(INCFLAGS) -MMD -MP
LDFLAGS := -Wl,-Map="$(OUT_DIR)/$(PROGRAM_NAME).map" $(LINKFLAGS)
EXTRAFLAGS := -fdiagnostics-color=always

SRC_FILES := $(shell $(FIND) $(SRC_DIR) -name '*.cpp' -or -name '*.c')
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)
DEP_FILES := $(OBJ_FILES:.o=.d)

.PHONY: all clean
.DEFAULT: all

all: $(PROGRAM_NAME)

$(BUILD_DIR):
	$(AT)-$(MKDIR) -p $(BUILD_DIR)

$(OUT_DIR):
	$(AT)-$(MKDIR) -p $(OUT_DIR)

$(PROGRAM_NAME): $(OBJ_FILES) | $(OUT_DIR)
	$(V_CXX) $(OBJ_FILES) $(EXTRAFLAGS) -o $(OUT_DIR)/$@ $(LDFLAGS)
	@echo Copying dependency libraries...
	@-$(CP) $(LIB_DIR)/SDL2-2.30.2/x86_64-w64-mingw32/bin/SDL2.dll $(OUT_DIR)
	@-$(CP) $(LIB_DIR)/glew-2.2.0/bin/Release/x64/glew32.dll $(OUT_DIR)
	@-$(CP) $(MSYS64)/ucrt64/bin/libstdc++-6.dll $(OUT_DIR)
	@echo Build finished.

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(AT)-$(MKDIR) -p $(dir $@)
	$(V_CC) $(CPPFLAGS) $(CFLAGS) $(EXTRAFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(AT)-$(MKDIR) -p $(dir $@)
	$(V_CXX) $(CPPFLAGS) $(CXXFLAGS) $(EXTRAFLAGS) -c $< -o $@

clean:
	@echo Deleting object files...
	$(AT)-rm -f $(OBJ_FILES)
	$(AT)-rm -f $(DEP_FILES)
	@echo Deleting binary files...
	$(AT)-rm -f $(OUT_DIR)/$(PROGRAM_NAME) $(OUT_DIR)/$(PROGRAM_NAME).map

-include $(DEP_FILES)