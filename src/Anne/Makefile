include ../../Makefile.in

TMP_DEBUG=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_RELEASE}%,${PWD}}/

all: debug release
debug: ${VEDO_BIN}Anne-Debug
release: ${VEDO_BIN}Anne

${VEDO_BIN}Anne-Debug: ${TMP_DEBUG}Anne.o
	${LNK} -o $@ $< ${LNK_FLAGd}

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGd} -c $<

${VEDO_BIN}Anne: ${TMP_RELEASE}Anne.o
	${LNK} -o $@ $< ${LNK_FLAG}

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<

