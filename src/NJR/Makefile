include ../../Makefile.in

TMP_DEBUG=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_RELEASE}%,${PWD}}/

all: ${VEDO_LIB}libVEDO-NJR-Debug.a ${VEDO_LIB}libVEDO-NJR.a

${VEDO_LIB}libVEDO-NJR-Debug.a: ${TMP_DEBUG}acadxf.o            \
                                ${TMP_DEBUG}dxfsolid.o          \
                                ${TMP_DEBUG}GeoUtility.o        \
                                ${TMP_DEBUG}halfspace.o         \
                                ${TMP_DEBUG}LinearProgramming.o \
                                ${TMP_DEBUG}matrix.o            \
                                ${TMP_DEBUG}polygon.o           \
                                ${TMP_DEBUG}polyhedra.o         \
                                ${TMP_DEBUG}random.o            \
                                ${TMP_DEBUG}RickyRandom.o       \
                                ${TMP_DEBUG}Utility.o           \
                                ${TMP_DEBUG}vector2d.o          \
                                ${TMP_DEBUG}vector3d.o
	${AR} ${AR_FLAG} $@ $^

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGd} -c $<

${VEDO_LIB}libVEDO-NJR.a: ${TMP_RELEASE}acadxf.o            \
                          ${TMP_RELEASE}dxfsolid.o          \
                          ${TMP_RELEASE}GeoUtility.o        \
                          ${TMP_RELEASE}halfspace.o         \
                          ${TMP_RELEASE}LinearProgramming.o \
                          ${TMP_RELEASE}matrix.o            \
                          ${TMP_RELEASE}polygon.o           \
                          ${TMP_RELEASE}polyhedra.o         \
                          ${TMP_RELEASE}random.o            \
                          ${TMP_RELEASE}RickyRandom.o       \
                          ${TMP_RELEASE}Utility.o           \
                          ${TMP_RELEASE}vector2d.o          \
                          ${TMP_RELEASE}vector3d.o
	${AR} ${AR_FLAG} $@ $^

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<
