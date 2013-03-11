include ../../Makefile.in

TMP=${patsubst ${VEDO_SRC}%,${VEDO_OBJ}%,${PWD}}/

all: ${VEDO_LIB}libVEDO-Framework.a ${VEDO_LIB}libVEDO-FrameworkX.a

${VEDO_LIB}libVEDO-Framework.a: ${TMP}Assembler.o           \
                                ${TMP}Boundary.o            \
                                ${TMP}Boundary2d.o          \
                                ${TMP}CDCFactoryDecorator.o \
                                ${TMP}Consultant.o          \
                                ${TMP}ContactDetector.o     \
                                ${TMP}DiscreteObject.o      \
                                ${TMP}DOCFactoryDecorator.o \
                                ${TMP}DOContainer.o         \
                                ${TMP}DOMap.o               \
                                ${TMP}DOModel.o             \
                                ${TMP}DOStatus.o            \
                                ${TMP}DOWorld.o             \
                                ${TMP}DOWorld_DrawDxf.o     \
                                ${TMP}DOWorld_ReadXML.o     \
                                ${TMP}DOWorld_RWIDO.o       \
                                ${TMP}DOWorld_WriteVPF.o    \
                                ${TMP}DOWorld_WriteXML.o    \
                                ${TMP}GeometricShape.o      \
                                ${TMP}IactContainer.o       \
                                ${TMP}IactModel.o           \
                                ${TMP}IactRecordTab.o       \
                                ${TMP}ImpactSolver.o        \
                                ${TMP}ImpactStatus.o        \
                                ${TMP}Interaction.o         \
                                ${TMP}ISCFactoryDecorator.o \
                                ${TMP}ISwModels.o           \
                                ${TMP}SimMediator.o         \
                                ${TMP}SystemParameter.o
	${AR} ${AR_FLAG} $@ $^

${VEDO_LIB}libVEDO-FrameworkX.a: ${TMP}Assembler.o           \
                                 ${TMP}Boundary.o            \
                                 ${TMP}Boundary2d.o          \
                                 ${TMP}CDCFactoryDecorator.o \
                                 ${TMP}Consultant.o          \
                                 ${TMP}ContactDetector.o     \
                                 ${TMP}DiscreteObject.o      \
                                 ${TMP}DOCFactoryDecorator.o \
                                 ${TMP}DOContainer.o         \
                                 ${TMP}DOMap.o               \
                                 ${TMP}DOModel.o             \
                                 ${TMP}DOStatus.o            \
                                 ${TMP}DOWorld.o             \
                                 ${TMP}DOWorld_DrawDxf.o     \
                                 ${TMP}DOWorld_ReadXML.o     \
                                 ${TMP}DOWorld_RWIDO.o       \
                                 ${TMP}DOWorld_WriteVPF.o    \
                                 ${TMP}DOWorld_WriteXML.o    \
                                 ${TMP}GeometricShape.o      \
                                 ${TMP}IactContainer.o       \
                                 ${TMP}IactModel.o           \
                                 ${TMP}IactRecordTab.o       \
                                 ${TMP}ImpactSolver.o        \
                                 ${TMP}ImpactStatus.o        \
                                 ${TMP}Interaction.o         \
                                 ${TMP}ISCFactoryDecorator.o \
                                 ${TMP}ISwModels.o           \
                                 ${TMP}SimMediator.o         \
                                 ${TMP}SystemParameter.o
	${AR} ${AR_FLAGx} $@ $^

${TMP}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGx} -c $<
