# Makefile for Anne-1.0b
# Created by C.-C. Chou (Mustang) 2004 July.
# Modifed by C.-T. Yang  (Jitin)  2004 Sep.

SRC = /home/shhsieh/KnightAnne-Beta-1.01/

BCH = ${SRC}BenchMark/
CMN = ${SRC}Common/
FWK = ${SRC}FrameWork/
KNA = ${SRC}Knight-Anne/
NLB = ${SRC}NJRLib/
DLVO = ${SRC}Knight-Anne/Anne_DLVO/
IRIS = $(SRC)Iris/
IDOEdit = $(SRC)IDOEdit/
IDOInfo = $(SRC)IDOInfo/
SCP = $(SRC)SCCPack/
AWP = $(SRC)Anne_w_Pack/
PKT = ${SRC}PackIpt/
CUT = ${SRC}CutIDO/

IMP = Implementation/
INC = Interfaces/

OBJ = /cal/wokerla/obj/
BUILD = /cal/wokerla/bin/
LIB   = /cal/wokerla/lib/
BUILD_ROOT = /cal/wokerla/


LOCAL_LIB = /usr/local/lib
LOCAL_INCLUDE = /usr/local/include

XERCES_LIB = /cal/Utility/xerces-c/lib
XERCES_INCLUDE = /cal/Utility/xerces-c/include 


CC = /usr/vacpp/bin/xlC
AR = /usr/bin/ar
LNK = /usr/vacpp/bin/xlC
mCC = /cal/wokerla/mpich-1.2.6/bin/mpiCC
mLNK = /cal/wokerla/mpich-1.2.6/bin/mpiCC

CC_OPTION = -I${SRC} -I${XERCES_INCLUDE} 

LNK_OPTION =  -O5 -lm -L${XERCES_LIB} -L${LIB} -lxerces-c -lVEDO -lNJR -lalgorithm

all: knight anne iris idoedit idoinfo sccpack Anne_w_Pack packipt cutido
	make knight anne iris idoedit idoinfo sccpack Anne_w_Pack packipt cutido

#
# Knight Program
#
knight: ${BUILD}knight

${BUILD}knight: libs ${OBJ}knight.o ${OBJ}vedo_bs1.o ${OBJ}vedo_bs4.o\
                ${OBJ}CalcCoordinateNum.o ${OBJ}CalcDensity.o
	${LNK} -o ${BUILD}knight ${OBJ}knight.o ${OBJ}vedo_bs1.o\
                  ${OBJ}vedo_bs4.o\
                ${OBJ}CalcCoordinateNum.o ${OBJ}CalcDensity.o ${LNK_OPTION}

${OBJ}knight.o: ${KNA}Knight/${IMP}/knight.cpp
	${CC} -o ${OBJ}knight.o\
        ${CC_OPTION} -c ${KNA}Knight/${IMP}/knight.cpp	

${OBJ}vedo_bs1.o: ${KNA}Knight/${IMP}/vedo_bs1.cpp
	${CC} -o ${OBJ}vedo_bs1.o\
        ${CC_OPTION} -c ${KNA}Knight/${IMP}/vedo_bs1.cpp	

${OBJ}vedo_bs4.o: ${KNA}Knight/${IMP}/vedo_bs4.cpp
	${CC} -o ${OBJ}vedo_bs4.o\
        ${CC_OPTION} -c ${KNA}Knight/${IMP}/vedo_bs4.cpp	

${OBJ}CalcCoordinateNum.o: ${KNA}Knight/${IMP}CalcCoordinateNum.cpp
	${CC} -o ${OBJ}CalcCoordinateNum.o\
        ${CC_OPTION} -c ${KNA}Knight/${IMP}CalcCoordinateNum.cpp	

${OBJ}CalcDensity.o: ${KNA}Knight/${IMP}CalcDensity.cpp
	${CC} -o ${OBJ}CalcDensity.o\
        ${CC_OPTION} -c ${KNA}Knight/${IMP}CalcDensity.cpp	


#
# Anne Program
#

anne: ${BUILD}anne

${BUILD}anne:	libs ${OBJ}anne.o
	${LNK} -o ${BUILD}anne ${OBJ}anne.o ${LNK_OPTION}

${OBJ}anne.o: ${KNA}Anne/Anne.cpp
	${CC} -o ${OBJ}anne.o ${CC_OPTION} -c ${KNA}Anne/Anne.cpp	

#
# Anne with DLVO Theory  
#

dlvo: ${BUILD}AnneDLVO

