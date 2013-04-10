include ../../Makefile.in

TMP_DEBUG=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_RELEASE}%,${PWD}}/

all: debug release
debug: ${VEDO_BIN}knight-debug
release: ${VEDO_BIN}knight

${VEDO_BIN}knight-debug: ${TMP_DEBUG}3DLattices.o                    \
                         ${TMP_DEBUG}BravaisLatticeWithBasis.o       \
                         ${TMP_DEBUG}CalcCoordinateNum.o             \
                         ${TMP_DEBUG}CalcDensity.o                   \
                         ${TMP_DEBUG}ClusterInitializer.o            \
                         ${TMP_DEBUG}Knight.o                        \
                         ${TMP_DEBUG}RectangularClusterInitializer.o \
                         ${TMP_DEBUG}svg.o
	${LNK} -o $@ $^ ${LNK_FLAGxd}

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGxd} -c $<

${VEDO_BIN}knight: ${TMP_RELEASE}3DLattices.o                    \
                   ${TMP_RELEASE}BravaisLatticeWithBasis.o       \
                   ${TMP_RELEASE}CalcCoordinateNum.o             \
                   ${TMP_RELEASE}CalcDensity.o                   \
                   ${TMP_RELEASE}ClusterInitializer.o            \
                   ${TMP_RELEASE}Knight.o                        \
                   ${TMP_RELEASE}RectangularClusterInitializer.o \
                   ${TMP_RELEASE}svg.o
	${LNK} -o $@ $^ ${LNK_FLAGx}

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGx} -c $<
