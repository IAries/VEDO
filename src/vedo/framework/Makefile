PROJECT = vedo
TARGET  = ${notdir ${PWD}}

include ../../Makefile.in

TMP_DEBUG=${patsubst ${PREFIX}src/%,${PROJECT_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${PREFIX}src/%,${PROJECT_OBJ_RELEASE}%,${PWD}}/

TMP_OBJS_DEBUG=${patsubst %,${TMP_DEBUG}%,${patsubst %.cpp,%.o,${wildcard *.cpp}}}
TMP_OBJS_RELEASE=${patsubst %,${TMP_RELEASE}%,${patsubst %.cpp,%.o,${wildcard *.cpp}}}

all: debug release
debug: ${PROJECT_LIB}lib${PROJECT}-${TARGET}-debug.a
release: ${PROJECT_LIB}lib${PROJECT}-${TARGET}.a

${PROJECT_LIB}lib${PROJECT}-${TARGET}-debug.a: ${TMP_OBJS_DEBUG}
	${AR} ${AR_FLAGd} $@ $^

${TMP_DEBUG}%.o: %.cpp
	${CXX} -o $@ ${CXX_FLAGd} -c $<

${PROJECT_LIB}lib${PROJECT}-${TARGET}.a: ${TMP_OBJS_RELEASE}
	${AR} ${AR_FLAG} $@ $^

${TMP_RELEASE}%.o: %.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<
