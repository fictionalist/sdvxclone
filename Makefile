PROGRAM_NAME := sdvxclone

CC := gcc
CXX := g++

ifeq ($(VERBOSE),YES)
	V_CC = $(CC)
	V_CXX = $(CXX)
	AT := 
else
	V_CC = @echo "Building $<..."; $(CC)
	V_CXX = @echo "Building $<..."; $(CXX)
	AT := @
endif

FIND := C:/msys64/usr/bin/find
MKDIR := C:/msys64/usr/bin/mkdir
CP := C:/msys64/usr/bin/cp

SRC_DIR := ./src
OUT_DIR := ./bin
BUILD_DIR := ./build
LIB_DIR := ./lib

INCDIRS := $(shell $(FIND) $(SRC_DIR) -type d) $(LIB_DIR)/glew-2.2.0/include $(LIB_DIR)/SDL2-2.30.2/x86_64-w64-mingw32/include
INCFLAGS := $(addprefix -I,$(INCDIRS))

LINKDIRS := $(LIB_DIR)/glew-2.2.0/lib/Release/x64 $(LIB_DIR)/SDL2-2.30.2/x86_64-w64-mingw32/lib
LINKLIBS := glew32 libSDL2
LINKFLAGS := $(addprefix -L,$(LINKDIRS)) $(addprefix -l,$(LINKLIBS))

CFLAGS := -std=c17 -O2 -g3
CXXFLAGS := -std=c++23 -O2 -g3
CPPFLAGS := $(INCFLAGS) -MMD -MP
LDFLAGS := -Wl,-Map="$(OUT_DIR)/$(PROGRAM_NAME).map" $(LINKFLAGS)

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
	$(V_CXX) $(OBJ_FILES) $(LDFLAGS) -o $(OUT_DIR)/$@
	-$(CP) $(LIB_DIR)/SDL2-2.30.2/x86_64-w64-mingw32/bin/SDL2.dll $(OUT_DIR)

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(V_CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(V_CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	@echo Deleting object files...
	$(AT)-rm -f $(OBJ_FILES)
	$(AT)-rm -f $(DEP_FILES)
	@echo Deleting binary files...
	$(AT)-rm -f $(OUT_DIR)/$(PROGRAM_NAME) $(OUT_DIR)/$(PROGRAM_NAME).map

-include $(DEP_FILES)