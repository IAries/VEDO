include ../../Makefile.in

TMP=${patsubst ${VEDO_SRC}%,${VEDO_OBJ}%,${PWD}}/

all: ${VEDO_BIN}IrisGM

${VEDO_BIN}IrisGM: ${TMP}Iris_geometric.o        \
                   ${TMP}NBSParallelConsultant.o
	${mLNK} -o $@ $^ ${mLNK_FLAG}

${TMP}%.o: ${IMP}%.cpp
	${mCXX} -o $@ ${mCXX_FLAG} -c $<