${BUILD}AnneDLVO: libs\
          ${OBJ}AnneDLVO.o ${OBJ}DLVOBenchMark.o ${OBJ}LongRangeForce.o\
          ${OBJ}LongRangeForceWall.o ${OBJ}DOColloid.o ${OBJ}DOPBColloid.o\
          ${OBJ}ISDLVO.o ${OBJ}ISDLVOWall.o ${OBJ}CDColloid2Colloid.o\
          ${OBJ}CDColloid2Plate.o ${OBJ}CDPBColloid2PBColloid.o  

	${LNK} -o ${BUILD}AnneDLVO ${LNK_OPTION}\
          ${OBJ}AnneDLVO.o ${OBJ}DLVOBenchMark.o ${OBJ}LongRangeForce.o\
          ${OBJ}LongRangeForceWall.o ${OBJ}DOColloid.o ${OBJ}DOPBColloid.o\
          ${OBJ}ISDLVO.o ${OBJ}ISDLVOWall.o ${OBJ}CDColloid2Colloid.o\
          ${OBJ}CDColloid2Plate.o ${OBJ}CDPBColloid2PBColloid.o  

${OBJ}AnneDLVO.o: ${DLVO}${IMP}ANNEDLVO.cpp
	${CC} -o ${OBJ}AnneDLVO.o\
        ${CC_OPTION} -c ${DLVO}${IMP}ANNEDLVO.cpp	

${OBJ}DLVOBenchMark.o: ${DLVO}${IMP}DLVOBenchMark.cpp
	${CC} -o ${OBJ}DLVOBenchMark.o\
        ${CC_OPTION} -c ${DLVO}${IMP}DLVOBenchMark.cpp	

${OBJ}LongRangeForce.o: ${DLVO}${IMP}LongRangeForce.cpp
	${CC} -o ${OBJ}LongRangeForce.o\
        ${CC_OPTION} -c ${DLVO}${IMP}LongRangeForce.cpp	

${OBJ}LongRangeForceWall.o: ${DLVO}${IMP}LongRangeForceWall.cpp
	${CC} -o ${OBJ}LongRangeForceWall.o\
        ${CC_OPTION} -c ${DLVO}${IMP}LongRangeForceWall.cpp	

${OBJ}DOColloid.o: ${DLVO}${IMP}DOColloid.cpp
	${CC} -o ${OBJ}DOColloid.o\
        ${CC_OPTION} -c ${DLVO}${IMP}DOColloid.cpp	

${OBJ}DOPBColloid.o: ${DLVO}${IMP}DOPBColloid.cpp
	${CC} -o ${OBJ}DOPBColloid.o\
        ${CC_OPTION} -c ${DLVO}${IMP}DOPBColloid.cpp	

${OBJ}ISDLVO.o: ${DLVO}${IMP}ISDLVO.cpp
	${CC} -o ${OBJ}ISDLVO.o\
        ${CC_OPTION} -c ${DLVO}${IMP}ISDLVO.cpp	

${OBJ}ISDLVOWall.o: ${DLVO}${IMP}ISDLVOWall.cpp
	${CC} -o ${OBJ}ISDLVOWall.o\
        ${CC_OPTION} -c ${DLVO}${IMP}ISDLVOWall.cpp	

${OBJ}CDColloid2Colloid.o: ${DLVO}${IMP}CDColloid2Colloid.cpp
	${CC} -o ${OBJ}CDColloid2Colloid.o\
        ${CC_OPTION} -c ${DLVO}${IMP}CDColloid2Colloid.cpp	

${OBJ}CDColloid2Plate.o: ${DLVO}${IMP}CDColloid2Plate.cpp
	${CC} -o ${OBJ}CDColloid2Plate.o\
        ${CC_OPTION} -c ${DLVO}${IMP}CDColloid2Plate.cpp	

${OBJ}CDPBColloid2PBColloid.o: ${DLVO}${IMP}CDPBColloid2PBColloid.cpp
	${CC} -o ${OBJ}CDPBColloid2PBColloid.o\
        ${CC_OPTION} -c ${DLVO}${IMP}CDPBColloid2PBColloid.cpp	

#
# All Static Libraries
#
libs: Common FrameWork NJRLib sccpack

#
# NJRLib
#
njrlib: ${LIB}libNJR.a
NJRLib: ${LIB}libNJR.a

${LIB}libNJR.a: ${OBJ}vector3d.o ${OBJ}random.o\
	${OBJ}halfspace.o ${OBJ}RickyRandom.o\
	${OBJ}Utility.o ${OBJ}GeoUtility.o\
	${OBJ}acadxf.o ${OBJ}dxfsolid.o\
	${OBJ}matrix.o ${OBJ}LinearProgramming.o\
	${OBJ}polygon.o  ${OBJ}polyhedra.o 

	${AR} -r ${LIB}libNJR.a ${OBJ}vector3d.o ${OBJ}random.o\
	${OBJ}halfspace.o ${OBJ}RickyRandom.o\
	${OBJ}Utility.o ${OBJ}GeoUtility.o\
	${OBJ}acadxf.o ${OBJ}dxfsolid.o\
	${OBJ}matrix.o ${OBJ}LinearProgramming.o\
	${OBJ}polygon.o  ${OBJ}polyhedra.o 

