#include <aries/utility/BinaryIO.h>
#include <aries/utility/Constants.h>
//#include <njr/Utility.h>
#include <aries/utility/DataType.h>
#include <aries/utility/Vector3d.h>
#include <vedo/framework/Boundary.h>
#include <vedo/framework/Consultant.h>
#include <vedo/framework/DOModel.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/DOWorld_WriteVTK.h>
#include <vedo/framework/GeometricShape.h>
#include <vedo/framework/IactRecordTab.h>
#include <vedo/common/GSComplexShape.h>
#include <vedo/common/GSCylinder.h>
#include <vedo/common/GSRectangle.h>
#include <vedo/common/GSSphere.h>
#include <vedo/common/GSEllipsoid.h>
#include <vedo/knight/CalcDensity.h>
#include <vedo/knight/CalcCoordinateNum.h>
#include <vedo/knight/RectangularClusterInitializer.h>
#include <vedo/knight/3DLattices.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

aries::Constants* aries_cp = aries::Constants::Instance();
vedo::_float_t SafetyFactor = aries_cp->SafetyFactor();
vedo::_uint_t uNumUDDDOStatus = aries_cp->NumUDDDOStatus();
vedo::_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

void usage()
{
	std::ofstream ofMessageFile("knight.txt", std::ios::out);

	std::cout
		<< "Knight " << aries::information::_Version                 << std::endl
		<< aries::information::_Information                          << std::endl << std::endl
		<< "Unknown command, please read the file \"knight.txt\" for more information." << std::endl;

	ofMessageFile
		<< "Knight " << aries::information::_Version                 << std::endl
		<< aries::information::_Information                          << std::endl << std::endl
		<< "Usage:" << std::endl
		<< std::endl
		<< "* Knight -add_elements_in_body-centered_cubic_style"     << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_elements_in_face-centered_cubic_style"     << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_elements_in_simple_cubic_style"            << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_pontential_energy_from_sorce_point"        << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <source x> <source y> <source z>"               << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_elements_in_circular_path"                 << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <started angle (in degree)>"                    << std::endl
		<< "         <ended angle (in degree)>"                      << std::endl
		<< "         <inclined angle (in degree)>"                   << std::endl
		<< "         <number of elements>"                           << std::endl
		<< "         <radius of circular path> <Z of circular path>" << std::endl
		<< "         <Revolutions per time unit>"                    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_single_element"                            << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <position-x> <position-y> <position-z>"         << std::endl
		<< "         <velocity-x> <velocity-y> <velocity-z>"         << std::endl
		<< "         <orientation-x-x>"                              << std::endl
		<< "         <orientation-x-y>"                              << std::endl
		<< "         <orientation-x-z>"                              << std::endl
		<< "         <orientation-z-x>"                              << std::endl
		<< "         <orientation-z-y>"                              << std::endl
		<< "         <orientation-z-z>"                              << std::endl
		<< "         <angularvelocity-x>"                            << std::endl
		<< "         <angularvelocity-y>"                            << std::endl
		<< "         <angularvelocity-z>"                            << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_velocity"                                  << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <velocity-x> <velocity-y> <velocity-z>"         << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_angular_velocity"                          << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <angular velocity-x>"                           << std::endl
		<< "         <angular velocity-y>"                           << std::endl
		<< "         <angular velocity-z>"                           << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_elements_randomly_in_Cartesian_coordinate" << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <number of elements>"                           << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_elements_randomly_in_cylindrical_coordinate" << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <number of elements>"                           << std::endl
		<< "         <center of bottom-x>"                           << std::endl
		<< "         <center of bottom-y>"                           << std::endl
		<< "         <center of bottom-z>"                           << std::endl
		<< "         <radius of celinder>"                           << std::endl
		<< "         <height of celinder>"                           << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -add_elements_randomly_in_spherical_coordinate" << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <number of elements>"                           << std::endl
		<< "         <center of sphere-x>"                           << std::endl
		<< "         <center of sphere-y>"                           << std::endl
		<< "         <center of sphere-z>"                           << std::endl
		<< "         <radius of sphere>"                             << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
/*
		<< "* Knight -assemble_xml(-al)"                             << std::endl
		<< "         <reference1 .xml> <refreence2 .xml>"            << std::endl
		<< "         <destination .xml>"                             << std::endl
		<< std::endl
*/
		<< "* Knight -average_information"                           << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <mesh size>"                                    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -average_information"                           << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <mesh size>"                                    << std::endl
		<< "         <reference1 .xml | .ido>"                       << std::endl
		<< "         <reference2 .xml | .ido>"                       << std::endl
		<< "         (velocity is estimated by positions between reference files)" << std::endl
		<< std::endl
		<< "* Knight -average_information_projected_in_xy_plane"     << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <mesh size>"                                    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -average_information_projected_in_xy_plane"     << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <mesh size>"                                    << std::endl
		<< "         <reference1 .xml | .ido>"                       << std::endl
		<< "         <reference2 .xml | .ido>"                       << std::endl
		<< "         (velocity is estimated by positions between reference files)" << std::endl
		<< std::endl
		<< "* Knight -average_information_projected_in_yz_plane"     << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <mesh size>"                                    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -average_information_projected_in_yz_plane"     << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <mesh size>"                                    << std::endl
		<< "         <reference1 .xml | .ido>"                       << std::endl
		<< "         <reference2 .xml | .ido>"                       << std::endl
		<< "         (velocity is estimated by positions between reference files)" << std::endl
		<< std::endl
		<< "* Knight -average_information_projected_in_xz_plane"     << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <mesh size>"                                    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -average_information_projected_in_xz_plane"     << std::endl
		<< "         <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>"      << std::endl
		<< "         <mesh size>"                                    << std::endl
		<< "         <reference1 .xml | .ido>"                       << std::endl
		<< "         <reference2 .xml | .ido>"                       << std::endl
		<< "         (velocity is estimated by positions between reference files)" << std::endl
		<< std::endl
		<< "* Knight -convert"                                       << std::endl
		<< "         <source .xml | .ido>"                           << std::endl
		<< "         <destination .xml | .ido | .csv | .vpf | .dxf | .vtu>" << std::endl
		<< std::endl
		<< "* Knight -cross_section_area_on_surface"                 << std::endl
		<< "         <element name>"                                 << std::endl
		<< "	     <a> <b> <c> <d> "                               << std::endl
		<< "             (Surface function: ax+by+cz=d)"             << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -cross_section_flux_on_surface"                 << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <a> <b> <c> <d> "                               << std::endl
		<< "             (Surface function: ax+by+cz=d)"             << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
/*
		<< "	-cal: calculate average density and coordinate number" << std::endl
		<< "* Knight -cal <filename> <xmin,xmax,ymin,ymax,zmin,zmax>" << std::endl
		<< std::endl
		<< "	-cl: convert std::list (Hexadecimal)" << std::endl
		<< "* Knight -cl <filename> <begin num> <the end>" << std::endl
		<< std::endl
*/
		<< "* Knight -clean_user_defined_value"                      << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
/*
		<< "	-cld: convert std::list (Decimalism)" << std::endl
		<< "* Knight -cld <filename> <begin num> <the end>" << std::endl
		<< std::endl
*/
		<< "* Knight -change_models"                                 << std::endl
		<< "         <new models .xml | .ido>"                       << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -combine"                                       << std::endl
		<< "         <reference1 .xml | .ido>"                       << std::endl
		<< "         <reference2 .xml | .ido>"                       << std::endl
		<< "             (provide mobile elements)"                  << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -combine"                                       << std::endl
		<< "         <reference1 .xml | .ido>"                       << std::endl
		<< "         <reference2 .xml | .ido>"                       << std::endl
		<< "             (provide assigned elements)"                << std::endl
		<< "         <name of assigned elements>"                    << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
/*
		<< "	-coord_num: calculate coordinate number for spheres and output to VTK file" << std::endl
		<< "* Knight -coord_num <.xml | .ido>  <.vtu>" << std::endl
		<< std::endl
*/
		<< "* Knight -convert_sudo_vpf"                              << std::endl
		<< "         <reference1 .xml | .ido>"                       << std::endl
		<< "             (control the number of elements)"           << std::endl
		<< "         <reference2 .xml | .ido>"                       << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
/*
		<< "	-b2h: convert DOS text file to HEX" << std::endl
		<< "* Knight -b2h <origin> <destination>" << std::endl
		<< std::endl
		<< "	-d2u: convert DOS text file to UNIX text" << std::endl
		<< "* Knight -d2u <origin> <destination>" << std::endl
		<< std::endl
*/
		<< "* Knight -distance_to_point"                             << std::endl
		<< "         <source x> <source y> <source z>"               << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -distance_to_xy_plane"                          << std::endl
		<< "         <source x> <source y>"                          << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -distance_to_yz_plane"                          << std::endl
		<< "         <source y> <source z>"                          << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -distance_to_xz_plane"                          << std::endl
		<< "         <source x> <source z>"                          << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -dump_status"                                   << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination (DOStatus ) .csv>"                 << std::endl
		<< "         <destination (IactStatus ) .csv>"               << std::endl
		<< std::endl
/*
		<< "	-u2d: convert UNIX text file to DOS text" << std::endl
		<< "* Knight -u2d <origin> <destination>" << std::endl
		<< std::endl
		<< "	-cse: clear separated elements" << std::endl
		<< "* Knight -cs <.ido> <.ido>" << std::endl
		<< std::endl
		<< "	-distribution: determine the distribution of certain elements" << std::endl
		<< "* Knight -distribution <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			<Mesh size> <.xml | .ido>" << std::endl
		<< std::endl
*/
		<< "* Knight -erase_single_element"                          << std::endl
		<< "         <element ID>"                                   << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -erase_elements"                                << std::endl
		<< "         <element ID (started)>"                         << std::endl
		<< "         <element ID (ended)>"                           << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -erase_elements"                                << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -erase_elements"                                << std::endl
		<< "         <element name>"                                 << std::endl
		<< "	     <a> <b> <c> <d> "                               << std::endl
		<< "             (Elements in ax+by+cz>d will be erased)"    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -erase_elements"                                << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <number of faces>"                              << std::endl
		<< "	     <a1> <b1> <c1> <d1> "                           << std::endl
		<< "             (Elements in a1 x+b1 y+c1 z>d1 will be erased)"    << std::endl
		<< "	     <a2> <b2> <c2> <d2> "                           << std::endl
		<< "             (Elements in a2 x+b2 y+c2 z>d2 will be erased)"    << std::endl
		<< "	     <a3> <b3> <c3> <d3> "                           << std::endl
		<< "             (Elements in a3 x+b3 y+c3 z>d3 will be erased)"    << std::endl
		<< "	     :"                                              << std::endl
		<< "	     :"                                              << std::endl
		<< "	     <an> <bn> <cn> <dn> "                           << std::endl
		<< "             (Elements in an x+bn y+cn z>dn will be erased)"    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -changeDOModel"                                 << std::endl
		<< "         <old element name>"                             << std::endl
		<< "         <new element name>"                             << std::endl
		<< "         <number of faces>"                              << std::endl
		<< "	     <a1> <b1> <c1> <d1> "                           << std::endl
		<< "             (Elements in a1 x+b1 y+c1 z>d1 will be replace its model)" << std::endl
		<< "	     <a2> <b2> <c2> <d2> "                           << std::endl
		<< "             (Elements in a2 x+b2 y+c2 z>d2 will be replace its model)" << std::endl
		<< "	     <a3> <b3> <c3> <d3> "                           << std::endl
		<< "             (Elements in a3 x+b3 y+c3 z>d3 will be replace its model)" << std::endl
		<< "	     :"                                              << std::endl
		<< "	     :"                                              << std::endl
		<< "	     <an> <bn> <cn> <dn> "                           << std::endl
		<< "             (Elements in an x+bn y+cn z>dn will be replace its model)" << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -erase_spherical_elements"                      << std::endl
		<< "	     <a> <b> <c> <d> "                               << std::endl
		<< "             (Spherical elements in ax+by+cz>d will be erased)" << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -energy"                                        << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -freeze_all_elements"                           << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -freeze_elements"                               << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
/*
		<< "* Knight -gt_auto <.xml | .ido> <DOName> <CutNumberX> <CutNumberY> <CutNumberZ>" << std::endl
		<< "			<Angle2XAxis> <Angle2YAxis> <Angle2ZAxis>" << std::endl
		<< std::endl
*/
		<< "* Knight -granular_temperature_in_box"                   << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <box center-x> <box center-y> <box center-z>"   << std::endl
		<< "         <box width> <box length> <box height>"          << std::endl
		<< "         <box Xx> <box Xy> <box Xz>"                     << std::endl
		<< "         <box Zx> <box Zy> <box Zz>"                     << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -granular_temperature_in_cylinder"              << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <cylinder center-x>"                            << std::endl
		<< "         <cylinder center-y>"                            << std::endl
		<< "         <cylinder center-z>"                            << std::endl
		<< "         <cylinder radius> <cylinder height>"            << std::endl
		<< "         <cylinder Xx> <cylinder Xy> <cylinder Xz>"      << std::endl
		<< "         <cylinder Zx> <cylinder Zy> <cylinder Zz>"      << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -granular_temperature_in_sphere"                << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <sphere center-x>"                              << std::endl
		<< "         <sphere center-y>"                              << std::endl
		<< "         <sphere center-z>"                              << std::endl
		<< "         <sphere radius>"                                << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -granular_temperature_in_ellipsoid"             << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <ellipsoid center-x>"                           << std::endl
		<< "         <ellipsoid center-y>"                           << std::endl
		<< "         <ellipsoid center-z>"                           << std::endl
		<< "         <ellipsoid length-x>"                           << std::endl
		<< "         <ellipsoid length-y>"                           << std::endl
		<< "         <ellipsoid length-z>"                           << std::endl
		<< "         <ellipsoid Xx> <ellipsoid Xy> <ellipsoid Xz>"   << std::endl
		<< "         <ellipsoid Zx> <ellipsoid Zy> <ellipsoid Zz>"   << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
		<< "* Knight -information"                                   << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl
/*
		<< "	-inside: check what elements are inside" << std::endl
		<< "* Knight -inside <.xml | .ido> <XMin> <XMax> <YMin> <YMax> <ZMin> <ZMax>" << std::endl
		<< std::endl
*/
/*
		<< "	-lattice_bcc: rectangular element initialization on the basis of BCC Lattice" << std::endl
		<< "* Knight -lattice_bcc <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-lattice_fcc: rectangular element initialization on the basis of FCC Lattice" << std::endl
		<< "* Knight -lattice_fcc <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
		<< "	-lattice_scc: rectangular element initialization on the basis of Simple Cubic Lattice" << std::endl
		<< "* Knight -lattice_scc <DOName> <xmin> <xmax> <ymin> <ymax> <zmin> <zmax>" << std::endl
		<< "			(Original)<.xml | .ido> <.xml | .ido>" << std::endl
		<< std::endl
*/
/*
		<< "	-profile_Info2Y: calculate the profile \"Information-Y\" in defined space" << std::endl
		<< "* Knight -profile_Info2Y <.xml | .ido> <Xmin> <XMax> <Zmin> <Zmax>" << std::endl
		<< std::endl
		<< "	-projected_area_x: calculate the projected area in Y-Z domain" << std::endl
		<< "* Knight -projected_area_x <mesh length> <.ido>" << std::endl
		<< std::endl
		<< "	-projected_area_y: calculate the projected area in Z-X domain" << std::endl
		<< "* Knight -projected_area_y <mesh length> <.ido>" << std::endl
		<< std::endl
		<< "	-projected_area_z: calculate the projected area in X-Y domain" << std::endl
		<< "* Knight -projected_area_z <mesh length> <.ido>" << std::endl
		<< std::endl
*/
		<< "* Knight -modify_element_status"                         << std::endl
		<< "         <element ID>"                                   << std::endl
		<< "         <position-x> <position-y> <position-z>"         << std::endl
		<< "         <velocity-x> <velocity-y> <velocity-z>"         << std::endl
		<< "         <orientation-x-x>"                              << std::endl
		<< "         <orientation-x-y>"                              << std::endl
		<< "         <orientation-x-z>"                              << std::endl
		<< "         <orientation-z-x>"                              << std::endl
		<< "         <orientation-z-y>"                              << std::endl
		<< "         <orientation-z-z>"                              << std::endl
		<< "         <angularvelocity-x>"                            << std::endl
		<< "         <angularvelocity-y>"                            << std::endl
		<< "         <angularvelocity-z>"                            << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -modify_element_velocity_and_angular_velocity"  << std::endl
		<< "         <element ID>"                                   << std::endl
		<< "         <velocity-x> <velocity-y> <velocity-z>"         << std::endl
		<< "         <angularvelocity-x>"                            << std::endl
		<< "         <angularvelocity-y>"                            << std::endl
		<< "         <angularvelocity-z>"                            << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -rotate_all_elements"                           << std::endl
		<< "         <angle to axis x (in degree)>"                  << std::endl
		<< "         <angle to axis y (in degree)>"                  << std::endl
		<< "         <angle to axis z (in degree)>"                  << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -shift_elements"                                << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <x> <y> <z>"                                    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -shift_all_elements"                            << std::endl
		<< "         <x> <y> <z>"                                    << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "	-sort: sort elements" << std::endl
		<< "* Knight -sort (Original)<.xml | .ido> (New)<.xml | .ido>" << std::endl
		<< std::endl
/*
		<< "	-sort_distance: sort elements by the distance to a reference point" << std::endl
		<< "* Knight -sort_distance <Px> <Py> <Pz> <.xml | .ido>" << std::endl
		<< std::endl
*/
		<< "* Knight -set_time"                                      << std::endl
		<< "         <start time> <stop time>"                       << std::endl
		<< "         <time interval> <current time>"                 << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< "         <destination .xml | .ido>"                      << std::endl
		<< std::endl
		<< "* Knight -show_elements_motion_status"                   << std::endl
		<< "         <element name>"                                 << std::endl
		<< "         <reference .xml | .ido>"                        << std::endl
		<< std::endl;

	ofMessageFile.close();

	exit(0);
}

