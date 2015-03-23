PROJECT = vedo
TARGET  = ${notdir ${PWD}}

include ../../Makefile.in

TMP_DEBUG=${patsubst ${PREFIX}src/%,${PROJECT_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${PREFIX}src/%,${PROJECT_OBJ_RELEASE}%,${PWD}}/

TMP_OBJS_DEBUG=${patsubst %,${TMP_DEBUG}%,${patsubst %.cpp,%.o,${wildcard *.cpp}}}
TMP_OBJS_RELEASE=${patsubst %,${TMP_RELEASE}%,${patsubst %.cpp,%.o,${wildcard *.cpp}}}

all: debug release
debug: ${PROJECT_BIN}${TARGET}-debug
release: ${PROJECT_BIN}${TARGET}

${PROJECT_BIN}${TARGET}-debug: ${TMP_OBJS_DEBUG}
	${mLNK} -o $@ $^ ${mLNK_FLAGd}

${TMP_DEBUG}%.o: %.cpp
	${mCXX} -o $@ ${mCXX_FLAGd} -c $<

${PROJECT_BIN}${TARGET}: ${TMP_OBJS_RELEASE}
	${mLNK} -o $@ $^ ${mLNK_FLAG}

${TMP_RELEASE}%.o: %.cpp
	${mCXX} -o $@ ${mCXX_FLAG} -c $<