${OBJ}vector3d.o: ${NLB}${IMP}vector3d.cpp 
	${CC} -o ${OBJ}vector3d.o\
        ${CC_OPTION} -c ${NLB}${IMP}vector3d.cpp

${OBJ}GeoUtility.o: ${NLB}${IMP}GeoUtility.cpp 
	${CC} -o ${OBJ}GeoUtility.o\
        ${CC_OPTION} -c ${NLB}${IMP}GeoUtility.cpp

${OBJ}LinearProgramming.o: ${NLB}${IMP}LinearProgramming.cpp 
	${CC} -o ${OBJ}LinearProgramming.o\
        ${CC_OPTION} -c ${NLB}${IMP}LinearProgramming.cpp

${OBJ}QuadraticProgramming.o: ${NLB}${IMP}QuadraticProgramming.cpp 
	${CC} -o ${OBJ}QuadraticProgramming.o\
        ${CC_OPTION} -c ${NLB}${IMP}QuadraticProgramming.cpp

${OBJ}RickyRandom.o: ${NLB}${IMP}RickyRandom.cpp
	${CC} -o ${OBJ}RickyRandom.o\
        ${CC_OPTION} -c ${NLB}${IMP}RickyRandom.cpp

${OBJ}Utility.o: ${NLB}${IMP}Utility.cpp 
	${CC} -o ${OBJ}Utility.o\
        ${CC_OPTION} -c ${NLB}${IMP}Utility.cpp

${OBJ}acadxf.o:	${NLB}${IMP}acadxf.cpp 
	${CC} -o ${OBJ}acadxf.o\
        ${CC_OPTION} -c ${NLB}${IMP}acadxf.cpp

${OBJ}dxfsolid.o: ${NLB}${IMP}dxfsolid.cpp 
	${CC} -o ${OBJ}dxfsolid.o\
        ${CC_OPTION} -c ${NLB}${IMP}dxfsolid.cpp

${OBJ}halfspace.o: ${NLB}${IMP}halfspace.cpp
	${CC} -o ${OBJ}halfspace.o\
        ${CC_OPTION} -c ${NLB}${IMP}halfspace.cpp

${OBJ}matrix.o:	${NLB}${IMP}matrix.cpp 
	${CC} -o ${OBJ}matrix.o\
        ${CC_OPTION} -c ${NLB}${IMP}matrix.cpp

${OBJ}polygon.o: ${NLB}${IMP}polygon.cpp 
	${CC} -o ${OBJ}polygon.o\
        ${CC_OPTION} -c ${NLB}${IMP}polygon.cpp

${OBJ}polyhedra.o: ${NLB}${IMP}polyhedra.cpp
	${CC} -o ${OBJ}polyhedra.o\
        ${CC_OPTION} -c ${NLB}${IMP}polyhedra.cpp

${OBJ}random.o:	${NLB}${IMP}random.cpp
	${CC} -o ${OBJ}random.o\
        ${CC_OPTION} -c ${NLB}${IMP}random.cpp

#
# FrameWork
#

FrameWork: ${LIB}libVEDO.a 

framework: ${LIB}libVEDO.a 

${LIB}libVEDO.a: ${OBJ}Assembler.o ${OBJ}CDCFactoryDecorator.o \
	${OBJ}Consultant.o ${OBJ}ContactDetector.o \
	${OBJ}DOCFactoryDecorator.o ${OBJ}DOContainer.o \
	${OBJ}DOModel.o ${OBJ}DOStatus.o ${OBJ}DOWorld.o \
	${OBJ}DOWorld_DrawDxf.o ${OBJ}DOWorld_RWIDO.o \
	${OBJ}DOWorld_WriteVPF.o ${OBJ}DOWorld_WriteXML.o \
	${OBJ}DOWorld_ReadXML.o \
	${OBJ}DiscreteObject.o ${OBJ}ISCFactoryDecorator.o \
	${OBJ}IactContainer.o ${OBJ}IactModel.o \
	${OBJ}ImpactSolver.o ${OBJ}Interaction.o \
	${OBJ}SimMediator.o ${OBJ}SystemParameter.o

	${AR} -r ${LIB}libVEDO.a\
        ${OBJ}Assembler.o ${OBJ}CDCFactoryDecorator.o \
	${OBJ}Consultant.o ${OBJ}ContactDetector.o \
	${OBJ}DOCFactoryDecorator.o ${OBJ}DOContainer.o \
	${OBJ}DOModel.o ${OBJ}DOStatus.o ${OBJ}DOWorld.o \
	${OBJ}DOWorld_DrawDxf.o ${OBJ}DOWorld_RWIDO.o \
	${OBJ}DOWorld_WriteVPF.o ${OBJ}DOWorld_WriteXML.o \
	${OBJ}DOWorld_ReadXML.o \
	${OBJ}DiscreteObject.o ${OBJ}ISCFactoryDecorator.o \
	${OBJ}IactContainer.o ${OBJ}IactModel.o \
	${OBJ}ImpactSolver.o ${OBJ}Interaction.o \
	${OBJ}SimMediator.o ${OBJ}SystemParameter.o

