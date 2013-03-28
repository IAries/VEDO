include ../../Makefile.in

TMP_DEBUG=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_RELEASE}%,${PWD}}/

all: debug release
debug: ${VEDO_LIB}libVEDO-NJR-Debug.a
release: ${VEDO_LIB}libVEDO-NJR.a

${VEDO_LIB}libVEDO-NJR-Debug.a: ${TMP_DEBUG}ACADXF.o            \
                                ${TMP_DEBUG}ACADXF_Solid.o      \
                                ${TMP_DEBUG}GeoUtility.o        \
                                ${TMP_DEBUG}HalfSpace.o         \
                                ${TMP_DEBUG}LinearProgramming.o \
                                ${TMP_DEBUG}Matrix.o            \
                                ${TMP_DEBUG}Polygon.o           \
                                ${TMP_DEBUG}Polyhedra.o         \
                                ${TMP_DEBUG}RandomGenerator.o   \
                                ${TMP_DEBUG}RickyRandom.o       \
                                ${TMP_DEBUG}TimeHistory.o       \
                                ${TMP_DEBUG}Utility.o           \
                                ${TMP_DEBUG}Vector2d.o          \
                                ${TMP_DEBUG}Vector3d.o
	${AR} ${AR_FLAG} $@ $^

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGd} -c $<

${VEDO_LIB}libVEDO-NJR.a: ${TMP_RELEASE}ACADXF.o            \
                          ${TMP_RELEASE}ACADXF_Solid.o      \
                          ${TMP_RELEASE}GeoUtility.o        \
                          ${TMP_RELEASE}HalfSpace.o         \
                          ${TMP_RELEASE}LinearProgramming.o \
                          ${TMP_RELEASE}Matrix.o            \
                          ${TMP_RELEASE}Polygon.o           \
                          ${TMP_RELEASE}Polyhedra.o         \
                          ${TMP_RELEASE}RandomGenerator.o   \
                          ${TMP_RELEASE}RickyRandom.o       \
                          ${TMP_RELEASE}TimeHistory.o       \
                          ${TMP_RELEASE}Utility.o           \
                          ${TMP_RELEASE}Vector2d.o          \
                          ${TMP_RELEASE}Vector3d.o
	${AR} ${AR_FLAG} $@ $^

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<
