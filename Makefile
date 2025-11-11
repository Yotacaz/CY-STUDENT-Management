CC=gcc
TEST_MODE=0
CFLAGS=
ifeq ($(TEST_MODE), 1)
	CFLAGS=-Wall -Wextra -Wpedantic -Werror -O2 -g
else
	CFLAGS=-DNDEBUG -O3
endif

DYN_MODE=1
ifeq ($(DYN_MODE),1)
	CFLAGS += -fPIC
endif
LDFLAGS=#link flags

#Directory for source code	(.c)
SRC_DIR=src
#Directory for .o files
BUILD_DIR=build
#Directory for the librairie
LIB_DIR=lib
#Directory for the librairies to include (.h)
INC_DIR=inc
#executable name
# EXEC=exec

SRC := $(shell find $(SRC_DIR) -type f -iname '*.c')
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
INC:=$(shell find $(SRC_DIR) -type f -iname '*.h')
INC_API := $(wildcard $(LIB_DIR)/*.h)
INC += $(INC_API)

# STAT_LIB = $(patsubst $(LIB_DIR)/%.h, $(LIB_DIR)/lib%.a, $(INC_API))
# DYN_LIB = $(patsubst $(LIB_DIR)/%.h, $(LIB_DIR)/lib%.so, $(INC_API))
#lib name
STAT_LIB:=$(LIB_DIR)/libstudent_s.a
DYN_LIB:=$(LIB_DIR)/libstudent_d.so

.PHONY: default
default: $(BUILD_DIR)
	@echo "To generate a static librairie, edit this makefile and set DYN_MODE to 0. Then use 'make clean' followed by 'make static_lib'"
	@echo "To generate a dynamic librairie, edit this makefile and set DYN_MODE to 1. Then use 'make clean' followed by 'make static_lib'"
	@echo 'In case of a dynamic librairie, you may need to run in your shell something like : '
	@echo 'export LD_LIBRARY_PATH=./lib:$$LD_LIBRARY_PATH, where ./lib is the path to the directory where the librairie is in'

#generate test executable
# $(EXEC): $(OBJ)
# 	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: static_lib
static_lib: $(STAT_LIB)
$(STAT_LIB): $(OBJ)
	ar rcs $@ $^

.PHONY: dynamic_lib
dynamic_lib : $(DYN_LIB)
$(DYN_LIB): $(OBJ)
	$(CC) -shared -o $@ $^ $(CFLAGS)

#generate .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INC) | $(BUILD_DIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I $(INC_DIR)

#create the correct folder structure in build directory
$(BUILD_DIR):
	cd $(SRC_DIR) && find . -type d -exec mkdir -p "../build/{}" \;
	cd ..

#generate documentation file
.PHONY: documentation
documentation : 
	doxygen Doxyfile

.PHONY: clean
clean:
	@echo "cleaning workspace..."
	rm -rf $(BUILD_DIR)
# 	rm -f $(STAT_LIB)
# 	rm -f $(DYN_LIB)
# 	rm -f $(EXEC)

.PHONY: debug
debug:
	@echo "source code :"
	@echo $(SRC)
	@echo "objects files :"
	@echo $(OBJ)
	@echo "header files :"
	@echo $(INC)
	@echo "librairie name :"
	@echo $(DYN_LIB) " or " $(STAT_LIB)