include ../../Makefile.in

TMP_TARGET=${patsubst %/,%,${patsubst ${VEDO_SRC}vedo/%,%,${PWD}}}

TMP_DEBUG=${patsubst ${VEDO_SRC}vedo/%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}vedo/%,${VEDO_OBJ_RELEASE}%,${PWD}}/

TMP_OBJS_DEBUG=${patsubst ${IMP}%,${TMP_DEBUG}%,${patsubst %.cpp,%.o,${wildcard ${IMP}*.cpp}}}
TMP_OBJS_RELEASE=${patsubst ${IMP}%,${TMP_RELEASE}%,${patsubst %.cpp,%.o,${wildcard ${IMP}*.cpp}}}

all: debug release
debug: ${VEDO_LIB}libvedo-${TMP_TARGET}-debug.a
release: ${VEDO_LIB}libvedo-${TMP_TARGET}.a

${VEDO_LIB}libvedo-${TMP_TARGET}-debug.a: ${TMP_OBJS_DEBUG}
	${AR} ${AR_FLAGd} $@ $^

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGd} -c $<

${VEDO_LIB}libvedo-${TMP_TARGET}.a: ${TMP_OBJS_RELEASE}
	${AR} ${AR_FLAG} $@ $^

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<

