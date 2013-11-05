#include <NJR/Interfaces/Utility.h>
#include <NJR/Interfaces/Vector3d.h>
#include <Framework/Interfaces/Boundary.h>
#include <Framework/Interfaces/Constants.h>
#include <Framework/Interfaces/Consultant.h>
#include <Framework/Interfaces/DOWorld.h>
#include <Framework/Interfaces/DOWorld_WriteVTK.h>
#include <Framework/Interfaces/GeometricShape.h>
#include <Framework/Interfaces/IactRecordTab.h>
#include <Common/Interfaces/GSComplexShape.h>
#include <Common/Interfaces/GSCylinder.h>
#include <Common/Interfaces/GSRectangle.h>
#include <Common/Interfaces/GSSphere.h>
#include <Common/Interfaces/GSEllipsoid.h>
#include <Knight/Interfaces/CalcDensity.h>
#include <Knight/Interfaces/CalcCoordinateNum.h>
#include <Knight/Interfaces/RectangularClusterInitializer.h>
#include <Knight/Interfaces/3DLattices.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

void usage()
{
	std::cout
		<< "Knight " << vedo::sVersion << std::endl
		<< std::endl
		<< "Usage:" << std::endl
		<< std::endl
		<< "	-a: add elements in defined domain" << std::endl
		<< "	Knight -a <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-ap: add potential energy from a \"source point\"" << std::endl
		<< "	Knight -ap <DOName> (source point)<x> <y> <z>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-aecp: add elements in a circular path (X-Y plane)"                      << std::endl
		<< "	Knight -aecp <DOName>"                                                   << std::endl
		<< "                <started angle (degree)> <ended angle (degree)>"             << std::endl
		<< "                <number of elements>"                                        << std::endl
		<< "                <radius of circular path>"                                   << std::endl
		<< "                <coordinate Z of the circular path>"                         << std::endl
		<< "                (Original)<.xml | .ido> <.xml | .ido>"
		<< std::endl
		<< "	-ase: add single element"                                                << std::endl
		<< "	Knight -ase <DOName>"                                                    << std::endl
		<< "                <position-x> <position-y> <position-z>"                      << std::endl
		<< "                <velocity-x> <velocity-y> <velocity-z>"                      << std::endl
		<< "                <orientation-x-x> <orientation-x-y> <orientation-x-z>"       << std::endl
		<< "                <orientation-z-x> <orientation-z-y> <orientation-z-z>"       << std::endl
		<< "                <angularvelocity-x> <angularvelocity-y> <angularvelocity-z>" << std::endl
		<< "                (Original)<.xml | .ido> <.xml | .ido>"
		<< std::endl
		<< "	-av: add velocity" << std::endl
		<< "	Knight -av <DOName> (velocity)<x> <y> <z>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-aav: add angular velocity" << std::endl
		<< "	Knight -av <DOName> (angular velocity)<x> <y> <z>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-ar_cartesian: add elements (random distribution) in defined domain (cartesian coordinate)" << std::endl
		<< "	Knight -ar_cartesian <DOName> <number of elements> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-ar_cylindrical: add elements (random distribution) in defined domain (cylindrical coordinate)" << std::endl
		<< "	Knight -ar_cylindrical <DOName> <number of elements> <x> <y> <z> <r> <h>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-ar_spherical: add elements (random distribution) in defined domain (spherical coordinate)" << std::endl
		<< "	Knight -ar_spherical <DOName> <number of elements> <x> <y> <z> <r>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
//		<< "	-assm: assemble xml file (-al)" << std::endl
//		<< "	Knight -assm <f1.xml> <f2.xml> <des.xml>" << std::endl
//		<< std::endl
		<< "	-avg_info: calculate the average information in defined space" << std::endl
		<< "	Knight -avg_info <.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-avg_info: calculate the average information in defined space (velocity is estimated by positions in different step)" << std::endl
		<< "	Knight -avg_info (current step)<.xml | .ido> (next step)<.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-avg_info_xy: calculate the average information in defined x-y plane" << std::endl
		<< "	Knight -avg_info_xy <.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-avg_info_xy: calculate the average information in defined x-y plane (velocity is estimated by positions in different step)" << std::endl
		<< "	Knight -avg_info_xy (current step)<.xml | .ido> (next step)<.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-avg_info_yz: calculate the average information in defined y-z plane" << std::endl
		<< "	Knight -avg_info_yz <.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-avg_info_yz: calculate the average information in defined y-z plane (velocity is estimated by positions in different step)" << std::endl
		<< "	Knight -avg_info_yz (current step)<.xml | .ido> (next step)<.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-avg_info_xz: calculate the average information in defined x-z plane" << std::endl
		<< "	Knight -avg_info_xz <.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-avg_info_xz: calculate the average information in defined x-z plane (velocity is estimated by positions in different step)" << std::endl
		<< "	Knight -avg_info_xz(current step)<.xml | .ido> (next step)<.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-avg_rd_ntume: calculate the average information for NTUME experiment of rotating drum" << std::endl
		<< "	Knight -avg_rd_ntume <.xml | .ido> <.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax> <Mesh size>" << std::endl
		<< std::endl
		<< "	-c: convert" << std::endl
		<< "	Knight -c <.xml | .ido> <.xml | .ido | .csv | .vpf | .dxf | .vtu>" << std::endl
		<< std::endl
		<< "	-ca2s: calaulate area of cross section to surface, ax+by+cz=d" << std::endl
		<< "	Knight -ca2s <DOName> <a> <b> <c> <d> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-cf2s: calaulate flux of cross section to surface, ax+by+cz=d" << std::endl
		<< "	Knight -cf2s <DOName> <a> <b> <c> <d> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-cal: calculate average density and coordinate number" << std::endl
		<< "	Knight -cal <filename> <xmin,xmax,ymin,ymax,zmin,zmax>" << std::endl
		<< std::endl
		<< "	-cl: convert std::list (Hexadecimal)" << std::endl
		<< "	Knight -cl <filename> <begin num> <the end>" << std::endl
		<< std::endl
 		<< "	-clean_udv: clean the user-defined value in ido file" << std::endl
		<< "	Knight -clean_udv (Original)<.ido> (New)<.ido>" << std::endl
		<< std::endl
		<< "	-cld: convert std::list (Decimalism)" << std::endl
		<< "	Knight -cld <filename> <begin num> <the end>" << std::endl
		<< std::endl
		<< "	-cm: change models" << std::endl
		<< "	Knight -cm (Reference)<.xml | .ido> <.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-comb: combine two models" << std::endl
		<< "	Knight -comb (Reference)<.xml | .ido> (with Mobile Elements)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-comb: combine two models" << std::endl
		<< "	Knight -comb (Reference)<.xml | .ido> (with Added Elements)<.xml | .ido> <DOName> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-coord_num: calculate coordinate number for spheres and output to VTK file" << std::endl
		<< "	Knight -coord_num <.xml | .ido>  <.vtu>" << std::endl
		<< std::endl
		<< "	-cu: convert to sudo vpf file" << std::endl
		<< "	Knight -cu (Original)<.xml | .ido> (Current)<.xml | .ido> <.vpf>" << std::endl
		<< std::endl
		<< "	-b2h: convert DOS text file to HEX" << std::endl
		<< "	Knight -b2h <origin> <destination>" << std::endl
		<< std::endl
		<< "	-d2u: convert DOS text file to UNIX text" << std::endl
		<< "	Knight -d2u <origin> <destination>" << std::endl
		<< std::endl
		<< "	-distance: calculate the distance of objects from certain point" << std::endl
		<< "	Knight -distance <.xml | .ido> <X> <Y> <Z>" << std::endl
		<< std::endl
		<< "	-distance_xy: calculate the distance of objects from certain line" << std::endl
		<< "	Knight -distance <.xml | .ido> <X> <ZY>" << std::endl
		<< std::endl
		<< "	-distance_yz: calculate the distance of objects from certain line" << std::endl
		<< "	Knight -distance <.xml | .ido> <Y> <Z>" << std::endl
		<< std::endl
		<< "	-distance_zx: calculate the distance of objects from certain line" << std::endl
		<< "	Knight -distance <.xml | .ido> <Z> <X>" << std::endl
		<< std::endl
		<< "	-u2d: convert UNIX text file to DOS text" << std::endl
		<< "	Knight -u2d <origin> <destination>" << std::endl
		<< std::endl
		<< "	-cse: clear separated elements" << std::endl
		<< "	Knight -cs <.ido> <.ido>" << std::endl
		<< std::endl
		<< "	-distribution: determine the distribution of certain elements" << std::endl
		<< "	Knight -distribution <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			<Mesh size> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-e: erase elements" << std::endl
		<< "	Knight -e <DOName> <.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-erase_single_element: erase single element" << std::endl
		<< "	Knight -erase_single_element <ID> <.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-erase_elements: erase element in space ax+by+cz>d \"" << std::endl
		<< "	Knight -erase_elements <name of element> <a> <b> <c> <d> <.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-energy: calculate the energy of system" << std::endl
		<< "	Knight -energy <.ido>" << std::endl
		<< std::endl
		<< "	-freeze: freeze all \"Elements\"" << std::endl
		<< "	Knight -freeze <.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-freeze: freeze \"Elements\"" << std::endl
		<< "	Knight -freeze <DOName> <.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-gt_auto: calculate the granular temperature in defined domain (auto/rectangle shape)" << std::endl
		<< "	Knight -gt_auto <.xml | .ido> <DOName> <CutNumberX> <CutNumberY> <CutNumberZ>" << std::endl
		<< "			<Angle2XAxis> <Angle2YAxis> <Angle2ZAxis>" << std::endl
		<< std::endl
		<< "	-gt_rectangle: calculate the granular temperature in defined domain (rectangle shape)" << std::endl
		<< "	Knight -gt_rectangle <.xml | .ido> <DOName> <width> <length> <height> <X> <Y> <Z>" << std::endl
		<< "			<Xx> <Xy> <Xz> <Zx> <Zy> <Zz>" << std::endl
		<< std::endl
		<< "	-gt_cylinder: calculate the granular temperature in defined domain (culinder shape)" << std::endl
		<< "	Knight -gt_cylinder <.xml | .ido> <DOName> <radius> <height> <X> <Y> <Z>" << std::endl
		<< "			<Xx> <Xy> <Xz> <Zx> <Zy> <Zz>" << std::endl
		<< std::endl
		<< "	-gt_sphere: calculate the granular temperature in defined domain (sphere shape)" << std::endl
		<< "	Knight -gt_sphere <.xml | .ido> <DOName> <radius> <X> <Y> <Z>" << std::endl
		<< "			<Xx> <Xy> <Xz> <Zx> <Zy> <Zz>" << std::endl
		<< std::endl
		<< "	-gt_ellipsoid: calculate the granular temperature in defined domain (ellipsoid shape)" << std::endl
		<< "	Knight -gt_ellipsoid <.xml | .ido> <DOName> <XLength> <YLength> <ZLength> <X> <Y> <Z>" << std::endl
		<< "			<Xx> <Xy> <Xz> <Zx> <Zy> <Zz>" << std::endl
		<< std::endl
		<< "	-i: print infomation of data file" << std::endl
		<< "	Knight -i  <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-inside: check what elements are inside" << std::endl
		<< "	Knight -inside <.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax>" << std::endl
		<< std::endl
		<< "	-lattice_bcc: rectangular element initialization on the basis of BCC Lattice" << std::endl
		<< "	Knight -lattice_bcc <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-lattice_fcc: rectangular element initialization on the basis of FCC Lattice" << std::endl
		<< "	Knight -lattice_fcc <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-lattice_scc: rectangular element initialization on the basis of Simple Cubic Lattice" << std::endl
		<< "	Knight -lattice_scc <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-profile_Info2Y: calculate the profile \"Information-Y\" in defined space" << std::endl
		<< "	Knight -profile_Info2Y <.xml | .ido> <Xmin> <XMax> <Zmin> <Zmax>" << std::endl
		<< std::endl
		<< "	-projected_area_x: calculate the projected area in Y-Z domain" << std::endl
		<< "	Knight -projected_area_x <mesh length> <.ido>" << std::endl
		<< std::endl
		<< "	-projected_area_y: calculate the projected area in Z-X domain" << std::endl
		<< "	Knight -projected_area_y <mesh length> <.ido>" << std::endl
		<< std::endl
		<< "	-projected_area_z: calculate the projected area in X-Y domain" << std::endl
		<< "	Knight -projected_area_z <mesh length> <.ido>" << std::endl
		<< std::endl
		<< "	-rotate: rotate elements" << std::endl
		<< "	Knight -rotate <Angle2XAxis> <Angle2YAxis> <Angle2ZAxis>" << std::endl
		<< "			(Original)<.xml | .ido> (New)<.xml | .ido>" << std::endl
		<< std::endl
		<< "	-sccBm1: calculate the flowability index of SCC mortar" << std::endl
		<< "	Knight -sccBm1 <.ido>" << std::endl
		<< std::endl
		<< "	-sccBm2: calculate the flowability index of SCC" << std::endl
		<< "	Knight -sccBm2 <.ido>" << std::endl
		<< std::endl
		<< "	-sccRm: calculate the viscosity index of SCC mortar" << std::endl
		<< "	Knight -sccRm <.ido>" << std::endl
		<< std::endl
		<< "	-shift: shift elements" << std::endl
		<< "	Knight -shift <DOName> <x> <y> <z>" << std::endl
		<< "			(Original)<.xml | .ido> (New)<.xml | .ido>" << std::endl
		<< std::endl
		<< "	-shift_all: shift all elements" << std::endl
		<< "	Knight -shift_all <x> <y> <z> (Original)<.xml | .ido> (New)<.xml | .ido>" << std::endl
		<< std::endl
		<< "	-sort: sort elements" << std::endl
		<< "	Knight -sort (Original)<.xml | .ido> (New)<.xml | .ido>" << std::endl
		<< std::endl
		<< "	-sort_distance: sort elements by the distance to a reference point" << std::endl
		<< "	Knight -sort_distance <Px> <Py> <Pz> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-set_time: set simulated time" << std::endl
		<< "	Knight -set_time <start time> <stop time> <time interval> <current time> (Original)<.xml | .ido> (New)<.xml | .ido>" << std::endl
		<< std::endl
		<< "	-show: show the motion status of certain object" << std::endl
		<< "	Knight -show <.xml | .ido> <ObjectID>" << std::endl
		<< std::endl
 		<< "	-u11: update ido file & irt file (2011) to new version" << std::endl
		<< "	Knight -u11 (Original)<.ido> (Original)<.irt> (New)<.ido>" << std::endl
		<< std::endl
 		<< "	-u10: update ido file & irt file (2010) to new version" << std::endl
		<< "	Knight -u10 (Original)<.ido> (Original)<.irt> (New)<.ido>" << std::endl
		<< std::endl
 		<< "	-u9: update ido file (2009) to new version" << std::endl
		<< "	Knight -u9 (Original)<.ido> (New)<.ido>" << std::endl
		<< std::endl
 		<< "	-u8: update ido file (2008) to new version" << std::endl
		<< "	Knight -u8 (Original)<.ido> (New)<.ido>" << std::endl
		<< std::endl
 		<< "	-u7: update ido file (2007) to new version" << std::endl
		<< "	Knight -u7 (Original)<.ido> (New)<.ido>" << std::endl
		<< std::endl
		<< "	-u6: update ido file (2006) to new version" << std::endl
		<< "	Knight -u6 (Original)<.ido> (New)<.ido>" << std::endl
		<< std::endl
		<< "	-volume: calculate the volume in 'Zone of Interest'" << std::endl
		<< "	Knight -volume <mesh length> <.ido>" << std::endl
		<< std::endl;

	exit(0);
}

vedo::DOWorld* info(vedo::DOWorld *oWorld)
{
	const vedo::SystemParameter* sp = oWorld->GetSystemParameter();
	std::cout
		<< "<<SystemParameter Infomation>>" << std::endl
		<< "\tTitle             : " << sp->GetTitle() << std::endl
		<< "\tObjNumber         : " << sp->GetDONumber() << std::endl
		<< "\tTime              : "
			<< "start("    << sp->GetTimeStart()    << ") "
			<< "stop("     << sp->GetTimeStop()     << ") "
			<< "interval(" << sp->GetTimeInterval() << ") "
			<< "current("  << sp->GetTimeCurrent()  << ")" << std::endl
		<< "\tSimConstant       : "
			<< "ContactDetectSafetyFactor(" << vedo::dSafetyFactor << ") "
			<< "NumUDDOStatus("             << vedo::uNumUDDDOStatus     << ") "
			<< "NumUDIactStatus("           << vedo::uNumUDDImpactStatus << ")" << std::endl
		<< "\tField Acceleration: " << sp->GetFieldAcceleration()
		<< "\tZOI               : ";

	if(sp->GetZoneOfInterest().GetSwitch(0))
	{
		std::cout
			<< "XMin(" << sp->GetZoneOfInterest().GetLowerPoint().x() << ") "
			<< "XMax(" << sp->GetZoneOfInterest().GetUpperPoint().x() << ") ";
	};

	if(sp->GetZoneOfInterest().GetSwitch(1))
	{
		std::cout
			<< "YMin(" << sp->GetZoneOfInterest().GetLowerPoint().y() << ") "
			<< "YMax(" << sp->GetZoneOfInterest().GetUpperPoint().y() << ") ";
	};

	if(sp->GetZoneOfInterest().GetSwitch(2))
	{
		std::cout
			<< "ZMin(" << sp->GetZoneOfInterest().GetLowerPoint().z() << ") "
			<< "ZMax(" << sp->GetZoneOfInterest().GetUpperPoint().z() << ") ";
	};
	std::cout << std::endl;

	std::cout << "\tPBC               : ";

	if(sp->GetPeriodicBoundaryConditions().GetSwitch(0))
	{
		std::cout
			<< "XMin("
			<< sp->GetPeriodicBoundaryConditions().GetLowerPoint().x()
			<< ") "
			<< "XMax("
			<< sp->GetPeriodicBoundaryConditions().GetUpperPoint().x()
			<< ") ";
	};

	if(sp->GetPeriodicBoundaryConditions().GetSwitch(1))
	{
		std::cout
			<< "YMin("
			<< sp->GetPeriodicBoundaryConditions().GetLowerPoint().y()
			<< ") "
			<< "YMax("
			<< sp->GetPeriodicBoundaryConditions().GetUpperPoint().y()
			<< ") ";
	};

	if(sp->GetPeriodicBoundaryConditions().GetSwitch(2))
	{
		std::cout
			<< "ZMin("
			<< sp->GetPeriodicBoundaryConditions().GetLowerPoint().z()
			<< ") "
			<< "ZMax("
			<< sp->GetPeriodicBoundaryConditions().GetUpperPoint().z()
			<< ") ";
	};
	std::cout << std::endl << std::endl;

	const std::vector<vedo::DOStatus*>& rcDOStatus = oWorld->GetDOStatus();
	std::vector<vedo::DOStatus *>::const_iterator idos;
	std::map<std::string, unsigned long> counter;

	for (idos=rcDOStatus.begin(); idos!=rcDOStatus.end(); idos++)
	{
		counter[(*idos)->GetDOName()]++;
 	}

	std::cout << "<<vedo::DOModel Infomation>>" << std::endl;

	const std::list<vedo::DOModel*>& rcDOModel = oWorld->GetDOModel();
	std::list<vedo::DOModel*>::const_iterator idoml;
	std::string shape;
	for (idoml=rcDOModel.begin(); idoml!=rcDOModel.end(); idoml++)
	{
		switch((*idoml)->GetShapeType())
		{
			case vedo::Sphere:
				shape = "Sphere";
				break;
			case vedo::QuasiPlate:
				shape = "QuasiPlate";
				break;
			case vedo::QuasiPlateWithCircularHole:
				shape = "QuasiPlateWithCircularHole";
				break;
		 	case vedo::QuasiCylinder:
		 		shape = "QuasiCylinder";
		 		break;
			case vedo::Ellipsoid:
				shape = "Ellipsoid";
				break;
			case vedo::Polyhedra:
				shape = "Polyhedra";
				break;
			default:
				shape = "UnKnown";
		}
		std::cout
			<< "\t("
			<< (*idoml)->GetDOName()          << ", "
			<< (*idoml)->GetDOGroup()         << ", "
			<< (*idoml)->GetBehavior()        << ", "
			<< shape                          << ", "
			<< counter[(*idoml)->GetDOName()] << ")" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "<<Interaction Infomation>>" << std::endl;

	const std::list<vedo::IactModel*>& rcIactModel = oWorld->GetIactModel();
	std::list<vedo::IactModel*>::const_iterator iactiter;
	unsigned long i = 0;
	for (iactiter=rcIactModel.begin(); iactiter!=rcIactModel.end(); ++iactiter)
	{
		vedo::IactModel* im = *iactiter;
		std::string eqtype = im->GetEquationType();
		std::cout
			<< '\t'
			<< i++
			<< " : "
			<< im->GetMasterDOGroup().c_str()
			<< " with "
			<< im->GetSlaveDOGroup().c_str()
			<< "  "
			<< eqtype.c_str()
			<< std::endl;
	}
	std::cout << std::endl;
	return oWorld;
};

vedo::DOWorld* ReadDOWorld(std::string filename, vedo::IactRecordTab* irtp)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (njr::CheckSubName(filename,".xml"))
	{
		pWorld->ReadXML(filename.c_str(), irtp);
	}
	else if (njr::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO(filename.c_str(), irtp);
	}
	else
	{
		usage();
	}
	return pWorld;
};

