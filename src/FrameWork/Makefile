include ../../Makefile.in

TMP_DEBUG=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_RELEASE}%,${PWD}}/

all: debug release
debug: ${VEDO_LIB}libVEDO-Framework-Debug.a ${VEDO_LIB}libVEDO-FrameworkX-Debug.a
release: ${VEDO_LIB}libVEDO-Framework.a ${VEDO_LIB}libVEDO-FrameworkX.a

${VEDO_LIB}libVEDO-Framework-Debug.a: ${TMP_DEBUG}Assembler.o           \
                                      ${TMP_DEBUG}Boundary.o            \
                                      ${TMP_DEBUG}Boundary2d.o          \
                                      ${TMP_DEBUG}CDCFactoryDecorator.o \
                                      ${TMP_DEBUG}Consultant.o          \
                                      ${TMP_DEBUG}ContactDetector.o     \
                                      ${TMP_DEBUG}DiscreteObject.o      \
                                      ${TMP_DEBUG}DOCFactoryDecorator.o \
                                      ${TMP_DEBUG}DOContainer.o         \
                                      ${TMP_DEBUG}DOMap.o               \
                                      ${TMP_DEBUG}DOModel.o             \
                                      ${TMP_DEBUG}DOStatus.o            \
                                      ${TMP_DEBUG}DOWorld.o             \
                                      ${TMP_DEBUG}DOWorld_DrawDxf.o     \
                                      ${TMP_DEBUG}DOWorld_ReadXML.o     \
                                      ${TMP_DEBUG}DOWorld_RWIDO.o       \
                                      ${TMP_DEBUG}DOWorld_WriteVPF.o    \
                                      ${TMP_DEBUG}DOWorld_WriteXML.o    \
                                      ${TMP_DEBUG}GeometricShape.o      \
                                      ${TMP_DEBUG}IactContainer.o       \
                                      ${TMP_DEBUG}IactModel.o           \
                                      ${TMP_DEBUG}IactRecordTab.o       \
                                      ${TMP_DEBUG}ImpactSolver.o        \
                                      ${TMP_DEBUG}ImpactStatus.o        \
                                      ${TMP_DEBUG}Interaction.o         \
                                      ${TMP_DEBUG}ISCFactoryDecorator.o \
                                      ${TMP_DEBUG}ISwModels.o           \
                                      ${TMP_DEBUG}SimMediator.o         \
                                      ${TMP_DEBUG}SystemParameter.o     \
                                      ${TMP_DEBUG}SystemParameter2d.o
	${AR} ${AR_FLAG} $@ $^

${VEDO_LIB}libVEDO-FrameworkX-Debug.a: ${TMP_DEBUG}Assembler.o           \
                                       ${TMP_DEBUG}Boundary.o            \
                                       ${TMP_DEBUG}Boundary2d.o          \
                                       ${TMP_DEBUG}CDCFactoryDecorator.o \
                                       ${TMP_DEBUG}Consultant.o          \
                                       ${TMP_DEBUG}ContactDetector.o     \
                                       ${TMP_DEBUG}DiscreteObject.o      \
                                       ${TMP_DEBUG}DOCFactoryDecorator.o \
                                       ${TMP_DEBUG}DOContainer.o         \
                                       ${TMP_DEBUG}DOMap.o               \
                                       ${TMP_DEBUG}DOModel.o             \
                                       ${TMP_DEBUG}DOStatus.o            \
                                       ${TMP_DEBUG}DOWorld.o             \
                                       ${TMP_DEBUG}DOWorld_DrawDxf.o     \
                                       ${TMP_DEBUG}DOWorld_ReadXML.o     \
                                       ${TMP_DEBUG}DOWorld_RWIDO.o       \
                                       ${TMP_DEBUG}DOWorld_WriteVPF.o    \
                                       ${TMP_DEBUG}DOWorld_WriteXML.o    \
                                       ${TMP_DEBUG}GeometricShape.o      \
                                       ${TMP_DEBUG}IactContainer.o       \
                                       ${TMP_DEBUG}IactModel.o           \
                                       ${TMP_DEBUG}IactRecordTab.o       \
                                       ${TMP_DEBUG}ImpactSolver.o        \
                                       ${TMP_DEBUG}ImpactStatus.o        \
                                       ${TMP_DEBUG}Interaction.o         \
                                       ${TMP_DEBUG}ISCFactoryDecorator.o \
                                       ${TMP_DEBUG}ISwModels.o           \
                                       ${TMP_DEBUG}SimMediator.o         \
                                       ${TMP_DEBUG}SystemParameter.o     \
                                       ${TMP_DEBUG}SystemParameter2d.o
	${AR} ${AR_FLAGx} $@ $^

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGxd} -c $<

