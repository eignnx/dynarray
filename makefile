CC ?= cc
CFLAGS += -Wall -Wextra -g
CPPFLAGS += -I.

OUT_DIR = out
TEST_OUT = $(OUT_DIR)/tests

SRCS = dynarray.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean test build

all: build

build: $(OBJS)

test: $(TEST_OUT)
	./$(TEST_OUT)

$(TEST_OUT): tests/tests.c $(OBJS) | $(OUT_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -fv $(OBJS)
	rm -rf $(OUT_DIR)
	rm -fv tests/dynarray.o tests/dynarray.h