vedo::DOWorld* ReadDOWorld(std::string filename)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (njr::CheckSubName(filename,".xml"))
	{
		pWorld->ReadXML(filename.c_str());
	}
	else if (njr::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO(filename.c_str());
	}
	else
	{
		usage();
	}
	return pWorld;
};

vedo::DOWorld* ReadDOWorld2011(std::string filename)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (njr::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO2011(filename.c_str());
	}
	else
	{
		usage();
	}
	return pWorld;
};

vedo::DOWorld* ReadDOWorld2009(std::string filename)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (njr::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO2009(filename.c_str());
	}
	else
	{
		usage();
	}
	return pWorld;
};

vedo::DOWorld* ReadDOWorld2008(std::string filename)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (njr::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO2008(filename.c_str());
	}
	else
	{
		usage();
	}
	return pWorld;
};


vedo::DOWorld* ReadDOWorld2007(std::string filename)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (njr::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO(filename.c_str(), 2007);
	}
	else
	{
		usage();
	}
	return pWorld;
};

vedo::DOWorld* ReadDOWorld2006(std::string filename)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (njr::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO(filename.c_str(), 2006);
	}
	else
	{
		usage();
	}
	return pWorld;
};

vedo::DOWorld* WriteDOWorld (std::string filename, vedo::DOWorld* pw)
{
	if (njr::CheckSubName(filename, ".xml"))
	{
		pw->WriteXML(filename.c_str());
	}
	else if (njr::CheckSubName(filename, ".ido"))
	{
		pw->WriteIDO(filename.c_str());
	}
	else if (njr::CheckSubName(filename, ".vpf"))
	{
		pw->WriteVPF(filename.c_str());
	}
//	else if (njr::CheckSubName(filename, ".vtu"))
//	{
//		pw->WriteVTK<NullExternalFieldVTKWriter>(filename.c_str());
//	}
	else if (njr::CheckSubName(filename, "_h.dxf"))
	{
		pw->HighDraw(filename.c_str());
	}
	else if (njr::CheckSubName(filename, ".dxf"))
	{
		pw->Draw(filename.c_str());
	}
	else
	{
		usage();
	}
	return pw;
};

vedo::DOWorld* WriteDOWorld (std::string filename, vedo::DOWorld* pw, vedo::IactRecordTab* irtp)
{
	if (njr::CheckSubName(filename, ".xml"))
	{
		pw->WriteXML(filename.c_str(), irtp);
	}
	else if (njr::CheckSubName(filename, ".ido"))
	{
		pw->WriteIDO(filename.c_str(), irtp);
	}
	else if (njr::CheckSubName(filename, ".vpf"))
	{
		pw->WriteVPF(filename.c_str());
	}
//	else if (njr::CheckSubName(filename, ".vtu"))
//	{
//		pw->WriteVTK<NullExternalFieldVTKWriter>(filename.c_str());
//	}
	else if (njr::CheckSubName(filename, "_h.dxf"))
	{
		pw->HighDraw(filename.c_str());
	}
	else if (njr::CheckSubName(filename, ".dxf"))
	{
		pw->Draw(filename.c_str());
	}
	else
	{
		usage();
	}
	return pw;
};

vedo::DOWorld* WriteDOWorldVPF (std::string filename, vedo::DOWorld* opw, vedo::DOWorld* cpw)
{
	if (njr::CheckSubName(filename, ".vpf"))
	{
		cpw->WriteVPF(filename.c_str(), opw);
	}
	else
	{
		usage();
	}
	delete opw;
	return cpw;
};

/*
void assm(std::string& f1, std::string& f2, std::string& des)
{
	vedo::DOWorld *pw1 = ReadDOWorld(f1);
	vedo::DOWorld *pw2 = ReadDOWorld(f2);
	std::vector<vedo::DOStatus*> vcd1 = pw1->GetDOStatus();
	std::list<vedo::DOModel*> cDOModel = pw1->GetDOModel();
	std::list<vedo::IactModel*> cIactModel = pw1->GetIactModel();

	std::vector<vedo::DOStatus*> vcd2 = pw2->GetDOStatus();
	std::vector<vedo::DOStatus*> vcd3(0);

	transform
		(vcd1.begin(), vcd1.end(), back_inserter(vcd3), njr::Copy_obj());
	transform
		(vcd2.begin(), vcd2.end(), back_inserter(vcd3), njr::Copy_obj());

	vedo::SystemParameter *pSystemParameter
		= new vedo::SystemParameter
			("Domino Test Benchmark 4",
			"We Love YOU",
			0.0,
			5.0,
			5e-6,
			0.0,
			vcd3.size(),
			njr::Vector3d(0.0, 0.0, -980.0),
			vedo::Boundary()                    );

	vedo::DOWorld* pw3 = new vedo::DOWorld(pSystemParameter, cDOModel, cIactModel, vcd3);

	pw3->WriteXML(des.c_str());

	delete pw1;
	delete pw2;
	delete pw3;
};
*/

void ConList (std::string filename, unsigned long begin, unsigned long end)
{
	char ltoa[256];
	unsigned long ulRecordCount;
	for (ulRecordCount=begin; ulRecordCount<=end; ulRecordCount++)
	{
		std::string file = "knight -c " + filename + '_';
		std::string out = filename + '_';
		sprintf(ltoa, "%d.ido\0", 10000000 + ulRecordCount);
		file.append((ulRecordCount < 0x10010000) ? ltoa + 4 : ltoa);
		sprintf(ltoa, "%d.vpf\0", 10000000 + ulRecordCount);
		out.append((ulRecordCount < 0x10010000) ? ltoa + 4 : ltoa);
		std::cout << file.c_str() << ' ' << out.c_str() << std::endl;
	}
};

void ConListHEX (std::string filename, unsigned long begin, unsigned long end)
{
	char ltoa[256];
	unsigned long ulRecordCount;
	for (ulRecordCount=begin; ulRecordCount<=end; ulRecordCount++)
	{
		std::string file = "knight -c " + filename + '_';
		std::string out = filename + '_';
		sprintf(ltoa, "%p.ido\0", 0x10000000 + ulRecordCount);
		file.append((ulRecordCount < 0x10010000) ? ltoa + 4 : ltoa);
		sprintf(ltoa, "%p.vpf\0", 0x10000000 + ulRecordCount);
		out.append((ulRecordCount < 0x10010000) ? ltoa + 4 : ltoa);
		std::cout << file.c_str() << ' ' << out.c_str() << std::endl;
	}
};

void AssmListHEX ()
{
	char ltoa[256];
	unsigned long begin  = 0;
	unsigned long end    = 1474;
	std::string filename      = "bs";

	unsigned long ulRecordCount;
	for (ulRecordCount=begin; ulRecordCount<=end; ulRecordCount++)
	{
		std::string file = "knight -assm " + filename + "_A_";
		std::string out  = filename + "_B_";
		std::string des  = filename + "_C_";
		sprintf(ltoa, "%p.xml\0", 0x10000000 + ulRecordCount);
		file.append((ulRecordCount < 0x10010000) ? ltoa + 4 : ltoa);
		sprintf(ltoa, "%p.xml\0", 0x10000000 + ulRecordCount);
		out.append((ulRecordCount < 0x10010000) ? ltoa + 4 : ltoa);
		sprintf(ltoa, "%p.xml\0", 0x10000000 + ulRecordCount);
		des.append((ulRecordCount < 0x10010000) ? ltoa + 4 : ltoa);
		std::cout
			<< file.c_str()
			<< ' '
			<< out.c_str()
			<< ' '
			<< des.c_str()
			<< std::endl;
	}
};

void AddDOInSpace
	(njr::Vector3d* LowerBoundary,
	 njr::Vector3d* UpperBoundary,
	 std::string DOName,
	 vedo::DOWorld* oWorld            )
{
	double R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos
		(DOName,
		njr::ZERO,
		njr::ZERO,
		njr::AXIALX,
		njr::AXIALZ,
		njr::ZERO);
	*LowerBoundary += njr::Vector3d(R, R, R);
	*UpperBoundary -= njr::Vector3d(R, R, R);
	for(double y=(LowerBoundary->y()); y<=(UpperBoundary->y()); y+=(2.0*R))
    {
	    for(double z=(LowerBoundary->z()); z<=(UpperBoundary->z()); z+=(2.0*R))
    	{
		    for
		    	(double x=(LowerBoundary->x());
		    	 x<=(UpperBoundary->x());
		    	 x+=(2.0*R)                    )
    		{
				dos.SetPosition(njr::Vector3d(x, y, z));
				oWorld->AddDOStatus(new vedo::DOStatus(dos));
	    	}
	    }
    }
};

void AddPotentialEnergy
	(njr::Vector3d* SourcePoint,
	 std::string DOName,
	 vedo::DOWorld* oWorld          )
{
	      njr::Vector3d vel;
	const njr::Vector3d ff = oWorld->GetSystemParameter()->GetFieldAcceleration();
	vedo::DOStatus new_dos
		(DOName,
		 njr::ZERO,
		 njr::ZERO,
		 njr::AXIALX,
		 njr::AXIALZ,
		 njr::ZERO);
	const vedo::DOStatus* dos = 0;
    double TempVelocitySquare;
	for(unsigned long ul=0;
        ul < oWorld->GetSystemParameter()->GetDONumber();
        ul++                                             )
	{
		dos = oWorld->GetDOStatus(ul);
		if (dos->GetDOName() == DOName)
		{
			vel = dos->GetVelocity();
			new_dos = *dos;
            TempVelocitySquare
                = vel.Dot(vel)
                + 2.0 * ff.Dot(dos->GetPosition() - *SourcePoint);
            if(TempVelocitySquare >= 0.0)
            {
    			new_dos.SetVelocity(sqrt(TempVelocitySquare) * ff.direction());
            }
            else
            {
    			new_dos.SetVelocity
                    (-1.0 * sqrt(-1.0 * TempVelocitySquare) * ff.direction());
            }
			oWorld->SetDOStatus(ul, new_dos);
		}
	}
};

void AddExternalVelocity
	(njr::Vector3d* pExternalVelocity,
	 std::string DOName,
	 vedo::DOWorld* oWorld                )
{
	njr::Vector3d vel;
	vedo::DOStatus new_dos
		(DOName,
		 njr::ZERO,
		 njr::ZERO,
		 njr::AXIALX,
		 njr::AXIALZ,
		 njr::ZERO);
	const vedo::DOStatus* dos = 0;
	for(unsigned long ul=0;
        ul < oWorld->GetSystemParameter()->GetDONumber();
        ul++                                             )
	{
		dos = oWorld->GetDOStatus(ul);
		if (dos->GetDOName() == DOName)
		{
			vel = dos->GetVelocity() + (*pExternalVelocity);
			new_dos = *dos;
			new_dos.SetVelocity(vel);
			oWorld->SetDOStatus(ul, new_dos);
		}
	}
};

void AddExternalAngularVelocity
	(njr::Vector3d* pExternalAngularVelocity,
	 std::string DOName,
	 vedo::DOWorld* oWorld                       )
{
	njr::Vector3d avel;
	vedo::DOStatus new_dos
		(DOName,
		 njr::ZERO,
		 njr::ZERO,
		 njr::AXIALX,
		 njr::AXIALZ,
		 njr::ZERO);
	const vedo::DOStatus* dos = 0;
	for(unsigned long ul=0;
        ul < oWorld->GetSystemParameter()->GetDONumber();
        ul++                                             )
	{
		dos = oWorld->GetDOStatus(ul);
		if (dos->GetDOName() == DOName)
		{
			avel = dos->GetAngularVelocity() + (*pExternalAngularVelocity);
			new_dos = *dos;
			new_dos.SetAngularVelocity(avel);
			oWorld->SetDOStatus(ul, new_dos);
		}
	}
};

void AddRandomDOInCartesianSpace
	(njr::Vector3d* LowerBoundary,
	 njr::Vector3d* UpperBoundary,
	 std::string DOName,
	 unsigned long* num,
	 vedo::DOWorld* oWorld            )
{
	double R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos
		(DOName,
		njr::ZERO,
		njr::ZERO,
		njr::AXIALX,
		njr::AXIALZ,
		njr::ZERO);
	*LowerBoundary += njr::Vector3d(R, R, R);
	*UpperBoundary -= njr::Vector3d(R, R, R);
	double xRange = (UpperBoundary->x()) - (LowerBoundary->x());
	double yRange = (UpperBoundary->y()) - (LowerBoundary->y());
	double zRange = (UpperBoundary->z()) - (LowerBoundary->z());
	double temp_x, temp_y, temp_z;
	srand(time(0));
	for(unsigned long ul=0; ul<(*num); ul++)
	{
		temp_x = (double)(rand()) / (double)RAND_MAX * xRange;
		temp_y = (double)(rand()) / (double)RAND_MAX * yRange;
		temp_z = (double)(rand()) / (double)RAND_MAX * zRange;
		dos.SetPosition(*LowerBoundary + njr::Vector3d(temp_x, temp_y, temp_z));
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
	}
};

void AddRandomDOInCylindricalSpace
	(njr::Vector3d* Center,
	 double* r,
	 double* h,
	 std::string DOName,
	 unsigned long* num,
	 vedo::DOWorld* oWorld     )
{
	double R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos
		(DOName,
		njr::ZERO,
		njr::ZERO,
		njr::AXIALX,
		njr::AXIALZ,
		njr::ZERO);

	*r -= R;
	double dSquare_r = (*r) * (*r);
	double d2r       = 2.0  * (*r);
	double dXMin = Center->x() - (*r);
	double dXMax = Center->x() + (*r);
	double dYMin = Center->y() - (*r);
	double dYMax = Center->y() + (*r);
	double dZMin = Center->z() + R;
	double dZMax = Center->z() + (*h) - R;
	*h -= 2.0 * R;

	double temp_x, temp_y, temp_z;
	srand(time(0));
	unsigned long ul = 0;
	while(ul < (*num))
	{
		temp_x = dXMin + (double)(rand()) / (double)RAND_MAX * d2r;
		temp_y = dYMin + (double)(rand()) / (double)RAND_MAX * d2r;
		if(temp_x * temp_x + temp_y * temp_y <= dSquare_r)
		{
			temp_z = dZMin + (double)(rand()) / (double)RAND_MAX * (*h);
			dos.SetPosition(njr::Vector3d(temp_x, temp_y, temp_z));
			oWorld->AddDOStatus(new vedo::DOStatus(dos));
			ul++;
		}
	}
};

void AddRandomDOInSphericalSpace
	(njr::Vector3d* Center,
	 double* r,
	 std::string DOName,
	 unsigned long* num,
	 vedo::DOWorld* oWorld     )
{
	double R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos
		(DOName, njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO);
	*r -= R;
	double temp_x, temp_y, temp_z, temp_radius, temp_angle_1, temp_angle_2;
	srand(time(0));
	for(unsigned long ul=0; ul<(*num); ul++)
	{
		temp_radius  = (double)(rand()) / (double)RAND_MAX * (*r);
		temp_angle_1 = (double)(rand()) / (double)RAND_MAX * njr::dDoublePI;
		temp_angle_2 = (double)(rand()) / (double)RAND_MAX * njr::dDoublePI;
		temp_x
			= Center->x() + temp_radius * cos(temp_angle_1) * cos(temp_angle_2);
		temp_y
			= Center->y() + temp_radius * cos(temp_angle_1) * sin(temp_angle_2);
		temp_z
			= Center->z() + temp_radius * sin(temp_angle_1);
		dos.SetPosition(njr::Vector3d(temp_x, temp_y, temp_z));
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
	}
};

std::map<std::string, double> CalculateGranularTemperature
	(vedo::DOWorld* oWorld, vedo::GeometricShape& space, std::string DOName)
{
	const vedo::DOStatus* pDOS;
	njr::Vector3d     AvgV, AvgAV;
	unsigned long   Counter = 0;

	oWorld->TurnMonitor(false);
	// Calculate number of elements and average velocity
	for
		(unsigned long ul=0;
		 ul<oWorld->GetSystemParameter()->GetDONumber();
		 ul++                                           )
	{
		pDOS = oWorld->GetDOStatus(ul);
		if(pDOS->GetDOName() == DOName)
		{
			if(space.Inside(pDOS->GetPosition()))
			{
				AvgV  = AvgV  + pDOS->GetVelocity();
				AvgAV = AvgAV + pDOS->GetAngularVelocity();
				oWorld->SetMonitor(ul, true);
				Counter++;
			}
		}
	}
	if(Counter != 0)
	{
		AvgV  = 1.0 / double(Counter) * AvgV;
		AvgAV = 1.0 / double(Counter) * AvgAV;
	}

	// Calculate granular temperature
	double dGranularTemperatureV  = 0.0;
	double dGranularTemperatureAV = 0.0;
	for(unsigned ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
	{
		pDOS = oWorld->GetDOStatus(ul);
		if(pDOS->ISMonitored())
		{
			dGranularTemperatureV
				+= (pDOS->GetVelocity() - AvgV)
				 % (pDOS->GetVelocity() - AvgV);
			dGranularTemperatureAV
				+= (pDOS->GetAngularVelocity() - AvgAV)
				 % (pDOS->GetAngularVelocity() - AvgAV);
		}
	}
	if (Counter != 0)
	{
		dGranularTemperatureV  = dGranularTemperatureV  / (double)Counter / 3.0;
		dGranularTemperatureAV = dGranularTemperatureAV / (double)Counter / 3.0;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(pDOS->ISMonitored())
			{
				oWorld->SetGranularTemperatureV(ul, dGranularTemperatureV);
				oWorld->SetGranularTemperatureAV(ul, dGranularTemperatureAV);
			}
		}
	}
	std::map<std::string, double> gt;
	gt["Time"]
		= oWorld->GetSystemParameter()->GetTimeCurrent();
	gt["NumberOfElement"]                      = (double)Counter;
	gt["AverageVelocity"]                      = AvgV.length();
	gt["GranularTemperatureOfVelocity"]        = dGranularTemperatureV;
	gt["AverageAngularVelocity"]               = AvgAV.length();
	gt["GranularTemperatureOfAngularVelocity"] = dGranularTemperatureAV;
	return gt;
};

void CombineModels(vedo::DOWorld* rWorld, vedo::DOWorld* oWorld)
{
	unsigned long rNum = rWorld->GetSystemParameter()->GetDONumber();
	unsigned long oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName", njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO);
	for(unsigned long ul=0; ul<oNum; ul++)
	{
		dos = *(oWorld->GetDOStatus(ul));
		if (oWorld
			->GetDOModel(oWorld->GetDOStatus(ul)->GetDOName())->GetBehavior()
				== "mobile"                                                  )
		{
			rWorld->AddDOStatus(new vedo::DOStatus(dos));
		}
	}
};

