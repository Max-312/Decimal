CC = gcc
RANLIB = ranlib
CFLAGS = -Wall -Werror -Wextra -std=c11
LDFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
CHECK_LIBS = -lcheck -lm -lsubunit

SOURCE = $(wildcard s21_*.c)
OBJECTS = $(patsubst %.c,%.o, $(SOURCE))

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    CHECK_LIBS += -pthread
    TEST_FLAGS = -lcheck_pic
else ifeq ($(UNAME_S),Darwin)
    TEST_FLAGS = -framework CoreFoundation
endif

all: s21_decimal.a

s21_decimal.a: $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	$(RANLIB) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: test.c 
	$(CC) $(GCOV_FLAGS) $(CFLAGS) $(SOURCE) test.c -o test_decimal $(CHECK_LIBS) $(TEST_FLAGS)
	./test_decimal
	@rm -f *.gcno *.gcda *.gcov test_decimal

gcov_report: 
	$(CC) $(GCOV_FLAGS) $(CFLAGS) $(SOURCE) test.c -o test_decimal $(CHECK_LIBS) $(TEST_FLAGS)
	./test_decimal
	@lcov -t "test_decimal" -o test_decimal -c -d .
	@genhtml -o report test_decimal
#	@open ./report/src/index.html
	@rm -f *.gcno *.gcda *.gcov test_decimal

clean:
	@rm -f $(OBJECTS) s21_decimal.a test_decimal
	@rm -f *.gcno *.gcda *.gcov
	@rm -rf report

check_clang:
	@clang-format -n -style=file:../materials/linters/.clang-format *.c
	@clang-format -n -style=file:../materials/linters/.clang-format *.h

edit_clang:
	@clang-format -i -style=file:../materials/linters/.clang-format *.c
	@clang-format -i -style=file:../materials/linters/.clang-format *.h