${OBJ}Assembler.o: ${FWK}${IMP}Assembler.cpp
	${CC} -o ${OBJ}Assembler.o\
        ${CC_OPTION} -c ${FWK}${IMP}Assembler.cpp

${OBJ}CDCFactoryDecorator.o: ${FWK}${IMP}CDCFactoryDecorator.cpp
	${CC} -o ${OBJ}CDCFactoryDecorator.o\
        ${CC_OPTION} -c ${FWK}${IMP}CDCFactoryDecorator.cpp

${OBJ}Consultant.o: ${FWK}${IMP}Consultant.cpp
	${CC} -o ${OBJ}Consultant.o\
        ${CC_OPTION} -c ${FWK}${IMP}Consultant.cpp

${OBJ}ContactDetector.o: ${FWK}${IMP}ContactDetector.cpp
	${CC} -o ${OBJ}ContactDetector.o\
        ${CC_OPTION} -c ${FWK}${IMP}ContactDetector.cpp

${OBJ}DOCFactoryDecorator.o: ${FWK}${IMP}DOCFactoryDecorator.cpp
	${CC} -o ${OBJ}DOCFactoryDecorator.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOCFactoryDecorator.cpp

${OBJ}DOContainer.o: ${FWK}${IMP}DOContainer.cpp
	${CC} -o ${OBJ}DOContainer.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOContainer.cpp

${OBJ}DOModel.o: ${FWK}${IMP}DOModel.cpp 
	${CC} -o ${OBJ}DOModel.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOModel.cpp

${OBJ}DOStatus.o: ${FWK}${IMP}DOStatus.cpp 
	${CC} -o ${OBJ}DOStatus.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOStatus.cpp

${OBJ}DOWorld.o: ${FWK}${IMP}DOWorld.cpp
	${CC} -o ${OBJ}DOWorld.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOWorld.cpp

${OBJ}DOWorld_DrawDxf.o: ${FWK}${IMP}DOWorld_DrawDxf.cpp 
	${CC} -o ${OBJ}DOWorld_DrawDxf.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOWorld_DrawDxf.cpp

${OBJ}DOWorld_RWIDO.o: ${FWK}${IMP}DOWorld_RWIDO.cpp 
	${CC} -o ${OBJ}DOWorld_RWIDO.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOWorld_RWIDO.cpp

${OBJ}DOWorld_WriteVPF.o: ${FWK}${IMP}DOWorld_WriteVPF.cpp
	${CC} -o ${OBJ}DOWorld_WriteVPF.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOWorld_WriteVPF.cpp

${OBJ}DOWorld_WriteXML.o: ${FWK}${IMP}DOWorld_WriteXML.cpp
	${CC} -o ${OBJ}DOWorld_WriteXML.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOWorld_WriteXML.cpp

${OBJ}DiscreteObject.o: ${FWK}${IMP}DiscreteObject.cpp
	${CC} -o ${OBJ}DiscreteObject.o\
        ${CC_OPTION} -c ${FWK}${IMP}DiscreteObject.cpp

${OBJ}ISCFactoryDecorator.o: ${FWK}${IMP}ISCFactoryDecorator.cpp
	${CC} -o ${OBJ}ISCFactoryDecorator.o\
        ${CC_OPTION} -c ${FWK}${IMP}ISCFactoryDecorator.cpp

${OBJ}IactContainer.o: ${FWK}${IMP}IactContainer.cpp
	${CC} -o ${OBJ}IactContainer.o\
        ${CC_OPTION} -c ${FWK}${IMP}IactContainer.cpp

${OBJ}IactModel.o: ${FWK}${IMP}IactModel.cpp
	${CC} -o ${OBJ}IactModel.o\
        ${CC_OPTION} -c ${FWK}${IMP}IactModel.cpp

${OBJ}ImpactSolver.o:	${FWK}${IMP}ImpactSolver.cpp 
	${CC} -o ${OBJ}ImpactSolver.o\
        ${CC_OPTION} -c ${FWK}${IMP}ImpactSolver.cpp

${OBJ}Interaction.o: ${FWK}${IMP}Interaction.cpp
	${CC} -o ${OBJ}Interaction.o\
        ${CC_OPTION} -c ${FWK}${IMP}Interaction.cpp

