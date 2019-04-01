CC	= gcc
CFLAGS	= -g -Wall -std=gnu99 -fopenmp -mavx
LDFLAGS	= -g -Wall
SOURCES := matrix.c mat_test.c
HEADERS := matrix.h
OBJS = matrix.o matrix_test.o

ifeq ($(UNAME_S), Darwin)
    MEMCHECK=valgrind --tool=memcheck --leak-check=full --track-origins=yes --dsymutil=yes --suppressions=osx_vector.supp
endif

ifeq ($(UNAME_S), Linux)
    MEMCHECK=valgrind --tool=memcheck --leak-check=full --track-origins=yes
endif

check: testAll clean

speedup: build

testAll: test1 test2

test1: clean
	rm -rf tmp
	mkdir tmp
	$(CC) $(CFLAGS) ./naive/matrix.c ./naive/mat_test.c -o mat_test_1
	./mat_test_1

test2:
	$(CC) $(CFLAGS) ./performance/matrix.c ./performance/mat_test.c -o mat_test_2
	./mat_test_2

clean:
	rm -rf mat_test*
	bash install/uninstall.sh

very_clean: clean
	rm -rf mat_test* tmp matrix testing/tmp
	bash install/delete.sh

.PHONY: build

build: clean
	bash install/install.sh
