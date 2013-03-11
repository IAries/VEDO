include ../../Makefile.in

TMP=${patsubst ${VEDO_SRC}%,${VEDO_OBJ}%,${PWD}}/

#all: ${VEDO_BIN}Iris ${VEDO_BIN}IrisGM ${VEDO_BIN}IrisGF
all: ${VEDO_BIN}IrisGM

${VEDO_BIN}Iris: ${TMP}BakGraphPartition.o     \
                 ${TMP}GraphPartition.o        \
                 ${TMP}Iris.o                  \
                 ${TMP}NBSGPConsultant.o       \
                 ${TMP}NBSParallelConsultant.o \
                 ${TMP}SyncCon.o               \
                 ${TMP}Table.o
	${mLNK} -o $@ $^ ${mLNK_FLAGg}

${VEDO_BIN}IrisGM: ${TMP}Iris_geometric.o        \
                   ${TMP}NBSParallelConsultant.o
	${mLNK} -o $@ $^ ${mLNK_FLAG}

${VEDO_BIN}Iris_GF: ${TMP}BakGraphPartition.o \
                    ${TMP}GraphPartition.o    \
                    ${TMP}Iris_graph.o        \
                    ${TMP}NBSGPConsultant.o   \
                    ${TMP}SyncCon.o           \
                    ${TMP}Table.o
	${mLNK} -o $@ $^ ${mLNK_FLAGg}

${TMP}%.o: ${IMP}%.cpp
	${mCXX} -o $@ ${mCXX_FLAG} -c $<
