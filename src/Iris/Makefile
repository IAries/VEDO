include ../../Makefile.in

TMP_DEBUG=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_RELEASE}%,${PWD}}/

#all: ${VEDO_BIN}Iris-Debug ${VEDO_BIN}IrisGM-Debug ${VEDO_BIN}IrisGF-Debug ${VEDO_BIN}Iris ${VEDO_BIN}IrisGM ${VEDO_BIN}IrisGF
all: ${VEDO_BIN}IrisGM-Debug ${VEDO_BIN}IrisGM


${VEDO_BIN}Iris-Debug: ${TMP_DEBUG}BakGraphPartition.o     \
                       ${TMP_DEBUG}GraphPartition.o        \
                       ${TMP_DEBUG}Iris.o                  \
                       ${TMP_DEBUG}NBSGPConsultant.o       \
                       ${TMP_DEBUG}NBSParallelConsultant.o \
                       ${TMP_DEBUG}SyncCon.o               \
                       ${TMP_DEBUG}Table.o
	${mLNK} -o $@ $^ ${mLNK_FLAGgd}

${VEDO_BIN}IrisGM-Debug: ${TMP_DEBUG}Iris_geometric.o        \
                         ${TMP_DEBUG}NBSParallelConsultant.o
	${mLNK} -o $@ $^ ${mLNK_FLAGd}

${VEDO_BIN}Iris_GF-Debug: ${TMP_DEBUG}BakGraphPartition.o \
                          ${TMP_DEBUG}GraphPartition.o    \
                          ${TMP_DEBUG}Iris_graph.o        \
                          ${TMP_DEBUG}NBSGPConsultant.o   \
                          ${TMP_DEBUG}SyncCon.o           \
                          ${TMP_DEBUG}Table.o
	${mLNK} -o $@ $^ ${mLNK_FLAGgd}

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${mCXX} -o $@ ${mCXX_FLAGd} -c $<

${VEDO_BIN}Iris: ${TMP_RELEASE}BakGraphPartition.o     \
                 ${TMP_RELEASE}GraphPartition.o        \
                 ${TMP_RELEASE}Iris.o                  \
                 ${TMP_RELEASE}NBSGPConsultant.o       \
                 ${TMP_RELEASE}NBSParallelConsultant.o \
                 ${TMP_RELEASE}SyncCon.o               \
                 ${TMP_RELEASE}Table.o
	${mLNK} -o $@ $^ ${mLNK_FLAGg}

${VEDO_BIN}IrisGM: ${TMP_RELEASE}Iris_geometric.o        \
                   ${TMP_RELEASE}NBSParallelConsultant.o
	${mLNK} -o $@ $^ ${mLNK_FLAG}

${VEDO_BIN}Iris_GF: ${TMP_RELEASE}BakGraphPartition.o \
                    ${TMP_RELEASE}GraphPartition.o    \
                    ${TMP_RELEASE}Iris_graph.o        \
                    ${TMP_RELEASE}NBSGPConsultant.o   \
                    ${TMP_RELEASE}SyncCon.o           \
                    ${TMP_RELEASE}Table.o
	${mLNK} -o $@ $^ ${mLNK_FLAGg}

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${mCXX} -o $@ ${mCXX_FLAG} -c $<
