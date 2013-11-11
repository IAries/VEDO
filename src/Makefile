include ../Makefile.in

# Default Target
.PHONY: all
all: debug release

.PHONY: debug
debug: Knight-debug Anne-debug IrisGM-debug Test-debug

.PHONY: release
release: Knight Anne IrisGM Test

# Knight (Pre-/Post-processor)
.PHONY: Knight
knight: Knight
Knight: libs
	(cd Knight; ${MAKE} release)

.PHONY: Knight-debug
knight-debug: Knight-debug
Knight-debug: libs-debug
	(cd Knight; ${MAKE} debug)

# Anne (Sequential Analysis Engine)
.PHONY: Anne
anne: Anne
Anne: libs
	(cd Anne; ${MAKE} release)

.PHONY: Anne-debug
anne-debug: Anne-debug
Anne-debug: libs-debug
	(cd Anne; ${MAKE} debug)

# Iris (Parallel Analysis Engine)
.PHONY: IrisGM
irisGM: IrisGM
irisgm: IrisGM
IrisGM: libs
	(cd IrisGM; ${MAKE} release)

.PHONY: IrisGM-debug
irisGM-debug: IrisGM-debug
irisgm-debug: IrisGM-debug
IrisGM-debug: libs-debug
	(cd IrisGM; ${MAKE} debug)

# Test
.PHONY: Test
test: Test
Test:
	(cd Test; ${MAKE} release)

.PHONY: Test-debug
test-debug: Test-debug
Test-debug:
	(cd Test; ${MAKE} debug)

# All Static Libraries
.PHONY: libs
libs: NJR Framework Common

.PHONY: libs-debug
libs-debug: NJR-debug Framework-debug Common-debug

# NJR (Utility Library)
.PHONY: NJR
njr: NJR
NJR:
	(cd NJR; ${MAKE} release)

.PHONY: NJR-debug
njr-debug: NJR-debug
NJR-debug:
	(cd NJR; ${MAKE} debug)

# Abstract Class
.PHONY: Framework
framework: Framework
Framework:
	(cd Framework; ${MAKE} release)

.PHONY: Framework-debug
framework-debug: Framework-debug
Framework-debug:
	(cd Framework; ${MAKE} debug)

# Extended Class
.PHONY: Common
common: Common
Common:
	(cd Common; ${MAKE} release)

.PHONY: Common-debug
common-debug: Common-debug
Common-debug:
	(cd Common; ${MAKE} debug)

# Make necessary directories for building
.PHONY: BuildingDir
BuildingDir:
	mkdir -p ${PREFIX}bin
	rm -Rf ${VEDO_BIN}
	mkdir ${VEDO_BIN}
	mkdir -p ${PREFIX}lib
	rm -Rf ${VEDO_LIB}
	mkdir ${VEDO_LIB}
	mkdir -p ${PREFIX}obj
	rm -Rf ${VEDO_OBJ}
	mkdir ${VEDO_OBJ}
	mkdir ${VEDO_OBJ_DEBUG}
	mkdir ${VEDO_OBJ_DEBUG}NJR
	mkdir ${VEDO_OBJ_DEBUG}Framework
	mkdir ${VEDO_OBJ_DEBUG}Common
	mkdir ${VEDO_OBJ_DEBUG}Knight
	mkdir ${VEDO_OBJ_DEBUG}Test
	mkdir ${VEDO_OBJ_DEBUG}Anne
	mkdir ${VEDO_OBJ_DEBUG}IrisGM
	mkdir ${VEDO_OBJ_RELEASE}
	mkdir ${VEDO_OBJ_RELEASE}NJR
	mkdir ${VEDO_OBJ_RELEASE}Framework
	mkdir ${VEDO_OBJ_RELEASE}Common
	mkdir ${VEDO_OBJ_RELEASE}Knight
	mkdir ${VEDO_OBJ_RELEASE}Test
	mkdir ${VEDO_OBJ_RELEASE}Anne
	mkdir ${VEDO_OBJ_RELEASE}IrisGM

# Clean
.PHONY: clean
clean:
	rm -Rf ${VEDO_OBJ}*
