
SRC = $(wildcard src/*.cc)
OBJ = ${SRC:.cc=.o}

CC = clang++
CFLAGS = -Wall

TEST_SRC = $(shell find {tests,src}/*.cc | sed '/rivai/d')
TEST_OBJ = ${TEST_SRC:.cc=.o}

rivai: $(OBJ)
	$(CC) $^ $(LDFLAGS) -g -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -g -o $@

test_runner: $(TEST_OBJ)
	$(CC) $^ -o $@

test: test_runner
	./test_runner

clean:
	rm -f rivai test_runner $(OBJ) $(TEST_OBJ)

.PHONY: clean test
