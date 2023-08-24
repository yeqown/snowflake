CC     =     cc
CFLAGS =     -Wall -Wextra -Werror -std=c99 -pedantic -g
TEST   =     test
INCLUDE =    -I./include -I/usr/local/include
SRC =        src/*.c

# Test related
TEST_SRC =   tests/main.c

# CLI related
CLI =        snowflake
CLI_SRC =    cmd/snowflake/main.c

build-test:
	$(CC) $(CFLAGS) -o $(TEST) $(TEST_SRC) $(INCLUDE) $(SRC)

build-cli:
	$(CC) $(CFLAGS) -o $(CLI) $(CLI_SRC) $(INCLUDE) $(SRC)

test: build-test
	./$(TEST); rm -f $(TEST)

clean:
	rm -f $(TEST)