${OBJ}SimMediator.o: ${FWK}${IMP}SimMediator.cpp 
	${CC} -o ${OBJ}SimMediator.o\
        ${CC_OPTION} -c ${FWK}${IMP}SimMediator.cpp

${OBJ}SystemParameter.o: ${FWK}${IMP}SystemParameter.cpp 
	${CC} -o ${OBJ}SystemParameter.o\
        ${CC_OPTION} -c ${FWK}${IMP}SystemParameter.cpp

${OBJ}DOWorld_ReadXML.o: ${FWK}${IMP}DOWorld_ReadXML.cpp
	${CC} -o ${OBJ}DOWorld_ReadXML.o\
        ${CC_OPTION} -c ${FWK}${IMP}DOWorld_ReadXML.cpp

#
# Common
#
Common: ${LIB}libalgorithm.a

${LIB}libalgorithm.a: ${OBJ}CDSphere2QuasiCylinder.o\
        ${OBJ}CDSphere2QuasiPlate.o ${OBJ}CDSphere2Sphere.o\
        ${OBJ}CDSphere2QuasiCylinderAT.o ${OBJ}CDSphere2SphereAT.o\
        ${OBJ}DOFixedQuasiCylinder.o\
        ${OBJ}DOFixedQuasiPlate.o ${OBJ}DOMap.o ${OBJ}DOSphere.o\
        ${OBJ}ISwLSD.o ${OBJ}ISwSSD.o ${OBJ}ISwSSDB.o ${OBJ}ISwSSDBF.o\
        ${OBJ}ISwSSDF.o  ${OBJ}ISwLSDAT.o\
        ${OBJ}LeapConsultant.o ${OBJ}SafeConsultant.o ${OBJ}NearConsultant.o

	${AR} -r ${LIB}libalgorithm.a\
        ${OBJ}CDSphere2QuasiCylinder.o ${OBJ}CDSphere2QuasiPlate.o\
        ${OBJ}CDSphere2Sphere.o ${OBJ}DOFixedQuasiCylinder.o\
        ${OBJ}CDSphere2QuasiCylinderAT.o ${OBJ}CDSphere2SphereAT.o\
        ${OBJ}DOFixedQuasiPlate.o ${OBJ}DOMap.o ${OBJ}DOSphere.o\
        ${OBJ}ISwLSD.o ${OBJ}ISwSSD.o ${OBJ}ISwSSDB.o ${OBJ}ISwSSDBF.o\
        ${OBJ}ISwSSDF.o  ${OBJ}ISwLSDAT.o\
        ${OBJ}LeapConsultant.o ${OBJ}SafeConsultant.o ${OBJ}NearConsultant.o

${OBJ}CDSphere2QuasiCylinder.o: ${CMN}${IMP}CDSphere2QuasiCylinder.cpp 
	${CC} -o ${OBJ}CDSphere2QuasiCylinder.o\
        ${CC_OPTION} -c ${CMN}${IMP}CDSphere2QuasiCylinder.cpp

${OBJ}CDSphere2QuasiCylinderAT.o: ${CMN}${IMP}CDSphere2QuasiCylinderAT.cpp 
	${CC} -o ${OBJ}CDSphere2QuasiCylinderAT.o\
        ${CC_OPTION} -c ${CMN}${IMP}CDSphere2QuasiCylinderAT.cpp

${OBJ}CDSphere2QuasiPlate.o: ${CMN}${IMP}CDSphere2QuasiPlate.cpp 
	${CC} -o ${OBJ}CDSphere2QuasiPlate.o\
        ${CC_OPTION} -c ${CMN}${IMP}CDSphere2QuasiPlate.cpp

${OBJ}CDSphere2Sphere.o: ${CMN}${IMP}CDSphere2Sphere.cpp
	${CC} -o ${OBJ}CDSphere2Sphere.o\
        ${CC_OPTION} -c ${CMN}${IMP}CDSphere2Sphere.cpp

${OBJ}CDSphere2SphereAT.o: ${CMN}${IMP}CDSphere2SphereAT.cpp
	${CC} -o ${OBJ}CDSphere2SphereAT.o\
        ${CC_OPTION} -c ${CMN}${IMP}CDSphere2SphereAT.cpp

${OBJ}CDPolyhedra2Polyhedra.o: ${CMN}${IMP}CDPolyhedra2Polyhedra.cpp
	${CC} -o ${OBJ}CDPolyhedra2Polyhedra.o\
        ${CC_OPTION} -c ${CMN}${IMP}CDPolyhedra2Polyhedra.cpp

${OBJ}DOFixedQuasiCylinder.o: ${CMN}${IMP}DOFixedQuasiCylinder.cpp 
	${CC} -o ${OBJ}DOFixedQuasiCylinder.o\
        ${CC_OPTION} -c ${CMN}${IMP}DOFixedQuasiCylinder.cpp

