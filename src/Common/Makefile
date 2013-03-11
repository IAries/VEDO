include ../../Makefile.in

TMP=${patsubst ${VEDO_SRC}%,${VEDO_OBJ}%,${PWD}}/

all: ${VEDO_LIB}libVEDO-Common.a

${VEDO_LIB}libVEDO-Common.a: ${TMP}CDSphere2QuasiCylinder.o     \
                             ${TMP}CDSphere2QuasiCylinderAT.o   \
                             ${TMP}CDSphere2QuasiPlate.o        \
                             ${TMP}CDSphere2Sphere.o            \
                             ${TMP}CDSphere2SphereAT.o          \
                             ${TMP}DOConstrainedQuasiCylinder.o \
                             ${TMP}DOConstrainedQuasiPlate.o    \
                             ${TMP}DOFixedQuasiCylinder.o       \
                             ${TMP}DOFixedQuasiPlate.o          \
                             ${TMP}DOFixedSphere.o              \
                             ${TMP}DOQuasiCylinder.o            \
                             ${TMP}DOQuasiPlate.o               \
                             ${TMP}DOSphere.o                   \
                             ${TMP}GSComplexShape.o             \
                             ${TMP}GSCylinder.o                 \
                             ${TMP}GSEllipsoid.o                \
                             ${TMP}GSRectangle.o                \
                             ${TMP}GSSphere.o                   \
                             ${TMP}ISwBSDBF.o                   \
                             ${TMP}ISwBtSDBF.o                  \
                             ${TMP}ISwH.o                       \
                             ${TMP}ISwHWS.o                     \
                             ${TMP}ISwTest1.o                   \
                             ${TMP}ISwTest2.o                   \
                             ${TMP}LeapConsultant.o             \
                             ${TMP}LeapConsultant2.o            \
                             ${TMP}NBSConsultant.o              \
                             ${TMP}NearConsultant.o             \
                             ${TMP}SafeConsultant.o
	${AR} ${AR_FLAG} $@ $^

${TMP}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<
