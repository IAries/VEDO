# =============================================== #
# Makefile for KnightAnne                         #
# Created     by C.-C. Chou  (Mustang) 2004 July. #
# Modifed     by C.-T. Yang  (Jitin)   2004 Sep.  #
# Refactoring by W.-T. Chang (Aies)    2005 Aug.  #
# =============================================== #

include ../Makefile.in

# ============== #
# Default Target #
# ============== #

.PHONY: all
all: knight anne fiber idoinfo idoedit idocut Anne_w_Pack packipt iris

# ============== #
# Knight Program #
# ============== #

knight: libs
	(cd Knight-Anne/Knight ; ${MAKE})

Knight: knight

# ============ #
# Anne Program #
# ============ #

anne: libs
	(cd Knight-Anne/Anne ; ${MAKE})

Anne: anne

# ============= #
# Fiber Program #
# ============= #

fiber: libs
	(cd Knight-Anne/Fiber ; ${MAKE})

Fiber: fiber

# ============ #
# iris Program #
# ============ #

iris: libs
	(cd Iris ; ${MAKE})

Iris: iris

# =============== #
# idoinfo Program #
# =============== #

idoinfo: libs
	(cd Knight-Anne/IDOInfo ; ${MAKE})

IDOInfo: idoinfo

# =============== #
# idoedit Program #
# =============== #

idoedit: libs
	(cd Knight-Anne/IDOEdit ; ${MAKE})

IDOEdit: idoedit

# ============== #
# idocut Program #
# ============== #

idocut: libs
	(cd Knight-Anne/IDOCut ; ${MAKE})

IDOCut: idocut

# =================== #
# Anne_w_Pack Program #
# =================== #

Anne_w_Pack: libs
	(cd Anne_w_Pack ; ${MAKE})

annep: Anne_w_Pack

AnneP: Anne_w_Pack

# =============== #
# packipt Program #
# =============== #

packipt: libs
	(cd PackIpt ; ${MAKE})

PackIpt: packipt

# =============== #
# makescc Program #
# =============== #

makescc: libs
	(cd MakeSCC ; ${MAKE})

MakeSCC: makescc

# ==================== #
# All Static Libraries #
# ==================== #

.PHONY: libs
libs: Common FrameWork NJRLib sccpack

# ====== #
# NJRLib #
# ====== #

njrlib:
	(cd NJRLib ; ${MAKE})

NJRLib: njrlib

# ========= #
# FrameWork #
# ========= #

framework:
	(cd FrameWork ; ${MAKE})

FrameWork: framework

# ====== #
# Common #
# ====== #

common:
	(cd Common ; ${MAKE})

Common: common

# ======= #
# SCCPack #
# ======= #

sccpack:
	(cd SCCPack ; ${MAKE})

SCCPack: sccpack

# ==================================== #
# Make necessary directories for build #
# ==================================== #

.PHONY: BuildingDir
BuildingDir: clean
	rm -Rf ${BIN}
	mkdir ${BIN}
	rm -Rf ${LIB}
	mkdir ${LIB}
	rm -Rf ${OBJ}
	mkdir ${OBJ}
	mkdir ${OBJ}Knight-Anne
	mkdir ${OBJ}Knight-Anne/Knight
	mkdir ${OBJ}Knight-Anne/Fiber
	mkdir ${OBJ}Knight-Anne/Anne
	mkdir ${OBJ}Knight-Anne/IDOInfo
	mkdir ${OBJ}Knight-Anne/IDOEdit
	mkdir ${OBJ}Knight-Anne/IDOCut
	mkdir ${OBJ}PackIpt
	mkdir ${OBJ}Anne_w_Pack
	mkdir ${OBJ}MakeSCC
	mkdir ${OBJ}Iris
	mkdir ${OBJ}Lib
	mkdir ${OBJ}Lib/Common
	mkdir ${OBJ}Lib/FrameWork
	mkdir ${OBJ}Lib/NJRLib
	mkdir ${OBJ}Lib/SCCPack

# ==============================================================
# Clean
# ==============================================================

.PHONY: clean
clean:
	rm -Rf ${OBJ}*
