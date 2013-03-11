include ../../Makefile.in

TMP=${patsubst ${VEDO_SRC}%,${VEDO_OBJ}%,${PWD}}/

all: ${VEDO_LIB}libVEDO-NJR.a
${VEDO_LIB}libVEDO-NJR.a: ${TMP}acadxf.o            \
                          ${TMP}dxfsolid.o          \
                          ${TMP}GeoUtility.o        \
                          ${TMP}halfspace.o         \
                          ${TMP}LinearProgramming.o \
                          ${TMP}matrix.o            \
                          ${TMP}polygon.o           \
                          ${TMP}polyhedra.o         \
                          ${TMP}random.o            \
                          ${TMP}RickyRandom.o       \
                          ${TMP}Utility.o           \
                          ${TMP}vector2d.o          \
                          ${TMP}vector3d.o
	${AR} ${AR_FLAG} $@ $^

${TMP}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<
