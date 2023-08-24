CC     =     cc
CFLAGS =     -Wall -Wextra -Werror -std=c99 -pedantic -g
TEST   =     test
TEST_SRC =   tests/main.c
INCLUDE =    -I./include -I/usr/local/include
SRC =        src/*.c

build-test:
	$(CC) $(CFLAGS) -o $(TEST) $(TEST_SRC) $(INCLUDE) $(SRC)

test: build-test
	./$(TEST); rm -f $(TEST)

clean:
	rm -f $(TEST)