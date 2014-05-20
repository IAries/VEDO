include Makefile.in

# Default Target
.PHONY: all
all: debug release

.PHONY: debug
debug: knight-debug anne-debug anne-ei-debug iris-gm-debug iris-gm-ei-debug

.PHONY: release
release: knight anne anne-ei iris-gm iris-gm-ei

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

# anne-ei (Sequential Analysis Engine with External Force)
.PHONY: anne-ei
anne-ei: libs
	(cd vedo/anne-ei; ${MAKE} release)

.PHONY: anne-ei-debug
anne-ei-debug: libs-debug
	(cd vedo/anne-ei; ${MAKE} debug)

# iris (Parallel Analysis Engine)
.PHONY: iris-gm
iris-gm: libs
	(cd vedo/iris-gm; ${MAKE} release)

.PHONY: iris-gm-debug
iris-gm-debug: libs-debug
	(cd vedo/iris-gm; ${MAKE} debug)

# iris (Parallel Analysis Engine with External Force)
.PHONY: iris-gm
iris-gm-ei: libs
	(cd vedo/iris-gm-ei; ${MAKE} release)

.PHONY: iris-gm-ei-debug
iris-gm-ei-debug: libs-debug
	(cd vedo/iris-gm-ei; ${MAKE} debug)

# All Static Libraries
.PHONY: libs
libs: njr framework common

.PHONY: libs-debug
libs-debug: njr-debug framework-debug common-debug

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
	mkdir ${VEDO_OBJ_DEBUG}njr
	mkdir ${VEDO_OBJ_DEBUG}framework
	mkdir ${VEDO_OBJ_DEBUG}common
	mkdir ${VEDO_OBJ_DEBUG}knight
	mkdir ${VEDO_OBJ_DEBUG}anne
	mkdir ${VEDO_OBJ_DEBUG}anne-ei
	mkdir ${VEDO_OBJ_DEBUG}iris-gm
	mkdir ${VEDO_OBJ_DEBUG}iris-gm-ei
	mkdir ${VEDO_OBJ_RELEASE}
	mkdir ${VEDO_OBJ_RELEASE}njr
	mkdir ${VEDO_OBJ_RELEASE}framework
	mkdir ${VEDO_OBJ_RELEASE}common
	mkdir ${VEDO_OBJ_RELEASE}knight
	mkdir ${VEDO_OBJ_RELEASE}anne
	mkdir ${VEDO_OBJ_RELEASE}anne-ei
	mkdir ${VEDO_OBJ_RELEASE}iris-gm
	mkdir ${VEDO_OBJ_RELEASE}iris-gm-ei

# Clean
.PHONY: clean
clean:
	rm -Rf ${VEDO_OBJ}*
