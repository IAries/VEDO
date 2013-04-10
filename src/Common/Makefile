include ../../Makefile.in

TMP_DEBUG=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_DEBUG}%,${PWD}}/
TMP_RELEASE=${patsubst ${VEDO_SRC}%,${VEDO_OBJ_RELEASE}%,${PWD}}/

all: debug release
debug: ${VEDO_LIB}libvedo-common-debug.a
release: ${VEDO_LIB}libvedo-common.a

${VEDO_LIB}libvedo-common-debug.a: ${TMP_DEBUG}CDSphere2QuasiCylinder.o     \
                                   ${TMP_DEBUG}CDSphere2QuasiCylinderAT.o   \
                                   ${TMP_DEBUG}CDSphere2QuasiPlate.o        \
                                   ${TMP_DEBUG}CDSphere2Sphere.o            \
                                   ${TMP_DEBUG}CDSphere2SphereAT.o          \
                                   ${TMP_DEBUG}DOConstrainedQuasiCylinder.o \
                                   ${TMP_DEBUG}DOConstrainedQuasiPlate.o    \
                                   ${TMP_DEBUG}DOFixedQuasiCylinder.o       \
                                   ${TMP_DEBUG}DOFixedQuasiPlate.o          \
                                   ${TMP_DEBUG}DOFixedSphere.o              \
                                   ${TMP_DEBUG}DOQuasiCylinder.o            \
                                   ${TMP_DEBUG}DOQuasiPlate.o               \
                                   ${TMP_DEBUG}DOSphere.o                   \
                                   ${TMP_DEBUG}GSComplexShape.o             \
                                   ${TMP_DEBUG}GSCylinder.o                 \
                                   ${TMP_DEBUG}GSEllipsoid.o                \
                                   ${TMP_DEBUG}GSRectangle.o                \
                                   ${TMP_DEBUG}GSSphere.o                   \
                                   ${TMP_DEBUG}ISwBSDBF.o                   \
                                   ${TMP_DEBUG}ISwBtSDBF.o                  \
                                   ${TMP_DEBUG}LeapConsultant.o             \
                                   ${TMP_DEBUG}NBSConsultant.o              \
                                   ${TMP_DEBUG}NearConsultant.o             \
                                   ${TMP_DEBUG}SafeConsultant.o
	${AR} ${AR_FLAG} $@ $^

${TMP_DEBUG}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAGd} -c $<

${VEDO_LIB}libvedo-common.a: ${TMP_RELEASE}CDSphere2QuasiCylinder.o     \
                             ${TMP_RELEASE}CDSphere2QuasiCylinderAT.o   \
                             ${TMP_RELEASE}CDSphere2QuasiPlate.o        \
                             ${TMP_RELEASE}CDSphere2Sphere.o            \
                             ${TMP_RELEASE}CDSphere2SphereAT.o          \
                             ${TMP_RELEASE}DOConstrainedQuasiCylinder.o \
                             ${TMP_RELEASE}DOConstrainedQuasiPlate.o    \
                             ${TMP_RELEASE}DOFixedQuasiCylinder.o       \
                             ${TMP_RELEASE}DOFixedQuasiPlate.o          \
                             ${TMP_RELEASE}DOFixedSphere.o              \
                             ${TMP_RELEASE}DOQuasiCylinder.o            \
                             ${TMP_RELEASE}DOQuasiPlate.o               \
                             ${TMP_RELEASE}DOSphere.o                   \
                             ${TMP_RELEASE}GSComplexShape.o             \
                             ${TMP_RELEASE}GSCylinder.o                 \
                             ${TMP_RELEASE}GSEllipsoid.o                \
                             ${TMP_RELEASE}GSRectangle.o                \
                             ${TMP_RELEASE}GSSphere.o                   \
                             ${TMP_RELEASE}ISwBSDBF.o                   \
                             ${TMP_RELEASE}ISwBtSDBF.o                  \
                             ${TMP_RELEASE}LeapConsultant.o             \
                             ${TMP_RELEASE}NBSConsultant.o              \
                             ${TMP_RELEASE}NearConsultant.o             \
                             ${TMP_RELEASE}SafeConsultant.o
	${AR} ${AR_FLAG} $@ $^

${TMP_RELEASE}%.o: ${IMP}%.cpp
	${CXX} -o $@ ${CXX_FLAG} -c $<