vedo::DOWorld* info(vedo::DOWorld *oWorld)
{
	const vedo::SystemParameter* sp = oWorld->GetSystemParameter();
	std::cout
		<< "<<SystemParameter Infomation>>"                                      << std::endl
		<< "\tObjNumber         : " << sp->GetDONumber()                         << std::endl
		<< "\tTime              : "
			<< "start("    << sp->GetTimeStart()    << ") "
			<< "stop("     << sp->GetTimeStop()     << ") "
			<< "interval(" << sp->GetTimeInterval() << ") "
			<< "current("  << sp->GetTimeCurrent()  << ")"                       << std::endl
		<< "\tSimConstant       : "
			<< "ContactDetectSafetyFactor(" << SafetyFactor << ") "
			<< "NumUDDOStatus("             << uNumUDDDOStatus     << ") "
			<< "NumUDIactStatus("           << uNumUDDImpactStatus << ")"  << std::endl
		<< "\tField Acceleration: " << sp->GetFieldAcceleration()
		<< "\tZOI               : ";

	if (sp->GetZoneOfInterest().GetSwitch(0))
	{
		std::cout
			<< "XMin(" << sp->GetZoneOfInterest().GetLowerPoint().x() << ") "
			<< "XMax(" << sp->GetZoneOfInterest().GetUpperPoint().x() << ") ";
	}

	if (sp->GetZoneOfInterest().GetSwitch(1))
	{
		std::cout
			<< "YMin(" << sp->GetZoneOfInterest().GetLowerPoint().y() << ") "
			<< "YMax(" << sp->GetZoneOfInterest().GetUpperPoint().y() << ") ";
	}

	if (sp->GetZoneOfInterest().GetSwitch(2))
	{
		std::cout
			<< "ZMin(" << sp->GetZoneOfInterest().GetLowerPoint().z() << ") "
			<< "ZMax(" << sp->GetZoneOfInterest().GetUpperPoint().z() << ") ";
	}
	std::cout << std::endl;

	std::cout << "\tPBC               : ";

	if (sp->GetPeriodicBoundaryConditions().GetSwitch(0))
	{
		std::cout
			<< "XMin(" << sp->GetPeriodicBoundaryConditions().GetLowerPoint().x() << ") "
			<< "XMax(" << sp->GetPeriodicBoundaryConditions().GetUpperPoint().x() << ") ";
	}

	if (sp->GetPeriodicBoundaryConditions().GetSwitch(1))
	{
		std::cout
			<< "YMin(" << sp->GetPeriodicBoundaryConditions().GetLowerPoint().y() << ") "
			<< "YMax(" << sp->GetPeriodicBoundaryConditions().GetUpperPoint().y() << ") ";
	}

	if (sp->GetPeriodicBoundaryConditions().GetSwitch(2))
	{
		std::cout
			<< "ZMin(" << sp->GetPeriodicBoundaryConditions().GetLowerPoint().z() << ") "
			<< "ZMax(" << sp->GetPeriodicBoundaryConditions().GetUpperPoint().z() << ") ";
	}
	std::cout << std::endl << std::endl;

	const std::vector<vedo::DOStatus*>& rcDOStatus = oWorld->GetDOStatus();
	std::vector<vedo::DOStatus *>::const_iterator idos;
	std::map<std::string, vedo::_uint_t> counter;

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
	vedo::_uint_t i = 0;
	for (iactiter=rcIactModel.begin(); iactiter!=rcIactModel.end(); ++iactiter)
	{
		vedo::IactModel* im = *iactiter;
		std::string eqtype = im->GetEquationType();
		std::cout
			<< '\t' << i++ << ": " << im->GetMasterDOGroup().c_str() << " with " << im->GetSlaveDOGroup().c_str()
			<< "  " << eqtype.c_str() << std::endl;
	}
	std::cout << std::endl;
	return oWorld;
}

vedo::DOWorld* ReadDOWorld(std::string filename, vedo::IactRecordTab* irtp)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (aries::CheckSubName(filename,".xml"))
	{
		pWorld->ReadXML(filename, irtp);
	}
	else if (aries::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO(filename, irtp);
	}
	else
	{
		usage();
	}
	return pWorld;
}

vedo::DOWorld* ReadDOWorld(std::string filename)
{
	vedo::DOWorld* pWorld = new vedo::DOWorld;

	if (aries::CheckSubName(filename,".xml"))
	{
		pWorld->ReadXML(filename);
	}
	else if (aries::CheckSubName(filename, ".ido" ))
	{
		pWorld->ReadIDO(filename);
	}
	else
	{
		usage();
	}
	return pWorld;
}

vedo::DOWorld* WriteDOWorld (std::string filename, vedo::DOWorld* pw)
{
	if (aries::CheckSubName(filename, ".xml"))
	{
		pw->WriteXML(filename.c_str());
	}
	else if (aries::CheckSubName(filename, ".ido"))
	{
		pw->WriteIDO(filename.c_str());
	}
	else if (aries::CheckSubName(filename, ".vpf"))
	{
		pw->WriteVPF(filename.c_str());
	}
	else if (aries::CheckSubName(filename, ".vtu"))
	{
		#ifdef _STD_CPP_11
			pw->WriteVTK<vedo::NullExternalFieldVTKWriter>(filename.c_str());
		#endif   // _STD_CPP_11
	}
	else if (aries::CheckSubName(filename, "_h.dxf"))
	{
		pw->HighDraw(filename.c_str());
	}
	else if (aries::CheckSubName(filename, ".dxf"))
	{
		pw->Draw(filename.c_str());
	}
	else
	{
		usage();
	}
	return pw;
}

vedo::DOWorld* WriteDOWorld (std::string filename, vedo::DOWorld* pw, vedo::IactRecordTab* irtp)
{
	if (aries::CheckSubName(filename, ".xml"))
	{
		pw->WriteXML(filename.c_str(), irtp);
	}
	else if (aries::CheckSubName(filename, ".ido"))
	{
		pw->WriteIDO(filename.c_str(), irtp);
	}
	else if (aries::CheckSubName(filename, ".vpf"))
	{
		pw->WriteVPF(filename.c_str());
	}
	else if (aries::CheckSubName(filename, ".vtu"))
	{
		#ifdef _STD_CPP_11
			pw->WriteVTK<vedo::NullExternalFieldVTKWriter>(filename.c_str());
		#endif   // _STD_CPP_11
	}
	else if (aries::CheckSubName(filename, "_h.dxf"))
	{
		pw->HighDraw(filename.c_str());
	}
	else if (aries::CheckSubName(filename, ".dxf"))
	{
		pw->Draw(filename.c_str());
	}
	else
	{
		usage();
	}
	return pw;
}

vedo::DOWorld* WriteDOWorldVPF (std::string filename, vedo::DOWorld* opw, vedo::DOWorld* cpw)
{
	if (aries::CheckSubName(filename, ".vpf"))
	{
		cpw->WriteVPF(filename.c_str(), opw);
	}
	else
	{
		usage();
	}
	delete opw;
	return cpw;
}

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

	transform(vcd1.begin(), vcd1.end(), back_inserter(vcd3), njr::Copy_obj());
	transform(vcd2.begin(), vcd2.end(), back_inserter(vcd3), njr::Copy_obj());

	vedo::SystemParameter *pSystemParameter
		= new vedo::SystemParameter
			("Domino Test Benchmark 4",
			"We Love YOU",
			0.0,
			5.0,
			5e-6,
			0.0,
			vcd3.size(),
			vedo::Vector3df(0.0, 0.0, -980.0),
			vedo::Boundary()                    );

	vedo::DOWorld* pw3 = new vedo::DOWorld(pSystemParameter, cDOModel, cIactModel, vcd3);

	pw3->WriteXML(des.c_str());

	delete pw1;
	delete pw2;
	delete pw3;
}
*/

void ConList(std::string filename, vedo::_uint_t begin, vedo::_uint_t end)
{
	char ltoa[256];
	vedo::_uint_t ulRecordCount;
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
}

void ConListHEX (std::string filename, vedo::_uint_t begin, vedo::_uint_t end)
{
	char ltoa[256];
	vedo::_uint_t ulRecordCount;
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
}

void AssmListHEX()
{
	char ltoa[256];
	vedo::_uint_t begin = 0;
	vedo::_uint_t end   = 1474;
	std::string filename    = "bs";

	vedo::_uint_t ulRecordCount;
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
		std::cout << file.c_str() << ' ' << out.c_str() << ' ' << des.c_str() << std::endl;
	}
}

void AddDOInSpaceBCC(vedo::Vector3df* LowerBoundary, vedo::Vector3df* UpperBoundary, std::string DOName, vedo::DOWorld* oWorld)
{
	vedo::_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	*LowerBoundary += vedo::Vector3df(R, R, R);
	*UpperBoundary -= vedo::Vector3df(R, R, R);
	vedo::_float_t fCellSize = 4.0 * R / std::sqrt(3.0);

	bool bLayer    = true;
	bool bContinue = true;
	vedo::_float_t x, y;
	vedo::_float_t z = LowerBoundary->z();
	while (bContinue)
	{
		if(bLayer)
		{
			for (y=(LowerBoundary->y()); y<=(UpperBoundary->y()); y+=fCellSize)
			{
				for (x=(LowerBoundary->x()); x<=(UpperBoundary->x()); x+=fCellSize)
				{
					dos.SetPosition(vedo::Vector3df(x, y, z));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
				}
			}
			bLayer = false;
			if ((z + fCellSize) <= UpperBoundary->z())
			{
				z += 0.5 * fCellSize;
			}
			else
			{
				bContinue = false;
			}
		}
		else
		{
			for (y=(LowerBoundary->y())+0.5*fCellSize; y<=(UpperBoundary->y()); y+=fCellSize)
			{
				for (x=(LowerBoundary->x())+0.5*fCellSize; x<=(UpperBoundary->x()); x+=fCellSize)
				{
					dos.SetPosition(vedo::Vector3df(x, y, z));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
				}
			}
			bLayer = true;
			if ((z + fCellSize) <= UpperBoundary->z())
			{
				z += 0.5 * fCellSize;
			}
			else
			{
				bContinue = false;
			}
		}
	}
}

void AddDOInSpaceFCC(vedo::Vector3df* LowerBoundary, vedo::Vector3df* UpperBoundary, std::string DOName, vedo::DOWorld* oWorld)
{
	vedo::_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	*LowerBoundary += vedo::Vector3df(R, R, R);
	*UpperBoundary -= vedo::Vector3df(R, R, R);
	vedo::_float_t fCellSize = 2.0 * std::sqrt(2.0) * R;

	bool bLayer    = true;
	bool bContinue = true;
	vedo::_float_t x, y;
	vedo::_float_t z = LowerBoundary->z();
	while (bContinue)
	{
		if(bLayer)
		{
			for (y=(LowerBoundary->y()); y<=(UpperBoundary->y()); y+=fCellSize)
			{
				for (x=(LowerBoundary->x()); x<=(UpperBoundary->x()); x+=fCellSize)
				{
					dos.SetPosition(vedo::Vector3df(x, y, z));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
				}
			}
			for (y=(LowerBoundary->y())+0.5*fCellSize; y<=(UpperBoundary->y()); y+=fCellSize)
			{
				for (x=(LowerBoundary->x())+0.5*fCellSize; x<=(UpperBoundary->x()); x+=fCellSize)
				{
					dos.SetPosition(vedo::Vector3df(x, y, z));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
				}
			}
			bLayer = false;
			if ((z + fCellSize) <= UpperBoundary->z())
			{
				z += 0.5 * fCellSize;
			}
			else
			{
				bContinue = false;
			}
		}
		else
		{
			for (y=(LowerBoundary->y()); y<=(UpperBoundary->y()); y+=fCellSize)
			{
				for (x=(LowerBoundary->x())+0.5*fCellSize; x<=(UpperBoundary->x()); x+=fCellSize)
				{
					dos.SetPosition(vedo::Vector3df(x, y, z));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
				}
			}
			for (y=(LowerBoundary->y())+0.5*fCellSize; y<=(UpperBoundary->y()); y+=fCellSize)
			{
				for (x=(LowerBoundary->x()); x<=(UpperBoundary->x()); x+=fCellSize)
				{
					dos.SetPosition(vedo::Vector3df(x, y, z));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
				}
			}
			bLayer = true;
			if ((z + fCellSize) <= UpperBoundary->z())
			{
				z += 0.5 * fCellSize;
			}
			else
			{
				bContinue = false;
			}
		}
	}
}