${VEDO_LIB}libVEDO-Framework.a: ${TMP_RELEASE}Assembler.o           \
                                ${TMP_RELEASE}Boundary.o            \
                                ${TMP_RELEASE}Boundary2d.o          \
                                ${TMP_RELEASE}CDCFactoryDecorator.o \
                                ${TMP_RELEASE}Consultant.o          \
                                ${TMP_RELEASE}ContactDetector.o     \
                                ${TMP_RELEASE}DiscreteObject.o      \
                                ${TMP_RELEASE}DOCFactoryDecorator.o \
                                ${TMP_RELEASE}DOContainer.o         \
                                ${TMP_RELEASE}DOMap.o               \
                                ${TMP_RELEASE}DOModel.o             \
                                ${TMP_RELEASE}DOStatus.o            \
                                ${TMP_RELEASE}DOWorld.o             \
                                ${TMP_RELEASE}DOWorld_DrawDxf.o     \
                                ${TMP_RELEASE}DOWorld_ReadXML.o     \
                                ${TMP_RELEASE}DOWorld_RWIDO.o       \
                                ${TMP_RELEASE}DOWorld_WriteVPF.o    \
                                ${TMP_RELEASE}DOWorld_WriteXML.o    \
                                ${TMP_RELEASE}GeometricShape.o      \
                                ${TMP_RELEASE}IactContainer.o       \
                                ${TMP_RELEASE}IactModel.o           \
                                ${TMP_RELEASE}IactRecordTab.o       \
                                ${TMP_RELEASE}ImpactSolver.o        \
                                ${TMP_RELEASE}ImpactStatus.o        \
                                ${TMP_RELEASE}Interaction.o         \
                                ${TMP_RELEASE}ISCFactoryDecorator.o \
                                ${TMP_RELEASE}ISwModels.o           \
                                ${TMP_RELEASE}SimMediator.o         \
                                ${TMP_RELEASE}SystemParameter.o     \
                                ${TMP_RELEASE}SystemParameter2d.o
	${AR} ${AR_FLAG} $@ $^

${VEDO_LIB}libVEDO-FrameworkX.a: ${TMP_RELEASE}Assembler.o           \
                                 ${TMP_RELEASE}Boundary.o            \
                                 ${TMP_RELEASE}Boundary2d.o          \
                                 ${TMP_RELEASE}CDCFactoryDecorator.o \
                                 ${TMP_RELEASE}Consultant.o          \
                                 ${TMP_RELEASE}ContactDetector.o     \
                                 ${TMP_RELEASE}DiscreteObject.o      \
                                 ${TMP_RELEASE}DOCFactoryDecorator.o \
                                 ${TMP_RELEASE}DOContainer.o         \
                                 ${TMP_RELEASE}DOMap.o               \
                                 ${TMP_RELEASE}DOModel.o             \
                                 ${TMP_RELEASE}DOStatus.o            \
                                 ${TMP_RELEASE}DOWorld.o             \
                                 ${TMP_RELEASE}DOWorld_DrawDxf.o     \
                                 ${TMP_RELEASE}DOWorld_ReadXML.o     \
                                 ${TMP_RELEASE}DOWorld_RWIDO.o       \
                                 ${TMP_RELEASE}DOWorld_WriteVPF.o    \
                                 ${TMP_RELEASE}DOWorld_WriteXML.o    \
                                 ${TMP_RELEASE}GeometricShape.o      \
                                 ${TMP_RELEASE}IactContainer.o       \
                                 ${TMP_RELEASE}IactModel.o           \
                                 ${TMP_RELEASE}IactRecordTab.o       \
                                 ${TMP_RELEASE}ImpactSolver.o        \
                                 ${TMP_RELEASE}ImpactStatus.o        \
                                 ${TMP_RELEASE}Interaction.o         \
                                 ${TMP_RELEASE}ISCFactoryDecorator.o \
                                 ${TMP_RELEASE}ISwModels.o           \
                                 ${TMP_RELEASE}SimMediator.o         \
                                 ${TMP_RELEASE}SystemParameter.o     \
                                 ${TMP_RELEASE}SystemParameter2d.o
	${AR} ${AR_FLAGx} $@ $^

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGx} -c $<
