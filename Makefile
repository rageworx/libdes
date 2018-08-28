# Makefile for 'libdes'
# ----------------------------------------------------------------------
# Written by Raphael Kim
#

GCC = gcc
GPP = g++
AR  = ar

# Base PATH
BASE_PATH = .
LIB_PATH  = $(BASE_PATH)/lib
SRC_PATH  = $(BASE_PATH)/src
LOCI_PATH = /usr/local/include
LOCL_PATH = /usr/local/lib

# TARGET settings
TARGET_PKG = libdes.a
TARGET_DIR = ./lib
TARGET_OBJ = ./obj/Release

# DEFINITIONS
#DEFS  = -DUNICODE
DEFS  =

# Compiler options
COPTS  =

# CC FLAG
CFLAGS  = -I$(SRC_PATH)
CFLAGS += -I$(LIB_PATH)
CFLAGS += -I$(LOCI_PATH)
CFLAGS += $(DEFS)
CFLAGS += $(COPTS)

# LINK FLAG
LFLAGS  =
LFLAGS += -L$(LOCL_PATH)

# Sources ( for any cpp )
SRCS = $(wildcard $(SRC_PATH)/*.cpp)

# Make object targets from SRCS.
OBJS = $(SRCS:$(SRC_PATH)/%.cpp=$(TARGET_OBJ)/%.o)

.PHONY: prepare clean

all: prepare clean continue
continue: $(TARGET_DIR)/$(TARGET_PKG)

prepare:
	@mkdir -p $(TARGET_DIR)
	@mkdir -p $(TARGET_OBJ)

clean:
	@echo "Cleaning built targets ..."
	@rm -rf $(TARGET_DIR)/$(TARGET_PKG)
	@rm -rf $(TARGET_DIR)/*.h
	@rm -rf $(TARGET_OBJ)/*.o

$(OBJS): $(TARGET_OBJ)/%.o: $(SRC_PATH)/%.cpp
	@echo "Compiling $@ ... "
	@$(GPP) $(CFLAGS) $(LFLAGS )-c $< -o $@

$(TARGET_DIR)/$(TARGET_PKG): $(OBJS)
	@echo "Generating $@ ..."
	@$(AR) -q $@ $(TARGET_OBJ)/*.o
	@cp -f $(SRC_PATH)/des.h $(TARGET_DIR)
	@cp -f $(SRC_PATH)/destk.h $(TARGET_DIR)
	@cp -f $(SRC_PATH)/libdes.h $(TARGET_DIR)
	@echo "done."