${OBJ}DOFixedQuasiPlate.o: ${CMN}${IMP}DOFixedQuasiPlate.cpp 
	${CC} -o ${OBJ}DOFixedQuasiPlate.o\
        ${CC_OPTION} -c ${CMN}${IMP}DOFixedQuasiPlate.cpp

${OBJ}DOMap.o: ${CMN}${IMP}DOMap.cpp 
	${CC} -o ${OBJ}DOMap.o\
        ${CC_OPTION} -c ${CMN}${IMP}DOMap.cpp

${OBJ}DOSphere.o: ${CMN}${IMP}DOSphere.cpp 
	${CC} -o ${OBJ}DOSphere.o\
        ${CC_OPTION} -c ${CMN}${IMP}DOSphere.cpp

${OBJ}DOPolyhedra.o: ${CMN}${IMP}DOPolyhedra.cpp
	${CC} -o ${OBJ}DOPolyhedra.o\
        ${CC_OPTION} -c ${CMN}${IMP}DOPolyhedra.cpp

${OBJ}DOFixedPolyhedra.o: ${CMN}${IMP}DOFixedPolyhedra.cpp
	${CC} -o ${OBJ}DOFixedPolyhedra.o\
        ${CC_OPTION} -c ${CMN}${IMP}DOFixedPolyhedra.cpp

${OBJ}ISwLSD.o: ${CMN}${IMP}ISwLSD.cpp 
	${CC} -o ${OBJ}ISwLSD.o\
        ${CC_OPTION} -c ${CMN}${IMP}ISwLSD.cpp

${OBJ}ISwSSD.o: ${CMN}${IMP}ISwSSD.cpp 
	${CC} -o ${OBJ}ISwSSD.o\
        ${CC_OPTION} -c ${CMN}${IMP}ISwSSD.cpp

${OBJ}ISwSSDB.o: ${CMN}${IMP}ISwSSDB.cpp
	${CC} -o ${OBJ}ISwSSDB.o\
        ${CC_OPTION} -c ${CMN}${IMP}ISwSSDB.cpp

${OBJ}ISwSSDBF.o: ${CMN}${IMP}ISwSSDBF.cpp 
	${CC} -o ${OBJ}ISwSSDBF.o\
        ${CC_OPTION} -c ${CMN}${IMP}ISwSSDBF.cpp

${OBJ}ISwSSDF.o: ${CMN}${IMP}ISwSSDF.cpp
	${CC} -o ${OBJ}ISwSSDF.o\
        ${CC_OPTION} -c ${CMN}${IMP}ISwSSDF.cpp

${OBJ}ISwLSDAT.o: ${CMN}${IMP}ISwLSDAT.cpp
	${CC} -o ${OBJ}ISwLSDAT.o\
        ${CC_OPTION} -c ${CMN}${IMP}ISwLSDAT.cpp

${OBJ}ISwSSDFpoly.o: ${CMN}${IMP}ISwSSDFpoly.cpp
	${CC} -o ${OBJ}ISwSSDFpoly.o\
        ${CC_OPTION} -c ${CMN}${IMP}ISwSSDFpoly.cpp

${OBJ}LeapConsultant.o: ${CMN}${IMP}LeapConsultant.cpp 
	${CC} -o ${OBJ}LeapConsultant.o\
        ${CC_OPTION} -c ${CMN}${IMP}LeapConsultant.cpp

${OBJ}NearConsultant.o: ${CMN}${IMP}NearConsultant.cpp 
	${CC} -o ${OBJ}NearConsultant.o\
        ${CC_OPTION} -c ${CMN}${IMP}NearConsultant.cpp


${OBJ}LeapConsultantPoly.o: ${CMN}${IMP}LeapConsultantPoly.cpp 
	${CC} -o ${OBJ}LeapConsultantPoly.o\
        ${CC_OPTION} -c ${CMN}${IMP}LeapConsultantPoly.cpp

${OBJ}SafeConsultant.o: ${CMN}${IMP}SafeConsultant.cpp 
	${CC} -o ${OBJ}SafeConsultant.o\
        ${CC_OPTION} -c ${CMN}${IMP}SafeConsultant.cpp



#
# sccpack
#
sccpack: ${LIB}libsccpack.a

