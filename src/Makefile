include ../Makefile.in

# Default Target
.PHONY: all
all: debug release

.PHONY: debug
debug: knight-debug anne-debug iris-debug

.PHONY: release
release: knight anne iris

# Knight (Pre-/Post-processor)
.PHONY: knight
Knight: knight
knight: libs
	(cd Knight; ${MAKE} release)

.PHONY: knight-debug
Knight-Debug: knight-debug
Knight-debug: knight-debug
knight-debug: libs-debug
	(cd Knight; ${MAKE} debug)

# Anne (Sequential Analysis Engine)
.PHONY: anne
Anne: anne
anne: libs
	(cd Anne; ${MAKE} release)

.PHONY: anne-debug
Anne-Debug: anne-debug
Anne-debug: anne-debug
anne-debug: libs-debug
	(cd Anne; ${MAKE} debug)

# Iris (Parallel Analysis Engine)
.PHONY: iris
Iris: iris
iris: libs
	(cd Iris; ${MAKE} release)

.PHONY: iris-debug
Iris-Debug: iris-debug
Iris-debug: iris-debug
iris-debug: libs-debug
	(cd Iris; ${MAKE} debug)

# All Static Libraries
.PHONY: libs
libs: njr framework common

.PHONY: libs-debug
libs-debug: njr-debug framework-debug common-debug

# NJR (Utility Library)
.PHONY: njr
NJR: njr
njr:
	(cd NJR; ${MAKE} release)

.PHONY: njr-debug
NJR-Debug: njr-debug
NJR-debug: njr-debug
njr-debug:
	(cd NJR; ${MAKE} debug)

# Abstract Class
.PHONY: framework
Framework: framework
FrameWork: framework
framework:
	(cd Framework; ${MAKE} release)

.PHONY: framework-debug
FrameWork-Debug: framework-debug
FrameWork-debug: framework-debug
Framework-Debug: framework-debug
Framework-debug: framework-debug
framework-debug:
	(cd Framework; ${MAKE} debug)

# Extended Class
.PHONY: common
Common: common
common:
	(cd Common; ${MAKE} release)

.PHONY: common-debug
Common-Debug: common-debug
Common-debug: common-debug
common-debug:
	(cd Common; ${MAKE} debug)

# Make necessary directories for building
.PHONY: BuildingDir
BuildingDir: clean
	rm -Rf ${VEDO_BIN}
	mkdir ${VEDO_BIN}
	rm -Rf ${VEDO_LIB}
	mkdir ${VEDO_LIB}
	rm -Rf ${VEDO_OBJ}
	mkdir ${VEDO_OBJ}
	mkdir ${VEDO_OBJ_DEBUG}
	mkdir ${VEDO_OBJ_DEBUG}NJR
	mkdir ${VEDO_OBJ_DEBUG}Framework
	mkdir ${VEDO_OBJ_DEBUG}Common
	mkdir ${VEDO_OBJ_DEBUG}Knight
	mkdir ${VEDO_OBJ_DEBUG}Anne
	mkdir ${VEDO_OBJ_DEBUG}Iris
	mkdir ${VEDO_OBJ_RELEASE}
	mkdir ${VEDO_OBJ_RELEASE}NJR
	mkdir ${VEDO_OBJ_RELEASE}Framework
	mkdir ${VEDO_OBJ_RELEASE}Common
	mkdir ${VEDO_OBJ_RELEASE}Knight
	mkdir ${VEDO_OBJ_RELEASE}Anne
	mkdir ${VEDO_OBJ_RELEASE}Iris

# Clean
.PHONY: clean
clean:
	rm -Rf ${VEDO_OBJ}*
