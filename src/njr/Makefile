PROJECT = ${notdir ${PWD}}
TARGET  = ${PROJECT}

include ../Makefile.in

TMP_DEBUG=${patsubst ${PREFIX}src/%,${PROJECT_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${PREFIX}src/%,${PROJECT_OBJ_RELEASE}%,${PWD}}/

TMP_OBJS_DEBUG=${patsubst %,${TMP_DEBUG}%,${patsubst %.cpp,%.o,${wildcard *.cpp}}}
TMP_OBJS_RELEASE=${patsubst %,${TMP_RELEASE}%,${patsubst %.cpp,%.o,${wildcard *.cpp}}}

# Default Target
.PHONY: all
all: debug release

.PHONY: debug
debug: njr-debug

.PHONY: release
release: njr

# All Static Libraries
.PHONY: libs
libs: njr

.PHONY: libs-debug
libs-debug: njr-debug

# Utility
.PHONY: njr
njr: ${PROJECT_LIB}lib${TARGET}.a

.PHONY: njr-debug
njr-debug: ${PROJECT_LIB}lib${TARGET}-debug.a

${PROJECT_LIB}lib${TARGET}-debug.a: ${TMP_OBJS_DEBUG}
	${AR} ${AR_FLAGd} $@ $^

${TMP_DEBUG}%.o: %.cpp
	${CXX} -o $@ ${CXX_FLAGd} -c $<

${PROJECT_LIB}lib${TARGET}.a: ${TMP_OBJS_RELEASE}
	${AR} ${AR_FLAG} $@ $^

${TMP_RELEASE}%.o: %.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<

# Clean
.PHONY: clean
clean:
	mkdir -p ${PREFIX}bin
	mkdir -p ${PROJECT_BIN}
	mkdir -p ${PREFIX}lib
	mkdir -p ${PROJECT_LIB}
	rm -Rf ${PROJECT_LIB}lib${PROJECT}*
	mkdir -p ${PREFIX}obj
	mkdir -p ${PROJECT_OBJ}
	mkdir -p ${PROJECT_OBJ_DEBUG}
	mkdir -p ${PROJECT_OBJ_RELEASE}
	rm -Rf ${PROJECT_OBJ_DEBUG}${PROJECT}
	mkdir -p ${PROJECT_OBJ_DEBUG}${PROJECT}
	mkdir -p ${PROJECT_OBJ_DEBUG}${PROJECT}/njr
	rm -Rf ${PROJECT_OBJ_RELEASE}${PROJECT}
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}/njr
