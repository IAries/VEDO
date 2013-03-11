include ../../Makefile.in

TMP=${patsubst ${VEDO_SRC}%,${VEDO_OBJ}%,${PWD}}/

all: ${VEDO_BIN}Knight

${VEDO_BIN}Knight: ${TMP}3DLattices.o                    \
                   ${TMP}BravaisLatticeWithBasis.o       \
                   ${TMP}CalcCoordinateNum.o             \
                   ${TMP}CalcDensity.o                   \
                   ${TMP}ClusterInitializer.o            \
                   ${TMP}knight.o                        \
                   ${TMP}RectangularClusterInitializer.o \
                   ${TMP}svg.o
	${LNK} -o $@ $^ ${LNK_FLAGx}

${TMP}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGx} -c $<
