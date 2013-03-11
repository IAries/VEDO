# ========================== #
# Parameters of GNU Compiler #
# ========================== #

PARMETIS_ROOT = ${HOME}/iusr/local/ParMetis/
PARMETIS_INC  = -I${PARMETIS_ROOT} -I${PARMETIS_ROOT}ParMETISLib -I${PARMETIS_ROOT}METISLib -I${PARMETIS_ROOT}Programs
PARMETIS_LIB  = -L${PARMETIS_ROOT} -L${PARMETIS_ROOT}ParMETISLib -L${PARMETIS_ROOT}METISLib -L${PARMETIS_ROOT}Programs

MPICH_ROOT    = /opt/mpich/gnu/
MPICH_INC     = ${MPICH_ROOT}include
MPICH_LIB     = ${MPICH_ROOT}lib

CXX           = /opt/gcc/bin/g++
CXX_FLAG      = ${PARMETIS_INC} ${SHARE_CXX_FLAG}

mCXX          = ${MPICH_ROOT}bin/mpiCC
mCXX_FLAG     = ${PARMETIS_INC} -I${MPICH_INC} ${SHARE_CXX_FLAG}

AR            = /usr/bin/ar
AR_FLAG       = -r

LNK           = /opt/gcc/bin/g++
LNK_FLAG      = -lm ${SHARE_LNK_FLAG}

mLNK          = ${MPICH_ROOT}bin/mpiCC
mLNK_FLAG     = ${PARMETIS_LIB} -L${MPICH_ROOT}lib ${SHARE_LNK_FLAG} -lmpich -lpmpich++ -limf -lparmetis -lmetis