void CombineModels
	(vedo::DOWorld*       rWorld         ,
	 vedo::IactRecordTab* rpIactRecordTab,
	 vedo::DOWorld*       oWorld         ,
	 vedo::IactRecordTab* opIactRecordTab )
{
	unsigned long rNum = rWorld->GetSystemParameter()->GetDONumber();
	unsigned long oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName", njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO);
		unsigned long ulCounter = 0;
	for(unsigned long ul=0; ul<oNum; ul++)
	{
		dos = *(oWorld->GetDOStatus(ul));
		if (oWorld
			->GetDOModel(oWorld->GetDOStatus(ul)->GetDOName())->GetBehavior()
				== "mobile"                                                  )
		{
			ulCounter++;
			rWorld->AddDOStatus(new vedo::DOStatus(dos));
		}
	}
};

void CombineModels(vedo::DOWorld* rWorld, vedo::DOWorld* oWorld, const std::string& sDOName)
{
	unsigned long rNum = rWorld->GetSystemParameter()->GetDONumber();
	unsigned long oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName",
		njr::ZERO,
		njr::ZERO,
		njr::AXIALX,
		njr::AXIALZ,
		njr::ZERO);
	for(unsigned long ul=0; ul<oNum; ul++)
	{
		dos = *(oWorld->GetDOStatus(ul));
		if (oWorld->GetDOStatus(ul)->GetDOName() == sDOName)
		{
			rWorld->AddDOStatus(new vedo::DOStatus(dos));
		}
	}
};

double SCCFlowabilityIndex
	(vedo::DOWorld* oWorld,
	double StartPoint,
	double EndPoint)
{
	unsigned long         DOnum  = oWorld->GetSystemParameter()->GetDONumber();
	double                DOsize =  0.0;
	const vedo::DOStatus* dos    =  0;
	const vedo::DOModel*  dom    =  0;
	njr::Vector3d         DOpos;
	double                H1     =  0.0;
	double                H2     =  0.0;
	double                L1     =  0.0;
	double                L      = EndPoint - StartPoint;
	for(unsigned long i=0; i<DOnum; i++)
	{
		dos = oWorld->GetDOStatus(i);
		dom = oWorld->GetDOModel(dos->GetDOName());
		if (dom->GetBehavior() == "mobile")
		{
			DOsize = dom->GetShapeAttributes().sphere.radius;
			DOpos  = dos->GetPosition();
			if (   (DOpos.x() >= (StartPoint-DOsize))
				&& (DOpos.x() <= (StartPoint+DOsize)) )
			{
				if ((DOpos.y()+DOsize) >= H1)
				{
					H1 = DOpos.y() + DOsize;
				}
			}
			else
			{
				if (DOpos.x() >= (EndPoint-DOsize))
				{
					if ((DOpos.y()+DOsize) >= H2)
					{
						H2 = DOpos.y() + DOsize;
					}
				}
			}
			if (DOpos.x() >= (StartPoint+DOsize))
			{
				if ((DOpos.x()+DOsize-StartPoint) >= L1)
				{
					L1 = DOpos.x() + DOsize - StartPoint;
				}
			}
		}
	}

	std::cout << "H1 = " << H1 << std::endl;
	std::cout << "H2 = " << H2 << std::endl;
	std::cout << "L1 = " << L1 << std::endl;
	double Bm1 = (L1 - L) / L;
	double Bm2 = 0.0;
	if (H1 == 0.0)
	{
		std::cout << "Error!! Code: Knight.cpp - SCCFlowabilityIndex(vedo::DOWorld*, double, double)" << std::endl;
		exit(-1);
	}
	else
	{
		Bm2 = H2 / H1;
	}

	if (Bm1 >= 0.0)
	{
		return Bm2;
	}
	else
	{
		return Bm1;
	}
};

