PROJECT = ${notdir ${PWD}}

include ../Makefile.in

# Default Target
.PHONY: all
all: debug release

.PHONY: debug
debug: utility-debug

.PHONY: release
release: utility

# All Static Libraries
.PHONY: libs
libs: utility

.PHONY: libs-debug
libs-debug: utility-debug

# Utility
.PHONY: utility
utility:
	(cd utility; ${MAKE} release)

.PHONY: utility-debug
utility-debug:
	(cd utility; ${MAKE} debug)

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
	mkdir -p ${PROJECT_OBJ_DEBUG}${PROJECT}/utility
	rm -Rf ${PROJECT_OBJ_RELEASE}${PROJECT}
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}
	mkdir -p ${PROJECT_OBJ_RELEASE}${PROJECT}/utility