${LIB}libsccpack.a: ${OBJ}AllPackingState.o\
	${OBJ}BasketPackingManager.o ${OBJ}ComplexObject.o\
	${OBJ}ConsultantDecorator.o\
	${OBJ}CutoutConsultantDecorator.o ${OBJ}DOCluster.o\
	${OBJ}DOWorldChangeEventManager.o ${OBJ}LBox.o\
	${OBJ}LBoxPackingManager.o\
	${OBJ}PackingConsultantDecorator.o ${OBJ}PackingManager.o\
	${OBJ}SlumpPackingManager.o ${OBJ}VFPackingManager.o
	

	${AR} -r ${LIB}libsccpack.a ${OBJ}AllPackingState.o\
	${OBJ}BasketPackingManager.o ${OBJ}ComplexObject.o\
	${OBJ}ConsultantDecorator.o\
	${OBJ}CutoutConsultantDecorator.o ${OBJ}DOCluster.o\
	${OBJ}DOWorldChangeEventManager.o ${OBJ}LBox.o\
	${OBJ}LBoxPackingManager.o\
	${OBJ}PackingConsultantDecorator.o ${OBJ}PackingManager.o\
	${OBJ}SlumpPackingManager.o ${OBJ}VFPackingManager.o 

${OBJ}AllPackingState.o: ${SCP}${IMP}AllPackingState.cpp 
	${CC} -o ${OBJ}AllPackingState.o\
        ${CC_OPTION} -c ${SCP}${IMP}AllPackingState.cpp

${OBJ}BasketPackingManager.o: ${SCP}${IMP}BasketPackingManager.cpp 
	${CC} -o ${OBJ}BasketPackingManager.o\
        ${CC_OPTION} -c ${SCP}${IMP}BasketPackingManager.cpp

${OBJ}ComplexObject.o: ${SCP}${IMP}ComplexObject.cpp 
	${CC} -o ${OBJ}ComplexObject.o\
        ${CC_OPTION} -c ${SCP}${IMP}ComplexObject.cpp

${OBJ}ConsultantDecorator.o: ${SCP}${IMP}ConsultantDecorator.cpp 
	${CC} -o ${OBJ}ConsultantDecorator.o\
        ${CC_OPTION} -c ${SCP}${IMP}ConsultantDecorator.cpp

${OBJ}CutoutConsultantDecorator.o: ${SCP}${IMP}CutoutConsultantDecorator.cpp
	${CC} -o ${OBJ}CutoutConsultantDecorator.o\
        ${CC_OPTION} -c ${SCP}${IMP}CutoutConsultantDecorator.cpp

${OBJ}DOCluster.o: ${SCP}${IMP}DOCluster.cpp 
	${CC} -o ${OBJ}DOCluster.o\
        ${CC_OPTION} -c ${SCP}${IMP}DOCluster.cpp

${OBJ}DOWorldChangeEventManager.o:${SCP}${IMP}DOWorldChangeEventManager.cpp 
	${CC} -o ${OBJ}DOWorldChangeEventManager.o\
        ${CC_OPTION} -c ${SCP}${IMP}DOWorldChangeEventManager.cpp

${OBJ}LBox.o: ${SCP}${IMP}LBox.cpp 
	${CC} -o ${OBJ}LBox.o\
        ${CC_OPTION} -c ${SCP}${IMP}LBox.cpp

${OBJ}LBoxPackingManager.o: ${SCP}${IMP}LBoxPackingManager.cpp
	${CC} -o ${OBJ}LBoxPackingManager.o\
        ${CC_OPTION} -c ${SCP}${IMP}LBoxPackingManager.cpp

${OBJ}PackingConsultantDecorator.o: ${SCP}${IMP}PackingConsultantDecorator.cpp 
	${CC} -o ${OBJ}PackingConsultantDecorator.o\
        ${CC_OPTION} -c ${SCP}${IMP}PackingConsultantDecorator.cpp

${OBJ}PackingManager.o: ${SCP}${IMP}PackingManager.cpp 
	${CC} -o ${OBJ}PackingManager.o\
        ${CC_OPTION} -c ${SCP}${IMP}PackingManager.cpp

${OBJ}SlumpPackingManager.o: ${SCP}${IMP}SlumpPackingManager.cpp
	${CC} -o ${OBJ}SlumpPackingManager.o\
        ${CC_OPTION} -c ${SCP}${IMP}SlumpPackingManager.cpp

${OBJ}VFPackingManager.o: ${SCP}${IMP}VFPackingManager.cpp
	${CC} -o ${OBJ}VFPackingManager.o\
        ${CC_OPTION} -c ${SCP}${IMP}VFPackingManager.cpp  

       
        

#
# iris Program
#

iris: ${BUILD}iris

${BUILD}iris: libs ${OBJ}Iris.o ${OBJ}DOWorldSorter.o $(OBJ)ParallelConsultant.o $(OBJ)MPIAssistant.o
	${mLNK} -o ${BUILD}iris ${OBJ}Iris.o ${OBJ}DOWorldSorter.o $(OBJ)ParallelConsultant.o $(OBJ)MPIAssistant.o ${LNK_OPTION}

