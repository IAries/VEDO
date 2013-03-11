include ../Makefile.in
# Default Target
.PHONY: all
all: knight anne iris
#all: knight anne iris cecrops pcecrops

# Knight (Pre-/Post-processor)
.PHONY: knight
Knight: knight
knight: libs
	(cd Knight; ${MAKE})

# Anne (Sequential Analysis Engine)
.PHONY: anne
Anne: anne
anne: libs
	(cd Anne; ${MAKE})

# Iris (Parallel Analysis Engine)
.PHONY: iris
Iris: iris
iris: libs
	(cd Iris; ${MAKE})

# All Static Libraries
.PHONY: libs
libs: NJR FrameWork Common

# NJR (Utility Library)
.PHONY: njr
NJR: njr
njr:
	(cd NJR; ${MAKE})

# Abstract Class
.PHONY: framework
Framework: framework
FrameWork: framework
framework:
	(cd FrameWork; ${MAKE})

# Extended Class
.PHONY: common
Common: common
common:
	(cd Common; ${MAKE})

# Make necessary directories for building
.PHONY: BuildingDir
BuildingDir: clean
	rm -Rf ${VEDO_BIN}
	mkdir ${VEDO_BIN}
	rm -Rf ${VEDO_LIB}
	mkdir ${VEDO_LIB}
	rm -Rf ${VEDO_OBJ}
	mkdir ${VEDO_OBJ}
	mkdir ${VEDO_OBJ}Knight
	mkdir ${VEDO_OBJ}Anne
	mkdir ${VEDO_OBJ}Iris
	mkdir ${VEDO_OBJ}NJR
	mkdir ${VEDO_OBJ}FrameWork
	mkdir ${VEDO_OBJ}Common

# Clean
.PHONY: clean
clean:
	rm -Rf ${VEDO_OBJ}*
