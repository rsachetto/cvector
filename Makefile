MKDIR_P = mkdir -p
OUT_DIR = lib build
CVEC_PRECISION = CVEC_PRECISION_DOUBLE


all: directories cvector.a tests

.PHONY: directories clean tests

cvector.a: src/cvector.c src/cvector.h
	gcc -g3 -D${CVEC_PRECISION} -c src/cvector.c -o build/cvector.o
	ar rcs lib/cvector.a build/cvector.o
	ranlib lib/cvector.a

tests:
	gcc -g3 -D${CVEC_PRECISION} tests/test_cvector.c ./lib/cvector.a -o tests/test_cvector -lcriterion -lm

directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

clean:
	rm -fr build lib