void AddDOInSpaceSC(vedo::Vector3df* LowerBoundary, vedo::Vector3df* UpperBoundary, std::string DOName, vedo::DOWorld* oWorld)
{
	vedo::_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	*LowerBoundary += vedo::Vector3df(R, R, R);
	*UpperBoundary -= vedo::Vector3df(R, R, R);
	for (vedo::_float_t y=(LowerBoundary->y()); y<=(UpperBoundary->y()); y+=(2.0*R))
    {
	    for (vedo::_float_t z=(LowerBoundary->z()); z<=(UpperBoundary->z()); z+=(2.0*R))
    	{
		    for (vedo::_float_t x=(LowerBoundary->x()); x<=(UpperBoundary->x()); x+=(2.0*R))
    		{
				dos.SetPosition(vedo::Vector3df(x, y, z));
				oWorld->AddDOStatus(new vedo::DOStatus(dos));
	    	}
	    }
    }
}

void AddPotentialEnergy(vedo::Vector3df* SourcePoint, std::string DOName, vedo::DOWorld* oWorld)
{
	vedo::Vector3df vel;
	const vedo::Vector3df ff = oWorld->GetSystemParameter()->GetFieldAcceleration();
	vedo::DOStatus new_dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	const vedo::DOStatus* dos = 0;
    vedo::_float_t TempVelocitySquare;
	for (vedo::_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
	{
		dos = oWorld->GetDOStatus(ul);
		if (dos->GetDOName() == DOName)
		{
			vel = dos->GetVelocity();
			new_dos = *dos;
            TempVelocitySquare = vel.dot(vel) + 2.0 * ff.dot(dos->GetPosition() - *SourcePoint);
            if (TempVelocitySquare >= 0.0)
            {
    			new_dos.SetVelocity(sqrt(TempVelocitySquare) * ff.direction());
            }
            else
            {
    			new_dos.SetVelocity(-1.0 * sqrt(-1.0 * TempVelocitySquare) * ff.direction());
            }
			oWorld->SetDOStatus(ul, new_dos);
		}
	}
}

void AddExternalVelocity(vedo::Vector3df* pExternalVelocity, std::string DOName, vedo::DOWorld* oWorld)
{
	vedo::Vector3df vel;
	vedo::DOStatus new_dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	const vedo::DOStatus* dos = 0;
	for (vedo::_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
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
}

void AddExternalAngularVelocity(vedo::Vector3df* pExternalAngularVelocity, std::string DOName, vedo::DOWorld* oWorld)
{
	vedo::Vector3df avel;
	vedo::DOStatus new_dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	const vedo::DOStatus* dos = 0;
	for (vedo::_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
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
}

void AddRandomDOInCartesianSpace
	(vedo::Vector3df* LowerBoundary, vedo::Vector3df* UpperBoundary,
	 std::string DOName, vedo::_uint_t* num, vedo::DOWorld* oWorld)
{
	vedo::_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	*LowerBoundary += vedo::Vector3df(R, R, R);
	*UpperBoundary -= vedo::Vector3df(R, R, R);
	vedo::_float_t xRange = (UpperBoundary->x()) - (LowerBoundary->x());
	vedo::_float_t yRange = (UpperBoundary->y()) - (LowerBoundary->y());
	vedo::_float_t zRange = (UpperBoundary->z()) - (LowerBoundary->z());
	vedo::_float_t temp_x, temp_y, temp_z;
	srand(time(0));
	for (vedo::_uint_t ul=0; ul<(*num); ul++)
	{
		temp_x = (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * xRange;
		temp_y = (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * yRange;
		temp_z = (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * zRange;
		dos.SetPosition(*LowerBoundary + vedo::Vector3df(temp_x, temp_y, temp_z));
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
	}
}

void AddRandomDOInCylindricalSpace
	(vedo::Vector3df* Center, vedo::_float_t* r, vedo::_float_t* h, std::string DOName,
	 vedo::_uint_t* num, vedo::DOWorld* oWorld                                           )
{
	vedo::_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());

	*r -= R;
	vedo::_float_t dSquare_r = (*r) * (*r);
	vedo::_float_t d2r       = 2.0  * (*r);
	vedo::_float_t dXMin     = Center->x() - (*r);
	//vedo::_float_t dXMax     = Center->x() + (*r);
	vedo::_float_t dYMin     = Center->y() - (*r);
	//vedo::_float_t dYMax     = Center->y() + (*r);
	vedo::_float_t dZMin     = Center->z() + R;
	//vedo::_float_t dZMax     = Center->z() + (*h) - R;
	*h -= 2.0 * R;

	vedo::_float_t temp_x, temp_y, temp_z;
	srand(time(0));
	vedo::_uint_t ul = 0;
	while (ul < (*num))
	{
		temp_x = dXMin + (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * d2r;
		temp_y = dYMin + (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * d2r;
		if (temp_x * temp_x + temp_y * temp_y <= dSquare_r)
		{
			temp_z = dZMin + (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * (*h);
			dos.SetPosition(vedo::Vector3df(temp_x, temp_y, temp_z));
			oWorld->AddDOStatus(new vedo::DOStatus(dos));
			ul++;
		}
	}
}

void AddRandomDOInSphericalSpace
	(vedo::Vector3df* Center, vedo::_float_t* r, std::string DOName, vedo::_uint_t* num, vedo::DOWorld* oWorld)
{
	vedo::_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	*r -= R;
	vedo::_float_t temp_x, temp_y, temp_z, temp_radius, temp_angle_1, temp_angle_2;
	srand(time(0));
	for (vedo::_uint_t ul=0; ul<(*num); ul++)
	{
		temp_radius  = (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * (*r);
		temp_angle_1 = (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * aries::math::_DoublePI;
		temp_angle_2 = (vedo::_float_t)(rand()) / (vedo::_float_t)RAND_MAX * aries::math::_DoublePI;
		temp_x       = Center->x() + temp_radius * cos(temp_angle_1) * cos(temp_angle_2);
		temp_y       = Center->y() + temp_radius * cos(temp_angle_1) * sin(temp_angle_2);
		temp_z       = Center->z() + temp_radius * sin(temp_angle_1);
		dos.SetPosition(vedo::Vector3df(temp_x, temp_y, temp_z));
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
	}
}

std::map<std::string, vedo::_float_t> CalculateGranularTemperature
	(vedo::DOWorld* oWorld, vedo::GeometricShape& space, std::string DOName)
{
	const vedo::DOStatus* pDOS;
	vedo::Vector3df AvgV, AvgAV;
	vedo::_uint_t Counter = 0;

	oWorld->TurnMonitor(false);
	// Calculate number of elements and average velocity
	for (vedo::_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
	{
		pDOS = oWorld->GetDOStatus(ul);
		if (pDOS->GetDOName() == DOName)
		{
			if (space.Inside(pDOS->GetPosition()))
			{
				AvgV  = AvgV  + pDOS->GetVelocity();
				AvgAV = AvgAV + pDOS->GetAngularVelocity();
				oWorld->SetMonitor(ul, true);
				Counter++;
			}
		}
	}
	if (Counter != 0)
	{
		AvgV  = 1.0 / vedo::_float_t(Counter) * AvgV;
		AvgAV = 1.0 / vedo::_float_t(Counter) * AvgAV;
	}

	// Calculate granular temperature
	vedo::_float_t dGranularTemperatureV  = 0.0;
	vedo::_float_t dGranularTemperatureAV = 0.0;
	for (vedo::_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
	{
		pDOS = oWorld->GetDOStatus(ul);
		if (pDOS->ISMonitored())
		{
			dGranularTemperatureV  += (pDOS->GetVelocity()        - AvgV ).dot(pDOS->GetVelocity()        - AvgV );
			dGranularTemperatureAV += (pDOS->GetAngularVelocity() - AvgAV).dot(pDOS->GetAngularVelocity() - AvgAV);
		}
	}
	if (Counter != 0)
	{
		dGranularTemperatureV  = dGranularTemperatureV  / (vedo::_float_t)Counter / 3.0;
		dGranularTemperatureAV = dGranularTemperatureAV / (vedo::_float_t)Counter / 3.0;
		for (vedo::_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (pDOS->ISMonitored())
			{
				oWorld->SetGranularTemperatureV (ul, dGranularTemperatureV );
				oWorld->SetGranularTemperatureAV(ul, dGranularTemperatureAV);
			}
		}
	}
	std::map<std::string, vedo::_float_t> gt;
	gt["Time"]                                 = oWorld->GetSystemParameter()->GetTimeCurrent();
	gt["NumberOfElement"]                      = (vedo::_float_t)Counter;
	gt["AverageVelocity"]                      = AvgV.length();
	gt["GranularTemperatureOfVelocity"]        = dGranularTemperatureV;
	gt["AverageAngularVelocity"]               = AvgAV.length();
	gt["GranularTemperatureOfAngularVelocity"] = dGranularTemperatureAV;
	return gt;
}

void CombineModels(vedo::DOWorld* rWorld, vedo::DOWorld* oWorld)
{
	//vedo::_uint_t rNum = rWorld->GetSystemParameter()->GetDONumber();
	vedo::_uint_t oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName", vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
	for (vedo::_uint_t ul=0; ul<oNum; ul++)
	{
		dos = *(oWorld->GetDOStatus(ul));
		if (oWorld
			->GetDOModel(oWorld->GetDOStatus(ul)->GetDOName())->GetBehavior()
				== vedo::mobile                              )
		{
			rWorld->AddDOStatus(new vedo::DOStatus(dos));
		}
	}
}

void CombineModels
	(vedo::DOWorld*       rWorld         ,
	 vedo::IactRecordTab* rpIactRecordTab,
	 vedo::DOWorld*       oWorld         ,
	 vedo::IactRecordTab* opIactRecordTab )
{
	//vedo::_uint_t rNum = rWorld->GetSystemParameter()->GetDONumber();
	vedo::_uint_t oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName", vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df(1.0, 0.0, 0.0), vedo::Vector3df(0.0, 0.0, 1.0), vedo::Vector3df(), vedo::Vector3df(), vedo::Vector3df());
		vedo::_uint_t ulCounter = 0;
	for (vedo::_uint_t ul=0; ul<oNum; ul++)
	{
		dos = *(oWorld->GetDOStatus(ul));
		if (oWorld
			->GetDOModel(oWorld->GetDOStatus(ul)->GetDOName())->GetBehavior()
				== vedo::mobile                              )
		{
			ulCounter++;
			rWorld->AddDOStatus(new vedo::DOStatus(dos));
		}
	}
}

void CombineModels(vedo::DOWorld* rWorld, vedo::DOWorld* oWorld, const std::string& sDOName)
{
	//vedo::_uint_t rNum = rWorld->GetSystemParameter()->GetDONumber();
	vedo::_uint_t oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName",
		vedo::Vector3df(),
		vedo::Vector3df(),
		vedo::Vector3df(1.0, 0.0, 0.0),
		vedo::Vector3df(0.0, 0.0, 1.0),
		vedo::Vector3df(),
		vedo::Vector3df(),
		vedo::Vector3df() );
	for (vedo::_uint_t ul=0; ul<oNum; ul++)
	{
		dos = *(oWorld->GetDOStatus(ul));
		if (oWorld->GetDOStatus(ul)->GetDOName() == sDOName)
		{
			rWorld->AddDOStatus(new vedo::DOStatus(dos));
		}
	}
}

int main(int argc, char* argv[])
{
	std::vector<std::string> arg;
	for (int i=0; i<argc; i++)
	{
		arg.push_back(argv[i]);
	}

	if (arg.size() < 2)
	{
		usage();
	}
	else if ((arg[1] == "-add_elements_in_body-centered_cubic_style") && (arg.size() == 11))
	{
		vedo::_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = aries::String2T<vedo::_float_t>(arg[3]);
		xmax = aries::String2T<vedo::_float_t>(arg[4]);
		ymin = aries::String2T<vedo::_float_t>(arg[5]);
		ymax = aries::String2T<vedo::_float_t>(arg[6]);
		zmin = aries::String2T<vedo::_float_t>(arg[7]);
		zmax = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::Vector3df LowerBoundary(xmin, ymin, zmin);
		vedo::Vector3df UpperBoundary(xmax, ymax, zmax);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		AddDOInSpaceBCC(&LowerBoundary, &UpperBoundary, DOName, oWorld);
		delete WriteDOWorld (arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_in_face-centered_cubic_style") && (arg.size() == 11))
	{
		vedo::_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = aries::String2T<vedo::_float_t>(arg[3]);
		xmax = aries::String2T<vedo::_float_t>(arg[4]);
		ymin = aries::String2T<vedo::_float_t>(arg[5]);
		ymax = aries::String2T<vedo::_float_t>(arg[6]);
		zmin = aries::String2T<vedo::_float_t>(arg[7]);
		zmax = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::Vector3df LowerBoundary(xmin, ymin, zmin);
		vedo::Vector3df UpperBoundary(xmax, ymax, zmax);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		AddDOInSpaceFCC(&LowerBoundary, &UpperBoundary, DOName, oWorld);
		delete WriteDOWorld (arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_in_simple_cubic_style") && (arg.size() == 11))
	{
		vedo::_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = aries::String2T<vedo::_float_t>(arg[3]);
		xmax = aries::String2T<vedo::_float_t>(arg[4]);
		ymin = aries::String2T<vedo::_float_t>(arg[5]);
		ymax = aries::String2T<vedo::_float_t>(arg[6]);
		zmin = aries::String2T<vedo::_float_t>(arg[7]);
		zmax = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::Vector3df LowerBoundary(xmin, ymin, zmin);
		vedo::Vector3df UpperBoundary(xmax, ymax, zmax);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		AddDOInSpaceSC(&LowerBoundary, &UpperBoundary, DOName, oWorld);
		delete WriteDOWorld (arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_pontential_energy_from_sorce_point") && (arg.size() == 8))
	{
		vedo::_float_t x, y, z;
		std::string DOName = arg[2];
		x = aries::String2T<vedo::_float_t>(arg[3]);
		y = aries::String2T<vedo::_float_t>(arg[4]);
		z = aries::String2T<vedo::_float_t>(arg[5]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		vedo::Vector3df SourcePoint(x, y, z);
		AddPotentialEnergy(&SourcePoint, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_in_circular_path") && (arg.size() == 12))
	{
		std::string sDOName = arg[2];
		vedo::_float_t
			dAngleStarted, dAngleEnded, dInclinedAngle,
			dRevolutionsPerTimeUnit, dEdgeVelocity, dR, dZ;
		vedo::_uint_t uElementNumber;
		dAngleStarted = aries::String2T<vedo::_float_t>(arg[3]);
		dAngleEnded   = aries::String2T<vedo::_float_t>(arg[4]);
		if (dAngleEnded <= dAngleStarted)
		{
			std::cerr
				<< "Error!! Code: Knight.cpp (-aecp)" << std::endl
				<< "        Note: Started angle is smaller than ended angle" << std::endl;
			exit(-1);
		}
		uElementNumber = aries::String2T<vedo::_uint_t>(arg[6]);
		dR = aries::String2T<vedo::_float_t>(arg[7]);
		dZ = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[10], pIactRecordTab);
		vedo::DOStatus dos
			(sDOName,
			vedo::Vector3df(),
			vedo::Vector3df(),
			vedo::Vector3df(0.0, 0.0, 1.0),
			vedo::Vector3df(1.0, 0.0, 0.0),
			vedo::Vector3df(),
            vedo::Vector3df(),
            vedo::Vector3df()              );
		vedo::_float_t dAngleRange = (dAngleEnded - dAngleStarted) * aries::math::_Degree2PI;
		vedo::_float_t dAngleIncremental = dAngleRange / (vedo::_float_t)uElementNumber;
		vedo::_float_t dAngle = dAngleStarted * aries::math::_Degree2PI;
		vedo::_float_t dCosineAngle, dSineAngle;
		vedo::Vector3df vLocalX, vLocalZ, vRotatedLocalX, vRotatedLocalZ;

		dInclinedAngle          = aries::String2T<vedo::_float_t>(arg[5]);
		dRevolutionsPerTimeUnit = aries::String2T<vedo::_float_t>(arg[9]);
		if (dInclinedAngle == 0.0)
		{
			if (dRevolutionsPerTimeUnit == 0)
			{
				for (vedo::_uint_t u=0; u<uElementNumber; u++)
				{
					dCosineAngle = cos(dAngle);
					dSineAngle   = sin(dAngle);
					dos.SetPosition(vedo::Vector3df(dR*dCosineAngle, dR*dSineAngle, dZ));
					dos.SetOrientationZ(vedo::Vector3df(dCosineAngle, dSineAngle, 0.0));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
					dAngle += dAngleIncremental;
				}
			}
			else
			{
				dEdgeVelocity = dR * dRevolutionsPerTimeUnit;
				for (vedo::_uint_t u=0; u<uElementNumber; u++)
				{
					dCosineAngle = cos(dAngle);
					dSineAngle   = sin(dAngle);
					dos.SetPosition(vedo::Vector3df(dR*dCosineAngle, dR*dSineAngle, dZ));
					dos.SetOrientationZ(vedo::Vector3df(dCosineAngle, dSineAngle, 0.0));
					dos.SetVelocity(-dEdgeVelocity * dos.GetOrientationZ().cross(vedo::Vector3df(0.0, 0.0, 1.0)));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
					dAngle += dAngleIncremental;
				}
			}
		}
		else
		{
			dInclinedAngle *= aries::math::_Degree2PI;
			vedo::_float_t dInclinedCosineAngle = cos(dInclinedAngle);
			vedo::_float_t dInclinedSineAngle   = sin(dInclinedAngle);
			dR += dR * dInclinedSineAngle;
			if (dRevolutionsPerTimeUnit == 0)
			{
				for (vedo::_uint_t u=0; u<uElementNumber; u++)
				{
					dCosineAngle = cos(dAngle);
					dSineAngle   = sin(dAngle);
					dos.SetPosition(vedo::Vector3df(dR*dCosineAngle, dR*dSineAngle, dZ));
					vLocalX.set
						(dInclinedSineAngle   * dCosineAngle,
						 dInclinedSineAngle   * dSineAngle  ,
						 dInclinedCosineAngle                );
					vLocalZ.set
						( dInclinedCosineAngle * dCosineAngle,
						  dInclinedCosineAngle * dSineAngle  ,
						 -dInclinedSineAngle                  );
					dos.SetOrientationX(vLocalX);
					dos.SetOrientationZ(vLocalZ);
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
					dAngle += dAngleIncremental;
				}
			}
			else
			{
				dEdgeVelocity = dR * dRevolutionsPerTimeUnit;
				for (vedo::_uint_t u=0; u<uElementNumber; u++)
				{
					dCosineAngle = cos(dAngle);
					dSineAngle   = sin(dAngle);
					dos.SetPosition(vedo::Vector3df(dR*dCosineAngle, dR*dSineAngle, dZ));
					vLocalX.set
						(dInclinedSineAngle   * dCosineAngle,
						 dInclinedSineAngle   * dSineAngle  ,
						 dInclinedCosineAngle                );
					vLocalZ.set
						( dInclinedCosineAngle * dCosineAngle,
						  dInclinedCosineAngle * dSineAngle  ,
						 -dInclinedSineAngle                  );
					dos.SetVelocity(-dEdgeVelocity * vLocalZ.cross(vLocalX));
					dos.SetOrientationX(vLocalX);
					dos.SetOrientationZ(vLocalZ);
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
					dAngle += dAngleIncremental;
				}
			}
		}
		delete WriteDOWorld (arg[11], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_single_element") && (arg.size() == 20))
	{
		vedo::_float_t dPx, dPy, dPz, dVx, dVy, dVz, dAVx, dAVy, dAVz;
		vedo::_float_t dOxx, dOxy, dOxz, dOzx, dOzy, dOzz;
		std::string sDOName = arg[2];
		dPx  = aries::String2T<vedo::_float_t>(arg[ 3]);
		dPy  = aries::String2T<vedo::_float_t>(arg[ 4]);
		dPz  = aries::String2T<vedo::_float_t>(arg[ 5]);
		dVx  = aries::String2T<vedo::_float_t>(arg[ 6]);
		dVy  = aries::String2T<vedo::_float_t>(arg[ 7]);
		dVz  = aries::String2T<vedo::_float_t>(arg[ 8]);
		dOxx = aries::String2T<vedo::_float_t>(arg[ 9]);
		dOxy = aries::String2T<vedo::_float_t>(arg[10]);
		dOxz = aries::String2T<vedo::_float_t>(arg[11]);
		dOzx = aries::String2T<vedo::_float_t>(arg[12]);
		dOzy = aries::String2T<vedo::_float_t>(arg[13]);
		dOzz = aries::String2T<vedo::_float_t>(arg[14]);
		dAVx = aries::String2T<vedo::_float_t>(arg[15]);
		dAVy = aries::String2T<vedo::_float_t>(arg[16]);
		dAVz = aries::String2T<vedo::_float_t>(arg[17]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[18], pIactRecordTab);
		vedo::DOStatus dos
			(sDOName,
			vedo::Vector3df(dPx , dPy , dPz ),
			vedo::Vector3df(dVx , dVy , dVz ),
			vedo::Vector3df(dOxx, dOxy, dOxz).direction(),
			vedo::Vector3df(dOzx, dOzy, dOzz).direction(),
			vedo::Vector3df(dAVx, dAVy, dAVz),
            vedo::Vector3df(),
            vedo::Vector3df());
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
		delete WriteDOWorld (arg[19], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_velocity") && (arg.size() == 8))
	{
		vedo::_float_t dVx, dVy, dVz;
		std::string DOName = arg[2];
		dVx = aries::String2T<vedo::_float_t>(arg[3]);
		dVy = aries::String2T<vedo::_float_t>(arg[4]);
		dVz = aries::String2T<vedo::_float_t>(arg[5]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		vedo::Vector3df vExternalVelocity(dVx, dVy, dVz);
		AddExternalVelocity(&vExternalVelocity, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_angular_velocity") && (arg.size() == 8))
	{
		vedo::_float_t dAVx, dAVy, dAVz;
		std::string DOName = arg[2];
		dAVx = aries::String2T<vedo::_float_t>(arg[3]);
		dAVy = aries::String2T<vedo::_float_t>(arg[4]);
		dAVz = aries::String2T<vedo::_float_t>(arg[5]);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		vedo::Vector3df vExternalAngularVelocity(dAVx, dAVy, dAVz);
		AddExternalAngularVelocity(&vExternalAngularVelocity, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_randomly_in_Cartesian_coordinate") && (arg.size() == 12))
	{
		vedo::_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		vedo::_uint_t num = 0;
		std::string DOName = arg[2];
		num  = aries::String2T<vedo::_uint_t>(arg[3]);
		xmin = aries::String2T<vedo::_float_t>(arg[4]);
		xmax = aries::String2T<vedo::_float_t>(arg[5]);
		ymin = aries::String2T<vedo::_float_t>(arg[6]);
		ymax = aries::String2T<vedo::_float_t>(arg[7]);
		zmin = aries::String2T<vedo::_float_t>(arg[8]);
		zmax = aries::String2T<vedo::_float_t>(arg[9]);
		vedo::Vector3df LowerBoundary(xmin, ymin, zmin);
		vedo::Vector3df UpperBoundary(xmax, ymax, zmax);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[10], pIactRecordTab);
		AddRandomDOInCartesianSpace(&LowerBoundary, &UpperBoundary, DOName, &num, oWorld);
		delete WriteDOWorld (arg[11], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_randomly_in_cylindrical_coordinate") && (arg.size() == 11))
	{
		vedo::_float_t x, y, z, r, h;
		vedo::_uint_t num = 0;
		std::string DOName = arg[2];
		num = aries::String2T<vedo::_uint_t>(arg[3]);
		x   = aries::String2T<vedo::_float_t>(arg[4]);
		y   = aries::String2T<vedo::_float_t>(arg[5]);
		z   = aries::String2T<vedo::_float_t>(arg[6]);
		r   = aries::String2T<vedo::_float_t>(arg[7]);
		h   = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::Vector3df Center(x, y, z);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		AddRandomDOInCylindricalSpace(&Center, &r, &h, DOName, &num, oWorld);
		delete WriteDOWorld (arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_randomly_in_spherical_coordinate") && (arg.size() == 10))
	{
		vedo::_float_t x, y, z, r;
		vedo::_uint_t num = 0;
		std::string DOName = arg[2];
		num = aries::String2T<vedo::_uint_t>(arg[3]);
		x   = aries::String2T<vedo::_float_t>(arg[4]);
		y   = aries::String2T<vedo::_float_t>(arg[5]);
		z   = aries::String2T<vedo::_float_t>(arg[6]);
		r   = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Vector3df Center(x, y, z);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[8], pIactRecordTab);
		AddRandomDOInSphericalSpace(&Center, &r, DOName, &num, oWorld);
		delete WriteDOWorld (arg[9], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
/*
	else if (arg[1] == "-al")
	{
		AssmListHEX();
	}
	else if ((arg[1] == "-assemble_xml") && (arg.size() == 5))
	{
		assm(arg[2], arg[3], arg[4]);
	}
*/
	else if ((arg[1] == "-average_information") && (arg.size() == 10))
	{
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[2]);
		dXMax = aries::String2T<vedo::_float_t>(arg[3]);
		dYMin = aries::String2T<vedo::_float_t>(arg[4]);
		dYMax = aries::String2T<vedo::_float_t>(arg[5]);
		dZMin = aries::String2T<vedo::_float_t>(arg[6]);
		dZMax = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );
		dMeshSize = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		vedo::_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::_uint_t ul = 0;
		do
		{
			if (ul < oWorld->GetSystemParameter()->GetDONumber())
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
		for (ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
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
		}
		if (dXMin < dNewXMin) dXMin = dNewXMin;
		if (dXMax > dNewXMax) dXMax = dNewXMax;
		if (dYMin < dNewYMin) dYMin = dNewYMin;
		if (dYMax > dNewYMax) dYMax = dNewYMax;
		if (dZMin < dNewZMin) dZMin = dNewZMin;
		if (dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );

		// Calculate the average information
		vedo::_float_t dTotalVolume = 0.0;
		std::vector<vedo::_float_t> vVolume;
		vedo::_float_t dTotalMass   = 0.0;
		vedo::Vector3df vMassCenter, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::_float_t dWeighting;
		std::vector<vedo::Vector3df> vvAverageVelocity, vvAverageAngularVelocity;

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
		vedo::Vector3df vTempVelocity, vTempAngularVelocity;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::_float_t, vedo::Vector3df> pdvVolume
				 = dom
				 	->VolumeInsideBoundary(pDOS->GetPosition(), &BC, dMeshSize);
			if ((dom->GetShapeType() == vedo::Sphere) && (pdvVolume.first != 0.0))
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
			if (pdvVolume.first != 0.0)
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

		vedo::_float_t dVolumeRatio = dTotalVolume / OriginalBC.GetVolume();
		if (dTotalMass != 0.0)
		{
			vMassCenter = 1 / dTotalMass * vMassCenter;
		}
		else
		{
			vMassCenter = OriginalBC.GetCenter();
		}

		vedo::_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dTotalVolume != 0.0)
		{
			vAverageVelocity        = 1.0 / dTotalVolume * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dTotalVolume * vAverageAngularVelocity;
			vedo::Vector3df vVTemp1, vVTemp2;
			for (vedo::_uint_t ul=0; ul<vVolume.size(); ul++)
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
			}
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
			if (dGranularTemperatureVelocity != 0.0)
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
			}
			if (dGranularTemperatureAngularVelocity != 0.0)
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
			}
		}
		else
		{
			vAverageVelocity.set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.set(0.0, 0.0, 0.0);
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
	else if ((arg[1] == "-average_information") && (arg.size() == 11))
	{
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[2]);
		dXMax = aries::String2T<vedo::_float_t>(arg[3]);
		dYMin = aries::String2T<vedo::_float_t>(arg[4]);
		dYMax = aries::String2T<vedo::_float_t>(arg[5]);
		dZMin = aries::String2T<vedo::_float_t>(arg[6]);
		dZMax = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );
		dMeshSize = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[9]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[10]);
		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const vedo::_float_t dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		vedo::_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool bHasFindThe1stSphere = false;
		vedo::_uint_t ul = 0;
		do
		{
			if (ul < oWorld->GetSystemParameter()->GetDONumber())
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
		for (ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
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
		}
		if (dXMin < dNewXMin) dXMin = dNewXMin;
		if (dXMax > dNewXMax) dXMax = dNewXMax;
		if (dYMin < dNewYMin) dYMin = dNewYMin;
		if (dYMax > dNewYMax) dYMax = dNewYMax;
		if (dZMin < dNewZMin) dZMin = dNewZMin;
		if (dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );

		// Calculate the average information
		vedo::_float_t dTotalVolume = 0.0;
		std::vector<vedo::_float_t> vVolume;
		vedo::_float_t dTotalMass   = 0.0;
		vedo::Vector3df vMassCenter, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::_float_t dWeighting;
		std::vector<vedo::Vector3df> vvAverageVelocity, vvAverageAngularVelocity;

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
		vedo::Vector3df vTempVelocity, vTempAngularVelocity;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS         = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom          = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::_float_t, vedo::Vector3df> pdvVolume
				 = dom
				 	->VolumeInsideBoundary(pDOS->GetPosition(), &BC, dMeshSize);
			if ((dom->GetShapeType() == vedo::Sphere) && (pdvVolume.first != 0.0))
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
			if (pdvVolume.first != 0.0)
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

		vedo::_float_t dVolumeRatio = dTotalVolume / OriginalBC.GetVolume();
		if (dTotalMass != 0.0)
		{
			vMassCenter = 1 / dTotalMass * vMassCenter;
		}
		else
		{
			vMassCenter = OriginalBC.GetCenter();
		}

		vedo::_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dTotalVolume != 0.0)
		{
			vAverageVelocity        = 1.0 / dTotalVolume * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dTotalVolume * vAverageAngularVelocity;
			vedo::Vector3df vVTemp1, vVTemp2;
			for (vedo::_uint_t ul=0; ul<vVolume.size(); ul++)
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
			}
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
			if (dGranularTemperatureVelocity != 0.0)
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
			}
			if (dGranularTemperatureAngularVelocity != 0.0)
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
			}
		}
		else
		{
			vAverageVelocity.set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.set(0.0, 0.0, 0.0);
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
	else if ((arg[1] == "-average_information_projected_in_xy_plane") && (arg.size() == 10))
	{
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[2]);
		dXMax = aries::String2T<vedo::_float_t>(arg[3]);
		dYMin = aries::String2T<vedo::_float_t>(arg[4]);
		dYMax = aries::String2T<vedo::_float_t>(arg[5]);
		dZMin = aries::String2T<vedo::_float_t>(arg[6]);
		dZMax = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );
		dMeshSize = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		vedo::_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::_uint_t ul = 0;
		do
		{
			if (ul < oWorld->GetSystemParameter()->GetDONumber())
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
		for (ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
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
		}
		if (dXMin < dNewXMin) dXMin = dNewXMin;
		if (dXMax > dNewXMax) dXMax = dNewXMax;
		if (dYMin < dNewYMin) dYMin = dNewYMin;
		if (dYMax > dNewYMax) dYMax = dNewYMax;
		if (dZMin < dNewZMin) dZMin = dNewZMin;
		if (dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::_float_t dProjectedArea = 0.0;
		std::vector<vedo::_float_t> vProjectedArea;
		vedo::Vector3df vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::_float_t dWeighting;
		vedo::_float_t dTotalWeighting = 0.0;
		std::vector<vedo::Vector3df> vvAverageVelocity, vvAverageAngularVelocity;

		vedo::Vector3df vTempVelocity, vTempAngularVelocity;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::_float_t, vedo::Vector3df> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = pDOS->GetVelocity();
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if (pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		vedo::_float_t dProjectedAreaRatio
			= dProjectedArea
			/ OriginalBC.GetLength().x()
			/ OriginalBC.GetLength().y();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.set
				(vCentroid.x() / dTotalWeighting,
				 vCentroid.y() / dTotalWeighting,
				 OriginalBC.GetCenter().z()      );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			vedo::Vector3df vVTemp1, vVTemp2;
			for (vedo::_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
			}
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
			if (dGranularTemperatureVelocity != 0.0)
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
			}
			if (dGranularTemperatureAngularVelocity != 0.0)
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
			}
		}
		else
		{
			vAverageVelocity.set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.set(0.0, 0.0, 0.0);
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
	else if ((arg[1] == "-average_information_projected_in_xy_plane") && (arg.size() == 11))
	{
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[2]);
		dXMax = aries::String2T<vedo::_float_t>(arg[3]);
		dYMin = aries::String2T<vedo::_float_t>(arg[4]);
		dYMax = aries::String2T<vedo::_float_t>(arg[5]);
		dZMin = aries::String2T<vedo::_float_t>(arg[6]);
		dZMax = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );
		dMeshSize = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[9]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[10]);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const vedo::_float_t dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		vedo::_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::_uint_t ul = 0;
		do
		{
			if (ul < oWorld->GetSystemParameter()->GetDONumber())
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
		for (ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
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
		}
		if (dXMin < dNewXMin) dXMin = dNewXMin;
		if (dXMax > dNewXMax) dXMax = dNewXMax;
		if (dYMin < dNewYMin) dYMin = dNewYMin;
		if (dYMax > dNewYMax) dYMax = dNewYMax;
		if (dZMin < dNewZMin) dZMin = dNewZMin;
		if (dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::_float_t dProjectedArea = 0.0;
		std::vector<vedo::_float_t> vProjectedArea;
		vedo::Vector3df vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::_float_t dWeighting;
		vedo::_float_t dTotalWeighting = 0.0;
		std::vector<vedo::Vector3df> vvAverageVelocity, vvAverageAngularVelocity;

		vedo::Vector3df vTempVelocity, vTempAngularVelocity;
		for
			(vedo::_uint_t ul=0;
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

			std::pair<vedo::_float_t, vedo::Vector3df> pdvProjectedArea
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
			if (pdvProjectedArea.first != 0.0)
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

		vedo::_float_t dProjectedAreaRatio
			= dProjectedArea
			/ OriginalBC.GetLength().x()
			/ OriginalBC.GetLength().y();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.set
				(vCentroid.x() / dTotalWeighting,
				 vCentroid.y() / dTotalWeighting,
				 OriginalBC.GetCenter().z()      );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			vedo::Vector3df vVTemp1, vVTemp2;
			for (vedo::_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
			}
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
			if (dGranularTemperatureVelocity != 0.0)
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
			}
			if (dGranularTemperatureAngularVelocity != 0.0)
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
			}
		}
		else
		{
			vAverageVelocity.set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.set(0.0, 0.0, 0.0);
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
	else if ((arg[1] == "-average_information_projected_in_yz_plane") && (arg.size() == 10))
	{
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[2]);
		dXMax = aries::String2T<vedo::_float_t>(arg[3]);
		dYMin = aries::String2T<vedo::_float_t>(arg[4]);
		dYMax = aries::String2T<vedo::_float_t>(arg[5]);
		dZMin = aries::String2T<vedo::_float_t>(arg[6]);
		dZMax = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );
		dMeshSize = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		vedo::_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::_uint_t ul = 0;
		do
		{
			if (ul < oWorld->GetSystemParameter()->GetDONumber())
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
		for (ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
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
		}
		if (dXMin < dNewXMin) dXMin = dNewXMin;
		if (dXMax > dNewXMax) dXMax = dNewXMax;
		if (dYMin < dNewYMin) dYMin = dNewYMin;
		if (dYMax > dNewYMax) dYMax = dNewYMax;
		if (dZMin < dNewZMin) dZMin = dNewZMin;
		if (dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::_float_t dProjectedArea = 0.0;
		std::vector<vedo::_float_t> vProjectedArea;
		vedo::Vector3df vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::_float_t dWeighting;
		vedo::_float_t dTotalWeighting = 0.0;
		std::vector<vedo::Vector3df> vvAverageVelocity, vvAverageAngularVelocity;

		vedo::Vector3df vTempVelocity, vTempAngularVelocity;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::_float_t, vedo::Vector3df> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = pDOS->GetVelocity();
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if (pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		vedo::_float_t dProjectedAreaRatio
			= dProjectedArea / BC.GetLength().y() / OriginalBC.GetLength().z();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.set
				(OriginalBC.GetCenter().x()     ,
				 vCentroid.y() / dTotalWeighting,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			vedo::Vector3df vVTemp1, vVTemp2;
			for (vedo::_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
			}
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
			if (dGranularTemperatureVelocity != 0.0)
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
			}
			if (dGranularTemperatureAngularVelocity != 0.0)
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
			}
		}
		else
		{
			vAverageVelocity.set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.set(0.0, 0.0, 0.0);
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
	else if ((arg[1] == "-average_information_projected_in_yz_plane") && (arg.size() == 11))
	{
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[2]);
		dXMax = aries::String2T<vedo::_float_t>(arg[3]);
		dYMin = aries::String2T<vedo::_float_t>(arg[4]);
		dYMax = aries::String2T<vedo::_float_t>(arg[5]);
		dZMin = aries::String2T<vedo::_float_t>(arg[6]);
		dZMax = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );
		dMeshSize = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[9]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[10]);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const vedo::_float_t dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		vedo::_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::_uint_t ul = 0;
		do
		{
			if (ul < oWorld->GetSystemParameter()->GetDONumber())
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
		for (ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
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
		}
		if (dXMin < dNewXMin) dXMin = dNewXMin;
		if (dXMax > dNewXMax) dXMax = dNewXMax;
		if (dYMin < dNewYMin) dYMin = dNewYMin;
		if (dYMax > dNewYMax) dYMax = dNewYMax;
		if (dZMin < dNewZMin) dZMin = dNewZMin;
		if (dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::_float_t dProjectedArea = 0.0;
		std::vector<vedo::_float_t> vProjectedArea;
		vedo::Vector3df vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::_float_t dWeighting;
		vedo::_float_t dTotalWeighting = 0.0;
		std::vector<vedo::Vector3df> vvAverageVelocity, vvAverageAngularVelocity;

		vedo::Vector3df vTempVelocity, vTempAngularVelocity;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::_float_t, vedo::Vector3df> pdvProjectedArea
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
			if (pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		vedo::_float_t dProjectedAreaRatio
			= dProjectedArea / BC.GetLength().y() / OriginalBC.GetLength().z();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.set
				(OriginalBC.GetCenter().x()     ,
				 vCentroid.y() / dTotalWeighting,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			vedo::Vector3df vVTemp1, vVTemp2;
			for (vedo::_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
			}
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
			if (dGranularTemperatureVelocity != 0.0)
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
			}
			if (dGranularTemperatureAngularVelocity != 0.0)
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
			}
		}
		else
		{
			vAverageVelocity.set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.set(0.0, 0.0, 0.0);
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
	else if ((arg[1] == "-average_information_projected_in_xz_plane") && (arg.size() == 10))
	{
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[2]);
		dXMax = aries::String2T<vedo::_float_t>(arg[3]);
		dYMin = aries::String2T<vedo::_float_t>(arg[4]);
		dYMax = aries::String2T<vedo::_float_t>(arg[5]);
		dZMin = aries::String2T<vedo::_float_t>(arg[6]);
		dZMax = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );
		dMeshSize = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		vedo::_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::_uint_t ul = 0;
		do
		{
			if (ul < oWorld->GetSystemParameter()->GetDONumber())
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
		for (ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
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
		}
		if (dXMin < dNewXMin) dXMin = dNewXMin;
		if (dXMax > dNewXMax) dXMax = dNewXMax;
		if (dYMin < dNewYMin) dYMin = dNewYMin;
		if (dYMax > dNewYMax) dYMax = dNewYMax;
		if (dZMin < dNewZMin) dZMin = dNewZMin;
		if (dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::_float_t dProjectedArea = 0.0;
		std::vector<vedo::_float_t> vProjectedArea;
		vedo::Vector3df vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::_float_t dWeighting;
		vedo::_float_t dTotalWeighting = 0.0;
		std::vector<vedo::Vector3df> vvAverageVelocity, vvAverageAngularVelocity;

		vedo::Vector3df vTempVelocity, vTempAngularVelocity;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::_float_t, vedo::Vector3df> pdvProjectedArea
				 = dom
				 	->ProjectedAreaOnXYPlane
				 		(pDOS->GetPosition(), &BC, dMeshSize);
			dWeighting           = dom->GetDensity() * dom->GetDensityFactor();
			vTempVelocity        = pDOS->GetVelocity();
			vTempAngularVelocity = pDOS->GetAngularVelocity();
			vvAverageVelocity.push_back(vTempVelocity);
			vvAverageAngularVelocity.push_back(vTempAngularVelocity);
			if (pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		vedo::_float_t dProjectedAreaRatio
			= dProjectedArea / OriginalBC.GetLength().x() / OriginalBC.GetLength().z();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.set
				(vCentroid.x() / dTotalWeighting,
				 OriginalBC.GetCenter().y()     ,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			vedo::Vector3df vVTemp1, vVTemp2;
			for (vedo::_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
			}
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
			if (dGranularTemperatureVelocity != 0.0)
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
			}
			if (dGranularTemperatureAngularVelocity != 0.0)
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
			}
		}
		else
		{
			vAverageVelocity.set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.set(0.0, 0.0, 0.0);
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
	else if ((arg[1] == "-average_information_projected_in_xz_plane") && (arg.size() == 11))
	{
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[2]);
		dXMax = aries::String2T<vedo::_float_t>(arg[3]);
		dYMin = aries::String2T<vedo::_float_t>(arg[4]);
		dYMax = aries::String2T<vedo::_float_t>(arg[5]);
		dZMin = aries::String2T<vedo::_float_t>(arg[6]);
		dZMax = aries::String2T<vedo::_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );
		dMeshSize = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[9]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[10]);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const vedo::_float_t dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		vedo::_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::_uint_t ul = 0;
		do
		{
			if (ul < oWorld->GetSystemParameter()->GetDONumber())
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
		for (ul++;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
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
		}
		if (dXMin < dNewXMin) dXMin = dNewXMin;
		if (dXMax > dNewXMax) dXMax = dNewXMax;
		if (dYMin < dNewYMin) dYMin = dNewYMin;
		if (dYMax > dNewYMax) dYMax = dNewYMax;
		if (dZMin < dNewZMin) dZMin = dNewZMin;
		if (dZMax > dNewZMax) dZMax = dNewZMax;
		vedo::Boundary BC
			(vedo::Vector3df(dXMin, dYMin, dZMin),
			 vedo::Vector3df(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::_float_t dProjectedArea = 0.0;
		std::vector<vedo::_float_t> vProjectedArea;
		vedo::Vector3df vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::_float_t dWeighting;
		vedo::_float_t dTotalWeighting = 0.0;
		std::vector<vedo::Vector3df> vvAverageVelocity, vvAverageAngularVelocity;

		vedo::Vector3df vTempVelocity, vTempAngularVelocity;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::_float_t, vedo::Vector3df> pdvProjectedArea
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
			if (pdvProjectedArea.first != 0.0)
			{
				vProjectedArea.push_back(pdvProjectedArea.first);
				dProjectedArea          += pdvProjectedArea.first;
				dTotalWeighting         += pdvProjectedArea.first * dWeighting;
				vCentroid               += pdvProjectedArea.first * dWeighting * pdvProjectedArea.second;
				vAverageVelocity        += pdvProjectedArea.first * vTempVelocity;
				vAverageAngularVelocity += pdvProjectedArea.first * vTempAngularVelocity;
			}
		}

		vedo::_float_t dProjectedAreaRatio
			= dProjectedArea / OriginalBC.GetLength().x() / OriginalBC.GetLength().z();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.set
				(vCentroid.x() / dTotalWeighting,
				 OriginalBC.GetCenter().y()     ,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			vedo::Vector3df vVTemp1, vVTemp2;
			for (vedo::_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
			}
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
			if (dGranularTemperatureVelocity != 0.0)
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
			}
			if (dGranularTemperatureAngularVelocity != 0.0)
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
			}
		}
		else
		{
			vAverageVelocity.set(0.0, 0.0, 0.0);
			vAverageAngularVelocity.set(0.0, 0.0, 0.0);
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
	else if ((arg[1] == "-convert") && (arg.size() == 4))
	{
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		delete WriteDOWorld(arg[3], ReadDOWorld(arg[2], pIactRecordTab), pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-cross_section_area_on_surface") && (arg.size() == 8))
	{
		std::string DOName = arg[2];
		vedo::_float_t a, b, c, d;
		a = aries::String2T<vedo::_float_t>(arg[3]);
		b = aries::String2T<vedo::_float_t>(arg[4]);
		c = aries::String2T<vedo::_float_t>(arg[5]);
		d = aries::String2T<vedo::_float_t>(arg[6]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[7]);
		vedo::_uint_t counter   = 0;
		vedo::_float_t   dArea     = 0.0;
		vedo::_float_t   dAreaTemp;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			if (oWorld->GetDOStatus(ul)->GetDOName() == DOName)
			{
				dAreaTemp
					= oWorld
					->GetDOModel(DOName)
					->CrossAreaToSurface
						(oWorld->GetDOStatus(ul)->GetPosition(), a, b, c, d);
				if (dAreaTemp != 0.0)
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
			<< "cross area        : " << dArea
			<< '(' << counter << " elements)"           << std::endl;
	}
	else if ((arg[1] == "-cross_section_flux_on_surface") && (arg.size() == 8))
	{
		std::string DOName = arg[2];
		vedo::_float_t a, b, c, d;
		a = aries::String2T<vedo::_float_t>(arg[3]);
		b = aries::String2T<vedo::_float_t>(arg[4]);
		c = aries::String2T<vedo::_float_t>(arg[5]);
		d = aries::String2T<vedo::_float_t>(arg[6]);
		vedo::DOWorld*    oWorld        = ReadDOWorld(arg[7]);
		vedo::_uint_t    counter       = 0;
		vedo::_float_t      dArea         = 0.0;
		vedo::_float_t      dAreaTemp;
		vedo::Vector3df vFluxVelocity, vFluxAngularVelocity;
		for
			(vedo::_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			if (oWorld->GetDOStatus(ul)->GetDOName() == DOName)
			{
				dAreaTemp
					= oWorld
					->GetDOModel(DOName)
					->CrossAreaToSurface
						(oWorld->GetDOStatus(ul)->GetPosition(), a, b, c, d);
				if (dAreaTemp != 0.0)
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
/*
	else if ((arg[1] == "-cal") && (arg.size() == 9))
	{
		vedo::DOWorld *oWorld = ReadDOWorld(arg[2]);
		vedo::_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		CalcCoordinateNum CCN;
		CalcDensity CDN;
		xmin = aries::String2T<vedo::_float_t>(arg[3]);
		xmax = aries::String2T<vedo::_float_t>(arg[4]);
		ymin = aries::String2T<vedo::_float_t>(arg[5]);
		ymax = aries::String2T<vedo::_float_t>(arg[6]);
		zmin = aries::String2T<vedo::_float_t>(arg[7]);
		zmax = aries::String2T<vedo::_float_t>(arg[8]);
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
*/
/*
	else if ((arg[1] == "-cl") && (arg.size() == 5))
	{
		ConListHEX(arg[2], atol(arg[3].c_str()), atol(arg[4].c_str()));
	}
*/
	else if ((arg[1] == "-clean_user_defined_value") && (arg.size() == 4))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
        vedo::DOWorld* oWorld = ReadDOWorld(arg[2], pIactRecordTab);
		pIactRecordTab->CleanUserDefinedValueInImpactStatus();
        delete WriteDOWorld(arg[3], oWorld, pIactRecordTab);
        delete oWorld;
        delete pIactRecordTab;
	}
/*
	else if ((arg[1] == "-cld") && (arg.size() == 5))
	{
		ConList(arg[2], atol(arg[3].c_str()), atol(arg[4].c_str()));
	}
*/
	else if ((arg[1] == "-change_models") && (arg.size() == 5))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* rWorld = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3], pIactRecordTab);
		oWorld->SetModels(rWorld);
		delete WriteDOWorld(arg[4], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-combine") && (arg.size() == 5))
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
	else if ((arg[1] == "-combine") && (arg.size() == 6))
	{
	    // Aries: We only combine the DOStatus of two DOWorlds.
	    // In the future, we should also combine their Interactions (IactRecordTab).
		vedo::DOWorld* rWorld = ReadDOWorld(arg[2]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		std::string DOName = arg[4];
		CombineModels(rWorld, oWorld, DOName);
		delete WriteDOWorld(arg[5], rWorld);
	}
	else if ((arg[1] == "-convert_sudo_vpf") && (arg.size() == 5))
	{
		delete WriteDOWorldVPF(arg[4], ReadDOWorld(arg[2]), ReadDOWorld(arg[3]));
	}
/*
	else if ((arg[1] == "-b2h") && (arg.size() == 4))
	{
		njr::bin2hex(arg[2], arg[3]);
	}
	else if ((arg[1] == "-d2u") && (arg.size() == 4))
	{
		njr::dos2unix(arg[2], arg[3]);
	}
*/
	else if ((arg[1] == "-distance_to_point") && (arg.size() == 6))
	{
		vedo::_float_t dX, dY, dZ;
		dX = aries::String2T<vedo::_float_t>(arg[2]);
		dY = aries::String2T<vedo::_float_t>(arg[3]);
		dZ = aries::String2T<vedo::_float_t>(arg[4]);
		vedo::Vector3df vP(dX, dY, dZ);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5]);
		std::map<vedo::_float_t, vedo::_uint_t> mduDistance;

		for (vedo::_uint_t u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
			mduDistance[(oWorld->GetDOStatus(u)->GetPosition() - vP).length()] = u;

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ElementID, ElementName, Distance, X, Y, Z" << std::endl;
		vedo::_uint_t uRank = 1;
		for (std::map<vedo::_float_t, vedo::_uint_t>::iterator
			 mduiDistance  = mduDistance.begin(); mduiDistance != mduDistance.end(); mduiDistance++)
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
	else if ((arg[1] == "-distance_to_xy_plane") && (arg.size() == 5))
	{
		vedo::_float_t dX, dY;
		dX = aries::String2T<vedo::_float_t>(arg[2]);
		dY = aries::String2T<vedo::_float_t>(arg[3]);
		vedo::Vector3df vP(dX, dY, 0.0);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[4]);
		vedo::Vector3df vD;
		std::map<vedo::_float_t, vedo::_uint_t> mduDistance;

		for (vedo::_uint_t u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.set_z(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ElementID, ElementName, Distance, X, Y, Z" << std::endl;
		vedo::_uint_t uRank = 1;
		for (std::map<vedo::_float_t, vedo::_uint_t>::iterator
			 mduiDistance = mduDistance.begin(); mduiDistance != mduDistance.end(); mduiDistance++)
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
	else if ((arg[1] == "-distance_to_yz_plane") && (arg.size() == 5))
	{
		vedo::_float_t dY, dZ;
		dY = aries::String2T<vedo::_float_t>(arg[2]);
		dZ = aries::String2T<vedo::_float_t>(arg[3]);
		vedo::Vector3df vP(0.0, dY, dZ);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[4]);
		vedo::Vector3df vD;
		std::map<vedo::_float_t, vedo::_uint_t> mduDistance;

		for (vedo::_uint_t u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.set_x(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ElementID, ElementName, Distance, X, Y, Z" << std::endl;
		vedo::_uint_t uRank = 1;
		for (std::map<vedo::_float_t, vedo::_uint_t>::iterator
			 mduiDistance = mduDistance.begin(); mduiDistance != mduDistance.end(); mduiDistance++)
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
	else if ((arg[1] == "-distance_to_xz_plane") && (arg.size() == 5))
	{
		vedo::_float_t dX, dZ;
		dX = aries::String2T<vedo::_float_t>(arg[2]);
		dZ = aries::String2T<vedo::_float_t>(arg[3]);
		vedo::Vector3df vP(dX, 0.0, dZ);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[4]);
		vedo::Vector3df vD;
		std::map<vedo::_float_t, vedo::_uint_t> mduDistance;

		for (vedo::_uint_t u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.set_y(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ElementID, ElementName, Distance, X, Y, Z" << std::endl;
		vedo::_uint_t uRank = 1;
		for (std::map<vedo::_float_t, vedo::_uint_t>::iterator
			 mduiDistance = mduDistance.begin(); mduiDistance != mduDistance.end(); mduiDistance++)
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
	else if ((arg[1] == "-dump_status") && (arg.size() == 5))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[2], pIactRecordTab);
		pWorld->DumpDOStatus(arg[3].c_str());
		pIactRecordTab
			->DumpIactStatus
				(pWorld->GetSystemParameter()->GetTimeCurrent(), arg[4].c_str());
		delete pWorld;
        delete pIactRecordTab;
	}
/*
	else if ((arg[1] == "-u2d") && (arg.size() == 4))
	{
		njr::unix2dos(arg[2], arg[3]);
	}
*/
/*
	else if ((arg[1] == "-cse") && (arg.size() == 4))
	{
        // Aries: We have not deal with the interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		oWorld->ClearSeparatedElements();
		delete WriteDOWorld (arg[3], oWorld);
	}
*/
/*
	else if ((arg[1] == "-distribution") && (arg.size() >= 11))
	{
		std::string DOName = arg[2];
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[3]);
		dXMax = aries::String2T<vedo::_float_t>(arg[4]);
		dYMin = aries::String2T<vedo::_float_t>(arg[5]);
		dYMax = aries::String2T<vedo::_float_t>(arg[6]);
		dZMin = aries::String2T<vedo::_float_t>(arg[7]);
		dZMax = aries::String2T<vedo::_float_t>(arg[8]);
		dMeshSize = aries::String2T<vedo::_float_t>(arg[9]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[10]);
		oWorld
			->SetBoundary
			(vedo::Boundary
				(vedo::Vector3df(dXMin, dYMin, dZMin),
				 vedo::Vector3df(dXMax, dYMax, dZMax) ));
		std::pair<vedo::Vector3df, vedo::Vector3df> pDistribution
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
*/
	else if ((arg[1] == "-erase_single_element") && (arg.size() == 5))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[3], pIactRecordTab);
		vedo::_uint_t uID;
		uID = aries::String2T<vedo::_uint_t>(arg[2]);
		std::string idofilename = arg[3];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		pConsultant->EraseSingleElement(uID);
		delete WriteDOWorld(arg[4], pWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-erase_elements") && (arg.size() == 6))
	{
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[4], pIactRecordTab);
		vedo::_uint_t uIDStarted, uIDEnded;
		uIDStarted = aries::String2T<vedo::_uint_t>(arg[2]);
		uIDEnded   = aries::String2T<vedo::_uint_t>(arg[3]);
		std::string idofilename = arg[4];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::_uint_t> ulIDList;
		for (vedo::_uint_t uID=uIDStarted; uID<=uIDEnded; uID++)
		{
			ulIDList.push_back(uID);
		}
		pConsultant->EraseElements(ulIDList);
		delete WriteDOWorld(arg[5], pWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-erase_elements") && (arg.size() == 5))
	{
		std::string sDOName = arg[2];
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[3], pIactRecordTab);
		std::string idofilename = arg[3];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::_uint_t> ulIDList;
		const vedo::DOStatus* dosp;
		vedo::Vector3df vPosition;
		for
			(vedo::_uint_t ul=0;
			 ul<(pWorld->GetSystemParameter()->GetDONumber());
			 ul++                                           )
		{
			dosp = pWorld->GetDOStatus(ul);
			if (dosp->GetDOName() == sDOName)
			{
				ulIDList.push_back(ul);
			}
		}
		pConsultant->EraseElements(ulIDList);
		delete WriteDOWorld(arg[4], pWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-erase_elements") && (arg.size() == 9))
	{
		std::string sDOName = arg[2];
		vedo::_float_t da, db, dc, dd;
		da = aries::String2T<vedo::_float_t>(arg[3]);
		db = aries::String2T<vedo::_float_t>(arg[4]);
		dc = aries::String2T<vedo::_float_t>(arg[5]);
		dd = aries::String2T<vedo::_float_t>(arg[6]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[7], pIactRecordTab);
		std::string idofilename = arg[7];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::_uint_t> ulIDList;
		const vedo::DOStatus* dosp;
		vedo::Vector3df vPosition;
		for
			(vedo::_uint_t ul=0;
			 ul<(pWorld->GetSystemParameter()->GetDONumber());
			 ul++                                           )
		{
			dosp = pWorld->GetDOStatus(ul);
			if (dosp->GetDOName() == sDOName)
			{
				vPosition = dosp->GetPosition();
				if (da*vPosition.x()+db*vPosition.y()+dc*vPosition.z()>dd)
				{
					ulIDList.push_back(ul);
				}
			}
		}
		pConsultant->EraseElements(ulIDList);
		delete WriteDOWorld(arg[8], pWorld, pIactRecordTab);
		delete pIactRecordTab;
	}

	else if ((arg[1] == "-erase_elements") && (arg.size() >= 10))
	{
		std::string sDOName = arg[2];
		vedo::_uint_t fFaceNumber = aries::String2T<vedo::_uint_t>(arg[3]);
		//std::cout << "fFaceNumber = " << fFaceNumber << std::endl;
		if (arg.size() < (6 + fFaceNumber * 4))
		{
			std::cerr
				<< "Error!! Code: Knight.cpp (-erase_elements)" << std::endl
				<< "        Note: Number of cutting faces is wrong!!" << std::endl;
			exit(-1);
		}
		std::vector<vedo::_float_t> vfa, vfb, vfc, vfd;
		for (vedo::_uint_t u=0; u<fFaceNumber; u++)
		{
			vfa.push_back(aries::String2T<vedo::_float_t>(arg[4+u*4]));
			vfb.push_back(aries::String2T<vedo::_float_t>(arg[5+u*4]));
			vfc.push_back(aries::String2T<vedo::_float_t>(arg[6+u*4]));
			vfd.push_back(aries::String2T<vedo::_float_t>(arg[7+u*4]));
		}
		//for (vedo::_uint_t u=0; u<fFaceNumber; u++)
		//{
		//	std::cout << vfa[u] << " x + (" << vfb[u] << ") y + (" << vfc[u] << ") z > " << vfd[u] << std::endl;
		//}
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[4+fFaceNumber*4], pIactRecordTab);
		std::string idofilename = arg[4+fFaceNumber*4];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::_uint_t> ulIDList;
		const vedo::DOStatus* dosp;
		vedo::Vector3df vPosition;
		bool bShouldBeRemoved;
		for
			(vedo::_uint_t ul=0;
			 ul<(pWorld->GetSystemParameter()->GetDONumber());
			 ul++                                           )
		{
			dosp = pWorld->GetDOStatus(ul);
			if (dosp->GetDOName() == sDOName)
			{
				vPosition = dosp->GetPosition();
				bShouldBeRemoved = true;
				//std::cout << "Ball: " << vPosition;
				for (vedo::_uint_t ul2=0; ul2<fFaceNumber; ul2++)
				{
					//std::cout << "Check cutting face " << ul2 << ": ";
					if (vfa[ul2]*vPosition.x()+vfb[ul2]*vPosition.y()+vfc[ul2]*vPosition.z()<=vfd[ul2])
					{
						//std::cout << "Safe" << std::endl;
						bShouldBeRemoved = false;
						break;
					}
					//else
					//{
					//	std::cout << "X" << std::endl;
					//}
				}
				if (bShouldBeRemoved)
				{
					//std::cout << "Should be removed" << std::endl;
					ulIDList.push_back(ul);
				}
				//else
				//{
					//std::cout << "Should be keeped" << std::endl;
				//}
				//system("pause");
			}
		}
		pConsultant->EraseElements(ulIDList);
		delete WriteDOWorld(arg[5+fFaceNumber*4], pWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-changeDOModel") && (arg.size() >= 11))
	{
		std::string sDONameOld = arg[2];
		std::string sDONameNew = arg[3];
		vedo::_uint_t fFaceNumber = aries::String2T<vedo::_uint_t>(arg[4]);
		if (arg.size() < (7 + fFaceNumber * 4))
		{
			std::cerr
				<< "Error!! Code: Knight.cpp (-erase_elements)" << std::endl
				<< "        Note: Number of cutting faces is wrong!!" << std::endl;
			exit(-1);
		}
		std::vector<vedo::_float_t> vfa, vfb, vfc, vfd;
		for (vedo::_uint_t u=0; u<fFaceNumber; u++)
		{
			vfa.push_back(aries::String2T<vedo::_float_t>(arg[5+u*4]));
			vfb.push_back(aries::String2T<vedo::_float_t>(arg[6+u*4]));
			vfc.push_back(aries::String2T<vedo::_float_t>(arg[7+u*4]));
			vfd.push_back(aries::String2T<vedo::_float_t>(arg[8+u*4]));
		}
		//for (vedo::_uint_t u=0; u<fFaceNumber; u++)
		//{
		//	std::cout << vfa[u] << " x + (" << vfb[u] << ") y + (" << vfc[u] << ") z > " << vfd[u] << std::endl;
		//}
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[5+fFaceNumber*4], pIactRecordTab);
		std::string idofilename = arg[5+fFaceNumber*4];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::_uint_t> ulIDList;
		const vedo::DOStatus* dosp;
		vedo::Vector3df vPosition;
		bool bShouldBeChanged;
		for
			(vedo::_uint_t ul=0;
			 ul<(pWorld->GetSystemParameter()->GetDONumber());
			 ul++                                           )
		{
			dosp = pWorld->GetDOStatus(ul);
			if (dosp->GetDOName() == sDONameOld)
			{
				vPosition = dosp->GetPosition();
				bShouldBeChanged = true;
				//std::cout << "Ball: " << vPosition;
				for (vedo::_uint_t ul2=0; ul2<fFaceNumber; ul2++)
				{
					//std::cout << "Check cutting face " << ul2 << ": ";
					if (vfa[ul2]*vPosition.x()+vfb[ul2]*vPosition.y()+vfc[ul2]*vPosition.z()<=vfd[ul2])
					{
						//std::cout << "Safe" << std::endl;
						bShouldBeChanged = false;
						break;
					}
					//else
					//{
					//	std::cout << "X" << std::endl;
					//}
				}
				if (bShouldBeChanged)
				{
					//std::cout << "Should be removed" << std::endl;
					pWorld->ChangeDOStatusName(ul, sDONameNew);
				}
				//else
				//{
					//std::cout << "Should be keeped" << std::endl;
				//}
				//system("pause");
			}
		}
		delete WriteDOWorld(arg[6+fFaceNumber*4], pWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-erase_spherical_elements") && (arg.size() == 8))
	{
		vedo::_float_t da, db, dc, dd;
		da = aries::String2T<vedo::_float_t>(arg[2]);
		db = aries::String2T<vedo::_float_t>(arg[3]);
		dc = aries::String2T<vedo::_float_t>(arg[4]);
		dd = aries::String2T<vedo::_float_t>(arg[5]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[6], pIactRecordTab);
		std::string idofilename = arg[6];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::_uint_t> ulIDList;
		const vedo::DOStatus* dosp;
		vedo::Vector3df vPosition;
		for
			(vedo::_uint_t ul=0;
			 ul<(pWorld->GetSystemParameter()->GetDONumber());
			 ul++                                           )
		{
			dosp = pWorld->GetDOStatus(ul);
			if (   pWorld->GetDOModel(dosp->GetDOName())->GetShapeType()
			   == vedo::Sphere                                         )
			{
				vPosition = dosp->GetPosition();
				if (da*vPosition.x()+db*vPosition.y()+dc*vPosition.z()>dd)
				{
					ulIDList.push_back(ul);
				}
			}
		}
		pConsultant->EraseElements(ulIDList);
		delete WriteDOWorld(arg[7], pWorld, pIactRecordTab);
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
	else if ((arg[1] == "-freeze_all_elements") && (arg.size() == 4))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2], pIactRecordTab);
		oWorld->FreezeAllElements();
		delete WriteDOWorld (arg[3], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-freeze_elements") && (arg.size() == 5))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3], pIactRecordTab);
		oWorld->FreezeElements(arg[2]);
		delete WriteDOWorld (arg[4], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
/*
	else if ((arg[1] == "-gt_auto") && (arg.size() == 10))
	{
		vedo::_uint_t CutNumX      = 0;
		vedo::_uint_t CutNumY      = 0;
		vedo::_uint_t CutNumZ      = 0;
		vedo::_float_t        dAngle2XAxis = 0.0;
		vedo::_float_t        dAngle2YAxis = 0.0;
		vedo::_float_t        dAngle2ZAxis = 0.0;
		vedo::DOWorld* oWorld      = ReadDOWorld(arg[2]);
		std::string DOName   = arg[3];
		CutNumX = aries::String2T<vedo::_uint_t>(arg[4]);
		CutNumY = aries::String2T<vedo::_uint_t>(arg[5]);
		CutNumZ = aries::String2T<vedo::_uint_t>(arg[6]);
		dAngle2XAxis = aries::String2T<vedo::_float_t>(arg[7]);
		dAngle2YAxis = aries::String2T<vedo::_float_t>(arg[8]);
		dAngle2ZAxis = aries::String2T<vedo::_float_t>(arg[9]);
		oWorld->Rotate
			(dAngle2XAxis*0.01745329261,
			 dAngle2YAxis*0.01745329261,
			 dAngle2ZAxis*0.01745329261 );

		const vedo::DOStatus* pDOS;
		vedo::_uint_t StartNumber = 0;
		while(   StartNumber < oWorld->GetSystemParameter()->GetDONumber()
			  && (oWorld->GetDOStatus(StartNumber)->GetDOName() != DOName))
		{
			StartNumber++;
		}
		vedo::Vector3df PTemp(oWorld->GetDOStatus(StartNumber)->GetPosition());
		vedo::_float_t XMin = PTemp.x();
		vedo::_float_t YMin = PTemp.y();
		vedo::_float_t ZMin = PTemp.z();
		vedo::_float_t XMax = XMin;
		vedo::_float_t YMax = YMin;
		vedo::_float_t ZMax = ZMin;

		for (vedo::_uint_t ul=StartNumber+1;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			PTemp = oWorld->GetDOStatus(ul)->GetPosition();

			if (PTemp.x() > XMax)
			{
				XMax = PTemp.x();
			}
			else if (PTemp.x() < XMin)
			{
				XMin = PTemp.x();
			}

			if (PTemp.y() > YMax)
			{
				YMax = PTemp.y();
			}
			else if (PTemp.y() < YMin)
			{
				YMin = PTemp.y();
			}

			if (PTemp.z() > ZMax)
			{
				ZMax = PTemp.z();
			}
			else if (PTemp.z() < ZMin)
			{
				ZMin = PTemp.z();
			}
		}
		vedo::_float_t dCoverRange = oWorld->GetDOModel(DOName)->GetRange();
		XMax += dCoverRange;
		YMax += dCoverRange;
		ZMax += dCoverRange;
		XMin -= dCoverRange;
		YMin -= dCoverRange;
		ZMin -= dCoverRange;
		vedo::_float_t width  = (XMax - XMin) / (vedo::_float_t)CutNumX;
		vedo::_float_t length = (YMax - YMin) / (vedo::_float_t)CutNumY;
		vedo::_float_t height = (ZMax - ZMin) / (vedo::_float_t)CutNumZ;
		vedo::GSRectangle space("Granular Temperature Space", width, length, height);
		vedo::_float_t XCentral = 0.5 * (XMin + XMax);
		vedo::_float_t YCentral = 0.5 * (YMin + YMax);
		vedo::_float_t ZCentral = 0.5 * (ZMin + ZMax);

		vedo::_uint_t XCounter = 1;
		vedo::_uint_t YCounter;
		vedo::_uint_t ZCounter;

		std::map<std::string, vedo::_float_t> gt;
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
		for (vedo::_float_t x=XMin+0.5*width; XCounter<=CutNumX; x+=width, XCounter++)
		{
			YCounter = 1;
			for
				(vedo::_float_t y=YMin+0.5*length;
				 YCounter<=CutNumY;
				 y+=length, YCounter++    )
			{
				ZCounter = 1;
				for
					(vedo::_float_t z=ZMin+0.5*height;
					 ZCounter<=CutNumZ;
					 z+=height, ZCounter++    )
				{
					space.SetStatus
						(new vedo::DOStatus
							("No Name",
							 vedo::Vector3df(x, y, z),
							 vedo::Vector3df(),
							 vedo::Vector3df(1.0, 0.0, 0.0),
							 vedo::Vector3df(0.0, 0.0, 1.0),
							 vedo::Vector3df()    ));
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
						<< (vedo::_uint_t)gt["NumberOfElement"]       << ", "
						<< gt["AverageVelocity"]                      << ", "
						<< gt["GranularTemperatureOfVelocity"]        << ", "
						<< gt["AverageAngularVelocity"]               << ", "
						<< gt["GranularTemperatureOfAngularVelocity"] << std::endl;
				}
			}
			FileGT.close();
		}
		delete oWorld;
	}
*/
	else if ((arg[1] == "-granular_temperature_in_box") && (arg.size() == 16))
	{
		std::string DOName = arg[2];
		vedo::_float_t width, length, height, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		x      = aries::String2T<vedo::_float_t>(arg[ 3]);
		y      = aries::String2T<vedo::_float_t>(arg[ 4]);
		z      = aries::String2T<vedo::_float_t>(arg[ 5]);
		width  = aries::String2T<vedo::_float_t>(arg[ 6]);
		length = aries::String2T<vedo::_float_t>(arg[ 7]);
		height = aries::String2T<vedo::_float_t>(arg[ 8]);
		Xx     = aries::String2T<vedo::_float_t>(arg[ 9]);
		Xy     = aries::String2T<vedo::_float_t>(arg[10]);
		Xz     = aries::String2T<vedo::_float_t>(arg[11]);
		Zx     = aries::String2T<vedo::_float_t>(arg[12]);
		Zy     = aries::String2T<vedo::_float_t>(arg[13]);
		Zz     = aries::String2T<vedo::_float_t>(arg[14]);
		vedo::GSRectangle space("Granular Temperature Space", width, length, height);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 vedo::Vector3df(x, y, z),
				 vedo::Vector3df(),
				 vedo::Vector3df(Xx, Xy, Xz),
				 vedo::Vector3df(Zx, Zy, Zz),
				 vedo::Vector3df(),
				 vedo::Vector3df(),
				 vedo::Vector3df()           ));
		vedo::DOWorld* oWorld = ReadDOWorld(arg[15]);
		std::map<std::string, vedo::_float_t> gt
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
			<< (vedo::_uint_t)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-granular_temperature_in_cylinder") && (arg.size() == 15))
	{
		std::string DOName = arg[2];
		vedo::_float_t radius, height, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		x      = aries::String2T<vedo::_float_t>(arg[ 3]);
		y      = aries::String2T<vedo::_float_t>(arg[ 4]);
		z      = aries::String2T<vedo::_float_t>(arg[ 5]);
		radius = aries::String2T<vedo::_float_t>(arg[ 6]);
		height = aries::String2T<vedo::_float_t>(arg[ 7]);
		Xx     = aries::String2T<vedo::_float_t>(arg[ 8]);
		Xy     = aries::String2T<vedo::_float_t>(arg[ 9]);
		Xz     = aries::String2T<vedo::_float_t>(arg[10]);
		Zx     = aries::String2T<vedo::_float_t>(arg[11]);
		Zy     = aries::String2T<vedo::_float_t>(arg[12]);
		Zz     = aries::String2T<vedo::_float_t>(arg[13]);
		vedo::GSCylinder space("Granular Temperature Space", radius, height);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 vedo::Vector3df(x, y, z),
				 vedo::Vector3df(),
				 vedo::Vector3df(Xx, Xy, Xz),
				 vedo::Vector3df(Zx, Zy, Zz),
				 vedo::Vector3df(),
				 vedo::Vector3df(),
				 vedo::Vector3df()           ));
		vedo::DOWorld* oWorld = ReadDOWorld(arg[14]);
		std::map<std::string, vedo::_float_t> gt
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
			<< (vedo::_uint_t)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-granular_temperature_in_sphere") && (arg.size() == 8))
	{
		vedo::_float_t radius, x, y, z;
		std::string DOName = arg[2];
		x      = aries::String2T<vedo::_float_t>(arg[ 3]);
		y      = aries::String2T<vedo::_float_t>(arg[ 4]);
		z      = aries::String2T<vedo::_float_t>(arg[ 5]);
		radius = aries::String2T<vedo::_float_t>(arg[ 6]);
		vedo::GSSphere space("Granular Temperature Space", radius);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 vedo::Vector3df(x, y, z),
				 vedo::Vector3df(),
				 vedo::Vector3df(1.0, 0.0, 0.0),
				 vedo::Vector3df(0.0, 0.0, 1.0),
				 vedo::Vector3df(),
				 vedo::Vector3df(),
				 vedo::Vector3df()              ));
		vedo::DOWorld* oWorld = ReadDOWorld(arg[7]);
		std::map<std::string, vedo::_float_t> gt
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
			<< (vedo::_uint_t)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-granular_temperature_in_ellipsoid") && (arg.size() == 16))
	{
		std::string DOName = arg[2];
		vedo::_float_t XLength, YLength, ZLength, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		x       = aries::String2T<vedo::_float_t>(arg[ 3]);
		y       = aries::String2T<vedo::_float_t>(arg[ 4]);
		z       = aries::String2T<vedo::_float_t>(arg[ 5]);
		XLength = aries::String2T<vedo::_float_t>(arg[ 6]);
		YLength = aries::String2T<vedo::_float_t>(arg[ 7]);
		ZLength = aries::String2T<vedo::_float_t>(arg[ 8]);
		Xx      = aries::String2T<vedo::_float_t>(arg[ 9]);
		Xy      = aries::String2T<vedo::_float_t>(arg[10]);
		Xz      = aries::String2T<vedo::_float_t>(arg[11]);
		Zx      = aries::String2T<vedo::_float_t>(arg[12]);
		Zy      = aries::String2T<vedo::_float_t>(arg[13]);
		Zz      = aries::String2T<vedo::_float_t>(arg[14]);
		vedo::GSEllipsoid space
			("Granular Temperature Space", XLength, YLength, ZLength);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 vedo::Vector3df(x, y, z),
				 vedo::Vector3df(),
				 vedo::Vector3df(Xx, Xy, Xz),
				 vedo::Vector3df(Zx, Zy, Zz),
				 vedo::Vector3df(),
				 vedo::Vector3df(),
				 vedo::Vector3df()           ));
		vedo::DOWorld* oWorld = ReadDOWorld(arg[15]);
		std::map<std::string, vedo::_float_t> gt
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
			<< (vedo::_uint_t)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-information") && (arg.size() == 3))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		delete info(ReadDOWorld(arg[2], pIactRecordTab));
        delete pIactRecordTab;
	}
/*
	else if ((arg[1] == "-inside") && (arg.size() == 9))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		vedo::_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = aries::String2T<vedo::_float_t>(arg[3]);
		dXMax = aries::String2T<vedo::_float_t>(arg[4]);
		dYMin = aries::String2T<vedo::_float_t>(arg[5]);
		dYMax = aries::String2T<vedo::_float_t>(arg[6]);
		dZMin = aries::String2T<vedo::_float_t>(arg[7]);
		dZMax = aries::String2T<vedo::_float_t>(arg[8]);

		const vedo::DOStatus* pDOS;
		vedo::_float_t dRadius;
		vedo::Vector3df vPosition;
		std::ofstream oFileElementInside;
		oFileElementInside.open("ElementInside.txt", std::ios::out);

		for (vedo::_uint_t ul=0;
			ul<oWorld->GetSystemParameter()->GetDONumber();
			ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOS->GetPosition();
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				vPosition = pDOS->GetPosition();
				dRadius
					= oWorld
					->GetDOModel(pDOS->GetDOName())
					->GetShapeAttributes().sphere.radius;

				if ( (vPosition.x() + dRadius >= dXMin) &&
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
*/
/*
	else if ((arg[1] == "-lattice_bcc") && (arg.size() == 11))
	{
		vedo::_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = aries::String2T<vedo::_float_t>(arg[3]);
		xmax = aries::String2T<vedo::_float_t>(arg[4]);
		ymin = aries::String2T<vedo::_float_t>(arg[5]);
		ymax = aries::String2T<vedo::_float_t>(arg[6]);
		zmin = aries::String2T<vedo::_float_t>(arg[7]);
		zmax = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		vedo::_float_t d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		vedo::_float_t rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		vedo::_float_t rectCenter[3]
			= {0.5*(xmax+xmin), 0.5*(ymax+ymin), 0.5*(zmax+zmin)};
		BCCLattice lattice(2.*d/sqrt(3.));
		lattice.SetOrigin(rectCenter);
		RectangularClusterInitializer init(rectSize, &lattice);
		init.Create(DOName, oWorld);
		oWorld
			->Shift
				(vedo::Vector3df
					(rectCenter[0]-0.5*d,
					 rectCenter[1]-0.5*d,
					 rectCenter[2]-0.5*d), DOName);
		delete WriteDOWorld (arg[10], oWorld);
	}
	else if ((arg[1] == "-lattice_fcc") && (arg.size() == 11))
	{
		vedo::_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = aries::String2T<vedo::_float_t>(arg[3]);
		xmax = aries::String2T<vedo::_float_t>(arg[4]);
		ymin = aries::String2T<vedo::_float_t>(arg[5]);
		ymax = aries::String2T<vedo::_float_t>(arg[6]);
		zmin = aries::String2T<vedo::_float_t>(arg[7]);
		zmax = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		vedo::_float_t d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		vedo::_float_t rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		vedo::_float_t rectCenter[3]
			= {0.5*(xmax+xmin), 0.5*(ymax+ymin), 0.5*(zmax+zmin)};
		PrimitiveFCCLattice lattice(2.*d/sqrt(2.));
		lattice.SetOrigin(rectCenter);
		RectangularClusterInitializer init(rectSize, &lattice);
		init.Create(DOName, oWorld);
		oWorld
			->Shift
				(vedo::Vector3df
					(rectCenter[0]-0.5*d,
					 rectCenter[1]-0.5*d,
					 rectCenter[2]-0.5*d), DOName);
		delete WriteDOWorld (arg[10], oWorld);
	}
	else if ((arg[1] == "-lattice_scc") && (arg.size() == 11))
	{
		vedo::_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = aries::String2T<vedo::_float_t>(arg[3]);
		xmax = aries::String2T<vedo::_float_t>(arg[4]);
		ymin = aries::String2T<vedo::_float_t>(arg[5]);
		ymax = aries::String2T<vedo::_float_t>(arg[6]);
		zmin = aries::String2T<vedo::_float_t>(arg[7]);
		zmax = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		vedo::_float_t d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		vedo::_float_t rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		vedo::_float_t rectCenter[3]
			= {0.5*(xmax+xmin), 0.5*(ymax+ymin), 0.5*(zmax+zmin)};
		CubicLattice lattice(d);
		lattice.SetOrigin(rectCenter);
		RectangularClusterInitializer init(rectSize, &lattice);
		init.Create(DOName, oWorld);
		oWorld
			->Shift
				(vedo::Vector3df
					(rectCenter[0]-0.5*d,
					 rectCenter[1]-0.5*d,
					 rectCenter[2]-0.5*d), DOName);
		delete WriteDOWorld (arg[10], oWorld);
	}
*/
/*
	else if ((arg[1] == "-profile_Info2Y") && (arg.size() == 7))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		vedo::_float_t dXmin, dXmax, dZmin, dZmax;
		dXMin = aries::String2T<vedo::_float_t>(arg[3]);
		dXMax = aries::String2T<vedo::_float_t>(arg[4]);
		dZMin = aries::String2T<vedo::_float_t>(arg[5]);
		dZMax = aries::String2T<vedo::_float_t>(arg[6]);
		const vedo::DOStatus* pDOS;

		std::map<vedo::_float_t, vedo::_uint_t> mInformation;
		std::map<vedo::_float_t, vedo::_uint_t>::iterator mpInformation;
		for (vedo::_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (oWorld->GetDOModel(pDOS->GetDOName())->GetShapeType() == vedo::Sphere)
			{
				if (    (pDOS->GetPosition().x() >= dXmin)
					&& (pDOS->GetPosition().x() <= dXmax)
					&& (pDOS->GetPosition().z() >= dZmin)
					&& (pDOS->GetPosition().z() <= dZmax) )
				{
					mInformation[pDOS->GetPosition().y()] = ul;
				}
			}
		}

		std::ofstream FileProfile;
		FileProfile.open("Profile.csv", std::ios::out);
		FileProfile << "Px, Py, Pz, Vx, Vy, Vz, AVx, AVy, AVz, R" << std::endl;
		for (mpInformation  = mInformation.begin();
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
		}
		FileProfile.close();

		delete oWorld;
	}
	else if ((arg[1] == "-projected_area_x") && (arg.size() == 4))
	{
		vedo::_float_t dMeshSize;
		dMeshSize = aries::String2T<vedo::_float_t>(arg[2]);
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
		vedo::_float_t dMeshSize;
		dMeshSize = aries::String2T<vedo::_float_t>(arg[2]);
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
		vedo::_float_t dMeshSize;
		dMeshSize = aries::String2T<vedo::_float_t>(arg[2]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		std::cout
			<< "Time = "
			<< oWorld->GetSystemParameter()->GetTimeCurrent()
			<< '\t'
			<< "Projected Area (Z) = "
			<< oWorld->ProjectedAreaZ(dMeshSize)
			<< std::endl;
	}
*/
	else if ((arg[1] == "-modify_element_status") && (arg.size() == 20))
	{
		vedo::_uint_t uID;
		uID = aries::String2T<vedo::_uint_t>(arg[2]);
		vedo::_float_t dPx, dPy, dPz, dVx, dVy, dVz, dAVx, dAVy, dAVz;
		vedo::_float_t dOxx, dOxy, dOxz, dOzx, dOzy, dOzz;
		dPx  = aries::String2T<vedo::_float_t>(arg[ 3]);
		dPy  = aries::String2T<vedo::_float_t>(arg[ 4]);
		dPz  = aries::String2T<vedo::_float_t>(arg[ 5]);
		dVx  = aries::String2T<vedo::_float_t>(arg[ 6]);
		dVy  = aries::String2T<vedo::_float_t>(arg[ 7]);
		dVz  = aries::String2T<vedo::_float_t>(arg[ 8]);
		dOxx = aries::String2T<vedo::_float_t>(arg[ 9]);
		dOxy = aries::String2T<vedo::_float_t>(arg[10]);
		dOxz = aries::String2T<vedo::_float_t>(arg[11]);
		dOzx = aries::String2T<vedo::_float_t>(arg[12]);
		dOzy = aries::String2T<vedo::_float_t>(arg[13]);
		dOzz = aries::String2T<vedo::_float_t>(arg[14]);
		dAVx = aries::String2T<vedo::_float_t>(arg[15]);
		dAVy = aries::String2T<vedo::_float_t>(arg[16]);
		dAVz = aries::String2T<vedo::_float_t>(arg[17]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[18], pIactRecordTab);
		const vedo::DOStatus* dosp = oWorld->GetDOStatus(uID);
		vedo::DOStatus dos
			(oWorld->GetDOName(uID),
			vedo::Vector3df(dPx , dPy , dPz ),
			vedo::Vector3df(dVx , dVy , dVz ),
			vedo::Vector3df(dOxx, dOxy, dOxz).direction(),
			vedo::Vector3df(dOzx, dOzy, dOzz).direction(),
			vedo::Vector3df(dAVx, dAVy, dAVz),
            dosp->GetImpact(),
            dosp->GetAngularImpact()                    );
		oWorld->SetDOStatus(uID, dos);
		delete WriteDOWorld(arg[19], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if (   (arg[1] == "-modify_element_velocity_and_angular_velocity")
			 && (arg.size() == 11)                                         )
	{
		vedo::_uint_t uID;
		uID = aries::String2T<vedo::_uint_t>(arg[2]);
		vedo::_float_t dVx, dVy, dVz, dAVx, dAVy, dAVz;
		dVx  = aries::String2T<vedo::_float_t>(arg[3]);
		dVy  = aries::String2T<vedo::_float_t>(arg[4]);
		dVz  = aries::String2T<vedo::_float_t>(arg[5]);
		dAVx = aries::String2T<vedo::_float_t>(arg[6]);
		dAVy = aries::String2T<vedo::_float_t>(arg[7]);
		dAVz = aries::String2T<vedo::_float_t>(arg[8]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		oWorld
			->SetDOStatusVelocityAndAngularVelocity
				(uID,
				 vedo::Vector3df(dVx , dVy , dVz),
				 vedo::Vector3df(dAVx , dAVy , dAVz));
		delete WriteDOWorld(arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-rotate_all_elements") && (arg.size() == 7))
	{
		vedo::_float_t dAngle2XAxis = 0.0;
		vedo::_float_t dAngle2YAxis = 0.0;
		vedo::_float_t dAngle2ZAxis = 0.0;
		dAngle2XAxis = aries::String2T<vedo::_float_t>(arg[2]);
		dAngle2YAxis = aries::String2T<vedo::_float_t>(arg[3]);
		dAngle2ZAxis = aries::String2T<vedo::_float_t>(arg[4]);
        // We have not deal with interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5]);

		oWorld->Rotate
			(dAngle2XAxis*0.0174532925199433,
			 dAngle2YAxis*0.0174532925199433,
			 dAngle2ZAxis*0.0174532925199433 );
		delete WriteDOWorld (arg[6], oWorld);
	}
	else if ((arg[1] == "-shift_elements") && (arg.size() == 8))
	{
		vedo::_float_t x, y, z;
		std::string DOName = arg[2];
		x = aries::String2T<vedo::_float_t>(arg[3]);
		y = aries::String2T<vedo::_float_t>(arg[4]);
		z = aries::String2T<vedo::_float_t>(arg[5]);
        // We have noe deal with interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6]);
		oWorld->Shift(vedo::Vector3df(x, y, z), DOName);
		delete WriteDOWorld (arg[7], oWorld);
	}
	else if ((arg[1] == "-shift_all_elements") && (arg.size() == 7))
	{
		vedo::_float_t x, y, z;
		x = aries::String2T<vedo::_float_t>(arg[2]);
		y = aries::String2T<vedo::_float_t>(arg[3]);
		z = aries::String2T<vedo::_float_t>(arg[4]);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5], pIactRecordTab);
		oWorld->Shift(vedo::Vector3df(x, y, z));
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
/*
	else if ((arg[1] == "-sort_distance") && (arg.size() == 6))
	{
		vedo::_float_t dX, dY, dZ;
		dX = aries::String2T<vedo::_float_t>(arg[2]);
		dY = aries::String2T<vedo::_float_t>(arg[3]);
		dZ = aries::String2T<vedo::_float_t>(arg[4]);
		vedo::Vector3df vP(dX, dY, dZ);
	    // We have not deal with the interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5]);

		std::map<vedo::_float_t, vedo::_uint_t> mIndex;
		vedo::_float_t dD;
		for (vedo::_uint_t ul=0;
			 ul<(oWorld->GetSystemParameter()->GetDONumber());
			 ul++                                             )
		{
			dD = ((oWorld->GetDOStatus(ul)->GetPosition()) - vP).length();
			mIndex[dD] = ul;
		}

		vedo::_uint_t ulN;
		std::ofstream FileSortDistance;
		FileSortDistance.open("ElementSorting.csv", std::ios::out);
		FileSortDistance
			<< "Distance, SerialNumber, DOName, Px, Py, Pz" << std::endl;
		for (std::map<vedo::_float_t, vedo::_uint_t>::iterator
			 mpIndex=mIndex.begin(); mpIndex!=mIndex.end(); mpIndex++)
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
*/
	else if ((arg[1] == "-set_time") && (arg.size() == 8))
	{
		vedo::_float_t dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent;
		dTimeStart    = aries::String2T<vedo::_float_t>(arg[2]);
		dTimeStop     = aries::String2T<vedo::_float_t>(arg[3]);
		dTimeInterval = aries::String2T<vedo::_float_t>(arg[4]);
		dTimeCurrent  = aries::String2T<vedo::_float_t>(arg[5]);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		oWorld
			->SetSimulatedTime
				(dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
	else if ((arg[1] == "-show_elements_motion_status") && (arg.size() == 4))
	{
		std::string DOName = arg[2];
		vedo::DOWorld* oWorld = ReadDOWorld(arg[3]);
		std::ofstream ofInformation("ElementInformation.csv", std::ios::out);
		ofInformation
			<< "ID, Px, Py, Pz, Xx, Xy, Xz, Zx, Zy, Zz, Vx, Vy, Vz, AVx, AVy, AVz"
			<< std::endl;
		const vedo::DOStatus* dosp;
		vedo::Vector3df
			vPosition, vOrientationX, vOrientationZ, vVelocity, vAngularVelocity;
		for (vedo::_uint_t uID=0; uID<oWorld->GetSystemParameter()->GetDONumber(); uID++)
		{
			dosp             = oWorld->GetDOStatus(uID);
			if (dosp->GetDOName() == DOName)
			{
				vPosition        = dosp->GetPosition();
				vOrientationX    = dosp->GetOrientationX();
				vOrientationZ    = dosp->GetOrientationZ();
				vVelocity        = dosp->GetVelocity();
				vAngularVelocity = dosp->GetAngularVelocity();
				ofInformation
					<< uID                  << ", "
					<< vPosition.x()        << ", "
					<< vPosition.y()        << ", "
					<< vPosition.z()        << ", "
					<< vOrientationX.x()    << ", "
					<< vOrientationX.y()    << ", "
					<< vOrientationX.z()    << ", "
					<< vOrientationZ.x()    << ", "
					<< vOrientationZ.y()    << ", "
					<< vOrientationZ.z()    << ", "
					<< vVelocity.x()        << ", "
					<< vVelocity.y()        << ", "
					<< vVelocity.z()        << ", "
					<< vAngularVelocity.x() << ", "
					<< vAngularVelocity.y() << ", "
					<< vAngularVelocity.z() << std::endl;
			}
		}
		ofInformation.close();
		delete oWorld;
	}
	else
	{
		usage();
	}

	//njr::RunTime("No News is Good News");

	return true;
}