${OBJ}Iris.o: ${IRIS}$(IMP)IrisNCL.cpp
	${mCC} -o ${OBJ}Iris.o\
        ${CC_OPTION} -c ${IRIS}$(IMP)IrisNCL.cpp	

${OBJ}ParallelConsultant.o: ${IRIS}$(IMP)ParallelConsultant.cpp
	${mCC} -o ${OBJ}ParallelConsultant.o\
        ${CC_OPTION} -c ${IRIS}$(IMP)ParallelConsultant.cpp
        
${OBJ}MPIAssistant.o: ${IRIS}$(IMP)MPIAssistant.cpp
	${mCC} -o ${OBJ}MPIAssistant.o\
        ${CC_OPTION} -c ${IRIS}$(IMP)MPIAssistant.cpp
        
${OBJ}DOWorldSorter.o: ${IRIS}$(IMP)DOWorldSorter.cpp
	${CC} -o ${OBJ}DOWorldSorter.o\
        ${CC_OPTION} -c ${IRIS}$(IMP)DOWorldSorter.cpp


#
# Anne_w_Pack Program
#

Anne_w_Pack: ${BUILD}annep

${BUILD}annep:	libs ${OBJ}Anne_w_Pack.o
	${LNK} -o ${BUILD}annep ${OBJ}Anne_w_Pack.o ${LNK_OPTION} -lsccpack

${OBJ}Anne_w_Pack.o: ${AWP}Anne_w_Pack.cpp
	${CC} -o ${OBJ}Anne_w_Pack.o ${CC_OPTION} -c ${AWP}Anne_w_Pack.cpp	


#
# idoedit Program
#

idoedit: ${BUILD}idoedit

${BUILD}idoedit: libs ${OBJ}IDOEditClient.o 
	${LNK} -o ${BUILD}idoedit ${OBJ}IDOEditClient.o ${LNK_OPTION}

${OBJ}IDOEditClient.o: ${IDOEdit}IDOEditClient.cpp
	${CC} -o ${OBJ}IDOEditClient.o\
        ${CC_OPTION} -c ${IDOEdit}IDOEditClient.cpp	


#
# idoinfo Program
#

idoinfo: ${BUILD}idoinfo

${BUILD}idoinfo: libs ${OBJ}IDOInfoClient.o 
	${LNK} -o ${BUILD}idoinfo ${OBJ}IDOInfoClient.o ${LNK_OPTION}

${OBJ}IDOInfoClient.o: ${IDOInfo}IDOInfoClient.cpp
	${CC} -o ${OBJ}IDOInfoClient.o\
        ${CC_OPTION} -c ${IDOInfo}IDOInfoClient.cpp	

	
#
# packipt 
#
packipt: ${BUILD}packipt

${BUILD}packipt: libs ${OBJ}WeightDistribution.o ${OBJ}VFReader.o\
                ${OBJ}PackIptClient.o
	${LNK} -o ${BUILD}packipt ${OBJ}WeightDistribution.o ${OBJ}VFReader.o\
                  ${OBJ}PackIptClient.o\
                ${LNK_OPTION} -lsccpack

${OBJ}WeightDistribution.o: ${PKT}${IMP}/WeightDistribution.cpp
	${CC} -o ${OBJ}WeightDistribution.o\
        ${CC_OPTION} -c ${PKT}${IMP}/WeightDistribution.cpp	

${OBJ}VFReader.o: ${PKT}${IMP}/VFReader.cpp
	${CC} -o ${OBJ}VFReader.o\
        ${CC_OPTION} -c ${PKT}${IMP}/VFReader.cpp	

${OBJ}PackIptClient.o: ${PKT}${IMP}/PackIptClient.cpp
	${CC} -o ${OBJ}PackIptClient.o\
        ${CC_OPTION} -c ${PKT}${IMP}/PackIptClient.cpp	


#
# cutido Program
#

cutido: ${BUILD}cutido

${BUILD}cutido: libs ${OBJ}CutIDOClient.o 
	${LNK} -o ${BUILD}cutido ${OBJ}CutIDOClient.o ${LNK_OPTION}

${OBJ}CutIDOClient.o: ${CUT}CutIDOClient.cpp
	${CC} -o ${OBJ}CutIDOClient.o\
        ${CC_OPTION} -c ${CUT}CutIDOClient.cpp	


#
# Make necessary directories for build
#
BuildingDir:
	mkdir ${BUILD_ROOT}
	mkdir ${BUILD}
	mkdir ${OBJ}
	mkdir ${LIB}

#
# Clean
#
clean:
	rm ${OBJ}*
	rm ${LIB}*
	rm ${BUILD}iris
	rm ${BUILD}anne
	rm ${BUILD}knight	
	rm ${BUILD}idoedit
	rm ${BUILD}idoinfo	
