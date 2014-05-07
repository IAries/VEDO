include ../../Makefile.in

TMP_TARGET=${patsubst %/,%,${patsubst ${VEDO_SRC}vedo/%,%,${PWD}}}

TMP_DEBUG=${patsubst ${VEDO_SRC}vedo/%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}vedo/%,${VEDO_OBJ_RELEASE}%,${PWD}}/

TMP_OBJS_DEBUG=${patsubst ${IMP}%,${TMP_DEBUG}%,${patsubst %.cpp,%.o,${wildcard ${IMP}*.cpp}}}
TMP_OBJS_RELEASE=${patsubst ${IMP}%,${TMP_RELEASE}%,${patsubst %.cpp,%.o,${wildcard ${IMP}*.cpp}}}

all: debug release
debug: ${VEDO_BIN}${TMP_TARGET}-debug
release: ${VEDO_BIN}${TMP_TARGET}

${VEDO_BIN}${TMP_TARGET}-debug: ${TMP_OBJS_DEBUG}
	${LNK} -o $@ $^ ${LNK_FLAGd}

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGd} -c $<

${VEDO_BIN}${TMP_TARGET}: ${TMP_OBJS_RELEASE}
	${LNK} -o $@ $^ ${LNK_FLAG}

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<

