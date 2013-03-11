# ========================== #
# Parameters of GNU Compiler #
# ========================== #

PARMETIS_ROOT    = /usr/local/ParMetis/gcc
PARMETIS_INCLUDE = -I${PARMETIS_ROOT} -I${PARMETIS_ROOT}/ParMETISLib -I${PARMETIS_ROOT}/METISLib -I${PARMETIS_ROOT}/Programs
PARMETIS_LIB     = -L${PARMETIS_ROOT} -L${PARMETIS_ROOT}/ParMETISLib -L${PARMETIS_ROOT}/METISLib -L${PARMETIS_ROOT}/Programs

CXX              = /usr/bin/g++
mCXX             = ${MPICH_ROOT}bin/mpicxx
CXX_FLAG         = -I${PARMETIS_ROOT} ${SHARE_CXX_FLAG}

AR               = /usr/bin/ar
AR_FLAG          = -r

LNK              = /usr/bin/g++
LNK_FLAG         = -lm ${SHARE_LNK_FLAG}

mLNK             = ${MPICH_ROOT}bin/mpicxx
mLNK_FLAG        = -L${MPICH_ROOT}lib -L${PARMETIS_ROOT} ${SHARE_LNK_FLAG} -lmpich -lmpichcxx -limf -lmetis -lparmetis
