#include <vedo/constants/interfaces/Constants.h>
#include <vedo/njr/interfaces/Constants.h>
#include <vedo/njr/interfaces/Utility.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/Boundary.h>
#include <vedo/framework/interfaces/Consultant.h>
#include <vedo/framework/interfaces/DOModel.h>
#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/DOWorld_WriteVTK.h>
#include <vedo/framework/interfaces/GeometricShape.h>
#include <vedo/framework/interfaces/IactRecordTab.h>
#include <vedo/common/interfaces/GSComplexShape.h>
#include <vedo/common/interfaces/GSCylinder.h>
#include <vedo/common/interfaces/GSRectangle.h>
#include <vedo/common/interfaces/GSSphere.h>
#include <vedo/common/interfaces/GSEllipsoid.h>
#include <vedo/knight/interfaces/CalcDensity.h>
#include <vedo/knight/interfaces/CalcCoordinateNum.h>
#include <vedo/knight/interfaces/RectangularClusterInitializer.h>
#include <vedo/knight/interfaces/3DLattices.h>
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

vedo::Constants* vedo_cp = vedo::Constants::Instance();
vedo::vedo_float_t SafetyFactor = vedo_cp->SafetyFactor();

void usage()
{
	std::ofstream ofMessageFile("knight.txt", std::ios::out);

	std::cout
		<< "Knight " << vedo::sVersion << " " << vedo_cp->SysInfo() << std::endl << std::endl
		<< "Unknown command, please read the file \"knight.txt\" for more information." << std::endl;

	ofMessageFile
		<< "knight " << vedo::sVersion << std::endl
		<< std::endl
		<< "Usage:" << std::endl
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
/*
		<< "	-sort: sort elements" << std::endl
		<< "* Knight -sort (Original)<.xml | .ido> (New)<.xml | .ido>" << std::endl
		<< std::endl
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
			<< "NumUDDOStatus("             << vedo::uNumUDDDOStatus     << ") "
			<< "NumUDIactStatus("           << vedo::uNumUDDImpactStatus << ")"  << std::endl
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
	std::map<std::string, vedo::vedo_uint_t> counter;

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
	vedo::vedo_uint_t i = 0;
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

	if (njr::CheckSubName(filename,".xml"))
	{
		pWorld->ReadXML(filename, irtp);
	}
	else if (njr::CheckSubName(filename, ".ido" ))
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

	if (njr::CheckSubName(filename,".xml"))
	{
		pWorld->ReadXML(filename);
	}
	else if (njr::CheckSubName(filename, ".ido" ))
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
	else if (njr::CheckSubName(filename, ".vtu"))
	{
		#ifdef _STD_CPP_11
			pw->WriteVTK<vedo::NullExternalFieldVTKWriter>(filename.c_str());
		#endif   // _STD_CPP_11
	}
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
}

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
	else if (njr::CheckSubName(filename, ".vtu"))
	{
		#ifdef _STD_CPP_11
			pw->WriteVTK<vedo::NullExternalFieldVTKWriter>(filename.c_str());
		#endif   // _STD_CPP_11
	}
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
}

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
			njr::Vector3d(0.0, 0.0, -980.0),
			vedo::Boundary()                    );

	vedo::DOWorld* pw3 = new vedo::DOWorld(pSystemParameter, cDOModel, cIactModel, vcd3);

	pw3->WriteXML(des.c_str());

	delete pw1;
	delete pw2;
	delete pw3;
}
*/

void ConList(std::string filename, vedo::vedo_uint_t begin, vedo::vedo_uint_t end)
{
	char ltoa[256];
	vedo::vedo_uint_t ulRecordCount;
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

void ConListHEX (std::string filename, vedo::vedo_uint_t begin, vedo::vedo_uint_t end)
{
	char ltoa[256];
	vedo::vedo_uint_t ulRecordCount;
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
	vedo::vedo_uint_t begin = 0;
	vedo::vedo_uint_t end   = 1474;
	std::string filename    = "bs";

	vedo::vedo_uint_t ulRecordCount;
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

void AddDOInSpace(njr::Vector3d* LowerBoundary, njr::Vector3d* UpperBoundary, std::string DOName, vedo::DOWorld* oWorld)
{
	vedo::vedo_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);
	*LowerBoundary += njr::Vector3d(R, R, R);
	*UpperBoundary -= njr::Vector3d(R, R, R);
	for (vedo::vedo_float_t y=(LowerBoundary->y()); y<=(UpperBoundary->y()); y+=(2.0*R))
    {
	    for (vedo::vedo_float_t z=(LowerBoundary->z()); z<=(UpperBoundary->z()); z+=(2.0*R))
    	{
		    for (vedo::vedo_float_t x=(LowerBoundary->x()); x<=(UpperBoundary->x()); x+=(2.0*R))
    		{
				dos.SetPosition(njr::Vector3d(x, y, z));
				oWorld->AddDOStatus(new vedo::DOStatus(dos));
	    	}
	    }
    }
}

void AddPotentialEnergy(njr::Vector3d* SourcePoint, std::string DOName, vedo::DOWorld* oWorld)
{
	njr::Vector3d vel;
	const njr::Vector3d ff = oWorld->GetSystemParameter()->GetFieldAcceleration();
	vedo::DOStatus new_dos(DOName, njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);
	const vedo::DOStatus* dos = 0;
    vedo::vedo_float_t TempVelocitySquare;
	for (vedo::vedo_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
	{
		dos = oWorld->GetDOStatus(ul);
		if (dos->GetDOName() == DOName)
		{
			vel = dos->GetVelocity();
			new_dos = *dos;
            TempVelocitySquare = vel.Dot(vel) + 2.0 * ff.Dot(dos->GetPosition() - *SourcePoint);
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

void AddExternalVelocity(njr::Vector3d* pExternalVelocity, std::string DOName, vedo::DOWorld* oWorld)
{
	njr::Vector3d vel;
	vedo::DOStatus new_dos(DOName, njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);
	const vedo::DOStatus* dos = 0;
	for (vedo::vedo_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
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

void AddExternalAngularVelocity(njr::Vector3d* pExternalAngularVelocity, std::string DOName, vedo::DOWorld* oWorld)
{
	njr::Vector3d avel;
	vedo::DOStatus new_dos(DOName, njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);
	const vedo::DOStatus* dos = 0;
	for (vedo::vedo_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
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
	(njr::Vector3d* LowerBoundary, njr::Vector3d* UpperBoundary,
	 std::string DOName, vedo::vedo_uint_t* num, vedo::DOWorld* oWorld)
{
	vedo::vedo_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);
	*LowerBoundary += njr::Vector3d(R, R, R);
	*UpperBoundary -= njr::Vector3d(R, R, R);
	vedo::vedo_float_t xRange = (UpperBoundary->x()) - (LowerBoundary->x());
	vedo::vedo_float_t yRange = (UpperBoundary->y()) - (LowerBoundary->y());
	vedo::vedo_float_t zRange = (UpperBoundary->z()) - (LowerBoundary->z());
	vedo::vedo_float_t temp_x, temp_y, temp_z;
	srand(time(0));
	for (vedo::vedo_uint_t ul=0; ul<(*num); ul++)
	{
		temp_x = (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * xRange;
		temp_y = (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * yRange;
		temp_z = (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * zRange;
		dos.SetPosition(*LowerBoundary + njr::Vector3d(temp_x, temp_y, temp_z));
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
	}
}

void AddRandomDOInCylindricalSpace
	(njr::Vector3d* Center, vedo::vedo_float_t* r, vedo::vedo_float_t* h, std::string DOName,
	 vedo::vedo_uint_t* num, vedo::DOWorld* oWorld                                           )
{
	vedo::vedo_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);

	*r -= R;
	vedo::vedo_float_t dSquare_r = (*r) * (*r);
	vedo::vedo_float_t d2r       = 2.0  * (*r);
	vedo::vedo_float_t dXMin     = Center->x() - (*r);
	//vedo::vedo_float_t dXMax     = Center->x() + (*r);
	vedo::vedo_float_t dYMin     = Center->y() - (*r);
	//vedo::vedo_float_t dYMax     = Center->y() + (*r);
	vedo::vedo_float_t dZMin     = Center->z() + R;
	//vedo::vedo_float_t dZMax     = Center->z() + (*h) - R;
	*h -= 2.0 * R;

	vedo::vedo_float_t temp_x, temp_y, temp_z;
	srand(time(0));
	vedo::vedo_uint_t ul = 0;
	while (ul < (*num))
	{
		temp_x = dXMin + (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * d2r;
		temp_y = dYMin + (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * d2r;
		if (temp_x * temp_x + temp_y * temp_y <= dSquare_r)
		{
			temp_z = dZMin + (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * (*h);
			dos.SetPosition(njr::Vector3d(temp_x, temp_y, temp_z));
			oWorld->AddDOStatus(new vedo::DOStatus(dos));
			ul++;
		}
	}
}

void AddRandomDOInSphericalSpace
	(njr::Vector3d* Center, vedo::vedo_float_t* r, std::string DOName, vedo::vedo_uint_t* num, vedo::DOWorld* oWorld)
{
	vedo::vedo_float_t R = oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
	vedo::DOStatus dos(DOName, njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);
	*r -= R;
	vedo::vedo_float_t temp_x, temp_y, temp_z, temp_radius, temp_angle_1, temp_angle_2;
	srand(time(0));
	for (vedo::vedo_uint_t ul=0; ul<(*num); ul++)
	{
		temp_radius  = (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * (*r);
		temp_angle_1 = (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * njr::dDoublePI;
		temp_angle_2 = (vedo::vedo_float_t)(rand()) / (vedo::vedo_float_t)RAND_MAX * njr::dDoublePI;
		temp_x       = Center->x() + temp_radius * cos(temp_angle_1) * cos(temp_angle_2);
		temp_y       = Center->y() + temp_radius * cos(temp_angle_1) * sin(temp_angle_2);
		temp_z       = Center->z() + temp_radius * sin(temp_angle_1);
		dos.SetPosition(njr::Vector3d(temp_x, temp_y, temp_z));
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
	}
}

std::map<std::string, vedo::vedo_float_t> CalculateGranularTemperature
	(vedo::DOWorld* oWorld, vedo::GeometricShape& space, std::string DOName)
{
	const vedo::DOStatus* pDOS;
	njr::Vector3d AvgV, AvgAV;
	vedo::vedo_uint_t Counter = 0;

	oWorld->TurnMonitor(false);
	// Calculate number of elements and average velocity
	for (vedo::vedo_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
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
		AvgV  = 1.0 / vedo::vedo_float_t(Counter) * AvgV;
		AvgAV = 1.0 / vedo::vedo_float_t(Counter) * AvgAV;
	}

	// Calculate granular temperature
	vedo::vedo_float_t dGranularTemperatureV  = 0.0;
	vedo::vedo_float_t dGranularTemperatureAV = 0.0;
	for (vedo::vedo_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
	{
		pDOS = oWorld->GetDOStatus(ul);
		if (pDOS->ISMonitored())
		{
			dGranularTemperatureV  += (pDOS->GetVelocity()        - AvgV ) % (pDOS->GetVelocity()        - AvgV );
			dGranularTemperatureAV += (pDOS->GetAngularVelocity() - AvgAV) % (pDOS->GetAngularVelocity() - AvgAV);
		}
	}
	if (Counter != 0)
	{
		dGranularTemperatureV  = dGranularTemperatureV  / (vedo::vedo_float_t)Counter / 3.0;
		dGranularTemperatureAV = dGranularTemperatureAV / (vedo::vedo_float_t)Counter / 3.0;
		for (vedo::vedo_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
		{
			pDOS = oWorld->GetDOStatus(ul);
			if (pDOS->ISMonitored())
			{
				oWorld->SetGranularTemperatureV (ul, dGranularTemperatureV );
				oWorld->SetGranularTemperatureAV(ul, dGranularTemperatureAV);
			}
		}
	}
	std::map<std::string, vedo::vedo_float_t> gt;
	gt["Time"]                                 = oWorld->GetSystemParameter()->GetTimeCurrent();
	gt["NumberOfElement"]                      = (vedo::vedo_float_t)Counter;
	gt["AverageVelocity"]                      = AvgV.length();
	gt["GranularTemperatureOfVelocity"]        = dGranularTemperatureV;
	gt["AverageAngularVelocity"]               = AvgAV.length();
	gt["GranularTemperatureOfAngularVelocity"] = dGranularTemperatureAV;
	return gt;
}

void CombineModels(vedo::DOWorld* rWorld, vedo::DOWorld* oWorld)
{
	//vedo::vedo_uint_t rNum = rWorld->GetSystemParameter()->GetDONumber();
	vedo::vedo_uint_t oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName", njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);
	for (vedo::vedo_uint_t ul=0; ul<oNum; ul++)
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
	//vedo::vedo_uint_t rNum = rWorld->GetSystemParameter()->GetDONumber();
	vedo::vedo_uint_t oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName", njr::ZERO, njr::ZERO, njr::AXIALX, njr::AXIALZ, njr::ZERO, njr::ZERO, njr::ZERO);
		vedo::vedo_uint_t ulCounter = 0;
	for (vedo::vedo_uint_t ul=0; ul<oNum; ul++)
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
	//vedo::vedo_uint_t rNum = rWorld->GetSystemParameter()->GetDONumber();
	vedo::vedo_uint_t oNum = oWorld->GetSystemParameter()->GetDONumber();
	vedo::DOStatus dos
		("NoName",
		njr::ZERO,
		njr::ZERO,
		njr::AXIALX,
		njr::AXIALZ,
		njr::ZERO,
		njr::ZERO,
		njr::ZERO   );
	for (vedo::vedo_uint_t ul=0; ul<oNum; ul++)
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
	else if ((arg[1] == "-add_elements_in_simple_cubic_style") && (arg.size() == 11))
	{
		vedo::vedo_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		xmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		ymin = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		ymax = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		zmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		zmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		njr::Vector3d LowerBoundary(xmin, ymin, zmin);
		njr::Vector3d UpperBoundary(xmax, ymax, zmax);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		AddDOInSpace(&LowerBoundary, &UpperBoundary, DOName, oWorld);
		delete WriteDOWorld (arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_pontential_energy_from_sorce_point") && (arg.size() == 8))
	{
		vedo::vedo_float_t x, y, z;
		std::string DOName = arg[2];
		x = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		y = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		z = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		njr::Vector3d SourcePoint(x, y, z);
		AddPotentialEnergy(&SourcePoint, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_in_circular_path") && (arg.size() == 12))
	{
		std::string sDOName = arg[2];
		vedo::vedo_float_t
			dAngleStarted, dAngleEnded, dInclinedAngle,
			dRevolutionsPerTimeUnit, dEdgeVelocity, dR, dZ;
		vedo::vedo_uint_t uElementNumber;
		dAngleStarted = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dAngleEnded   = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		if (dAngleEnded <= dAngleStarted)
		{
			std::cerr
				<< "Error!! Code: Knight.cpp (-aecp)" << std::endl
				<< "        Note: Started angle is smaller than ended angle" << std::endl;
			exit(-1);
		}
		uElementNumber = vedo_cp->String2T<vedo::vedo_uint_t>(arg[6]);
		dR = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		dZ = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[10], pIactRecordTab);
		vedo::DOStatus dos
			(sDOName,
			njr::Vector3d(njr::ZERO),
			njr::Vector3d(njr::ZERO),
			njr::Vector3d(njr::AXIALZ),
			njr::Vector3d(njr::AXIALX),
			njr::Vector3d(njr::ZERO),
            njr::Vector3d(njr::ZERO),
            njr::Vector3d(njr::ZERO)   );
		vedo::vedo_float_t dAngleRange = (dAngleEnded - dAngleStarted) * njr::dDegree2PI;
		vedo::vedo_float_t dAngleIncremental = dAngleRange / (vedo::vedo_float_t)uElementNumber;
		vedo::vedo_float_t dAngle = dAngleStarted * njr::dDegree2PI;
		vedo::vedo_float_t dCosineAngle, dSineAngle;
		njr::Vector3d vLocalX, vLocalZ, vRotatedLocalX, vRotatedLocalZ;

		dInclinedAngle          = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dRevolutionsPerTimeUnit = vedo_cp->String2T<vedo::vedo_float_t>(arg[9]);
		if (dInclinedAngle == 0.0)
		{
			if (dRevolutionsPerTimeUnit == 0)
			{
				for (vedo::vedo_uint_t u=0; u<uElementNumber; u++)
				{
					dCosineAngle = cos(dAngle);
					dSineAngle   = sin(dAngle);
					dos.SetPosition(njr::Vector3d(dR*dCosineAngle, dR*dSineAngle, dZ));
					dos.SetOrientationZ(njr::Vector3d(dCosineAngle, dSineAngle, 0.0));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
					dAngle += dAngleIncremental;
				}
			}
			else
			{
				dEdgeVelocity = dR * dRevolutionsPerTimeUnit;
				for (vedo::vedo_uint_t u=0; u<uElementNumber; u++)
				{
					dCosineAngle = cos(dAngle);
					dSineAngle   = sin(dAngle);
					dos.SetPosition(njr::Vector3d(dR*dCosineAngle, dR*dSineAngle, dZ));
					dos.SetOrientationZ(njr::Vector3d(dCosineAngle, dSineAngle, 0.0));
					dos.SetVelocity(-dEdgeVelocity * dos.GetOrientationZ().Cross(njr::AXIALZ));
					oWorld->AddDOStatus(new vedo::DOStatus(dos));
					dAngle += dAngleIncremental;
				}
			}
		}
		else
		{
			dInclinedAngle *= njr::dDegree2PI;
			vedo::vedo_float_t dInclinedCosineAngle = cos(dInclinedAngle);
			vedo::vedo_float_t dInclinedSineAngle   = sin(dInclinedAngle);
			dR += dR * dInclinedSineAngle;
			if (dRevolutionsPerTimeUnit == 0)
			{
				for (vedo::vedo_uint_t u=0; u<uElementNumber; u++)
				{
					dCosineAngle = cos(dAngle);
					dSineAngle   = sin(dAngle);
					dos.SetPosition(njr::Vector3d(dR*dCosineAngle, dR*dSineAngle, dZ));
					vLocalX.Set
						(dInclinedSineAngle   * dCosineAngle,
						 dInclinedSineAngle   * dSineAngle  ,
						 dInclinedCosineAngle                );
					vLocalZ.Set
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
				for (vedo::vedo_uint_t u=0; u<uElementNumber; u++)
				{
					dCosineAngle = cos(dAngle);
					dSineAngle   = sin(dAngle);
					dos.SetPosition(njr::Vector3d(dR*dCosineAngle, dR*dSineAngle, dZ));
					vLocalX.Set
						(dInclinedSineAngle   * dCosineAngle,
						 dInclinedSineAngle   * dSineAngle  ,
						 dInclinedCosineAngle                );
					vLocalZ.Set
						( dInclinedCosineAngle * dCosineAngle,
						  dInclinedCosineAngle * dSineAngle  ,
						 -dInclinedSineAngle                  );
					dos.SetVelocity(-dEdgeVelocity * vLocalZ.Cross(vLocalX));
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
		vedo::vedo_float_t dPx, dPy, dPz, dVx, dVy, dVz, dAVx, dAVy, dAVz;
		vedo::vedo_float_t dOxx, dOxy, dOxz, dOzx, dOzy, dOzz;
		std::string sDOName = arg[2];
		dPx  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 3]);
		dPy  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 4]);
		dPz  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 5]);
		dVx  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 6]);
		dVy  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 7]);
		dVz  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 8]);
		dOxx = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 9]);
		dOxy = vedo_cp->String2T<vedo::vedo_float_t>(arg[10]);
		dOxz = vedo_cp->String2T<vedo::vedo_float_t>(arg[11]);
		dOzx = vedo_cp->String2T<vedo::vedo_float_t>(arg[12]);
		dOzy = vedo_cp->String2T<vedo::vedo_float_t>(arg[13]);
		dOzz = vedo_cp->String2T<vedo::vedo_float_t>(arg[14]);
		dAVx = vedo_cp->String2T<vedo::vedo_float_t>(arg[15]);
		dAVy = vedo_cp->String2T<vedo::vedo_float_t>(arg[16]);
		dAVz = vedo_cp->String2T<vedo::vedo_float_t>(arg[17]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[18], pIactRecordTab);
		vedo::DOStatus dos
			(sDOName,
			njr::Vector3d(dPx , dPy , dPz ),
			njr::Vector3d(dVx , dVy , dVz ),
			njr::Vector3d(dOxx, dOxy, dOxz).direction(),
			njr::Vector3d(dOzx, dOzy, dOzz).direction(),
			njr::Vector3d(dAVx, dAVy, dAVz),
            njr::Vector3d(njr::ZERO),
            njr::Vector3d(njr::ZERO));
		oWorld->AddDOStatus(new vedo::DOStatus(dos));
		delete WriteDOWorld (arg[19], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_velocity") && (arg.size() == 8))
	{
		vedo::vedo_float_t dVx, dVy, dVz;
		std::string DOName = arg[2];
		dVx = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dVy = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dVz = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		njr::Vector3d vExternalVelocity(dVx, dVy, dVz);
		AddExternalVelocity(&vExternalVelocity, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_angular_velocity") && (arg.size() == 8))
	{
		vedo::vedo_float_t dAVx, dAVy, dAVz;
		std::string DOName = arg[2];
		dAVx = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dAVy = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dAVz = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6], pIactRecordTab);
		njr::Vector3d vExternalAngularVelocity(dAVx, dAVy, dAVz);
		AddExternalAngularVelocity(&vExternalAngularVelocity, DOName, oWorld);
		delete WriteDOWorld (arg[7], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_randomly_in_Cartesian_coordinate") && (arg.size() == 12))
	{
		vedo::vedo_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		vedo::vedo_uint_t num = 0;
		std::string DOName = arg[2];
		num  = vedo_cp->String2T<vedo::vedo_uint_t>(arg[3]);
		xmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		xmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		ymin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		ymax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		zmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		zmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[9]);
		njr::Vector3d LowerBoundary(xmin, ymin, zmin);
		njr::Vector3d UpperBoundary(xmax, ymax, zmax);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[10], pIactRecordTab);
		AddRandomDOInCartesianSpace(&LowerBoundary, &UpperBoundary, DOName, &num, oWorld);
		delete WriteDOWorld (arg[11], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_randomly_in_cylindrical_coordinate") && (arg.size() == 11))
	{
		vedo::vedo_float_t x, y, z, r, h;
		vedo::vedo_uint_t num = 0;
		std::string DOName = arg[2];
		num = vedo_cp->String2T<vedo::vedo_uint_t>(arg[3]);
		x   = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		y   = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		z   = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		r   = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		h   = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		njr::Vector3d Center(x, y, z);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		AddRandomDOInCylindricalSpace(&Center, &r, &h, DOName, &num, oWorld);
		delete WriteDOWorld (arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-add_elements_randomly_in_spherical_coordinate") && (arg.size() == 10))
	{
		vedo::vedo_float_t x, y, z, r;
		vedo::vedo_uint_t num = 0;
		std::string DOName = arg[2];
		num = vedo_cp->String2T<vedo::vedo_uint_t>(arg[3]);
		x   = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		y   = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		z   = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		r   = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		njr::Vector3d Center(x, y, z);
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
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		vedo::vedo_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::vedo_uint_t ul = 0;
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
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the average information
		vedo::vedo_float_t dTotalVolume = 0.0;
		std::vector<vedo::vedo_float_t> vVolume;
		vedo::vedo_float_t dTotalMass   = 0.0;
		njr::Vector3d vMassCenter, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::vedo_float_t dWeighting;
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
			(vedo::vedo_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::vedo_float_t, njr::Vector3d> pdvVolume
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

		vedo::vedo_float_t dVolumeRatio = dTotalVolume / OriginalBC.GetVolume();
		if (dTotalMass != 0.0)
		{
			vMassCenter = 1 / dTotalMass * vMassCenter;
		}
		else
		{
			vMassCenter = OriginalBC.GetCenter();
		}

		vedo::vedo_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dTotalVolume != 0.0)
		{
			vAverageVelocity        = 1.0 / dTotalVolume * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dTotalVolume * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for (vedo::vedo_uint_t ul=0; ul<vVolume.size(); ul++)
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
	else if ((arg[1] == "-average_information") && (arg.size() == 11))
	{
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[9]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[10]);
		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const vedo::vedo_float_t dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		vedo::vedo_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool bHasFindThe1stSphere = false;
		vedo::vedo_uint_t ul = 0;
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
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the average information
		vedo::vedo_float_t dTotalVolume = 0.0;
		std::vector<vedo::vedo_float_t> vVolume;
		vedo::vedo_float_t dTotalMass   = 0.0;
		njr::Vector3d vMassCenter, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::vedo_float_t dWeighting;
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
			(vedo::vedo_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS         = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom          = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::vedo_float_t, njr::Vector3d> pdvVolume
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

		vedo::vedo_float_t dVolumeRatio = dTotalVolume / OriginalBC.GetVolume();
		if (dTotalMass != 0.0)
		{
			vMassCenter = 1 / dTotalMass * vMassCenter;
		}
		else
		{
			vMassCenter = OriginalBC.GetCenter();
		}

		vedo::vedo_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dTotalVolume != 0.0)
		{
			vAverageVelocity        = 1.0 / dTotalVolume * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dTotalVolume * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for (vedo::vedo_uint_t ul=0; ul<vVolume.size(); ul++)
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
	else if ((arg[1] == "-average_information_projected_in_xy_plane") && (arg.size() == 10))
	{
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		vedo::vedo_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::vedo_uint_t ul = 0;
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
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::vedo_float_t dProjectedArea = 0.0;
		std::vector<vedo::vedo_float_t> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::vedo_float_t dWeighting;
		vedo::vedo_float_t dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(vedo::vedo_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::vedo_float_t, njr::Vector3d> pdvProjectedArea
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

		vedo::vedo_float_t dProjectedAreaRatio
			= dProjectedArea
			/ OriginalBC.GetLength().x()
			/ OriginalBC.GetLength().y();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 vCentroid.y() / dTotalWeighting,
				 OriginalBC.GetCenter().z()      );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::vedo_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for (vedo::vedo_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
	else if ((arg[1] == "-average_information_projected_in_xy_plane") && (arg.size() == 11))
	{
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[9]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[10]);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const vedo::vedo_float_t dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		vedo::vedo_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::vedo_uint_t ul = 0;
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
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::vedo_float_t dProjectedArea = 0.0;
		std::vector<vedo::vedo_float_t> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::vedo_float_t dWeighting;
		vedo::vedo_float_t dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(vedo::vedo_uint_t ul=0;
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

			std::pair<vedo::vedo_float_t, njr::Vector3d> pdvProjectedArea
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

		vedo::vedo_float_t dProjectedAreaRatio
			= dProjectedArea
			/ OriginalBC.GetLength().x()
			/ OriginalBC.GetLength().y();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 vCentroid.y() / dTotalWeighting,
				 OriginalBC.GetCenter().z()      );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::vedo_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for (vedo::vedo_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
	else if ((arg[1] == "-average_information_projected_in_yz_plane") && (arg.size() == 10))
	{
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		vedo::vedo_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::vedo_uint_t ul = 0;
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
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::vedo_float_t dProjectedArea = 0.0;
		std::vector<vedo::vedo_float_t> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::vedo_float_t dWeighting;
		vedo::vedo_float_t dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(vedo::vedo_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::vedo_float_t, njr::Vector3d> pdvProjectedArea
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

		vedo::vedo_float_t dProjectedAreaRatio
			= dProjectedArea / BC.GetLength().y() / OriginalBC.GetLength().z();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(OriginalBC.GetCenter().x()     ,
				 vCentroid.y() / dTotalWeighting,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::vedo_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for (vedo::vedo_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
	else if ((arg[1] == "-average_information_projected_in_yz_plane") && (arg.size() == 11))
	{
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[9]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[10]);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const vedo::vedo_float_t dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		vedo::vedo_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::vedo_uint_t ul = 0;
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
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::vedo_float_t dProjectedArea = 0.0;
		std::vector<vedo::vedo_float_t> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::vedo_float_t dWeighting;
		vedo::vedo_float_t dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(vedo::vedo_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::vedo_float_t, njr::Vector3d> pdvProjectedArea
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

		vedo::vedo_float_t dProjectedAreaRatio
			= dProjectedArea / BC.GetLength().y() / OriginalBC.GetLength().z();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(OriginalBC.GetCenter().x()     ,
				 vCentroid.y() / dTotalWeighting,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::vedo_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for (vedo::vedo_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
	else if ((arg[1] == "-average_information_projected_in_xz_plane") && (arg.size() == 10))
	{
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);

		const vedo::DOStatus* pDOS;

		// Fix boundary (stage 1: find first data)
		vedo::vedo_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::vedo_uint_t ul = 0;
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
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::vedo_float_t dProjectedArea = 0.0;
		std::vector<vedo::vedo_float_t> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::vedo_float_t dWeighting;
		vedo::vedo_float_t dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(vedo::vedo_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::vedo_float_t, njr::Vector3d> pdvProjectedArea
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

		vedo::vedo_float_t dProjectedAreaRatio
			= dProjectedArea / OriginalBC.GetLength().x() / OriginalBC.GetLength().z();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 OriginalBC.GetCenter().y()     ,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::vedo_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for (vedo::vedo_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
	else if ((arg[1] == "-average_information_projected_in_xz_plane") && (arg.size() == 11))
	{
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		vedo::Boundary OriginalBC
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld         = ReadDOWorld(arg[9]);
		vedo::DOWorld* oWorldNextStep = ReadDOWorld(arg[10]);

		const vedo::DOStatus *pDOS, *pDOSNextStep;
		const vedo::vedo_float_t dTimeStep
			= oWorldNextStep->GetSystemParameter()->GetTimeCurrent()
			- oWorld->GetSystemParameter()->GetTimeCurrent();

		// Fix boundary (stage 1: find first data)
		vedo::vedo_float_t
			dNewXMin, dNewXMax, dNewYMin, dNewYMax, dNewZMin, dNewZMax, dRadius;
		bool   bHasFindThe1stSphere = false;
		vedo::vedo_uint_t ul = 0;
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
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );

		// Calculate the projected information
		vedo::vedo_float_t dProjectedArea = 0.0;
		std::vector<vedo::vedo_float_t> vProjectedArea;
		njr::Vector3d vCentroid, vAverageVelocity, vAverageAngularVelocity;

		const  vedo::DOModel* dom = 0;
		vedo::vedo_float_t dWeighting;
		vedo::vedo_float_t dTotalWeighting = 0.0;
		std::vector<njr::Vector3d> vvAverageVelocity, vvAverageAngularVelocity;

		njr::Vector3d vTempVelocity, vTempAngularVelocity;
		for
			(vedo::vedo_uint_t ul=0;
			 ul<oWorld->GetSystemParameter()->GetDONumber();
			 ul++                                           )
		{
			pDOS = oWorld->GetDOStatus(ul);
			pDOSNextStep = oWorldNextStep->GetDOStatus(ul);
			dom  = oWorld->GetDOModel(pDOS->GetDOName());
			std::pair<vedo::vedo_float_t, njr::Vector3d> pdvProjectedArea
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

		vedo::vedo_float_t dProjectedAreaRatio
			= dProjectedArea / OriginalBC.GetLength().x() / OriginalBC.GetLength().z();
		if (dTotalWeighting != 0.0)
		{
			vCentroid.Set
				(vCentroid.x() / dTotalWeighting,
				 OriginalBC.GetCenter().y()     ,
				 vCentroid.z() / dTotalWeighting );
		}
		else
		{
			vCentroid = OriginalBC.GetCenter();
		}

		vedo::vedo_float_t dGranularTemperatureVelocity         = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityX        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityY        = 0.0;
		vedo::vedo_float_t dGranularTemperatureVelocityZ        = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocity  = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityX = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityY = 0.0;
		vedo::vedo_float_t dGranularTemperatureAngularVelocityZ = 0.0;
		if (dProjectedArea != 0.0)
		{
			vAverageVelocity        = 1.0 / dProjectedArea * vAverageVelocity;
			vAverageAngularVelocity = 1.0 / dProjectedArea * vAverageAngularVelocity;
			njr::Vector3d vVTemp1, vVTemp2;
			for (vedo::vedo_uint_t ul=0; ul<vProjectedArea.size(); ul++)
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
	else if ((arg[1] == "-convert") && (arg.size() == 4))
	{
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		delete WriteDOWorld(arg[3], ReadDOWorld(arg[2], pIactRecordTab), pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-cross_section_area_on_surface") && (arg.size() == 8))
	{
		std::string DOName = arg[2];
		vedo::vedo_float_t a, b, c, d;
		a = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		b = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		c = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		d = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[7]);
		vedo::vedo_uint_t counter   = 0;
		vedo::vedo_float_t   dArea     = 0.0;
		vedo::vedo_float_t   dAreaTemp;
		for
			(vedo::vedo_uint_t ul=0;
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
			<< "Cross area        : " << dArea
			<< '(' << counter << " elements)"           << std::endl;
	}
	else if ((arg[1] == "-cross_section_flux_on_surface") && (arg.size() == 8))
	{
		std::string DOName = arg[2];
		vedo::vedo_float_t a, b, c, d;
		a = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		b = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		c = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		d = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		vedo::DOWorld*    oWorld        = ReadDOWorld(arg[7]);
		vedo::vedo_uint_t    counter       = 0;
		vedo::vedo_float_t      dArea         = 0.0;
		vedo::vedo_float_t      dAreaTemp;
		njr::Vector3d vFluxVelocity, vFluxAngularVelocity;
		for
			(vedo::vedo_uint_t ul=0;
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
		vedo::vedo_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		CalcCoordinateNum CCN;
		CalcDensity CDN;
		xmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		xmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		ymin = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		ymax = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		zmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		zmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
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
		vedo::vedo_float_t dX, dY, dZ;
		dX = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dY = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dZ = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		njr::Vector3d vP(dX, dY, dZ);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5]);
		std::map<vedo::vedo_float_t, vedo::vedo_uint_t> mduDistance;

		for (vedo::vedo_uint_t u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
			mduDistance[(oWorld->GetDOStatus(u)->GetPosition() - vP).length()] = u;

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ElementID, ElementName, Distance, X, Y, Z" << std::endl;
		vedo::vedo_uint_t uRank = 1;
		for (std::map<vedo::vedo_float_t, vedo::vedo_uint_t>::iterator
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
		vedo::vedo_float_t dX, dY;
		dX = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dY = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		njr::Vector3d vP(dX, dY, 0.0);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[4]);
		njr::Vector3d vD;
		std::map<vedo::vedo_float_t, vedo::vedo_uint_t> mduDistance;

		for (vedo::vedo_uint_t u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.SetZ(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ElementID, ElementName, Distance, X, Y, Z" << std::endl;
		vedo::vedo_uint_t uRank = 1;
		for (std::map<vedo::vedo_float_t, vedo::vedo_uint_t>::iterator
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
		vedo::vedo_float_t dY, dZ;
		dY = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dZ = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		njr::Vector3d vP(0.0, dY, dZ);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[4]);
		njr::Vector3d vD;
		std::map<vedo::vedo_float_t, vedo::vedo_uint_t> mduDistance;

		for (vedo::vedo_uint_t u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.SetX(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ElementID, ElementName, Distance, X, Y, Z" << std::endl;
		vedo::vedo_uint_t uRank = 1;
		for (std::map<vedo::vedo_float_t, vedo::vedo_uint_t>::iterator
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
		vedo::vedo_float_t dX, dZ;
		dX = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dZ = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		njr::Vector3d vP(dX, 0.0, dZ);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[4]);
		njr::Vector3d vD;
		std::map<vedo::vedo_float_t, vedo::vedo_uint_t> mduDistance;

		for (vedo::vedo_uint_t u=0; u<oWorld->GetSystemParameter()->GetDONumber(); u++)
		{
			vD = oWorld->GetDOStatus(u)->GetPosition() - vP;
			vD.SetY(0.0);
			mduDistance[vD.length()] = u;
		}

		std::ofstream ofDistance("Distance.csv", std::ios::out);
		ofDistance << "Rank, ElementID, ElementName, Distance, X, Y, Z" << std::endl;
		vedo::vedo_uint_t uRank = 1;
		for (std::map<vedo::vedo_float_t, vedo::vedo_uint_t>::iterator
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
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[9]);
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
*/
	else if ((arg[1] == "-erase_single_element") && (arg.size() == 5))
	{
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[3], pIactRecordTab);
		vedo::vedo_uint_t uID;
		uID = vedo_cp->String2T<vedo::vedo_uint_t>(arg[2]);
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
		vedo::vedo_uint_t uIDStarted, uIDEnded;
		uIDStarted = vedo_cp->String2T<vedo::vedo_uint_t>(arg[2]);
		uIDEnded   = vedo_cp->String2T<vedo::vedo_uint_t>(arg[3]);
		std::string idofilename = arg[4];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::vedo_uint_t> ulIDList;
		for (vedo::vedo_uint_t uID=uIDStarted; uID<=uIDEnded; uID++)
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
		std::vector<vedo::vedo_uint_t> ulIDList;
		const vedo::DOStatus* dosp;
		njr::Vector3d vPosition;
		for
			(vedo::vedo_uint_t ul=0;
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
		vedo::vedo_float_t da, db, dc, dd;
		da = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		db = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dc = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dd = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[7], pIactRecordTab);
		std::string idofilename = arg[7];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::vedo_uint_t> ulIDList;
		const vedo::DOStatus* dosp;
		njr::Vector3d vPosition;
		for
			(vedo::vedo_uint_t ul=0;
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
	else if ((arg[1] == "-erase_spherical_elements") && (arg.size() == 8))
	{
		vedo::vedo_float_t da, db, dc, dd;
		da = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		db = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dc = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dd = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* pWorld = ReadDOWorld(arg[6], pIactRecordTab);
		std::string idofilename = arg[6];
		vedo::Consultant* pConsultant
			= new vedo::Consultant(pWorld, pIactRecordTab, idofilename, 1);
		std::vector<vedo::vedo_uint_t> ulIDList;
		const vedo::DOStatus* dosp;
		njr::Vector3d vPosition;
		for
			(vedo::vedo_uint_t ul=0;
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
		vedo::vedo_uint_t CutNumX      = 0;
		vedo::vedo_uint_t CutNumY      = 0;
		vedo::vedo_uint_t CutNumZ      = 0;
		vedo::vedo_float_t        dAngle2XAxis = 0.0;
		vedo::vedo_float_t        dAngle2YAxis = 0.0;
		vedo::vedo_float_t        dAngle2ZAxis = 0.0;
		vedo::DOWorld* oWorld      = ReadDOWorld(arg[2]);
		std::string DOName   = arg[3];
		CutNumX = vedo_cp->String2T<vedo::vedo_uint_t>(arg[4]);
		CutNumY = vedo_cp->String2T<vedo::vedo_uint_t>(arg[5]);
		CutNumZ = vedo_cp->String2T<vedo::vedo_uint_t>(arg[6]);
		dAngle2XAxis = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		dAngle2YAxis = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		dAngle2ZAxis = vedo_cp->String2T<vedo::vedo_float_t>(arg[9]);
		oWorld->Rotate
			(dAngle2XAxis*0.01745329261,
			 dAngle2YAxis*0.01745329261,
			 dAngle2ZAxis*0.01745329261 );

		const vedo::DOStatus* pDOS;
		vedo::vedo_uint_t StartNumber = 0;
		while(   StartNumber < oWorld->GetSystemParameter()->GetDONumber()
			  && (oWorld->GetDOStatus(StartNumber)->GetDOName() != DOName))
		{
			StartNumber++;
		}
		njr::Vector3d PTemp(oWorld->GetDOStatus(StartNumber)->GetPosition());
		vedo::vedo_float_t XMin = PTemp.x();
		vedo::vedo_float_t YMin = PTemp.y();
		vedo::vedo_float_t ZMin = PTemp.z();
		vedo::vedo_float_t XMax = XMin;
		vedo::vedo_float_t YMax = YMin;
		vedo::vedo_float_t ZMax = ZMin;

		for (vedo::vedo_uint_t ul=StartNumber+1;
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
		vedo::vedo_float_t dCoverRange = oWorld->GetDOModel(DOName)->GetRange();
		XMax += dCoverRange;
		YMax += dCoverRange;
		ZMax += dCoverRange;
		XMin -= dCoverRange;
		YMin -= dCoverRange;
		ZMin -= dCoverRange;
		vedo::vedo_float_t width  = (XMax - XMin) / (vedo::vedo_float_t)CutNumX;
		vedo::vedo_float_t length = (YMax - YMin) / (vedo::vedo_float_t)CutNumY;
		vedo::vedo_float_t height = (ZMax - ZMin) / (vedo::vedo_float_t)CutNumZ;
		vedo::GSRectangle space("Granular Temperature Space", width, length, height);
		vedo::vedo_float_t XCentral = 0.5 * (XMin + XMax);
		vedo::vedo_float_t YCentral = 0.5 * (YMin + YMax);
		vedo::vedo_float_t ZCentral = 0.5 * (ZMin + ZMax);

		vedo::vedo_uint_t XCounter = 1;
		vedo::vedo_uint_t YCounter;
		vedo::vedo_uint_t ZCounter;

		std::map<std::string, vedo::vedo_float_t> gt;
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
		for (vedo::vedo_float_t x=XMin+0.5*width; XCounter<=CutNumX; x+=width, XCounter++)
		{
			YCounter = 1;
			for
				(vedo::vedo_float_t y=YMin+0.5*length;
				 YCounter<=CutNumY;
				 y+=length, YCounter++    )
			{
				ZCounter = 1;
				for
					(vedo::vedo_float_t z=ZMin+0.5*height;
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
						<< (vedo::vedo_uint_t)gt["NumberOfElement"]       << ", "
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
		vedo::vedo_float_t width, length, height, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		x      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 3]);
		y      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 4]);
		z      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 5]);
		width  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 6]);
		length = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 7]);
		height = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 8]);
		Xx     = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 9]);
		Xy     = vedo_cp->String2T<vedo::vedo_float_t>(arg[10]);
		Xz     = vedo_cp->String2T<vedo::vedo_float_t>(arg[11]);
		Zx     = vedo_cp->String2T<vedo::vedo_float_t>(arg[12]);
		Zy     = vedo_cp->String2T<vedo::vedo_float_t>(arg[13]);
		Zz     = vedo_cp->String2T<vedo::vedo_float_t>(arg[14]);
		vedo::GSRectangle space("Granular Temperature Space", width, length, height);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 njr::Vector3d(x, y, z),
				 njr::ZERO,
				 njr::Vector3d(Xx, Xy, Xz),
				 njr::Vector3d(Zx, Zy, Zz),
				 njr::ZERO,
				 njr::ZERO,
				 njr::ZERO                 ));
		vedo::DOWorld* oWorld = ReadDOWorld(arg[15]);
		std::map<std::string, vedo::vedo_float_t> gt
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
			<< (vedo::vedo_uint_t)gt["NumberOfElement"]        << ", "
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
		vedo::vedo_float_t radius, height, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		x      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 3]);
		y      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 4]);
		z      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 5]);
		radius = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 6]);
		height = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 7]);
		Xx     = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 8]);
		Xy     = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 9]);
		Xz     = vedo_cp->String2T<vedo::vedo_float_t>(arg[10]);
		Zx     = vedo_cp->String2T<vedo::vedo_float_t>(arg[11]);
		Zy     = vedo_cp->String2T<vedo::vedo_float_t>(arg[12]);
		Zz     = vedo_cp->String2T<vedo::vedo_float_t>(arg[13]);
		vedo::GSCylinder space("Granular Temperature Space", radius, height);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 njr::Vector3d(x, y, z),
				 njr::ZERO,
				 njr::Vector3d(Xx, Xy, Xz),
				 njr::Vector3d(Zx, Zy, Zz),
				 njr::ZERO,
				 njr::ZERO,
				 njr::ZERO                 ));
		vedo::DOWorld* oWorld = ReadDOWorld(arg[14]);
		std::map<std::string, vedo::vedo_float_t> gt
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
			<< (vedo::vedo_uint_t)gt["NumberOfElement"]        << ", "
			<< gt["AverageVelocity"]                       << ", "
			<< gt["GranularTemperatureOfVelocity"]         << ", "
			<< gt["AverageAngularVelocity"]                << ", "
			<< gt["GranularTemperatureOfAngularVelocity"]  << std::endl;
		FileGT.close();
		delete oWorld;
	}
	else if ((arg[1] == "-granular_temperature_in_sphere") && (arg.size() == 8))
	{
		vedo::vedo_float_t radius, x, y, z;
		std::string DOName = arg[2];
		x      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 3]);
		y      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 4]);
		z      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 5]);
		radius = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 6]);
		vedo::GSSphere space("Granular Temperature Space", radius);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 njr::Vector3d(x, y, z),
				 njr::ZERO,
				 njr::AXIALX,
				 njr::AXIALZ,
				 njr::ZERO,
				 njr::ZERO,
				 njr::ZERO              ));
		vedo::DOWorld* oWorld = ReadDOWorld(arg[7]);
		std::map<std::string, vedo::vedo_float_t> gt
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
			<< (vedo::vedo_uint_t)gt["NumberOfElement"]        << ", "
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
		vedo::vedo_float_t XLength, YLength, ZLength, x, y, z, Xx, Xy, Xz, Zx, Zy, Zz;
		x       = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 3]);
		y       = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 4]);
		z       = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 5]);
		XLength = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 6]);
		YLength = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 7]);
		ZLength = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 8]);
		Xx      = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 9]);
		Xy      = vedo_cp->String2T<vedo::vedo_float_t>(arg[10]);
		Xz      = vedo_cp->String2T<vedo::vedo_float_t>(arg[11]);
		Zx      = vedo_cp->String2T<vedo::vedo_float_t>(arg[12]);
		Zy      = vedo_cp->String2T<vedo::vedo_float_t>(arg[13]);
		Zz      = vedo_cp->String2T<vedo::vedo_float_t>(arg[14]);
		vedo::GSEllipsoid space
			("Granular Temperature Space", XLength, YLength, ZLength);
		space.SetStatus
			(new vedo::DOStatus
				("No Name",
				 njr::Vector3d(x, y, z),
				 njr::ZERO,
				 njr::Vector3d(Xx, Xy, Xz),
				 njr::Vector3d(Zx, Zy, Zz),
				 njr::ZERO,
				 njr::ZERO,
				 njr::ZERO                 ));
		vedo::DOWorld* oWorld = ReadDOWorld(arg[15]);
		std::map<std::string, vedo::vedo_float_t> gt
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
			<< (vedo::vedo_uint_t)gt["NumberOfElement"]        << ", "
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
		vedo::vedo_float_t dXMin, dXMax, dYMin, dYMax, dZMin, dZMax, dMeshSize;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dYMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dYMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);

		const vedo::DOStatus* pDOS;
		vedo::vedo_float_t dRadius;
		njr::Vector3d vPosition;
		std::ofstream oFileElementInside;
		oFileElementInside.open("ElementInside.txt", std::ios::out);

		for (vedo::vedo_uint_t ul=0;
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
		vedo::vedo_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		xmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		ymin = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		ymax = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		zmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		zmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		vedo::vedo_float_t d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		vedo::vedo_float_t rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		vedo::vedo_float_t rectCenter[3]
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
		vedo::vedo_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		xmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		ymin = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		ymax = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		zmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		zmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		vedo::vedo_float_t d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		vedo::vedo_float_t rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		vedo::vedo_float_t rectCenter[3]
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
		vedo::vedo_float_t xmin, xmax, ymin, ymax, zmin, zmax;
		std::string DOName = arg[2];
		xmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		xmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		ymin = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		ymax = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		zmin = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		zmax = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9]);
		vedo::vedo_float_t d
			= 2.0
			* oWorld->GetDOModel(DOName)->GetShapeAttributes().sphere.radius;
		vedo::vedo_float_t rectSize[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
		vedo::vedo_float_t rectCenter[3]
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
*/
/*
	else if ((arg[1] == "-profile_Info2Y") && (arg.size() == 7))
	{
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		vedo::vedo_float_t dXmin, dXmax, dZmin, dZmax;
		dXMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dXMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dZMin = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dZMax = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		const vedo::DOStatus* pDOS;

		std::map<vedo::vedo_float_t, vedo::vedo_uint_t> mInformation;
		std::map<vedo::vedo_float_t, vedo::vedo_uint_t>::iterator mpInformation;
		for (vedo::vedo_uint_t ul=0; ul<oWorld->GetSystemParameter()->GetDONumber(); ul++)
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
		vedo::vedo_float_t dMeshSize;
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
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
		vedo::vedo_float_t dMeshSize;
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
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
		vedo::vedo_float_t dMeshSize;
		dMeshSize = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
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
		vedo::vedo_uint_t uID;
		uID = vedo_cp->String2T<vedo::vedo_uint_t>(arg[2]);
		vedo::vedo_float_t dPx, dPy, dPz, dVx, dVy, dVz, dAVx, dAVy, dAVz;
		vedo::vedo_float_t dOxx, dOxy, dOxz, dOzx, dOzy, dOzz;
		dPx  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 3]);
		dPy  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 4]);
		dPz  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 5]);
		dVx  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 6]);
		dVy  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 7]);
		dVz  = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 8]);
		dOxx = vedo_cp->String2T<vedo::vedo_float_t>(arg[ 9]);
		dOxy = vedo_cp->String2T<vedo::vedo_float_t>(arg[10]);
		dOxz = vedo_cp->String2T<vedo::vedo_float_t>(arg[11]);
		dOzx = vedo_cp->String2T<vedo::vedo_float_t>(arg[12]);
		dOzy = vedo_cp->String2T<vedo::vedo_float_t>(arg[13]);
		dOzz = vedo_cp->String2T<vedo::vedo_float_t>(arg[14]);
		dAVx = vedo_cp->String2T<vedo::vedo_float_t>(arg[15]);
		dAVy = vedo_cp->String2T<vedo::vedo_float_t>(arg[16]);
		dAVz = vedo_cp->String2T<vedo::vedo_float_t>(arg[17]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[18], pIactRecordTab);
		const vedo::DOStatus* dosp = oWorld->GetDOStatus(uID);
		vedo::DOStatus dos
			(oWorld->GetDOName(uID),
			njr::Vector3d(dPx , dPy , dPz ),
			njr::Vector3d(dVx , dVy , dVz ),
			njr::Vector3d(dOxx, dOxy, dOxz).direction(),
			njr::Vector3d(dOzx, dOzy, dOzz).direction(),
			njr::Vector3d(dAVx, dAVy, dAVz),
            dosp->GetImpact(),
            dosp->GetAngularImpact()                    );
		oWorld->SetDOStatus(uID, dos);
		delete WriteDOWorld(arg[19], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if (   (arg[1] == "-modify_element_velocity_and_angular_velocity")
			 && (arg.size() == 11)                                         )
	{
		vedo::vedo_uint_t uID;
		uID = vedo_cp->String2T<vedo::vedo_uint_t>(arg[2]);
		vedo::vedo_float_t dVx, dVy, dVz, dAVx, dAVy, dAVz;
		dVx  = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dVy  = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dVz  = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
		dAVx = vedo_cp->String2T<vedo::vedo_float_t>(arg[6]);
		dAVy = vedo_cp->String2T<vedo::vedo_float_t>(arg[7]);
		dAVz = vedo_cp->String2T<vedo::vedo_float_t>(arg[8]);
		vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[9], pIactRecordTab);
		oWorld
			->SetDOStatusVelocityAndAngularVelocity
				(uID,
				 njr::Vector3d(dVx , dVy , dVz),
				 njr::Vector3d(dAVx , dAVy , dAVz));
		delete WriteDOWorld(arg[10], oWorld, pIactRecordTab);
		delete pIactRecordTab;
	}
	else if ((arg[1] == "-rotate_all_elements") && (arg.size() == 7))
	{
		vedo::vedo_float_t dAngle2XAxis = 0.0;
		vedo::vedo_float_t dAngle2YAxis = 0.0;
		vedo::vedo_float_t dAngle2ZAxis = 0.0;
		dAngle2XAxis = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dAngle2YAxis = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dAngle2ZAxis = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
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
		vedo::vedo_float_t x, y, z;
		std::string DOName = arg[2];
		x = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		y = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		z = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
        // We have noe deal with interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[6]);
		oWorld->Shift(njr::Vector3d(x, y, z), DOName);
		delete WriteDOWorld (arg[7], oWorld);
	}
	else if ((arg[1] == "-shift_all_elements") && (arg.size() == 7))
	{
		vedo::vedo_float_t x, y, z;
		x = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		y = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		z = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
        vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5], pIactRecordTab);
		oWorld->Shift(njr::Vector3d(x, y, z));
		delete WriteDOWorld (arg[6], oWorld, pIactRecordTab);
        delete pIactRecordTab;
	}
/*
	else if ((arg[1] == "-sort") && (arg.size() == 4))
	{
	    // We have not deal with the interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[2]);
		oWorld->SortingDOStatus();
		delete WriteDOWorld (arg[3], oWorld);
	}
	else if ((arg[1] == "-sort_distance") && (arg.size() == 6))
	{
		vedo::vedo_float_t dX, dY, dZ;
		dX = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dY = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dZ = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		njr::Vector3d vP(dX, dY, dZ);
	    // We have not deal with the interactions. At here, we clear all of them.
		vedo::DOWorld* oWorld = ReadDOWorld(arg[5]);

		std::map<vedo::vedo_float_t, vedo::vedo_uint_t> mIndex;
		vedo::vedo_float_t dD;
		for (vedo::vedo_uint_t ul=0;
			 ul<(oWorld->GetSystemParameter()->GetDONumber());
			 ul++                                             )
		{
			dD = ((oWorld->GetDOStatus(ul)->GetPosition()) - vP).length();
			mIndex[dD] = ul;
		}

		vedo::vedo_uint_t ulN;
		std::ofstream FileSortDistance;
		FileSortDistance.open("ElementSorting.csv", std::ios::out);
		FileSortDistance
			<< "Distance, SerialNumber, DOName, Px, Py, Pz" << std::endl;
		for (std::map<vedo::vedo_float_t, vedo::vedo_uint_t>::iterator
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
		vedo::vedo_float_t dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent;
		dTimeStart    = vedo_cp->String2T<vedo::vedo_float_t>(arg[2]);
		dTimeStop     = vedo_cp->String2T<vedo::vedo_float_t>(arg[3]);
		dTimeInterval = vedo_cp->String2T<vedo::vedo_float_t>(arg[4]);
		dTimeCurrent  = vedo_cp->String2T<vedo::vedo_float_t>(arg[5]);
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
		njr::Vector3d
			vPosition, vOrientationX, vOrientationZ, vVelocity, vAngularVelocity;
		for (vedo::vedo_uint_t uID=0; uID<oWorld->GetSystemParameter()->GetDONumber(); uID++)
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
