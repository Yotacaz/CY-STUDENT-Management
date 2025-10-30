CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror -O2 -g
LDFLAGS=#link flags

#Directory for source code	(.c)
SRC_DIR=src
BUILD_DIR=build
#Directory for .o files
INC_DIR=inc
#Directory for the librairies to include (.h)
EXEC=exec
#executable name

SRC = $(shell find $(SRC_DIR) -type f -iname '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
INC=$(shell find $(SRC_DIR) -type f -iname '*.h')


.PHONY: all
all: $(BUILD_DIR) $(EXEC)

#generate executable
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

#generate .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INC) | $(BUILD_DIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I $(INC_DIR)

#create the correct folder structure in build directory
$(BUILD_DIR):
	cd $(SRC_DIR) && find . -type d -exec mkdir -p "../build/{}" \;
	cd ..

#generate documentation file
.PHONY: documentation
documentation : exec
	doxygen Doxyfile

.PHONY: clean
clean:
	@echo "cleaning workspace..."
	rm -rf $(BUILD_DIR)
	rm -f $(EXEC)

.PHONY: debug
debug:
	@echo "source code :"
	@echo $(SRC)
	@echo "objects files :"
	@echo $(OBJ)
	@echo "librairies files :"
	@echo $(INC)