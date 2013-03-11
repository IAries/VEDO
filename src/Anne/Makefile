include ../../Makefile.in

TMP=${patsubst ${VEDO_SRC}%,${VEDO_OBJ}%,${PWD}}/

all: ${VEDO_BIN}Anne

${VEDO_BIN}Anne: ${TMP}Anne.o
	${LNK} -o $@ $< ${LNK_FLAG}

${TMP}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<

