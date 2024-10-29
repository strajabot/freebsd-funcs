
BUILD_DIR = build

IMPL_NAME = strrchr.S
TEST_NAME = test.c

IMPL_DIR = ./
TEST_DIR = test

IMPL = $(addprefix ${IMPL_DIR}/, ${IMPL_NAME})
TEST = $(addprefix ${TEST_DIR}/, ${TEST_NAME})

TOOLCHAIN = riscv64-linux-gnu-
CC = ${TOOLCHAIN}gcc

${BUILD_DIR}/executable: ${BUILD_DIR}/test.o ${BUILD_DIR}/impl.o  
	${CC} -g -static -o $@ $^

${BUILD_DIR}/test.o: ${TEST_DIR}/${TEST_NAME} Makefile | ${BUILD_DIR}
	${CC} -c -g -o $@ $<

${BUILD_DIR}/impl.o: ${IMPL_DIR}/${IMPL_NAME} Makefile | ${BUILD_DIR}
	${CC} -c -g -O3 -o $@ $<
	
${BUILD_DIR}: ${BUILD_DIR}
	mkdir -p $@
