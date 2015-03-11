include Makefile.in

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
	(cd vedo/knight; ${MAKE} release)

.PHONY: knight-debug
knight-debug: libs-debug
	(cd vedo/knight; ${MAKE} debug)

# anne (Sequential Analysis Engine)
.PHONY: anne
anne: libs
	(cd vedo/anne; ${MAKE} release)

.PHONY: anne-debug
anne-debug: libs-debug
	(cd vedo/anne; ${MAKE} debug)

# iris (Parallel Analysis Engine)
.PHONY: iris-gm
iris-gm: libs
	(cd vedo/iris-gm; ${MAKE} release)

.PHONY: iris-gm-debug
iris-gm-debug: libs-debug
	(cd vedo/iris-gm; ${MAKE} debug)

# All Static Libraries
.PHONY: libs
libs: constants njr framework common

.PHONY: libs-debug
libs-debug: constants-debug njr-debug framework-debug common-debug

# Constants
.PHONY: constants
constants:
	(cd vedo/constants; ${MAKE} release)

.PHONY: constants-debug
constants-debug:
	(cd vedo/constants; ${MAKE} debug)

# NJR (Utility Library)
.PHONY: njr
njr:
	(cd vedo/njr; ${MAKE} release)

.PHONY: njr-debug
njr-debug:
	(cd vedo/njr; ${MAKE} debug)

# Abstract Class
.PHONY: framework
framework:
	(cd vedo/framework; ${MAKE} release)

.PHONY: framework-debug
framework-debug:
	(cd vedo/framework; ${MAKE} debug)

# Extended Class
.PHONY: common
common:
	(cd vedo/common; ${MAKE} release)

.PHONY: common-debug
common-debug:
	(cd vedo/common; ${MAKE} debug)

# Make necessary directories for building
.PHONY: buildingdir
buildingdir:
	mkdir -p ${PREFIX}bin
	mkdir -p ${PREFIX}bin/vedo
	rm -Rf ${VEDO_BIN}
	mkdir ${VEDO_BIN}
	mkdir -p ${PREFIX}lib
	mkdir -p ${PREFIX}lib/vedo
	rm -Rf ${VEDO_LIB}
	mkdir ${VEDO_LIB}
	mkdir -p ${PREFIX}obj
	mkdir -p ${PREFIX}obj/vedo
	rm -Rf ${VEDO_OBJ}
	mkdir ${VEDO_OBJ}
	mkdir ${VEDO_OBJ_DEBUG}
	mkdir ${VEDO_OBJ_DEBUG}constants
	mkdir ${VEDO_OBJ_DEBUG}njr
	mkdir ${VEDO_OBJ_DEBUG}framework
	mkdir ${VEDO_OBJ_DEBUG}common
	mkdir ${VEDO_OBJ_DEBUG}knight
	mkdir ${VEDO_OBJ_DEBUG}anne
	mkdir ${VEDO_OBJ_DEBUG}iris-gm
	mkdir ${VEDO_OBJ_RELEASE}
	mkdir ${VEDO_OBJ_RELEASE}constants
	mkdir ${VEDO_OBJ_RELEASE}njr
	mkdir ${VEDO_OBJ_RELEASE}framework
	mkdir ${VEDO_OBJ_RELEASE}common
	mkdir ${VEDO_OBJ_RELEASE}knight
	mkdir ${VEDO_OBJ_RELEASE}anne
	mkdir ${VEDO_OBJ_RELEASE}iris-gm

# Clean
.PHONY: clean
clean:
	rm -Rf ${VEDO_OBJ}*
