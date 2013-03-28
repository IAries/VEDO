include ../../Makefile.in

TMP_DEBUG=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_RELEASE}%,${PWD}}/

all: debug release
debug: ${VEDO_BIN}IrisGM-Debug
release: ${VEDO_BIN}IrisGM

${VEDO_BIN}IrisGM-Debug: ${TMP_DEBUG}IrisGM.o                \
                         ${TMP_DEBUG}NBSParallelConsultant.o
	${mLNK} -o $@ $^ ${mLNK_FLAGd}

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${mCXX} -o $@ ${mCXX_FLAGd} -c $<

${VEDO_BIN}IrisGM: ${TMP_RELEASE}IrisGM.o                \
                   ${TMP_RELEASE}NBSParallelConsultant.o
	${mLNK} -o $@ $^ ${mLNK_FLAG}

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${mCXX} -o $@ ${mCXX_FLAG} -c $<
