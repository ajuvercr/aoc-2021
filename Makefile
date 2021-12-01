CC ?= gcc
DAY ?= $(shell date '+%d')
FILE ?= ./input/$(DAY).txt
TARGET_EXEC ?= $(DAY).out

BUILD_DIR ?= ./build
LIB_DIR ?= ./lib
SRC_DIR ?= ./days

EXEC := $(SRC_DIR)/$(DAY).c
LIBS := $(shell find $(LIB_DIR) -type f -name "*.c" -or -name *.s)
OBJS := $(EXEC:%.c=$(BUILD_DIR)/%.o) $(LIBS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(LIBS:%.c=%.h)
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.o: %.c $(DEPS)
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

run: $(BUILD_DIR)/$(TARGET_EXEC)
	@echo "-------------------------------------------------------------"
	$(BUILD_DIR)/$(TARGET_EXEC) $(FILE)

clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p
