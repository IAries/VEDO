PROJECT = ${notdir ${PWD}}

include ../Makefile.in

# Default Target
.PHONY: all
all: debug release

.PHONY: debug
debug: knight-debug anne-debug iris-gm-debug

.PHONY: release
release: knight anne iris-gm

# Knight (Pre-/Post-processor)
.PHONY: knight
knight: libs
	(cd knight; ${MAKE} release)

.PHONY: knight-debug
knight-debug: libs-debug
	(cd knight; ${MAKE} debug)

# anne (Sequential Analysis Engine)
.PHONY: anne
anne: libs
	(cd anne; ${MAKE} release)

.PHONY: anne-debug
anne-debug: libs-debug
	(cd anne; ${MAKE} debug)

# iris (Parallel Analysis Engine)
.PHONY: iris-gm
iris-gm: libs
	(cd iris-gm; ${MAKE} release)

.PHONY: iris-gm-debug
iris-gm-debug: libs-debug
	(cd iris-gm; ${MAKE} debug)

# All Static Libraries
.PHONY: libs
libs: framework common

.PHONY: libs-debug
libs-debug: framework-debug common-debug

# Abstract Class
.PHONY: framework
framework:
	(cd framework; ${MAKE} release)

.PHONY: framework-debug
framework-debug:
	(cd framework; ${MAKE} debug)

# Extended Class
.PHONY: common
common:
	(cd common; ${MAKE} release)

.PHONY: common-debug
common-debug:
	(cd common; ${MAKE} debug)

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
	mkdir -p ${PROJECT_OBJ_DEBUG}${PROJECT}/framework
	mkdir -p ${PROJECT_OBJ_DEBUG}${PROJECT}/common
	mkdir -p ${PROJECT_OBJ_DEBUG}${PROJECT}/knight
	mkdir -p ${PROJECT_OBJ_DEBUG}${PROJECT}/anne
	mkdir -p ${PROJECT_OBJ_DEBUG}${PROJECT}/iris-gm
	rm -Rf ${PROJECT_OBJ_RELEASE}${PROJECT}
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}/framework
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}/common
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}/knight
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}/anne
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}/iris-gm