bool SCCViscosityIndex(vedo::DOWorld* oWorld)
{
	unsigned long   DOnum
		= oWorld->GetSystemParameter()->GetDONumber();
	const vedo::DOStatus* dos                = 0;
	const vedo::DOModel*  dom                = 0;
	unsigned long   NumElements        = 0;
	unsigned long   NumElementsInside  = 0;
	unsigned long   NumElementsOutside = 0;
	double          DOsize             = 0.0;
	njr::Vector3d   DOpos;
	for(unsigned long i=0; i<DOnum; i++)
	{
		dos = oWorld->GetDOStatus(i);
		dom = oWorld->GetDOModel(dos->GetDOName());
		if (dom->GetBehavior() == "mobile")
		{
			DOsize = dom->GetShapeAttributes().sphere.radius;
			DOpos  = dos->GetPosition();
			if ((DOpos.y()+DOsize) >= -0.5)
			{
				NumElementsInside++;
			}
			else
			{
				NumElementsOutside++;
			}
		}
	}
	NumElements = NumElementsInside + NumElementsOutside;

	std::cout
		<< "Number of Mobile Elements = "
		<< NumElements
		<< std::endl
		<< "Inside elements           = "
		<< NumElementsInside
		<< "\t("
		<< (double)NumElementsInside / (double)NumElements * 100.0
		<< " %)"
		<< std::endl
		<< "Outside elements          = "
		<< NumElementsOutside
		<< "\t("
		<< (double)NumElementsOutside / (double)NumElements * 100.0
		<< " %)"
		<< std::endl;

	if (NumElementsInside == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
};

int main (int argc, char* argv[])
{
	//njr::RunTime("Anne's Knight Start");
	std::vector<std::string> arg;
	for (int i=0; i<argc; i++)
	{
		arg.push_back(argv[i]);
	}

	if (arg.size() < 2)
	{
		usage();
	}
	else if ((arg[1] == "-a") && (arg.size() == 11))
	{
		double xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = argv[2];
		sscanf(argv[3], "%lg", &xmin);
		sscanf(argv[4], "%lg", &xmax);
		sscanf(argv[5], "%lg", &ymin);
		sscanf(argv[6], "%lg", &ymax);
		sscanf(argv[7], "%lg", &zmin);
		sscanf(argv[8], "%lg", &zmax);
		njr::Vector3d LowerBoundary(xmin, ymin, zmin);
		njr::Vector3d UpperBoundary(xmax, ymax, zmax);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		AddDOInSpace(&LowerBoundary, &UpperBoundary, DOName, oWorld);
		delete WriteDOWorld (arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-ap") && (arg.size() == 8))
	{
		double x, y, z;
		std::string DOName = argv[2];
		sscanf(argv[3], "%lg", &x);
		sscanf(argv[4], "%lg", &y);
		sscanf(argv[5], "%lg", &z);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		njr::Vector3d SourcePoint(x, y, z);
		AddPotentialEnergy(&SourcePoint, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-aecp") && (arg.size() == 10))
	{
		std::string sDOName = argv[2];
		double dAngleStarted, dAngleEnded, dR, dZ;
		unsigned uElementSize;
		sscanf(argv[3] , "%lg", &dAngleStarted);
		sscanf(argv[4] , "%lg", &dAngleEnded);
		if(dAngleEnded <= dAngleStarted)
		{
			std::cerr
				<< "Error!! Code: Knight.cpp (-aecp)" << std::endl
				<< "        Note: Started angle is smaller than ended angle" << std::endl;
			exit(-1);
		}
		sscanf(argv[5] , "%d" , &uElementSize);
		sscanf(argv[6] , "%lg", &dR);
		sscanf(argv[7] , "%lg", &dZ);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[8], pIactRecordTab);
		vedo::DOStatus dos
			(sDOName,
			njr::Vector3d(njr::ZERO),
			njr::Vector3d(njr::ZERO),
			njr::Vector3d(njr::AXIALZ),
			njr::Vector3d(njr::AXIALX),
			njr::Vector3d(njr::ZERO)   );
		double dAngleRange = (dAngleEnded - dAngleStarted) * njr::dDegree2PI;
		double dAngleIncremental = dAngleRange / (double)uElementSize;
		double dAngle = dAngleStarted * njr::dDegree2PI;
		double dCosineAngle, dSineAngle;
		for(unsigned u=0; u<uElementSize; u++)
		{
			dCosineAngle = cos(dAngle);
			dSineAngle   = sin(dAngle);
			dos.SetPosition(njr::Vector3d(dR*dCosineAngle, dR*dSineAngle, dZ));
			dos.SetOrientationZ(njr::Vector3d(dCosineAngle, dSineAngle, 0.0));
			oWorld->AddDOStatus(new vedo::DOStatus(dos));
			dAngle += dAngleIncremental;
		}
		delete WriteDOWorld (arg[9], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-ase") && (arg.size() == 20))
	{
		double dPx, dPy, dPz, dVx, dVy, dVz, dAVx, dAVy, dAVz;
		double dOxx, dOxy, dOxz, dOzx, dOzy, dOzz;
		std::string sDOName = argv[2];
		sscanf(argv[3] , "%lg", &dPx);
		sscanf(argv[4] , "%lg", &dPy);
		sscanf(argv[5] , "%lg", &dPz);
		sscanf(argv[6] , "%lg", &dVx);
		sscanf(argv[7] , "%lg", &dVy);
		sscanf(argv[8] , "%lg", &dVz);
		sscanf(argv[9] , "%lg", &dOxx);
		sscanf(argv[10], "%lg", &dOxy);
		sscanf(argv[11], "%lg", &dOxz);
		sscanf(argv[12], "%lg", &dOzx);
		sscanf(argv[13], "%lg", &dOzy);
		sscanf(argv[14], "%lg", &dOzz);
		sscanf(argv[15], "%lg", &dAVx);
		sscanf(argv[16], "%lg", &dAVy);
		sscanf(argv[17], "%lg", &dAVz);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[18], pIactRecordTab);
		vedo::DOStatus dos
			(sDOName,
			njr::Vector3d(dPx , dPy , dPz ),
			njr::Vector3d(dVx , dVy , dVz ),
			njr::Vector3d(dOxx, dOxy, dOxz).direction(),
			njr::Vector3d(dOzx, dOzy, dOzz).direction(),
			njr::Vector3d(dAVx, dAVy, dAVz) );
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
		delete WriteDOWorld (arg[19], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-av") && (arg.size() == 8))
	{
		double dVx, dVy, dVz;
		std::string DOName = argv[2];
		sscanf(argv[3], "%lg", &dVx);
		sscanf(argv[4], "%lg", &dVy);
		sscanf(argv[5], "%lg", &dVz);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		njr::Vector3d vExternalVelocity(dVx, dVy, dVz);
		AddExternalVelocity(&vExternalVelocity, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-aav") && (arg.size() == 8))
	{
		double dAVx, dAVy, dAVz;
		std::string DOName = argv[2];
		sscanf(argv[3], "%lg", &dAVx);
		sscanf(argv[4], "%lg", &dAVy);
		sscanf(argv[5], "%lg", &dAVz);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		njr::Vector3d vExternalAngularVelocity(dAVx, dAVy, dAVz);
		AddExternalAngularVelocity(&vExternalAngularVelocity, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-ar_cartesian") && (arg.size() == 12))
	{
		double xmin, xmax, ymin, ymax, zmin, zmax;
		unsigned long num = 0;
		std::string DOName = argv[2];
		sscanf(argv[3], "%d",  &num );
		sscanf(argv[4], "%lg", &xmin);
		sscanf(argv[5], "%lg", &xmax);
		sscanf(argv[6], "%lg", &ymin);
		sscanf(argv[7], "%lg", &ymax);
		sscanf(argv[8], "%lg", &zmin);
		sscanf(argv[9], "%lg", &zmax);
		njr::Vector3d LowerBoundary(xmin, ymin, zmin);
		njr::Vector3d UpperBoundary(xmax, ymax, zmax);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[10], pIactRecordTab);
		AddRandomDOInCartesianSpace
			(&LowerBoundary, &UpperBoundary, DOName, &num, oWorld);
		delete WriteDOWorld (arg[11], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-ar_cylindrical") && (arg.size() == 11))
	{
		double x, y, z, r, h;
		unsigned long num = 0;
		std::string DOName = argv[2];
		sscanf(argv[3], "%d", &num);
		sscanf(argv[4], "%lg", &x);
		sscanf(argv[5], "%lg", &y);
		sscanf(argv[6], "%lg", &z);
		sscanf(argv[7], "%lg", &r);
		sscanf(argv[8], "%lg", &h);
		njr::Vector3d Center(x, y, z);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		AddRandomDOInCylindricalSpace(&Center, &r, &h, DOName, &num, oWorld);
		delete WriteDOWorld (arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-ar_spherical") && (arg.size() == 10))
	{
		double x, y, z, r;
		unsigned long num = 0;
		std::string DOName = argv[2];
		sscanf(argv[3], "%d", &num);
		sscanf(argv[4], "%lg", &x);
		sscanf(argv[5], "%lg", &y);
		sscanf(argv[6], "%lg", &z);
		sscanf(argv[7], "%lg", &r);
		njr::Vector3d Center(x, y, z);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[8], pIactRecordTab);
		AddRandomDOInSphericalSpace(&Center, &r, DOName, &num, oWorld);
		delete WriteDOWorld (arg[9], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if (arg[1] == "-al")
	{
		AssmListHEX();
	}
//	else if ((arg[1] == "-assm") && (arg.size() >= 5))
//	{
//		assm (arg[2], arg[3], arg[4]);
//	}
	else if ((arg[1] == "-avg_info") && (arg.size() == 10))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[3], "%lg", &dXMin);
		sscanf(argv[4], "%lg", &dXMax);
		sscanf(argv[5], "%lg", &dYMin);
		sscanf(argv[6], "%lg", &dYMax);
		sscanf(argv[7], "%lg", &dZMin);
		sscanf(argv[8], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[9], "%lg", &dMeshSize);
		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld->GetSystemParameter()->GetDONumber())
			{
				pDOS = oWorld->GetDOStatus(ul);
				if
					(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType()
					 == vedo::Sphere                                            )
				{
					dRadius
						= oWorld
						->GetDOModel(pDOS->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS->GetPosition().x() - dRadius;
					dNewXMax = pDOS->GetPosition().x() + dRadius;
					dNewYMin = pDOS->GetPosition().y() - dRadius;
					dNewYMax = pDOS->GetPosition().y() + dRadius;
					dNewZMin = pDOS->GetPosition().z() - dRadius;
					dNewZMax = pDOS->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the average information
		double dTotalVolume = 0.0;
		std::vector<double> vVolume;
		double dTotalMass   = 0.0;
		njr::Vector3d vMassCenter, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		std::ofstream FileSphericalElementVolume;
		FileSphericalElementVolume.open("SphericalElementVolume.csv", std::ios::out);
		FileSphericalElementVolume
			<< "ElementID, "
			<< "PositionX, PositionY, PositionZ, "
			<< "VelocityX, VelocityY, VelocityZ, "
			<< "AngularVelocityX, AngularVelocityY, AngularVelocityZ, "
			<< "Radius, InsideVolume, "
			<< "MassCenterX, MassCenterY, MassCenterZ"
			<< std::endl;
		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<double, njr::Vector3d> pdvVolume
				 = dom
				 	->VolumeInsideBoundary(pDOS->GetPosition(), &BC, dMeshSize);
			if((dom->GetShapeType() == vedo::Sphere) && (pdvVolume.first != 0.0))
			{
				FileSphericalElementVolume
					<< ul                                      << ", "
					<< pDOS->GetPosition().x()                 << ", "
					<< pDOS->GetPosition().y()                 << ", "
					<< pDOS->GetPosition().z()                 << ", "
					<< pDOS->GetVelocity().x()                 << ", "
					<< pDOS->GetVelocity().y()                 << ", "
					<< pDOS->GetVelocity().z()                 << ", "
					<< pDOS->GetAngularVelocity().x()          << ", "
					<< pDOS->GetAngularVelocity().y()          << ", "
					<< pDOS->GetAngularVelocity().z()          << ", "
					<< dom->GetShapeAttributes().sphere.radius << ", "
					<< pdvVolume.first                         << ", "
					<< pdvVolume.second.x()                    << ", "
					<< pdvVolume.second.y()                    << ", "
					<< pdvVolume.second.z()                    << std::endl;
			}

			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = pDOS->GetVelocity();
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if(pdvVolume.first != 0.0)
			{
				vVolume.push_back(pdvVolume.first);
				dTotalVolume            += pdvVolume.first;
				dTotalMass              += pdvVolume.first * dWeighting;
				vMassCenter             += pdvVolume.first * dWeighting * pdvVolume.second;
				vAverageVelocity        += pdvVolume.first * vTempVelocity;
				vAverageAngularVelocity += pdvVolume.first * vTempAngularVelocity;
			}
		}
		FileSphericalElementVolume.close();

		double dVolumeRatio = dTotalVolume / OriginalBC.GetVolume();
		if(dTotalMass != 0.0)
		{
			vMassCenter = 1 / dTotalMass * vMassCenter;
		}
		else
		{
			vMassCenter = OriginalBC.GetCenter();
		};

		double dGranularTemperatureVelocity         = 0.0;
		double dGranularTemperatureVelocityX        = 0.0;
		double dGranularTemperatureVelocityY        = 0.0;
		double dGranularTemperatureVelocityZ        = 0.0;
		double dGranularTemperatureAngularVelocity  = 0.0;
		double dGranularTemperatureAngularVelocityX = 0.0;
		double dGranularTemperatureAngularVelocityY = 0.0;
		double dGranularTemperatureAngularVelocityZ = 0.0;
		if(dTotalVolume != 0.0)
		{
			vAverageVelocity        = 1.0 / dTotalVolume * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dTotalVolume * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for(unsigned long ul=0; ul<vVolume.size(); ul++)
			{
				vVTemp1
					= vvAverageVelocity[ul] - vAverageVelocity;
				vVTemp2
					= vvAverageAngularVelocity[ul] - vAverageAngularVelocity;
				dGranularTemperatureVelocityX        += vVolume[ul] * vVTemp1.x() * vVTemp1.x();
				dGranularTemperatureVelocityY        += vVolume[ul] * vVTemp1.y() * vVTemp1.y();
				dGranularTemperatureVelocityZ        += vVolume[ul] * vVTemp1.z() * vVTemp1.z();
				dGranularTemperatureAngularVelocityX += vVolume[ul] * vVTemp2.x() * vVTemp2.x();
				dGranularTemperatureAngularVelocityY += vVolume[ul] * vVTemp2.y() * vVTemp2.y();
				dGranularTemperatureAngularVelocityZ += vVolume[ul] * vVTemp2.z() * vVTemp2.z();
			};
			dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityX / 3.0 / dTotalVolume;
			dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityY / 3.0 / dTotalVolume;
			dGranularTemperatureVelocityZ
				= dGranularTemperatureVelocityZ / 3.0 / dTotalVolume;
			dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityX / 3.0 / dTotalVolume;
			dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityY / 3.0 / dTotalVolume;
			dGranularTemperatureAngularVelocityZ
				= dGranularTemperatureAngularVelocityZ / 3.0 / dTotalVolume;
			dGranularTemperatureVelocity
				= dGranularTemperatureVelocityX
				+ dGranularTemperatureVelocityY
				+ dGranularTemperatureVelocityZ;
			dGranularTemperatureAngularVelocity
				= dGranularTemperatureAngularVelocityX
				+ dGranularTemperatureAngularVelocityY
				+ dGranularTemperatureAngularVelocityZ;
			if(dGranularTemperatureVelocity != 0.0)
			{
				dGranularTemperatureVelocityX /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityY /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityZ /= dGranularTemperatureVelocity;
			}
			else
			{
				dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityZ = 0.0;
			};
			if(dGranularTemperatureAngularVelocity != 0.0)
			{
				dGranularTemperatureAngularVelocityX
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityY
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityZ
					/= dGranularTemperatureAngularVelocity;
			}
			else
			{
				dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityZ = 0.0;
			};
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.Set(0.0, 0.0, 0.0);
		}

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time, "
			<< "BoxCenterX, BoxCenterY, BoxCenterZ, "
			<< "NewBoxCenterX, NewBoxCenterY, NewBoxCenterZ, "
			<< "BoxSizeX, BoxSizeY, BoxSizeZ, "
			<< "NewBoxSizeX, NewBoxSizeY, NewBoxSizeZ, "
			<< "MassCenterX, MassCenterY, MassCenterZ, "
			<< "ElementNumber, ElementVolume, ElementMass, VolumeRatio, "
			<< "AverageVelocityX, AverageVelocityY, AverageVelocityZ, "
			<< "AverageAngularVelocityX, "
			<< "AverageAngularVelocityY, "
			<< "AverageAngularVelocityZ, "
			<< "GranularTemperatureV, "
			<< "GranularTemperatureVxOverV, "
			<< "GranularTemperatureVyOverV, "
			<< "GranularTemperatureVzOverV, "
			<< "GranularTemperatureAV, "
			<< "GranularTemperatureAVxOverAV, "
			<< "GranularTemperatureAVyOverAV, "
			<< "GranularTemperatureAVzOverAV"                 << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().x()                     << ", "
			<< OriginalBC.GetCenter().y()                     << ", "
			<< OriginalBC.GetCenter().z()                     << ", "
			<< BC.GetCenter().x()                             << ", "
			<< BC.GetCenter().y()                             << ", "
			<< BC.GetCenter().z()                             << ", "
			<< OriginalBC.GetRange().x()                      << ", "
			<< OriginalBC.GetRange().y()                      << ", "
			<< OriginalBC.GetRange().z()                      << ", "
			<< BC.GetRange().x()                              << ", "
			<< BC.GetRange().y()                              << ", "
			<< BC.GetRange().z()                              << ", "
			<< vMassCenter.x()                                << ", "
			<< vMassCenter.y()                                << ", "
			<< vMassCenter.z()                                << ", "
			<< vVolume.size()                                 << ", "
			<< dTotalVolume                                   << ", "
			<< dTotalMass                                     << ", "
			<< dVolumeRatio                                   << ", "
			<< vAverageVelocity.x()                           << ", "
			<< vAverageVelocity.y()                           << ", "
			<< vAverageVelocity.z()                           << ", "
			<< vAverageAngularVelocity.x()                    << ", "
			<< vAverageAngularVelocity.y()                    << ", "
			<< vAverageAngularVelocity.z()                    << ", "
			<< dGranularTemperatureVelocity                   << ", "
			<< dGranularTemperatureVelocityX                  << ", "
			<< dGranularTemperatureVelocityY                  << ", "
			<< dGranularTemperatureVelocityZ                  << ", "
			<< dGranularTemperatureAngularVelocity            << ", "
			<< dGranularTemperatureAngularVelocityX           << ", "
			<< dGranularTemperatureAngularVelocityY           << ", "
			<< dGranularTemperatureAngularVelocityZ           << std::endl;
		FileAverageInformation.close();

		delete oWorld;
	}
	else if ((arg[1] == "-avg_info") && (arg.size() == 11))
	{
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[3]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[4], "%lg", &dXMin);
		sscanf(argv[5], "%lg", &dXMax);
		sscanf(argv[6], "%lg", &dYMin);
		sscanf(argv[7], "%lg", &dYMax);
		sscanf(argv[8], "%lg", &dZMin);
		sscanf(argv[9], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[10], "%lg", &dMeshSize);
		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const double dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld->GetSystemParameter()->GetDONumber())
			{
				pDOS = oWorld->GetDOStatus(ul);
				if
					(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType()
					 == vedo::Sphere                                            )
				{
					dRadius
						= oWorld
						->GetDOModel(pDOS->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS->GetPosition().x() - dRadius;
					dNewXMax = pDOS->GetPosition().x() + dRadius;
					dNewYMin = pDOS->GetPosition().y() - dRadius;
					dNewYMax = pDOS->GetPosition().y() + dRadius;
					dNewZMin = pDOS->GetPosition().z() - dRadius;
					dNewZMax = pDOS->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the average information
		double dTotalVolume = 0.0;
		std::vector<double> vVolume;
		double dTotalMass   = 0.0;
		njr::Vector3d vMassCenter, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		std::ofstream FileSphericalElementVolume;
		FileSphericalElementVolume.open("SphericalElementVolume.csv", std::ios::out);
		FileSphericalElementVolume
			<< "ElementID, "
			<< "PositionX, PositionY, PositionZ, "
			<< "VelocityX, VelocityY, VelocityZ, "
			<< "AngularVelocityX, AngularVelocityY, AngularVelocityZ, "
			<< "Radius, InsideVolume, "
			<< "MassCenterX, MassCenterY, MassCenterZ"
			<< std::endl;
		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS         = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom          = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<double, njr::Vector3d> pdvVolume
				 = dom
				 	->VolumeInsideBoundary(pDOS->GetPosition(), &BC, dMeshSize);
			if((dom->GetShapeType() == vedo::Sphere) && (pdvVolume.first != 0.0))
			{
				FileSphericalElementVolume
					<< ul                                      << ", "
					<< pDOS->GetPosition().x()                 << ", "
					<< pDOS->GetPosition().y()                 << ", "
					<< pDOS->GetPosition().z()                 << ", "
					<< 1.0 / dTimeStep
					   * (  pDOSNextStep->GetPosition().x()
					      - pDOS->GetPosition().x()        )   << ", "
					<< 1.0 / dTimeStep
					   * (  pDOSNextStep->GetPosition().y()
					      - pDOS->GetPosition().y()        )   << ", "
					<< 1.0 / dTimeStep
					   * (  pDOSNextStep->GetPosition().z()
					      - pDOS->GetPosition().z()        )   << ", "
					<< pDOS->GetAngularVelocity().x()          << ", "
					<< pDOS->GetAngularVelocity().y()          << ", "
					<< pDOS->GetAngularVelocity().z()          << ", "
					<< dom->GetShapeAttributes().sphere.radius << ", "
					<< pdvVolume.first                         << ", "
					<< pdvVolume.second.x()                    << ", "
					<< pdvVolume.second.y()                    << ", "
					<< pdvVolume.second.z()                    << std::endl;
			}

			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = 1.0 / dTimeStep
				                   * (  pDOSNextStep->GetPosition()
								      - pDOS->GetPosition()        );
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if(pdvVolume.first != 0.0)
			{
				vVolume.push_back(pdvVolume.first);
				dTotalVolume            += pdvVolume.first;
				dTotalMass              += pdvVolume.first * dWeighting;
				vMassCenter             += pdvVolume.first * dWeighting * pdvVolume.second;
				vAverageVelocity        += pdvVolume.first * vTempVelocity;
				vAverageAngularVelocity += pdvVolume.first * vTempAngularVelocity;
			}
		}
		FileSphericalElementVolume.close();

		double dVolumeRatio = dTotalVolume / OriginalBC.GetVolume();
		if(dTotalMass != 0.0)
		{
			vMassCenter = 1 / dTotalMass * vMassCenter;
		}
		else
		{
			vMassCenter = OriginalBC.GetCenter();
		};

		double dGranularTemperatureVelocity         = 0.0;
		double dGranularTemperatureVelocityX        = 0.0;
		double dGranularTemperatureVelocityY        = 0.0;
		double dGranularTemperatureVelocityZ        = 0.0;
		double dGranularTemperatureAngularVelocity  = 0.0;
		double dGranularTemperatureAngularVelocityX = 0.0;
		double dGranularTemperatureAngularVelocityY = 0.0;
		double dGranularTemperatureAngularVelocityZ = 0.0;
		if(dTotalVolume != 0.0)
		{
			vAverageVelocity        = 1.0 / dTotalVolume * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dTotalVolume * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for(unsigned long ul=0; ul<vVolume.size(); ul++)
			{
				vVTemp1
					= vvAverageVelocity[ul] - vAverageVelocity;
				vVTemp2
					= vvAverageAngularVelocity[ul] - vAverageAngularVelocity;
				dGranularTemperatureVelocityX        += vVolume[ul] * vVTemp1.x() * vVTemp1.x();
				dGranularTemperatureVelocityY        += vVolume[ul] * vVTemp1.y() * vVTemp1.y();
				dGranularTemperatureVelocityZ        += vVolume[ul] * vVTemp1.z() * vVTemp1.z();
				dGranularTemperatureAngularVelocityX += vVolume[ul] * vVTemp2.x() * vVTemp2.x();
				dGranularTemperatureAngularVelocityY += vVolume[ul] * vVTemp2.y() * vVTemp2.y();
				dGranularTemperatureAngularVelocityZ += vVolume[ul] * vVTemp2.z() * vVTemp2.z();
			};
			dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityX / 3.0 / dTotalVolume;
			dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityY / 3.0 / dTotalVolume;
			dGranularTemperatureVelocityZ
				= dGranularTemperatureVelocityZ / 3.0 / dTotalVolume;
			dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityX / 3.0 / dTotalVolume;
			dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityY / 3.0 / dTotalVolume;
			dGranularTemperatureAngularVelocityZ
				= dGranularTemperatureAngularVelocityZ / 3.0 / dTotalVolume;
			dGranularTemperatureVelocity
				= dGranularTemperatureVelocityX
				+ dGranularTemperatureVelocityY
				+ dGranularTemperatureVelocityZ;
			dGranularTemperatureAngularVelocity
				= dGranularTemperatureAngularVelocityX
				+ dGranularTemperatureAngularVelocityY
				+ dGranularTemperatureAngularVelocityZ;
			if(dGranularTemperatureVelocity != 0.0)
			{
				dGranularTemperatureVelocityX /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityY /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityZ /= dGranularTemperatureVelocity;
			}
			else
			{
				dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityZ = 0.0;
			};
			if(dGranularTemperatureAngularVelocity != 0.0)
			{
				dGranularTemperatureAngularVelocityX
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityY
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityZ
					/= dGranularTemperatureAngularVelocity;
			}
			else
			{
				dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityZ = 0.0;
			};
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.Set(0.0, 0.0, 0.0);
		}

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time, "
			<< "BoxCenterX, BoxCenterY, BoxCenterZ, "
			<< "NewBoxCenterX, NewBoxCenterY, NewBoxCenterZ, "
			<< "BoxSizeX, BoxSizeY, BoxSizeZ, "
			<< "NewBoxSizeX, NewBoxSizeY, NewBoxSizeZ, "
			<< "MassCenterX, MassCenterY, MassCenterZ, "
			<< "ElementNumber, ElementVolume, ElementMass, VolumeRatio, "
			<< "AverageVelocityX, AverageVelocityY, AverageVelocityZ, "
			<< "AverageAngularVelocityX, "
			<< "AverageAngularVelocityY, "
			<< "AverageAngularVelocityZ, "
			<< "GranularTemperatureV, "
			<< "GranularTemperatureVxOverV, "
			<< "GranularTemperatureVyOverV, "
			<< "GranularTemperatureVzOverV, "
			<< "GranularTemperatureAV, "
			<< "GranularTemperatureAVxOverAV, "
			<< "GranularTemperatureAVyOverAV, "
			<< "GranularTemperatureAVzOverAV"                 << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().x()                     << ", "
			<< OriginalBC.GetCenter().y()                     << ", "
			<< OriginalBC.GetCenter().z()                     << ", "
			<< BC.GetCenter().x()                             << ", "
			<< BC.GetCenter().y()                             << ", "
			<< BC.GetCenter().z()                             << ", "
			<< OriginalBC.GetRange().x()                      << ", "
			<< OriginalBC.GetRange().y()                      << ", "
			<< OriginalBC.GetRange().z()                      << ", "
			<< BC.GetRange().x()                              << ", "
			<< BC.GetRange().y()                              << ", "
			<< BC.GetRange().z()                              << ", "
			<< vMassCenter.x()                                << ", "
			<< vMassCenter.y()                                << ", "
			<< vMassCenter.z()                                << ", "
			<< vVolume.size()                                 << ", "
			<< dTotalVolume                                   << ", "
			<< dTotalMass                                     << ", "
			<< dVolumeRatio                                   << ", "
			<< vAverageVelocity.x()                           << ", "
			<< vAverageVelocity.y()                           << ", "
			<< vAverageVelocity.z()                           << ", "
			<< vAverageAngularVelocity.x()                    << ", "
			<< vAverageAngularVelocity.y()                    << ", "
			<< vAverageAngularVelocity.z()                    << ", "
			<< dGranularTemperatureVelocity                   << ", "
			<< dGranularTemperatureVelocityX                  << ", "
			<< dGranularTemperatureVelocityY                  << ", "
			<< dGranularTemperatureVelocityZ                  << ", "
			<< dGranularTemperatureAngularVelocity            << ", "
			<< dGranularTemperatureAngularVelocityX           << ", "
			<< dGranularTemperatureAngularVelocityY           << ", "
			<< dGranularTemperatureAngularVelocityZ           << std::endl;
		FileAverageInformation.close();

		delete oWorld;
	}
	else if ((arg[1] == "-avg_info_xy") && (arg.size() == 10))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[3], "%lg", &dXMin);
		sscanf(argv[4], "%lg", &dXMax);
		sscanf(argv[5], "%lg", &dYMin);
		sscanf(argv[6], "%lg", &dYMax);
		sscanf(argv[7], "%lg", &dZMin);
		sscanf(argv[8], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[9], "%lg", &dMeshSize);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld->GetSystemParameter()->GetDONumber())
			{
				pDOS = oWorld->GetDOStatus(ul);
				if
					(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType()
					 == vedo::Sphere                                            )
				{
					dRadius
						= oWorld
						->GetDOModel(pDOS->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS->GetPosition().x() - dRadius;
					dNewXMax = pDOS->GetPosition().x() + dRadius;
					dNewYMin = pDOS->GetPosition().y() - dRadius;
					dNewYMax = pDOS->GetPosition().y() + dRadius;
					dNewZMin = pDOS->GetPosition().z() - dRadius;
					dNewZMax = pDOS->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		double dProjectedArea = 0.0;
		std::vector<double> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		double dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<double, njr::Vector3d> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = pDOS->GetVelocity();
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if(pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		double dProjectedAreaRatio
			= dProjectedArea
			/ OriginalBC.GetLength().x()
			/ OriginalBC.GetLength().y();
		if(dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 vCentroid.y() / dTotalWeighting,
				 OriginalBC.GetCenter().z()      );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		};

		double dGranularTemperatureVelocity         = 0.0;
		double dGranularTemperatureVelocityX        = 0.0;
		double dGranularTemperatureVelocityY        = 0.0;
		double dGranularTemperatureVelocityZ        = 0.0;
		double dGranularTemperatureAngularVelocity  = 0.0;
		double dGranularTemperatureAngularVelocityX = 0.0;
		double dGranularTemperatureAngularVelocityY = 0.0;
		double dGranularTemperatureAngularVelocityZ = 0.0;
		if(dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for(unsigned long ul=0; ul<vProjectedArea.size(); ul++)
			{
				vVTemp1
					= vvAverageVelocity[ul] - vAverageVelocity;
				vVTemp2
					= vvAverageAngularVelocity[ul] - vAverageAngularVelocity;
				dGranularTemperatureVelocityX        += vProjectedArea[ul] * vVTemp1.x() * vVTemp1.x();
				dGranularTemperatureVelocityY        += vProjectedArea[ul] * vVTemp1.y() * vVTemp1.y();
				dGranularTemperatureVelocityZ        += vProjectedArea[ul] * vVTemp1.z() * vVTemp1.z();
				dGranularTemperatureAngularVelocityX += vProjectedArea[ul] * vVTemp2.x() * vVTemp2.x();
				dGranularTemperatureAngularVelocityY += vProjectedArea[ul] * vVTemp2.y() * vVTemp2.y();
				dGranularTemperatureAngularVelocityZ += vProjectedArea[ul] * vVTemp2.z() * vVTemp2.z();
			};
			dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityZ
				= dGranularTemperatureVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityZ
				= dGranularTemperatureAngularVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureVelocity
				= dGranularTemperatureVelocityX
				+ dGranularTemperatureVelocityY
				+ dGranularTemperatureVelocityZ;
			dGranularTemperatureAngularVelocity
				= dGranularTemperatureAngularVelocityX
				+ dGranularTemperatureAngularVelocityY
				+ dGranularTemperatureAngularVelocityZ;
			if(dGranularTemperatureVelocity != 0.0)
			{
				dGranularTemperatureVelocityX /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityY /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityZ /= dGranularTemperatureVelocity;
			}
			else
			{
				dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityZ = 0.0;
			};
			if(dGranularTemperatureAngularVelocity != 0.0)
			{
				dGranularTemperatureAngularVelocityX
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityY
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityZ
					/= dGranularTemperatureAngularVelocity;
			}
			else
			{
				dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityZ = 0.0;
			};
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.Set(0.0, 0.0, 0.0);
		}

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time, "
			<< "BoxCenterX, "
			<< "BoxCenterY, "
			<< "BoxSizeX, "
			<< "BoxSizeY, "
			<< "CentroidX, "
			<< "CentroidY, "
			<< "ElementNumber, "
			<< "ProjectedArea, "
			<< "ProjectedAreaRatio, "
			<< "AverageVelocityX, "
			<< "AverageVelocityY, "
			<< "AverageVelocityZ, "
			<< "AverageAngularVelocityX, "
			<< "AverageAngularVelocityY, "
			<< "AverageAngularVelocityZ, "
			<< "GranularTemperatureV, "
			<< "GranularTemperatureVxOverV, "
			<< "GranularTemperatureVyOverV, "
			<< "GranularTemperatureVzOverV, "
			<< "GranularTemperatureAV, "
			<< "GranularTemperatureAVxOverAV, "
			<< "GranularTemperatureAVyOverAV, "
			<< "GranularTemperatureAVzOverAV" << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().x()                     << ", "
			<< OriginalBC.GetCenter().y()                     << ", "
			<< OriginalBC.GetRange().x()                      << ", "
			<< OriginalBC.GetRange().y()                      << ", "
			<< vCentroid.x()                                  << ", "
			<< vCentroid.y()                                  << ", "
			<< vProjectedArea.size()                          << ", "
			<< dProjectedArea                                 << ", "
			<< dProjectedAreaRatio                            << ", "
			<< vAverageVelocity.x()                           << ", "
			<< vAverageVelocity.y()                           << ", "
			<< vAverageVelocity.z()                           << ", "
			<< vAverageAngularVelocity.x()                    << ", "
			<< vAverageAngularVelocity.y()                    << ", "
			<< vAverageAngularVelocity.z()                    << ", "
			<< dGranularTemperatureVelocity                   << ", "
			<< dGranularTemperatureVelocityX                  << ", "
			<< dGranularTemperatureVelocityY                  << ", "
			<< dGranularTemperatureVelocityZ                  << ", "
			<< dGranularTemperatureAngularVelocity            << ", "
			<< dGranularTemperatureAngularVelocityX           << ", "
			<< dGranularTemperatureAngularVelocityY           << ", "
			<< dGranularTemperatureAngularVelocityZ           << std::endl;
		FileAverageInformation.close();

		delete oWorld;
	}
	else if ((arg[1] == "-avg_info_xy") && (arg.size() == 11))
	{
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[3]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[4], "%lg", &dXMin);
		sscanf(argv[5], "%lg", &dXMax);
		sscanf(argv[6], "%lg", &dYMin);
		sscanf(argv[7], "%lg", &dYMax);
		sscanf(argv[8], "%lg", &dZMin);
		sscanf(argv[9], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[10], "%lg", &dMeshSize);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const double dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld->GetSystemParameter()->GetDONumber())
			{
				pDOS = oWorld->GetDOStatus(ul);
				if
					(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType()
					 == vedo::Sphere                                            )
				{
					dRadius
						= oWorld
						->GetDOModel(pDOS->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS->GetPosition().x() - dRadius;
					dNewXMax = pDOS->GetPosition().x() + dRadius;
					dNewYMin = pDOS->GetPosition().y() - dRadius;
					dNewYMax = pDOS->GetPosition().y() + dRadius;
					dNewZMin = pDOS->GetPosition().z() - dRadius;
					dNewZMax = pDOS->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		double dProjectedArea = 0.0;
		std::vector<double> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		double dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());

			// Special requirement from NTUME's lab
//			if (   pDOS->GetPosition().z() > BC.GetLowerPoint().z()
//				&& pDOS->GetPosition().z() < BC.GetUpperPoint().z() )
//			{

			std::pair<double, njr::Vector3d> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = 1.0 / dTimeStep
				                   * (  pDOSNextStep->GetPosition()
								      - pDOS->GetPosition()        );
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if(pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
//			}
		}

		double dProjectedAreaRatio
			= dProjectedArea
			/ OriginalBC.GetLength().x()
			/ OriginalBC.GetLength().y();
		if(dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 vCentroid.y() / dTotalWeighting,
				 OriginalBC.GetCenter().z()      );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		};

		double dGranularTemperatureVelocity         = 0.0;
		double dGranularTemperatureVelocityX        = 0.0;
		double dGranularTemperatureVelocityY        = 0.0;
		double dGranularTemperatureVelocityZ        = 0.0;
		double dGranularTemperatureAngularVelocity  = 0.0;
		double dGranularTemperatureAngularVelocityX = 0.0;
		double dGranularTemperatureAngularVelocityY = 0.0;
		double dGranularTemperatureAngularVelocityZ = 0.0;
		if(dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for(unsigned long ul=0; ul<vProjectedArea.size(); ul++)
			{
				vVTemp1
					= vvAverageVelocity[ul] - vAverageVelocity;
				vVTemp2
					= vvAverageAngularVelocity[ul] - vAverageAngularVelocity;
				dGranularTemperatureVelocityX        += vProjectedArea[ul] * vVTemp1.x() * vVTemp1.x();
				dGranularTemperatureVelocityY        += vProjectedArea[ul] * vVTemp1.y() * vVTemp1.y();
				dGranularTemperatureVelocityZ        += vProjectedArea[ul] * vVTemp1.z() * vVTemp1.z();
				dGranularTemperatureAngularVelocityX += vProjectedArea[ul] * vVTemp2.x() * vVTemp2.x();
				dGranularTemperatureAngularVelocityY += vProjectedArea[ul] * vVTemp2.y() * vVTemp2.y();
				dGranularTemperatureAngularVelocityZ += vProjectedArea[ul] * vVTemp2.z() * vVTemp2.z();
			};
			dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityZ
				= dGranularTemperatureVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityZ
				= dGranularTemperatureAngularVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureVelocity
				= dGranularTemperatureVelocityX
				+ dGranularTemperatureVelocityY
				+ dGranularTemperatureVelocityZ;
			dGranularTemperatureAngularVelocity
				= dGranularTemperatureAngularVelocityX
				+ dGranularTemperatureAngularVelocityY
				+ dGranularTemperatureAngularVelocityZ;
			if(dGranularTemperatureVelocity != 0.0)
			{
				dGranularTemperatureVelocityX /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityY /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityZ /= dGranularTemperatureVelocity;
			}
			else
			{
				dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityZ = 0.0;
			};
			if(dGranularTemperatureAngularVelocity != 0.0)
			{
				dGranularTemperatureAngularVelocityX
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityY
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityZ
					/= dGranularTemperatureAngularVelocity;
			}
			else
			{
				dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityZ = 0.0;
			};
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.Set(0.0, 0.0, 0.0);
		}

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time, "
			<< "BoxCenterX, "
			<< "BoxCenterY, "
			<< "BoxSizeX, "
			<< "BoxSizeY, "
			<< "CentroidX, "
			<< "CentroidY, "
			<< "ElementNumber, "
			<< "ProjectedArea, "
			<< "ProjectedAreaRatio, "
			<< "AverageVelocityX, "
			<< "AverageVelocityY, "
			<< "AverageVelocityZ, "
			<< "AverageAngularVelocityX, "
			<< "AverageAngularVelocityY, "
			<< "AverageAngularVelocityZ, "
			<< "GranularTemperatureV, "
			<< "GranularTemperatureVxOverV, "
			<< "GranularTemperatureVyOverV, "
			<< "GranularTemperatureVzOverV, "
			<< "GranularTemperatureAV, "
			<< "GranularTemperatureAVxOverAV, "
			<< "GranularTemperatureAVyOverAV, "
			<< "GranularTemperatureAVzOverAV" << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().x()                     << ", "
			<< OriginalBC.GetCenter().y()                     << ", "
			<< OriginalBC.GetRange().x()                      << ", "
			<< OriginalBC.GetRange().y()                      << ", "
			<< vCentroid.x()                                  << ", "
			<< vCentroid.y()                                  << ", "
			<< vProjectedArea.size()                          << ", "
			<< dProjectedArea                                 << ", "
			<< dProjectedAreaRatio                            << ", "
			<< vAverageVelocity.x()                           << ", "
			<< vAverageVelocity.y()                           << ", "
			<< vAverageVelocity.z()                           << ", "
			<< vAverageAngularVelocity.x()                    << ", "
			<< vAverageAngularVelocity.y()                    << ", "
			<< vAverageAngularVelocity.z()                    << ", "
			<< dGranularTemperatureVelocity                   << ", "
			<< dGranularTemperatureVelocityX                  << ", "
			<< dGranularTemperatureVelocityY                  << ", "
			<< dGranularTemperatureVelocityZ                  << ", "
			<< dGranularTemperatureAngularVelocity            << ", "
			<< dGranularTemperatureAngularVelocityX           << ", "
			<< dGranularTemperatureAngularVelocityY           << ", "
			<< dGranularTemperatureAngularVelocityZ           << std::endl;
		FileAverageInformation.close();

		delete oWorld;
	}
	else if ((arg[1] == "-avg_info_yz") && (arg.size() == 10))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[3], "%lg", &dXMin);
		sscanf(argv[4], "%lg", &dXMax);
		sscanf(argv[5], "%lg", &dYMin);
		sscanf(argv[6], "%lg", &dYMax);
		sscanf(argv[7], "%lg", &dZMin);
		sscanf(argv[8], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[9], "%lg", &dMeshSize);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld->GetSystemParameter()->GetDONumber())
			{
				pDOS = oWorld->GetDOStatus(ul);
				if
					(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType()
					 == vedo::Sphere                                            )
				{
					dRadius
						= oWorld
						->GetDOModel(pDOS->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS->GetPosition().x() - dRadius;
					dNewXMax = pDOS->GetPosition().x() + dRadius;
					dNewYMin = pDOS->GetPosition().y() - dRadius;
					dNewYMax = pDOS->GetPosition().y() + dRadius;
					dNewZMin = pDOS->GetPosition().z() - dRadius;
					dNewZMax = pDOS->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		double dProjectedArea = 0.0;
		std::vector<double> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		double dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<double, njr::Vector3d> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = pDOS->GetVelocity();
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if(pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		double dProjectedAreaRatio
			= dProjectedArea / BC.GetLength().y() / OriginalBC.GetLength().z();
		if(dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(OriginalBC.GetCenter().x()     ,
				 vCentroid.y() / dTotalWeighting,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		};

		double dGranularTemperatureVelocity         = 0.0;
		double dGranularTemperatureVelocityX        = 0.0;
		double dGranularTemperatureVelocityY        = 0.0;
		double dGranularTemperatureVelocityZ        = 0.0;
		double dGranularTemperatureAngularVelocity  = 0.0;
		double dGranularTemperatureAngularVelocityX = 0.0;
		double dGranularTemperatureAngularVelocityY = 0.0;
		double dGranularTemperatureAngularVelocityZ = 0.0;
		if(dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for(unsigned long ul=0; ul<vProjectedArea.size(); ul++)
			{
				vVTemp1
					= vvAverageVelocity[ul] - vAverageVelocity;
				vVTemp2
					= vvAverageAngularVelocity[ul] - vAverageAngularVelocity;
				dGranularTemperatureVelocityX        += vProjectedArea[ul] * vVTemp1.x() * vVTemp1.x();
				dGranularTemperatureVelocityY        += vProjectedArea[ul] * vVTemp1.y() * vVTemp1.y();
				dGranularTemperatureVelocityZ        += vProjectedArea[ul] * vVTemp1.z() * vVTemp1.z();
				dGranularTemperatureAngularVelocityX += vProjectedArea[ul] * vVTemp2.x() * vVTemp2.x();
				dGranularTemperatureAngularVelocityY += vProjectedArea[ul] * vVTemp2.y() * vVTemp2.y();
				dGranularTemperatureAngularVelocityZ += vProjectedArea[ul] * vVTemp2.z() * vVTemp2.z();
			};
			dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityZ
				= dGranularTemperatureVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityZ
				= dGranularTemperatureAngularVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureVelocity
				= dGranularTemperatureVelocityX
				+ dGranularTemperatureVelocityY
				+ dGranularTemperatureVelocityZ;
			dGranularTemperatureAngularVelocity
				= dGranularTemperatureAngularVelocityX
				+ dGranularTemperatureAngularVelocityY
				+ dGranularTemperatureAngularVelocityZ;
			if(dGranularTemperatureVelocity != 0.0)
			{
				dGranularTemperatureVelocityX /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityY /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityZ /= dGranularTemperatureVelocity;
			}
			else
			{
				dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityZ = 0.0;
			};
			if(dGranularTemperatureAngularVelocity != 0.0)
			{
				dGranularTemperatureAngularVelocityX
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityY
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityZ
					/= dGranularTemperatureAngularVelocity;
			}
			else
			{
				dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityZ = 0.0;
			};
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.Set(0.0, 0.0, 0.0);
		}

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time, "
			<< "BoxCenterY, "
			<< "BoxCenterZ, "
			<< "BoxSizeY, "
			<< "BoxSizeZ, "
			<< "CentroidY, "
			<< "CentroidZ, "
			<< "ElementNumber, "
			<< "ProjectedArea, "
			<< "ProjectedAreaRatio, "
			<< "AverageVelocityX, "
			<< "AverageVelocityY, "
			<< "AverageVelocityZ, "
			<< "AverageAngularVelocityX, "
			<< "AverageAngularVelocityY, "
			<< "AverageAngularVelocityZ, "
			<< "GranularTemperatureV, "
			<< "GranularTemperatureVxOverV, "
			<< "GranularTemperatureVyOverV, "
			<< "GranularTemperatureVzOverV, "
			<< "GranularTemperatureAV, "
			<< "GranularTemperatureAVxOverAV, "
			<< "GranularTemperatureAVyOverAV, "
			<< "GranularTemperatureAVzOverAV" << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().y()                     << ", "
			<< OriginalBC.GetCenter().z()                     << ", "
			<< OriginalBC.GetRange().y()                      << ", "
			<< OriginalBC.GetRange().z()                      << ", "
			<< vCentroid.y()                                  << ", "
			<< vCentroid.z()                                  << ", "
			<< vProjectedArea.size()                          << ", "
			<< dProjectedArea                                 << ", "
			<< dProjectedAreaRatio                            << ", "
			<< vAverageVelocity.x()                           << ", "
			<< vAverageVelocity.y()                           << ", "
			<< vAverageVelocity.z()                           << ", "
			<< vAverageAngularVelocity.x()                    << ", "
			<< vAverageAngularVelocity.y()                    << ", "
			<< vAverageAngularVelocity.z()                    << ", "
			<< dGranularTemperatureVelocity                   << ", "
			<< dGranularTemperatureVelocityX                  << ", "
			<< dGranularTemperatureVelocityY                  << ", "
			<< dGranularTemperatureVelocityZ                  << ", "
			<< dGranularTemperatureAngularVelocity            << ", "
			<< dGranularTemperatureAngularVelocityX           << ", "
			<< dGranularTemperatureAngularVelocityY           << ", "
			<< dGranularTemperatureAngularVelocityZ           << std::endl;
		FileAverageInformation.close();

		delete oWorld;
	}
	else if ((arg[1] == "-avg_info_yz") && (arg.size() == 11))
	{
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[3]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[4], "%lg", &dXMin);
		sscanf(argv[5], "%lg", &dXMax);
		sscanf(argv[6], "%lg", &dYMin);
		sscanf(argv[7], "%lg", &dYMax);
		sscanf(argv[8], "%lg", &dZMin);
		sscanf(argv[9], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[10], "%lg", &dMeshSize);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const double dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld->GetSystemParameter()->GetDONumber())
			{
				pDOS = oWorld->GetDOStatus(ul);
				if
					(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType()
					 == vedo::Sphere                                            )
				{
					dRadius
						= oWorld
						->GetDOModel(pDOS->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS->GetPosition().x() - dRadius;
					dNewXMax = pDOS->GetPosition().x() + dRadius;
					dNewYMin = pDOS->GetPosition().y() - dRadius;
					dNewYMax = pDOS->GetPosition().y() + dRadius;
					dNewZMin = pDOS->GetPosition().z() - dRadius;
					dNewZMax = pDOS->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		double dProjectedArea = 0.0;
		std::vector<double> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		double dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<double, njr::Vector3d> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = 1.0 / dTimeStep
				                   * (  pDOSNextStep->GetPosition()
								      - pDOS->GetPosition()        );
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if(pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		double dProjectedAreaRatio
			= dProjectedArea / BC.GetLength().y() / OriginalBC.GetLength().z();
		if(dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(OriginalBC.GetCenter().x()     ,
				 vCentroid.y() / dTotalWeighting,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		};

		double dGranularTemperatureVelocity         = 0.0;
		double dGranularTemperatureVelocityX        = 0.0;
		double dGranularTemperatureVelocityY        = 0.0;
		double dGranularTemperatureVelocityZ        = 0.0;
		double dGranularTemperatureAngularVelocity  = 0.0;
		double dGranularTemperatureAngularVelocityX = 0.0;
		double dGranularTemperatureAngularVelocityY = 0.0;
		double dGranularTemperatureAngularVelocityZ = 0.0;
		if(dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for(unsigned long ul=0; ul<vProjectedArea.size(); ul++)
			{
				vVTemp1
					= vvAverageVelocity[ul] - vAverageVelocity;
				vVTemp2
					= vvAverageAngularVelocity[ul] - vAverageAngularVelocity;
				dGranularTemperatureVelocityX        += vProjectedArea[ul] * vVTemp1.x() * vVTemp1.x();
				dGranularTemperatureVelocityY        += vProjectedArea[ul] * vVTemp1.y() * vVTemp1.y();
				dGranularTemperatureVelocityZ        += vProjectedArea[ul] * vVTemp1.z() * vVTemp1.z();
				dGranularTemperatureAngularVelocityX += vProjectedArea[ul] * vVTemp2.x() * vVTemp2.x();
				dGranularTemperatureAngularVelocityY += vProjectedArea[ul] * vVTemp2.y() * vVTemp2.y();
				dGranularTemperatureAngularVelocityZ += vProjectedArea[ul] * vVTemp2.z() * vVTemp2.z();
			};
			dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityZ
				= dGranularTemperatureVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityZ
				= dGranularTemperatureAngularVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureVelocity
				= dGranularTemperatureVelocityX
				+ dGranularTemperatureVelocityY
				+ dGranularTemperatureVelocityZ;
			dGranularTemperatureAngularVelocity
				= dGranularTemperatureAngularVelocityX
				+ dGranularTemperatureAngularVelocityY
				+ dGranularTemperatureAngularVelocityZ;
			if(dGranularTemperatureVelocity != 0.0)
			{
				dGranularTemperatureVelocityX /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityY /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityZ /= dGranularTemperatureVelocity;
			}
			else
			{
				dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityZ = 0.0;
			};
			if(dGranularTemperatureAngularVelocity != 0.0)
			{
				dGranularTemperatureAngularVelocityX
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityY
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityZ
					/= dGranularTemperatureAngularVelocity;
			}
			else
			{
				dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityZ = 0.0;
			};
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.Set(0.0, 0.0, 0.0);
		}

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time, "
			<< "BoxCenterY, "
			<< "BoxCenterZ, "
			<< "BoxSizeY, "
			<< "BoxSizeZ, "
			<< "CentroidY, "
			<< "CentroidZ, "
			<< "ElementNumber, "
			<< "ProjectedArea, "
			<< "ProjectedAreaRatio, "
			<< "AverageVelocityX, "
			<< "AverageVelocityY, "
			<< "AverageVelocityZ, "
			<< "AverageAngularVelocityX, "
			<< "AverageAngularVelocityY, "
			<< "AverageAngularVelocityZ, "
			<< "GranularTemperatureV, "
			<< "GranularTemperatureVxOverV, "
			<< "GranularTemperatureVyOverV, "
			<< "GranularTemperatureVzOverV, "
			<< "GranularTemperatureAV, "
			<< "GranularTemperatureAVxOverAV, "
			<< "GranularTemperatureAVyOverAV, "
			<< "GranularTemperatureAVzOverAV" << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().y()                     << ", "
			<< OriginalBC.GetCenter().z()                     << ", "
			<< OriginalBC.GetRange().y()                      << ", "
			<< OriginalBC.GetRange().z()                      << ", "
			<< vCentroid.y()                                  << ", "
			<< vCentroid.z()                                  << ", "
			<< vProjectedArea.size()                          << ", "
			<< dProjectedArea                                 << ", "
			<< dProjectedAreaRatio                            << ", "
			<< vAverageVelocity.x()                           << ", "
			<< vAverageVelocity.y()                           << ", "
			<< vAverageVelocity.z()                           << ", "
			<< vAverageAngularVelocity.x()                    << ", "
			<< vAverageAngularVelocity.y()                    << ", "
			<< vAverageAngularVelocity.z()                    << ", "
			<< dGranularTemperatureVelocity                   << ", "
			<< dGranularTemperatureVelocityX                  << ", "
			<< dGranularTemperatureVelocityY                  << ", "
			<< dGranularTemperatureVelocityZ                  << ", "
			<< dGranularTemperatureAngularVelocity            << ", "
			<< dGranularTemperatureAngularVelocityX           << ", "
			<< dGranularTemperatureAngularVelocityY           << ", "
			<< dGranularTemperatureAngularVelocityZ           << std::endl;
		FileAverageInformation.close();

		delete oWorld;
	}
	else if ((arg[1] == "-avg_info_xz") && (arg.size() == 10))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[3], "%lg", &dXMin);
		sscanf(argv[4], "%lg", &dXMax);
		sscanf(argv[5], "%lg", &dYMin);
		sscanf(argv[6], "%lg", &dYMax);
		sscanf(argv[7], "%lg", &dZMin);
		sscanf(argv[8], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[9], "%lg", &dMeshSize);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld->GetSystemParameter()->GetDONumber())
			{
				pDOS = oWorld->GetDOStatus(ul);
				if
					(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType()
					 == vedo::Sphere                                            )
				{
					dRadius
						= oWorld
						->GetDOModel(pDOS->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS->GetPosition().x() - dRadius;
					dNewXMax = pDOS->GetPosition().x() + dRadius;
					dNewYMin = pDOS->GetPosition().y() - dRadius;
					dNewYMax = pDOS->GetPosition().y() + dRadius;
					dNewZMin = pDOS->GetPosition().z() - dRadius;
					dNewZMax = pDOS->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		double dProjectedArea = 0.0;
		std::vector<double> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		double dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<double, njr::Vector3d> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = pDOS->GetVelocity();
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if(pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		double dProjectedAreaRatio
			= dProjectedArea / OriginalBC.GetLength().x() / OriginalBC.GetLength().z();
		if(dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 OriginalBC.GetCenter().y()     ,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		};

		double dGranularTemperatureVelocity         = 0.0;
		double dGranularTemperatureVelocityX        = 0.0;
		double dGranularTemperatureVelocityY        = 0.0;
		double dGranularTemperatureVelocityZ        = 0.0;
		double dGranularTemperatureAngularVelocity  = 0.0;
		double dGranularTemperatureAngularVelocityX = 0.0;
		double dGranularTemperatureAngularVelocityY = 0.0;
		double dGranularTemperatureAngularVelocityZ = 0.0;
		if(dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for(unsigned long ul=0; ul<vProjectedArea.size(); ul++)
			{
				vVTemp1
					= vvAverageVelocity[ul] - vAverageVelocity;
				vVTemp2
					= vvAverageAngularVelocity[ul] - vAverageAngularVelocity;
				dGranularTemperatureVelocityX        += vProjectedArea[ul] * vVTemp1.x() * vVTemp1.x();
				dGranularTemperatureVelocityY        += vProjectedArea[ul] * vVTemp1.y() * vVTemp1.y();
				dGranularTemperatureVelocityZ        += vProjectedArea[ul] * vVTemp1.z() * vVTemp1.z();
				dGranularTemperatureAngularVelocityX += vProjectedArea[ul] * vVTemp2.x() * vVTemp2.x();
				dGranularTemperatureAngularVelocityY += vProjectedArea[ul] * vVTemp2.y() * vVTemp2.y();
				dGranularTemperatureAngularVelocityZ += vProjectedArea[ul] * vVTemp2.z() * vVTemp2.z();
			};
			dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityZ
				= dGranularTemperatureVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityZ
				= dGranularTemperatureAngularVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureVelocity
				= dGranularTemperatureVelocityX
				+ dGranularTemperatureVelocityY
				+ dGranularTemperatureVelocityZ;
			dGranularTemperatureAngularVelocity
				= dGranularTemperatureAngularVelocityX
				+ dGranularTemperatureAngularVelocityY
				+ dGranularTemperatureAngularVelocityZ;
			if(dGranularTemperatureVelocity != 0.0)
			{
				dGranularTemperatureVelocityX /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityY /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityZ /= dGranularTemperatureVelocity;
			}
			else
			{
				dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityZ = 0.0;
			};
			if(dGranularTemperatureAngularVelocity != 0.0)
			{
				dGranularTemperatureAngularVelocityX
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityY
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityZ
					/= dGranularTemperatureAngularVelocity;
			}
			else
			{
				dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityZ = 0.0;
			};
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.Set(0.0, 0.0, 0.0);
		}

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time, "
			<< "BoxCenterX, "
			<< "BoxCenterZ, "
			<< "BoxSizeX, "
			<< "BoxSizeZ, "
			<< "CentroidX, "
			<< "CentroidZ, "
			<< "ElementNumber, "
			<< "ProjectedArea, "
			<< "ProjectedAreaRatio, "
			<< "AverageVelocityX, "
			<< "AverageVelocityY, "
			<< "AverageVelocityZ, "
			<< "AverageAngularVelocityX, "
			<< "AverageAngularVelocityY, "
			<< "AverageAngularVelocityZ, "
			<< "GranularTemperatureV, "
			<< "GranularTemperatureVxOverV, "
			<< "GranularTemperatureVyOverV, "
			<< "GranularTemperatureVzOverV, "
			<< "GranularTemperatureAV, "
			<< "GranularTemperatureAVxOverAV, "
			<< "GranularTemperatureAVyOverAV, "
			<< "GranularTemperatureAVzOverAV" << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().x()                     << ", "
			<< OriginalBC.GetCenter().z()                     << ", "
			<< OriginalBC.GetRange().x()                      << ", "
			<< OriginalBC.GetRange().z()                      << ", "
			<< vCentroid.x()                                  << ", "
			<< vCentroid.z()                                  << ", "
			<< vProjectedArea.size()                          << ", "
			<< dProjectedArea                                 << ", "
			<< dProjectedAreaRatio                            << ", "
			<< vAverageVelocity.x()                           << ", "
			<< vAverageVelocity.y()                           << ", "
			<< vAverageVelocity.z()                           << ", "
			<< vAverageAngularVelocity.x()                    << ", "
			<< vAverageAngularVelocity.y()                    << ", "
			<< vAverageAngularVelocity.z()                    << ", "
			<< dGranularTemperatureVelocity                   << ", "
			<< dGranularTemperatureVelocityX                  << ", "
			<< dGranularTemperatureVelocityY                  << ", "
			<< dGranularTemperatureVelocityZ                  << ", "
			<< dGranularTemperatureAngularVelocity            << ", "
			<< dGranularTemperatureAngularVelocityX           << ", "
			<< dGranularTemperatureAngularVelocityY           << ", "
			<< dGranularTemperatureAngularVelocityZ           << std::endl;
		FileAverageInformation.close();

		delete oWorld;
	}
	else if ((arg[1] == "-avg_info_xz") && (arg.size() == 11))
	{
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[3]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[4], "%lg", &dXMin);
		sscanf(argv[5], "%lg", &dXMax);
		sscanf(argv[6], "%lg", &dYMin);
		sscanf(argv[7], "%lg", &dYMax);
		sscanf(argv[8], "%lg", &dZMin);
		sscanf(argv[9], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[10], "%lg", &dMeshSize);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const double dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld->GetSystemParameter()->GetDONumber())
			{
				pDOS = oWorld->GetDOStatus(ul);
				if
					(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType()
					 == vedo::Sphere                                            )
				{
					dRadius
						= oWorld
						->GetDOModel(pDOS->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS->GetPosition().x() - dRadius;
					dNewXMax = pDOS->GetPosition().x() + dRadius;
					dNewYMin = pDOS->GetPosition().y() - dRadius;
					dNewYMax = pDOS->GetPosition().y() + dRadius;
					dNewZMin = pDOS->GetPosition().z() - dRadius;
					dNewZMax = pDOS->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		double dProjectedArea = 0.0;
		std::vector<double> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		double dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<double, njr::Vector3d> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = 1.0 / dTimeStep
				                   * (  pDOSNextStep->GetPosition()
								      - pDOS->GetPosition()        );
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if(pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		double dProjectedAreaRatio
			= dProjectedArea / OriginalBC.GetLength().x() / OriginalBC.GetLength().z();
		if(dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 OriginalBC.GetCenter().y()     ,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		};

		double dGranularTemperatureVelocity         = 0.0;
		double dGranularTemperatureVelocityX        = 0.0;
		double dGranularTemperatureVelocityY        = 0.0;
		double dGranularTemperatureVelocityZ        = 0.0;
		double dGranularTemperatureAngularVelocity  = 0.0;
		double dGranularTemperatureAngularVelocityX = 0.0;
		double dGranularTemperatureAngularVelocityY = 0.0;
		double dGranularTemperatureAngularVelocityZ = 0.0;
		if(dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for(unsigned long ul=0; ul<vProjectedArea.size(); ul++)
			{
				vVTemp1
					= vvAverageVelocity[ul] - vAverageVelocity;
				vVTemp2
					= vvAverageAngularVelocity[ul] - vAverageAngularVelocity;
				dGranularTemperatureVelocityX        += vProjectedArea[ul] * vVTemp1.x() * vVTemp1.x();
				dGranularTemperatureVelocityY        += vProjectedArea[ul] * vVTemp1.y() * vVTemp1.y();
				dGranularTemperatureVelocityZ        += vProjectedArea[ul] * vVTemp1.z() * vVTemp1.z();
				dGranularTemperatureAngularVelocityX += vProjectedArea[ul] * vVTemp2.x() * vVTemp2.x();
				dGranularTemperatureAngularVelocityY += vProjectedArea[ul] * vVTemp2.y() * vVTemp2.y();
				dGranularTemperatureAngularVelocityZ += vProjectedArea[ul] * vVTemp2.z() * vVTemp2.z();
			};
			dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureVelocityZ
				= dGranularTemperatureVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityX / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityY / 3.0 / dProjectedArea;
			dGranularTemperatureAngularVelocityZ
				= dGranularTemperatureAngularVelocityZ / 3.0 / dProjectedArea;
			dGranularTemperatureVelocity
				= dGranularTemperatureVelocityX
				+ dGranularTemperatureVelocityY
				+ dGranularTemperatureVelocityZ;
			dGranularTemperatureAngularVelocity
				= dGranularTemperatureAngularVelocityX
				+ dGranularTemperatureAngularVelocityY
				+ dGranularTemperatureAngularVelocityZ;
			if(dGranularTemperatureVelocity != 0.0)
			{
				dGranularTemperatureVelocityX /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityY /= dGranularTemperatureVelocity;
				dGranularTemperatureVelocityZ /= dGranularTemperatureVelocity;
			}
			else
			{
				dGranularTemperatureVelocityX
				= dGranularTemperatureVelocityY
				= dGranularTemperatureVelocityZ = 0.0;
			};
			if(dGranularTemperatureAngularVelocity != 0.0)
			{
				dGranularTemperatureAngularVelocityX
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityY
					/= dGranularTemperatureAngularVelocity;
				dGranularTemperatureAngularVelocityZ
					/= dGranularTemperatureAngularVelocity;
			}
			else
			{
				dGranularTemperatureAngularVelocityX
				= dGranularTemperatureAngularVelocityY
				= dGranularTemperatureAngularVelocityZ = 0.0;
			};
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.Set(0.0, 0.0, 0.0);
		}

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time, "
			<< "BoxCenterX, "
			<< "BoxCenterZ, "
			<< "BoxSizeX, "
			<< "BoxSizeZ, "
			<< "CentroidX, "
			<< "CentroidZ, "
			<< "ElementNumber, "
			<< "ProjectedArea, "
			<< "ProjectedAreaRatio, "
			<< "AverageVelocityX, "
			<< "AverageVelocityY, "
			<< "AverageVelocityZ, "
			<< "AverageAngularVelocityX, "
			<< "AverageAngularVelocityY, "
			<< "AverageAngularVelocityZ, "
			<< "GranularTemperatureV, "
			<< "GranularTemperatureVxOverV, "
			<< "GranularTemperatureVyOverV, "
			<< "GranularTemperatureVzOverV, "
			<< "GranularTemperatureAV, "
			<< "GranularTemperatureAVxOverAV, "
			<< "GranularTemperatureAVyOverAV, "
			<< "GranularTemperatureAVzOverAV" << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().x()                     << ", "
			<< OriginalBC.GetCenter().z()                     << ", "
			<< OriginalBC.GetRange().x()                      << ", "
			<< OriginalBC.GetRange().z()                      << ", "
			<< vCentroid.x()                                  << ", "
			<< vCentroid.z()                                  << ", "
			<< vProjectedArea.size()                          << ", "
			<< dProjectedArea                                 << ", "
			<< dProjectedAreaRatio                            << ", "
			<< vAverageVelocity.x()                           << ", "
			<< vAverageVelocity.y()                           << ", "
			<< vAverageVelocity.z()                           << ", "
			<< vAverageAngularVelocity.x()                    << ", "
			<< vAverageAngularVelocity.y()                    << ", "
			<< vAverageAngularVelocity.z()                    << ", "
			<< dGranularTemperatureVelocity                   << ", "
			<< dGranularTemperatureVelocityX                  << ", "
			<< dGranularTemperatureVelocityY                  << ", "
			<< dGranularTemperatureVelocityZ                  << ", "
			<< dGranularTemperatureAngularVelocity            << ", "
			<< dGranularTemperatureAngularVelocityX           << ", "
			<< dGranularTemperatureAngularVelocityY           << ", "
			<< dGranularTemperatureAngularVelocityZ           << std::endl;
		FileAverageInformation.close();

		delete oWorld;
	}
	else if ((arg[1] == "-avg_rd_ntume") && (arg.size() == 11))
	{
		vedo::DOWorld* oWorld_1 = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorld_2 = ReadDOWorld(arg[3]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[4], "%lg", &dXMin);
		sscanf(argv[5], "%lg", &dXMax);
		sscanf(argv[6], "%lg", &dYMin);
		sscanf(argv[7], "%lg", &dYMax);
		sscanf(argv[8], "%lg", &dZMin);
		sscanf(argv[9], "%lg", &dZMax);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		sscanf(argv[10], "%lg", &dMeshSize);

		const vedo::DOStatus* pDOS_1;
		const vedo::DOStatus* pDOS_2;

		// Fix boundary (stage 1: find first data)
		double
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		unsigned long ul = 0;
		do
		{
			if(ul < oWorld_1->GetSystemParameter()->GetDONumber())
			{
				pDOS_1 = oWorld_1->GetDOStatus(ul);
				if
					(oWorld_1->GetDOModel(pDOS_1->GetDOName())->GetShapeType()
					 == vedo::Sphere                                                )
				{
					dRadius
						= oWorld_1
						->GetDOModel(pDOS_1->GetDOName())
						->GetShapeAttributes().sphere.radius;
					dNewXMin = pDOS_1->GetPosition().x() - dRadius;
					dNewXMax = pDOS_1->GetPosition().x() + dRadius;
					dNewYMin = pDOS_1->GetPosition().y() - dRadius;
					dNewYMax = pDOS_1->GetPosition().y() + dRadius;
					dNewZMin = pDOS_1->GetPosition().z() - dRadius;
					dNewZMax = pDOS_1->GetPosition().z() + dRadius;
					bHasFindThe1stSphere = true;
				}
				ul++;
			}
			else
			{
				bHasFindThe1stSphere = true;
			}
		} while(!bHasFindThe1stSphere);

		// Fix boundary (stage 2: fixed boundary)
		for(ul++;
			ul<oWorld_1->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS_1 = oWorld_1->GetDOStatus(ul);
			if(oWorld_1->GetDOModel(pDOS_1->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				dRadius
					= oWorld_1
					->GetDOModel(pDOS_1->GetDOName())
					->GetShapeAttributes().sphere.radius;
				dNewXMin = std::min(pDOS_1->GetPosition().x() - dRadius, dNewXMin);
				dNewXMax = std::max(pDOS_1->GetPosition().x() + dRadius, dNewXMax);
				dNewYMin = std::min(pDOS_1->GetPosition().y() - dRadius, dNewYMin);
				dNewYMax = std::max(pDOS_1->GetPosition().y() + dRadius, dNewYMax);
				dNewZMin = std::min(pDOS_1->GetPosition().z() - dRadius, dNewZMin);
				dNewZMax = std::max(pDOS_1->GetPosition().z() + dRadius, dNewZMax);
			}
		};
		if(dXMin < dNewXMin) dXMin = dNewXMin;
		if(dXMax > dNewXMax) dXMax = dNewXMax;
		if(dYMin < dNewYMin) dYMin = dNewYMin;
		if(dYMax > dNewYMax) dYMax = dNewYMax;
		if(dZMin < dNewZMin) dZMin = dNewZMin;
		if(dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		double dProjectedArea = 0.0;
		std::vector<double> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		double dWeighting;
		double dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity;

		njr::Vector3d vTempVelocity;

		double dTimeFrequency
			= oWorld_2->GetSystemParameter()->GetTimeCurrent()
			- oWorld_1->GetSystemParameter()->GetTimeCurrent();
		if (dTimeFrequency <= 0.0)
		{
			std::cout
				<< "Error!! Code: Knight -avg_rd_ntume" << std::endl
				<< "        Note: The order of <.xml | .ido> file error" << std::endl;
			exit(0);
		}
		dTimeFrequency = 1.0 / dTimeFrequency;

		for
			(unsigned long ul=0;
			 ul<oWorld_1->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS_1 = oWorld_1->GetDOStatus(ul);
			pDOS_2 = oWorld_2->GetDOStatus(ul);
			dom    = oWorld_1->GetDOModel(pDOS_1->GetDOName());

			std::pair<double, njr::Vector3d> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS_1->GetPosition(), &BC, dMeshSize);

			dWeighting    = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity = dTimeFrequency * (pDOS_2->GetPosition() - pDOS_1->GetPosition());
			vvAverageVelocity.push_back(vTempVelocity);
			if(pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea   += pdvProjectedArea.first;
				dTotalWeighting  += pdvProjectedArea.first * dWeighting;
				vCentroid        += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity += pdvProjectedArea.first * vTempVelocity;
			}
		}

		if(dProjectedArea != 0.0)
		{
			vAverageVelocity = 1.0 / dProjectedArea * vAverageVelocity;
		}
		else
		{
			vAverageVelocity.Set(0.0, 0.0, 0.0);
		}

		if(dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 vCentroid.y() / dTotalWeighting,
				 OriginalBC.GetCenter().z()      );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		};

		std::ofstream FileAverageInformation;
		FileAverageInformation.open("AverageInformation.csv", std::ios::app);
		FileAverageInformation
			<< "Time_1, Time_2, "
			<< "BoxCenterX, "
			<< "BoxCenterY, "
			<< "BoxSizeX, "
			<< "BoxSizeY, "
			<< "CentroidX, "
			<< "CentroidY, "
			<< "ElementNumber, "
			<< "ProjectedArea, "
			<< "AverageVelocityX, "
			<< "AverageVelocityY, "
			<< "AverageVelocityZ" << std::endl
			<< oWorld_1->GetSystemParameter()->GetTimeCurrent() << ", "
			<< oWorld_2->GetSystemParameter()->GetTimeCurrent() << ", "
			<< OriginalBC.GetCenter().x()                       << ", "
			<< OriginalBC.GetCenter().y()                       << ", "
			<< OriginalBC.GetRange().x()                        << ", "
			<< OriginalBC.GetRange().y()                        << ", "
			<< vCentroid.x()                                    << ", "
			<< vCentroid.y()                                    << ", "
			<< vProjectedArea.size()                            << ", "
			<< dProjectedArea                                   << ", "
			<< vAverageVelocity.x()                             << ", "
			<< vAverageVelocity.y()                             << ", "
			<< vAverageVelocity.z()                             << std::endl;
		FileAverageInformation.close();

		delete oWorld_1, oWorld_2;
	}
	else if ((arg[1] == "-c") && (arg.size() == 4))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();

		if (njr::CheckSubName(arg[3], ".csv"))
		{
            delete ReadDOWorld(arg[2], pIactRecordTab);
			pIactRecordTab->WriteCSV(arg[3]);
		}
        else
        {
            delete WriteDOWorld(arg[3], ReadDOWorld(arg[2], pIactRecordTab), pIactRecordTab);
        }

        delete pIactRecordTab;
	}
	else if ((arg[1] == "-ca2s") && (arg.size() == 8))
	{
		std::string DOName = argv[2];
		double a, b, c, d;
		sscanf(argv[3], "%lg", &a);
		sscanf(argv[4], "%lg", &b);
		sscanf(argv[5], "%lg", &c);
		sscanf(argv[6], "%lg", &d);
		vedo::DOWorld* oWorld    = ReadDOWorld(arg[7]);
		unsigned counter   = 0;
		double   dArea     = 0.0;
		double   dAreaTemp;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			if(oWorld->GetDOStatus(ul)->GetDOName() == DOName)
			{
				dAreaTemp
					= oWorld
					->GetDOModel(DOName)
					->CrossAreaToSurface
						(oWorld->GetDOStatus(ul)->GetPosition(), a, b, c, d);
				if(dAreaTemp != 0.0)
				{
					counter++;
					dArea += dAreaTemp;
				}
			}
		}
		std::cout
			<< "Surface           : "
			<< a << "x+" << b << "y+" << c << "z=" << d << std::endl
			<< "Element's name    : " << DOName         << std::endl
			<< "Cross area        : " << dArea
			<< '(' << counter << " elements)"           << std::endl;
	}
	else if ((arg[1] == "-cf2s") && (arg.size() == 8))
	{
		std::string DOName = argv[2];
		double a, b, c, d;
		sscanf(argv[3], "%lg", &a);
		sscanf(argv[4], "%lg", &b);
		sscanf(argv[5], "%lg", &c);
		sscanf(argv[6], "%lg", &d);
		vedo::DOWorld*    oWorld        = ReadDOWorld(arg[7]);
		unsigned    counter       = 0;
		double      dArea         = 0.0;
		double      dAreaTemp;
		njr::Vector3d vFluxVelocity, vFluxAngularVelocity;
		for
			(unsigned long ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			if(oWorld->GetDOStatus(ul)->GetDOName() == DOName)
			{
				dAreaTemp
					= oWorld
					->GetDOModel(DOName)
					->CrossAreaToSurface
						(oWorld->GetDOStatus(ul)->GetPosition(), a, b, c, d);
				if(dAreaTemp != 0.0)
				{
					counter++;
					dArea += dAreaTemp;
					vFluxVelocity
						+= dAreaTemp * oWorld->GetDOStatus(ul)->GetVelocity();
					vFluxAngularVelocity
						+= dAreaTemp
						 * oWorld->GetDOStatus(ul)->GetAngularVelocity();
				}
			}
		}
 		std::ofstream FileFlux;
		FileFlux.open("Flux.csv", std::ios::out);
		FileFlux
			<< "Time"                                         << ", "
			<< "a"                                            << ", "
			<< "b"                                            << ", "
			<< "c"                                            << ", "
			<< "d"                                            << ", "
			<< "ElementNumber"                                << ", "
			<< "CrossArea"                                    << ", "
			<< "FluxVelocity"                                 << ", "
			<< "FluxVelocityX"                                << ", "
			<< "FluxVelocityY"                                << ", "
			<< "FluxVelocityZ"                                << ", "
			<< "FluxAngularVelocity"                          << ", "
			<< "FluxAngularVelocityX"                         << ", "
			<< "FluxAngularVelocityY"                         << ", "
			<< "FluxAngularVelocityZ"                         << std::endl
			<< oWorld->GetSystemParameter()->GetTimeCurrent() << ", "
			<< a                                              << ", "
			<< b                                              << ", "
			<< c                                              << ", "
			<< d                                              << ", "
			<< counter                                        << ", "
			<< dArea                                          << ", "
			<< vFluxVelocity.length()                         << ", "
			<< vFluxVelocity.x()                              << ", "
			<< vFluxVelocity.y()                              << ", "
			<< vFluxVelocity.z()                              << ", "
			<< vFluxAngularVelocity.length()                  << ", "
			<< vFluxAngularVelocity.x()                       << ", "
			<< vFluxAngularVelocity.y()                       << ", "
			<< vFluxAngularVelocity.z()                       << std::endl;
		FileFlux.close();
	}
	else if ((arg[1] == "-cal") && (arg.size() == 9))
	{
		vedo::DOWorld *oWorld = ReadDOWorld(arg[2]);
		double xmin, xmax, ymin, ymax, zmin, zmax;
		CalcCoordinateNum CCN;
		CalcDensity CDN;
		sscanf(argv[3], "%lg", &xmin);
		sscanf(argv[4], "%lg", &xmax);
		sscanf(argv[5], "%lg", &ymin);
		sscanf(argv[6], "%lg", &ymax);
		sscanf(argv[7], "%lg", &zmin);
		sscanf(argv[8], "%lg", &zmax);
		std::cout
			<< "CoordinatNumber = "
			<< CCN.computeAvgCoordinateNum
				(oWorld, xmin, xmax, ymin, ymax, zmin, zmax)
			<< std::endl
			<< "CalculateDensity = "
			<< CDN.computeDensity(oWorld, xmin, xmax, ymin, ymax, zmin, zmax)
			<< std::endl;
	}
	else if ((arg[1] == "-coord_num") && (arg.size() == 4))
	{
		vedo::DOWorld *oWorld = ReadDOWorld(arg[2]);
		CalcCoordinateNum CCN;
		CCN.outputCoordinateNumVTK(oWorld, arg[3]);
	}
	else if ((arg[1] == "-cl") && (arg.size() == 5))
	{
		ConListHEX(arg[2], atol(arg[3].c_str()), atol(arg[4].c_str()));
	}
	else if ((arg[1] == "-clean_udv") && (arg.size() == 4))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
        vedo::DOWorld* oWorld = ReadDOWorld(arg[2], pIactRecordTab);
		pIactRecordTab->CleanUserDefinedValueInImpactStatus();
        delete WriteDOWorld(arg[3], oWorld, pIactRecordTab);
        delete oWorld;
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-cld") && (arg.size() == 5))
	{
		ConList(arg[2], atol(arg[3].c_str()), atol(arg[4].c_str()));
	}
	else if ((arg[1] == "-cm") && (arg.size() == 5))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* rWorld = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3], pIactRecordTab);
		oWorld->SetModels(rWorld);
		delete WriteDOWorld(arg[4], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-comb") && (arg.size() == 5))
	{
	    // Aries: We only combine the DOStatus of two DOWorlds.
	    // In the future, we should also combine their Interactions (IactRecordTab).
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* rWorld = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3], pIactRecordTab);
		CombineModels(rWorld, oWorld);
		delete WriteDOWorld(arg[4], rWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-comb") && (arg.size() == 6))
	{
	    // Aries: We only combine the DOStatus of two DOWorlds.
	    // In the future, we should also combine their Interactions (IactRecordTab).
		vedo::DOWorld* rWorld = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		std::string DOName = argv[4];
		CombineModels(rWorld, oWorld, DOName);
		delete WriteDOWorld(arg[5], rWorld);
	}
	else if ((arg[1] == "-cu") && (arg.size() == 5))
	{
		delete WriteDOWorldVPF(arg[4], ReadDOWorld(arg[2]), ReadDOWorld(arg[3]));
	}
	else if ((arg[1] == "-b2h") && (arg.size() == 4))
	{
		njr::bin2hex(arg[2], arg[3]);
	}
	else if ((arg[1] == "-d2u") && (arg.size() == 4))
	{
		njr::dos2unix(arg[2], arg[3]);
	}
	else if ((arg[1] == "-distance") && (arg.size() == 6))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dX, dY, dZ;
		sscanf(argv[3], "%lg", &dX);
		sscanf(argv[4], "%lg", &dY);
		sscanf(argv[5], "%lg", &dZ);
		njr::Vector3d vP(dX, dY, dZ);
		std::map<double, unsigned> mduDistance;

		for(unsigned u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
			mduDistance[(oWorld->GetDOStatus(u)->GetPosition() - vP).length()] = u;

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ObjectID, ObjectName, Distance, X, Y, Z" << std::endl;
		unsigned uRank = 1;
		for (std::map<double, unsigned>::iterator mduiDistance  = mduDistance.begin();
			                                 mduiDistance != mduDistance.end();
											 mduiDistance++                      )
		{
			ofDistance
				<< uRank << ", "
				<< mduiDistance->second << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetDOName() << ", "
				<< mduiDistance->first << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().x() << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().y() << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().z() << std::endl;
			uRank++;
		}
		ofDistance.close();
		delete oWorld;
	}
	else if ((arg[1] == "-distance_xy") && (arg.size() == 5))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dX, dY;
		sscanf(argv[3], "%lg", &dX);
		sscanf(argv[4], "%lg", &dY);
		njr::Vector3d vP(dX, dY, 0.0);
		njr::Vector3d vD;
		std::map<double, unsigned> mduDistance;

		for(unsigned u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.SetZ(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ObjectID, ObjectName, Distance, X, Y, Z" << std::endl;
		unsigned uRank = 1;
		for (std::map<double, unsigned>::iterator mduiDistance  = mduDistance.begin();
			                                 mduiDistance != mduDistance.end();
											 mduiDistance++                      )
		{
			ofDistance
				<< uRank << ", "
				<< mduiDistance->second << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetDOName() << ", "
				<< mduiDistance->first << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().x() << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().y() << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().z() << std::endl;
			uRank++;
		}
		ofDistance.close();
		delete oWorld;
	}
	else if ((arg[1] == "-distance_yz") && (arg.size() == 5))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dY, dZ;
		sscanf(argv[3], "%lg", &dY);
		sscanf(argv[4], "%lg", &dZ);
		njr::Vector3d vP(0.0, dY, dZ);
		njr::Vector3d vD;
		std::map<double, unsigned> mduDistance;

		for(unsigned u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.SetX(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ObjectID, ObjectName, Distance, X, Y, Z" << std::endl;
		unsigned uRank = 1;
		for (std::map<double, unsigned>::iterator mduiDistance  = mduDistance.begin();
			                                 mduiDistance != mduDistance.end();
											 mduiDistance++                      )
		{
			ofDistance
				<< uRank << ", "
				<< mduiDistance->second << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetDOName() << ", "
				<< mduiDistance->first << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().x() << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().y() << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().z() << std::endl;
			uRank++;
		}
		ofDistance.close();
		delete oWorld;
	}
	else if ((arg[1] == "-distance_zx") && (arg.size() == 5))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dX, dZ;
		sscanf(argv[3], "%lg", &dZ);
		sscanf(argv[4], "%lg", &dX);
		njr::Vector3d vP(dX, 0.0, dZ);
		njr::Vector3d vD;
		std::map<double, unsigned> mduDistance;

		for(unsigned u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.SetY(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ObjectID, ObjectName, Distance, X, Y, Z" << std::endl;
		unsigned uRank = 1;
		for (std::map<double, unsigned>::iterator mduiDistance  = mduDistance.begin();
			                                 mduiDistance != mduDistance.end();
											 mduiDistance++                      )
		{
			ofDistance
				<< uRank << ", "
				<< mduiDistance->second << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetDOName() << ", "
				<< mduiDistance->first << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().x() << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().y() << ", "
				<< oWorld->GetDOStatus(mduiDistance->second)->GetPosition().z() << std::endl;
			uRank++;
		}
		ofDistance.close();
		delete oWorld;
	}
	else if ((arg[1] == "-u2d") && (arg.size() == 4))
	{
		njr::unix2dos(arg[2], arg[3]);
	}
	else if ((arg[1] == "-cse") && (arg.size() == 4))
	{
        // Aries: We have not deal with the interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		oWorld->ClearSeparatedElements();
		delete WriteDOWorld (arg[3], oWorld);
	}
	else if ((arg[1] == "-distribution") && (arg.size() >= 11))
	{
		std::string DOName = argv[2];
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[3], "%lg", &dXMin);
		sscanf(argv[4], "%lg", &dXMax);
		sscanf(argv[5], "%lg", &dYMin);
		sscanf(argv[6], "%lg", &dYMax);
		sscanf(argv[7], "%lg", &dZMin);
		sscanf(argv[8], "%lg", &dZMax);
		sscanf(argv[9], "%lg", &dMeshSize);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[10]);
		oWorld
			->SetBoundary
			(vedo::Boundary
				(njr::Vector3d(dXMin, dYMin, dZMin),
				 njr::Vector3d(dXMax, dYMax, dZMax) ));
		std::pair<njr::Vector3d, njr::Vector3d> pDistribution
			= oWorld->Distribution(dMeshSize);
 		std::ofstream FileDistribution;
		FileDistribution.open("Distribution.csv", std::ios::out);
		FileDistribution
			<< "X, Y, Z, XMin, XMax, YMin, YMax, ZMin, ZMax" << std::endl
			<< 0.5 * (dXMin + dXMax)                         << ", "
			<< 0.5 * (dYMin + dYMax)                         << ", "
			<< 0.5 * (dZMin + dZMax)                         << ", "
			<< pDistribution.first.x()                       << ", "
			<< pDistribution.second.x()                      << ", "
			<< pDistribution.first.y()                       << ", "
			<< pDistribution.second.y()                      << ", "
			<< pDistribution.first.z()                       << ", "
			<< pDistribution.second.z()                      << std::endl;
		FileDistribution.close();
	}
	else if ((arg[1] == "-e") && (arg.size() == 5))
	{
	    // We have not deal with the interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		oWorld->EraseDOStatus(oWorld->FindDO(argv[2]));
		delete WriteDOWorld (arg[4], oWorld);
	}
	else if ((arg[1] == "-erase_single_element") && (arg.size() == 5))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[3], pIactRecordTab);
		unsigned uID;
		sscanf(argv[2], "%d", &uID);
		char* idofilename = argv[3];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		pConsultant->EraseElement(uID);
		delete WriteDOWorld(arg[4], pWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-erase_elements") && (arg.size() == 9))
	{
		std::string sDOName = argv[2];
		double da, db, dc, dd;
		sscanf(argv[3], "%lg", &da);
		sscanf(argv[4], "%lg", &db);
		sscanf(argv[5], "%lg", &dc);
		sscanf(argv[6], "%lg", &dd);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[7], pIactRecordTab);
		char* idofilename = argv[7];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<unsigned long> ulIDList;
		const vedo::DOStatus* dosp;
		njr::Vector3d vPosition;
		for
			(unsigned long ul=0;
			 ul<(pWorld->GetSystemParameter()->GetDONumber());
			 ul++                                           )
		{
			dosp = pWorld->GetDOStatus(ul);
			if(dosp->GetDOName() == sDOName)
			{
				vPosition = dosp->GetPosition();
				if(da*vPosition.x()+db*vPosition.y()+dc*vPosition.z()>dd)
				{
					ulIDList.push_back(ul);
				}
			}
		}
		pConsultant->EraseElements(ulIDList);
		delete WriteDOWorld(arg[8], pWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-energy") && (arg.size() == 3))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		oWorld->CalculateSystemEnergy();
		vedo::SystemParameter* sp = oWorld->GetSystemParameter();

		std::ofstream FileEnergy;
		FileEnergy.open("energy.txt", std::ios::out);
		FileEnergy
			<< "<Symbols>" << std::endl
			<< "\tST    : Simulated Time" << std::endl
			<< "\tSE    : System Energy (Total)" << std::endl
			<< "\tPE    : Potential Energy (Total)" << std::endl
			<< "\tKE    : Kinetic Energy (Total)" << std::endl
			<< "\tKET   : Kinetic Energy (Translation)" << std::endl
			<< "\tKER   : Kinetic Energy (Rotation)" << std::endl
			<< "\tMinV  : Minimal Velocity" << std::endl
			<< "\tMaxV  : Maximal Velocity" << std::endl
			<< "\tMinAV : Minimal Angular Velocity" << std::endl
			<< "\tMaxAV : Maximal Angular Velocity" << std::endl
			<< "\tNormV  : Norm of Momentum" << std::endl
			<< "\tNormAV : Norm of Angular Momentum" << std::endl
			<< std::endl
			<< "ST, SE, PE, KE, KET, KER, MinV, MaxV, MinAV, MaxAV, NormV, NormAV"
			<< std::endl
			<< sp->GetTimeCurrent()          << ", "
			<< sp->GetEnergyPotential()
			 + sp->GetEnergyTranslation()
			 + sp->GetEnergyRotation()       << ", "
			<< sp->GetEnergyPotential()      << ", "
			<< sp->GetEnergyTranslation()
			 + sp->GetEnergyRotation()       << ", "
			<< sp->GetEnergyTranslation()    << ", "
			<< sp->GetEnergyRotation()       << ", "
			<< sp->GetVelocityMin()          << ", "
			<< sp->GetVelocityMax()          << ", "
			<< sp->GetAngularVelocityMin()   << ", "
			<< sp->GetAngularVelocityMax()   << ", "
			<< sp->GetMomentumNorm()         << ", "
			<< sp->GetAngularMomentumNorm()  << std::endl;
		FileEnergy.close();
	}
	else if ((arg[1] == "-freeze") && (arg.size() == 4))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2], pIactRecordTab);
		oWorld->FreezeAllElements();
		delete WriteDOWorld (arg[3], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-freeze") && (arg.size() == 5))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3], pIactRecordTab);
		oWorld->FreezeElements(arg[2]);
		delete WriteDOWorld (arg[4], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-gt_auto") && (arg.size() == 10))
	{
		unsigned long CutNumX      = 0;
		unsigned long CutNumY      = 0;
		unsigned long CutNumZ      = 0;
		double        dAngle2XAxis = 0.0;
		double        dAngle2YAxis = 0.0;
		double        dAngle2ZAxis = 0.0;
		vedo::DOWorld* oWorld      = ReadDOWorld(arg[2]);
		std::string DOName   = argv[3];
		sscanf(argv[4] , "%d" , &CutNumX);
		sscanf(argv[5] , "%d" , &CutNumY);
		sscanf(argv[6] , "%d" , &CutNumZ);
		sscanf(argv[7] , "%lg", &dAngle2XAxis);
		sscanf(argv[8] , "%lg", &dAngle2YAxis);
		sscanf(argv[9] , "%lg", &dAngle2ZAxis);
		oWorld->Rotate
			(dAngle2XAxis*0.01745329261,
			 dAngle2YAxis*0.01745329261,
			 dAngle2ZAxis*0.01745329261 );

		const vedo::DOStatus* pDOS;
		unsigned long StartNumber = 0;
		while(   StartNumber < oWorld->GetSystemParameter()->GetDONumber()
			  && (oWorld->GetDOStatus(StartNumber)->GetDOName() != DOName))
		{
			StartNumber++;
		}
		njr::Vector3d PTemp(oWorld->GetDOStatus(StartNumber)->GetPosition());
		double XMin = PTemp.x();
		double YMin = PTemp.y();
		double ZMin = PTemp.z();
		double XMax = XMin;
		double YMax = YMin;
		double ZMax = ZMin;

		for(unsigned long ul=StartNumber+1;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			PTemp = oWorld->GetDOStatus(ul)->GetPosition();

			if(PTemp.x() > XMax)
			{
				XMax = PTemp.x();
			}
			else if(PTemp.x() < XMin)
			{
				XMin = PTemp.x();
			}

			if(PTemp.y() > YMax)
			{
				YMax = PTemp.y();
			}
			else if(PTemp.y() < YMin)
			{
				YMin = PTemp.y();
			}

			if(PTemp.z() > ZMax)
			{
				ZMax = PTemp.z();
			}
			else if(PTemp.z() < ZMin)
			{
				ZMin = PTemp.z();
			}
		};
		double dCoverRange = oWorld->GetDOModel(DOName)->GetRange();
		XMax += dCoverRange;
		YMax += dCoverRange;
		ZMax += dCoverRange;
		XMin -= dCoverRange;
		YMin -= dCoverRange;
		ZMin -= dCoverRange;
		double width  = (XMax - XMin) / (double)CutNumX;
		double length = (YMax - YMin) / (double)CutNumY;
		double height = (ZMax - ZMin) / (double)CutNumZ;
		vedo::GSRectangle space("Granular Temperature Space", width, length, height);
		double XCentral = 0.5 * (XMin + XMax);
		double YCentral = 0.5 * (YMin + YMax);
		double ZCentral = 0.5 * (ZMin + ZMax);

		unsigned long XCounter = 1;
		unsigned long YCounter;
		unsigned long ZCounter;

		std::map<std::string, double> gt;
		std::ofstream FileGT;
		FileGT.open("GranularTemperature.csv", std::ios::out);
		FileGT
			<< "Index, "
			<< "Time, "
			<< "TotalWidth, "
			<< "TotalLength, "
			<< "TotalHeight, "
			<< "ElementNumber, "
			<< "AergageVelocity, "
			<< "GranularTemperatureV, "
			<< "AergageAngularVelocity, "
			<< "GranularTemperatureAV"   << std::endl;
		for(double x=XMin+0.5*width; XCounter<=CutNumX; x+=width, XCounter++)
		{
			YCounter = 1;
			for
				(double y=YMin+0.5*length;
				 YCounter<=CutNumY;
				 y+=length, YCounter++    )
			{
				ZCounter = 1;
				for
					(double z=ZMin+0.5*height;
					 ZCounter<=CutNumZ;
					 z+=height, ZCounter++    )
				{
					space.SetStatus
						(new vedo::DOStatus
							("No Name",
							 njr::Vector3d(x, y, z),
							 njr::ZERO,
							 njr::AXIALX,
							 njr::AXIALZ,
							 njr::ZERO    ));
					gt = CalculateGranularTemperature(oWorld, space, DOName);
					FileGT
						<< "Zone-"
						<< XCounter << '-'
						<< YCounter << '-'
						<< ZCounter                                   << ", "
						<< gt["Time"]                                 << ", "
						<< XMax - XMin                                << ", "
						<< YMax - YMin                                << ", "
						<< ZMax - ZMin                                << ", "
						<< (unsigned long)gt["NumberOfElement"]       << ", "
						<< gt["AverageVelocity"]                      << ", "
						<< gt["GranularTemperatureOfVelocity"]        << ", "
						<< gt["AverageAngularVelocity"]               << ", "
						<< gt["GranularTemperatureOfAngularVelocity"] << std::endl;
				}
			}
			FileGT.close();
		};
		delete oWorld;
	}
	else if ((arg[1] == "-gt_rectangle") && (arg.size() == 16))
	{
		double width, length, height, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		std::string DOName = argv[3];
		sscanf(argv[4], "%lg", &width);
		sscanf(argv[5], "%lg", &length);
		sscanf(argv[6], "%lg", &height);
		sscanf(argv[7], "%lg", &x);
		sscanf(argv[8], "%lg", &y);
		sscanf(argv[9], "%lg", &z);
		sscanf(argv[10], "%lg", &Xx);
		sscanf(argv[11], "%lg", &Xy);
		sscanf(argv[12], "%lg", &Xz);
		sscanf(argv[13], "%lg", &Zx);
		sscanf(argv[14], "%lg", &Zy);
		sscanf(argv[15], "%lg", &Zz);
		vedo::GSRectangle space("Granular Temperature Space", width, length, height);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 njr::Vector3d(x, y, z),
				 njr::ZERO,
				 njr::Vector3d(Xx, Xy, Xz),
				 njr::Vector3d(Zx, Zy, Zz),
				 njr::ZERO       ));
		std::map<std::string, double> gt
			= CalculateGranularTemperature(oWorld, space, DOName);
		std::ofstream FileGT;
		FileGT.open("GranularTemperature.csv", std::ios::out);
		FileGT
			<< "Time, "
			<< "ElementNumber, "
			<< "AergageVelocity, "
			<< "GranularTemperatureV, "
			<< "AergageAngularVelocity, "
			<< "GranularTemperatureAV, "
			<< gt["Time"]                                  << ", "
			<< (unsigned long)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-gt_cylinder") && (arg.size() == 15))
	{
		double radius, height, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		std::string DOName = argv[3];
		sscanf(argv[4], "%lg", &radius);
		sscanf(argv[5], "%lg", &height);
		sscanf(argv[6], "%lg", &x);
		sscanf(argv[7], "%lg", &y);
		sscanf(argv[8], "%lg", &z);
		sscanf(argv[9], "%lg", &Xx);
		sscanf(argv[10], "%lg", &Xy);
		sscanf(argv[11], "%lg", &Xz);
		sscanf(argv[12], "%lg", &Zx);
		sscanf(argv[13], "%lg", &Zy);
		sscanf(argv[14], "%lg", &Zz);
		vedo::GSCylinder space("Granular Temperature Space", radius, height);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 njr::Vector3d(x, y, z),
				 njr::ZERO,
				 njr::Vector3d(Xx, Xy, Xz),
				 njr::Vector3d(Zx, Zy, Zz),
				 njr::ZERO       ));
		std::map<std::string, double> gt
			= CalculateGranularTemperature(oWorld, space, DOName);
		std::ofstream FileGT;
		FileGT.open("GranularTemperature.csv", std::ios::out);
		FileGT
			<< "Time, "
			<< "ElementNumber, "
			<< "AergageVelocity, "
			<< "GranularTemperatureV, "
			<< "AergageAngularVelocity, "
			<< "GranularTemperatureAV"                     << std::endl
			<< gt["Time"]                                  << ", "
			<< (unsigned long)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-gt_sphere") && (arg.size() == 14))
	{
		double radius, height, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		std::string DOName = argv[3];
		sscanf(argv[4], "%lg", &radius);
		sscanf(argv[5], "%lg", &x);
		sscanf(argv[6], "%lg", &y);
		sscanf(argv[7], "%lg", &z);
		sscanf(argv[8], "%lg", &Xx);
		sscanf(argv[9], "%lg", &Xy);
		sscanf(argv[10], "%lg", &Xz);
		sscanf(argv[11], "%lg", &Zx);
		sscanf(argv[12], "%lg", &Zy);
		sscanf(argv[13], "%lg", &Zz);
		vedo::GSSphere space("Granular Temperature Space", radius);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 njr::Vector3d(x, y, z),
				 njr::ZERO,
				 njr::Vector3d(Xx, Xy, Xz),
				 njr::Vector3d(Zx, Zy, Zz),
				 njr::ZERO       ));
		std::map<std::string, double> gt
			= CalculateGranularTemperature(oWorld, space, DOName);
		std::ofstream FileGT;
		FileGT.open("GranularTemperature.csv", std::ios::out);
		FileGT
			<< "Time, "
			<< "ElementNumber, "
			<< "AergageVelocity, "
			<< "GranularTemperatureV, "
			<< "AergageAngularVelocity, "
			<< "GranularTemperatureAV, "
			<< gt["Time"]                                  << ", "
			<< (unsigned long)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-gt_ellipsoid") && (arg.size() == 16))
	{
		double XLength, YLength, ZLength, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		std::string DOName = argv[3];
		sscanf(argv[4], "%lg", &XLength);
		sscanf(argv[5], "%lg", &YLength);
		sscanf(argv[6], "%lg", &ZLength);
		sscanf(argv[7], "%lg", &x);
		sscanf(argv[8], "%lg", &y);
		sscanf(argv[9], "%lg", &z);
		sscanf(argv[10], "%lg", &Xx);
		sscanf(argv[11], "%lg", &Xy);
		sscanf(argv[12], "%lg", &Xz);
		sscanf(argv[13], "%lg", &Zx);
		sscanf(argv[14], "%lg", &Zy);
		sscanf(argv[15], "%lg", &Zz);
		vedo::GSEllipsoid space
			("Granular Temperature Space", XLength, YLength, ZLength);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 njr::Vector3d(x, y, z),
				 njr::ZERO,
				 njr::Vector3d(Xx, Xy, Xz),
				 njr::Vector3d(Zx, Zy, Zz),
				 njr::ZERO       ));
		std::map<std::string, double> gt
			= CalculateGranularTemperature(oWorld, space, DOName);
		std::ofstream FileGT;
		FileGT.open("GranularTemperature.csv", std::ios::out);
		FileGT
			<< "Time, "
			<< "ElementNumber, "
			<< "AergageVelocity, "
			<< "GranularTemperatureV, "
			<< "AergageAngularVelocity, "
			<< "GranularTemperatureAV, "
			<< gt["Time"]                                  << ", "
			<< (unsigned long)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-i") && (arg.size() == 3))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		delete info(ReadDOWorld(arg[2], pIactRecordTab));
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-inside") && (arg.size() == 9))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		sscanf(argv[3], "%lg", &dXMin);
		sscanf(argv[4], "%lg", &dXMax);
		sscanf(argv[5], "%lg", &dYMin);
		sscanf(argv[6], "%lg", &dYMax);
		sscanf(argv[7], "%lg", &dZMin);
		sscanf(argv[8], "%lg", &dZMax);

		const vedo::DOStatus* pDOS;
		double dRadius;
		njr::Vector3d vPosition;
		std::ofstream oFileElementInside;
		oFileElementInside.open("ElementInside.txt", std::ios::out);

		for(unsigned long ul=0;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOS->GetPosition();
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				vPosition = pDOS->GetPosition();
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;

				if( (vPosition.x() + dRadius >= dXMin) &&
					(vPosition.x() - dRadius <= dXMax) &&
					(vPosition.y() + dRadius >= dYMin) &&
					(vPosition.y() - dRadius <= dYMax) &&
					(vPosition.z() + dRadius >= dZMin) &&
					(vPosition.z() - dRadius <= dZMax)   )
				{
					oFileElementInside << ul << " ";
				}
			}
		}

		oFileElementInside << std::endl;
		oFileElementInside.close();

		delete oWorld;
	}
	else if ((arg[1] == "-lattice_bcc") && (arg.size() == 11))
	{
		double xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = argv[2];
		sscanf(argv[3], "%lg", &xmin);
		sscanf(argv[4], "%lg", &xmax);
		sscanf(argv[5], "%lg", &ymin);
		sscanf(argv[6], "%lg", &ymax);
		sscanf(argv[7], "%lg", &zmin);
		sscanf(argv[8], "%lg", &zmax);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		double d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		double rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		double rectCenter[3]
			= {0.5*(xmax+xmin), 0.5*(ymax+ymin), 0.5*(zmax+zmin)};
		BCCLattice lattice(2.*d/sqrt(3.));
		lattice.SetOrigin(rectCenter);
		RectangularClusterInitializer init(rectSize, &lattice);
		init.Create(DOName, oWorld);
		oWorld
			->Shift
				(njr::Vector3d
					(rectCenter[0]-0.5*d,
					 rectCenter[1]-0.5*d,
					 rectCenter[2]-0.5*d), DOName);
		delete WriteDOWorld (arg[10], oWorld);
	}
	else if ((arg[1] == "-lattice_fcc") && (arg.size() == 11))
	{
		double xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = argv[2];
		sscanf(argv[3], "%lg", &xmin);
		sscanf(argv[4], "%lg", &xmax);
		sscanf(argv[5], "%lg", &ymin);
		sscanf(argv[6], "%lg", &ymax);
		sscanf(argv[7], "%lg", &zmin);
		sscanf(argv[8], "%lg", &zmax);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		double d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		double rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		double rectCenter[3]
			= {0.5*(xmax+xmin), 0.5*(ymax+ymin), 0.5*(zmax+zmin)};
		PrimitiveFCCLattice lattice(2.*d/sqrt(2.));
		lattice.SetOrigin(rectCenter);
		RectangularClusterInitializer init(rectSize, &lattice);
		init.Create(DOName, oWorld);
		oWorld
			->Shift
				(njr::Vector3d
					(rectCenter[0]-0.5*d,
					 rectCenter[1]-0.5*d,
					 rectCenter[2]-0.5*d), DOName);
		delete WriteDOWorld (arg[10], oWorld);
	}
	else if ((arg[1] == "-lattice_scc") && (arg.size() == 11))
	{
		double xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = argv[2];
		sscanf(argv[3], "%lg", &xmin);
		sscanf(argv[4], "%lg", &xmax);
		sscanf(argv[5], "%lg", &ymin);
		sscanf(argv[6], "%lg", &ymax);
		sscanf(argv[7], "%lg", &zmin);
		sscanf(argv[8], "%lg", &zmax);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		double d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		double rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		double rectCenter[3]
			= {0.5*(xmax+xmin), 0.5*(ymax+ymin), 0.5*(zmax+zmin)};
		CubicLattice lattice(d);
		lattice.SetOrigin(rectCenter);
		RectangularClusterInitializer init(rectSize, &lattice);
		init.Create(DOName, oWorld);
		oWorld
			->Shift
				(njr::Vector3d
					(rectCenter[0]-0.5*d,
					 rectCenter[1]-0.5*d,
					 rectCenter[2]-0.5*d), DOName);
		delete WriteDOWorld (arg[10], oWorld);
	}
	else if ((arg[1] == "-profile_Info2Y") && (arg.size() == 7))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double dXmin, dXmax, dZmin, dZmax;
		sscanf(argv[3], "%lg", &dXmin);
		sscanf(argv[4], "%lg", &dXmax);
		sscanf(argv[5], "%lg", &dZmin);
		sscanf(argv[6], "%lg", &dZmax);
		const vedo::DOStatus* pDOS;

		std::map<double, unsigned long> mInformation;
		std::map<double, unsigned long>::iterator mpInformation;
		for(unsigned long ul=0;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if(oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				if(    (pDOS->GetPosition().x() >= dXmin)
					&& (pDOS->GetPosition().x() <= dXmax)
					&& (pDOS->GetPosition().z() >= dZmin)
					&& (pDOS->GetPosition().z() <= dZmax) )
				{
					mInformation[pDOS->GetPosition().y()] = ul;
				};
			};
		};

		std::ofstream FileProfile;
		FileProfile.open("Profile.csv", std::ios::out);
		FileProfile << "Px, Py, Pz, Vx, Vy, Vz, AVx, AVy, AVz, R" << std::endl;
		for(mpInformation  = mInformation.begin();
			mpInformation != mInformation.end();
			mpInformation++                       )
		{
			pDOS = oWorld->GetDOStatus(mpInformation->second);
			FileProfile
				<< pDOS->GetPosition().x()        << ", "
				<< pDOS->GetPosition().y()        << ", "
				<< pDOS->GetPosition().z()        << ", "
				<< pDOS->GetVelocity().x()        << ", "
				<< pDOS->GetVelocity().y()        << ", "
				<< pDOS->GetVelocity().z()        << ", "
				<< pDOS->GetAngularVelocity().x() << ", "
				<< pDOS->GetAngularVelocity().y() << ", "
				<< pDOS->GetAngularVelocity().z() << ", "
				<< oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius << std::endl;
		};
		FileProfile.close();

		delete oWorld;
	}
	else if ((arg[1] == "-projected_area_x") && (arg.size() == 4))
	{
		double dMeshSize;
		sscanf(argv[2], "%lg", &dMeshSize);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		std::cout
			<< "Time = "
			<< oWorld->GetSystemParameter()->GetTimeCurrent()
			<< '\t'
			<< "Projected Area (X) = "
			<< oWorld->ProjectedAreaX(dMeshSize)
			<< std::endl;
	}
	else if ((arg[1] == "-projected_area_y") && (arg.size() == 4))
	{
		double dMeshSize;
		sscanf(argv[2], "%lg", &dMeshSize);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		std::cout
			<< "Time = "
			<< oWorld->GetSystemParameter()->GetTimeCurrent()
			<< '\t'
			<< "Projected Area (Y) = "
			<< oWorld->ProjectedAreaY(dMeshSize)
			<< std::endl;
	}
	else if ((arg[1] == "-projected_area_z") && (arg.size() == 4))
	{
		double dMeshSize;
		sscanf(argv[2], "%lg", &dMeshSize);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		std::cout
			<< "Time = "
			<< oWorld->GetSystemParameter()->GetTimeCurrent()
			<< '\t'
			<< "Projected Area (Z) = "
			<< oWorld->ProjectedAreaZ(dMeshSize)
			<< std::endl;
	}
	else if ((arg[1] == "-rotate") && (arg.size() == 7))
	{
		double dAngle2XAxis = 0.0;
		double dAngle2YAxis = 0.0;
		double dAngle2ZAxis = 0.0;
		sscanf(argv[2], "%lg", &dAngle2XAxis);
		sscanf(argv[3], "%lg", &dAngle2YAxis);
		sscanf(argv[4], "%lg", &dAngle2ZAxis);
        // We have not deal with interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5]);

		oWorld->Rotate
			(dAngle2XAxis*0.0174532925199433,
			 dAngle2YAxis*0.0174532925199433,
			 dAngle2ZAxis*0.0174532925199433 );
		delete WriteDOWorld (arg[6], oWorld);
	}
	else if ((arg[1] == "-sccBm1") && (arg.size() == 3))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double Bm = SCCFlowabilityIndex(oWorld, 2.0, 28.0);
		std::cout << std::endl << "Bm = " << Bm << std::endl << std::endl;
	}
	else if ((arg[1] == "-sccBm2") && (arg.size() == 3))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		double Bm = SCCFlowabilityIndex(oWorld, 8.0, 65.0);
		std::cout << std::endl << "Bm = " << Bm << std::endl << std::endl;
	}
	else if ((arg[1] == "-sccRm") && (arg.size() == 3))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		bool Rm = SCCViscosityIndex(oWorld);
		if (Rm)
		{
			std::cout
				<< std::endl
				<< "All of the elements have passed the gate!!"
				<< std::endl
				<< std::endl;
		}
		else
		{
			std::cout << std::endl << "Not yet..." << std::endl << std::endl;
		}
	}
	else if ((arg[1] == "-shift") && (arg.size() == 8))
	{
		double x, y, z;
		std::string DOName = argv[2];
		sscanf(argv[3], "%lg", &x);
		sscanf(argv[4], "%lg", &y);
		sscanf(argv[5], "%lg", &z);
        // We have noe deal with interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6]);
		oWorld->Shift(njr::Vector3d(x, y, z), DOName);
		delete WriteDOWorld (arg[7], oWorld);
	}
	else if ((arg[1] == "-shift_all") && (arg.size() == 7))
	{
		double x, y, z;
		sscanf(argv[2], "%lg", &x);
		sscanf(argv[3], "%lg", &y);
		sscanf(argv[4], "%lg", &z);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5], pIactRecordTab);
		oWorld->Shift(njr::Vector3d(x, y, z));
		delete WriteDOWorld (arg[6], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-sort") && (arg.size() == 4))
	{
	    // We have not deal with the interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		oWorld->SortingDOStatus();
		delete WriteDOWorld (arg[3], oWorld);
	}
	else if ((arg[1] == "-sort_distance") && (arg.size() == 6))
	{
		double dX, dY, dZ;
		sscanf(argv[2], "%lg", &dX);
		sscanf(argv[3], "%lg", &dY);
		sscanf(argv[4], "%lg", &dZ);
		njr::Vector3d vP(dX, dY, dZ);
	    // We have not deal with the interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5]);

		std::map<double, unsigned long> mIndex;
		double dD;
		for (unsigned long ul=0;
			 ul<(oWorld->GetSystemParameter()->GetDONumber());
			 ul++                                             )
		{
			dD = ((oWorld->GetDOStatus(ul)->GetPosition()) - vP).length();
			mIndex[dD] = ul;
		}

		unsigned long ulN;
		std::ofstream FileSortDistance;
		FileSortDistance.open("ElementSorting.csv", std::ios::out);
		FileSortDistance
			<< "Distance, SerialNumber, DOName, Px, Py, Pz" << std::endl;
		for (std::map<double, unsigned long>::iterator mpIndex=mIndex.begin();
			 mpIndex!=mIndex.end();
			 mpIndex++                                                   )
		{
			ulN = mpIndex->second;
			FileSortDistance
				<< mpIndex->first                              << ", "
				<< ulN                                         << ", "
				<< oWorld->GetDOStatus(ulN)->GetDOName()       << ", "
				<< oWorld->GetDOStatus(ulN)->GetPosition().x() << ", "
				<< oWorld->GetDOStatus(ulN)->GetPosition().y() << ", "
				<< oWorld->GetDOStatus(ulN)->GetPosition().z() << std::endl;
		}

		FileSortDistance.close();
		delete oWorld;
	}
	else if ((arg[1] == "-set_time") && (arg.size() == 8))
	{
		double dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent;
		sscanf(argv[2], "%lg", &dTimeStart);
		sscanf(argv[3], "%lg", &dTimeStop);
		sscanf(argv[4], "%lg", &dTimeInterval);
		sscanf(argv[5], "%lg", &dTimeCurrent);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		oWorld
			->SetSimulatedTime
				(dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-show") && (arg.size() == 4))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		unsigned uID;
		sscanf(argv[3], "%d", &uID);
		if(uID < oWorld->GetSystemParameter()->GetDONumber())
		{
			std::ofstream ofInformation("ObjectInformation.csv", std::ios::out);
			const vedo::DOStatus* dosp = oWorld->GetDOStatus(uID);
			njr::Vector3d vV;
			ofInformation << "Item, X, Y, Z" << std::endl;
			vV = dosp->GetOrientationX();
			ofInformation
				<< "OrientationX, "
				<< vV.x() << ", " << vV.y() << ", " << vV.z() << std::endl;
			vV = dosp->GetOrientationZ();
			ofInformation
				<< "OrientationZ, "
				<< vV.x() << ", " << vV.y() << ", " << vV.z() << std::endl;
			vV = dosp->GetPosition();
			ofInformation
				<< "Position, "
				<< vV.x() << ", " << vV.y() << ", " << vV.z() << std::endl;
			vV = dosp->GetVelocity();
			ofInformation
				<< "Velocity, "
				<< vV.x() << ", " << vV.y() << ", " << vV.z() << std::endl;
			vV = dosp->GetAngularVelocity();
			ofInformation
				<< "AngularVelocity, "
				<< vV.x() << ", " << vV.y() << ", " << vV.z() << std::endl;
			ofInformation.close();
		}
		delete oWorld;
	}
	else if ((arg[1] == "-u11") && (arg.size() == 5))
	{
		vedo::DOWorld* oWorld = ReadDOWorld2011(arg[2]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab;
		pIactRecordTab->ReadIRT2011(arg[3].c_str());
		delete WriteDOWorld(arg[4], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-u10") && (arg.size() == 5))
	{
		vedo::DOWorld* oWorld = ReadDOWorld2009(arg[2]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab;
		pIactRecordTab->ReadIRT2010(arg[3].c_str());
		delete WriteDOWorld(arg[4], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-u9") && (arg.size() == 4))
	{
		delete WriteDOWorld(arg[3], ReadDOWorld2009(arg[2]));
	}
	else if ((arg[1] == "-u8") && (arg.size() == 4))
	{
		delete WriteDOWorld(arg[3], ReadDOWorld2008(arg[2]));
	}
	else if ((arg[1] == "-u7") && (arg.size() == 4))
	{
		delete WriteDOWorld(arg[3], ReadDOWorld2007(arg[2]));
	}
	else if ((arg[1] == "-u6") && (arg.size() == 4))
	{
		delete WriteDOWorld(arg[3], ReadDOWorld2006(arg[2]));
	}
	else if ((arg[1] == "-volume") && (arg.size() == 4))
	{
		double dMeshSize;
		sscanf(argv[2], "%lg", &dMeshSize);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		std::cout
			<< "Time = "
			<< oWorld->GetSystemParameter()->GetTimeCurrent()
			<< ", "
			<< "Volume = "
			<< oWorld->Volume(dMeshSize)
			<< std::endl;
	}
	else
	{
		usage();
	}

	//njr::RunTime("No News is Good News");

	return true;
};
