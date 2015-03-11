#include <vedo/constants/interfaces/Constants.h>
#include <vedo/framework/interfaces/DOModel.h>
#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/IactRecordTab.h>
#include <vedo/framework/interfaces/ImpactStatus.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>

/*
std::ostream& operator <<
	(std::ostream& os,
	 const std::pair<vedo_int_t, const boost::property_tree::ptree&>& rNode)
{
	vedo_int_t iNext = rNode.first + 4;
	const boost::property_tree::ptree& rPT = rNode.second;
	os << "\"" << rPT.data() << "\"" << std::endl;
	for(auto it=rPT.begin(); it!=rPT.end(); ++it)
	{
		os.width(iNext);
		os << "";
		os << "\"" << it->first << "\"=";
		os << std::pair<vedo_int_t, const boost::property_tree::ptree&>(iNext, it->second);
	}
	return os;
}
*/

njr::Vector3d Node2Vector3d(boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree* pt2 = &(pt.get_child("<xmlattr>"));
	return njr::Vector3d(pt2->get<vedo::vedo_float_t>("x"), pt2->get<vedo::vedo_float_t>("y"), pt2->get<vedo::vedo_float_t>("z"));
}

njr::HalfSpace Node2HalfSpace(boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree* pt2 = &(pt.get_child("<xmlattr>"));
	std::string sense = pt2->get<std::string>("sense");
	Sense s;
	if (sense == "E")
	{
		s = E;
	}
	else if (sense == "L")
	{
		s = L;
	}
	else if (sense == "G")
	{
		s = G;
	}
	else
	{
		s = E;
	}

	return
		njr::HalfSpace
			(pt2->get<vedo::vedo_float_t>("a"),
			 pt2->get<vedo::vedo_float_t>("b"),
			 pt2->get<vedo::vedo_float_t>("c"),
			 s,
			 pt2->get<vedo::vedo_float_t>("d") );
}

std::pair<vedo::DOShapeType, vedo::DOShapeAttributes> Node2DOShapeTypeAndAttributes(boost::property_tree::ptree& pt)
{
	vedo::DOShapeType       st;
	vedo::DOShapeAttributes sa;

	boost::property_tree::ptree* pt2;

	if (pt.get_child_optional("Sphere"))
	{
		pt2 = &(pt.get_child("Sphere"));
		st = vedo::Sphere;
		sa.sphere.radius = pt2->get<vedo::vedo_float_t>("<xmlattr>.Radius", 0.0);
		if (sa.sphere.radius <= 0.0)
		{
			std::cout
				<< "Error!! Code: DOWorld_ReadXML.cpp: std::pair<vedo::DOShapeType, vedo::DOShapeAttributes> Node2DOShapeTypeAndAttributes(boost::property_tree::ptree&)" << std::endl
				<< "        Note: parameter of \"sphere\" is wrong!!" << std::endl;
			exit(0);
		}
	}
	else if (pt.get_child_optional("QuasiPlate"))
	{
		pt2 = &(pt.get_child("QuasiPlate"));
		st = vedo::QuasiPlate;
		sa.quasiplate.width  = pt2->get<vedo::vedo_float_t>("<xmlattr>.Width" , 0.0);
		sa.quasiplate.length = pt2->get<vedo::vedo_float_t>("<xmlattr>.Length", 0.0);
		sa.quasiplate.height = pt2->get<vedo::vedo_float_t>("<xmlattr>.Height", 0.0);
		if ((sa.quasiplate.width  <= 0.0) || (sa.quasiplate.length <= 0.0) || (sa.quasiplate.height <= 0.0))
		{
			std::cout
				<< "Error!! Code: DOWorld_ReadXML.cpp: std::pair<vedo::DOShapeType, vedo::DOShapeAttributes> Node2DOShapeTypeAndAttributes(boost::property_tree::ptree*)" << std::endl
				<< "        Note: parameter of \"QuasiPlate\" is wrong!!" << std::endl;
			exit(0);
		}
	}
	else if(pt.get_child_optional("QuasiPlateWithCircularHole"))
	{
		pt2 = &(pt.get_child("QuasiPlateWithCircularHole"));
		st = vedo::QuasiPlateWithCircularHole;
		sa.quasiplatewithcircularhole.width       = pt2->get<vedo::vedo_float_t>("<xmlattr>.Width"      , 0.0);
		sa.quasiplatewithcircularhole.length      = pt2->get<vedo::vedo_float_t>("<xmlattr>.Length"     , 0.0);
		sa.quasiplatewithcircularhole.height      = pt2->get<vedo::vedo_float_t>("<xmlattr>.Height"     , 0.0);
		sa.quasiplatewithcircularhole.holeradius  = pt2->get<vedo::vedo_float_t>("<xmlattr>.HoleRadius" , 0.0);
		sa.quasiplatewithcircularhole.holexoffset = pt2->get<vedo::vedo_float_t>("<xmlattr>.HoleXOffset", 0.0);
		sa.quasiplatewithcircularhole.holeyoffset = pt2->get<vedo::vedo_float_t>("<xmlattr>.HoleYOffset", 0.0);
		if (   (sa.quasiplatewithcircularhole.width      <= 0.0)
			|| (sa.quasiplatewithcircularhole.length     <= 0.0)
			|| (sa.quasiplatewithcircularhole.height     <= 0.0)
			|| (sa.quasiplatewithcircularhole.holeradius <= 0.0))
		{
			std::cout
				<< "Error!! Code: DOWorld_ReadXML.cpp: std::pair<vedo::DOShapeType, vedo::DOShapeAttributes> Node2DOShapeTypeAndAttributes(boost::property_tree::ptree*)" << std::endl
				<< "        Note: parameter of \"QuasiPlateWithCircularHole\" is wrong!!" << std::endl;
			exit(0);
		}
	}
	else if (pt.get_child_optional("QuasiCylinder"))
	{
		pt2 = &(pt.get_child("QuasiCylinder"));
		st = vedo::QuasiCylinder;
		sa.quasicylinder.radius = pt2->get<vedo::vedo_float_t>("<xmlattr>.Radius", 0.0);
		sa.quasicylinder.height = pt2->get<vedo::vedo_float_t>("<xmlattr>.Height", 0.0);
		if ((sa.quasicylinder.radius <= 0.0) || (sa.quasicylinder.height <= 0.0))
		{
			std::cout
				<< "Error!! Code: DOWorld_ReadXML.cpp: std::pair<vedo::DOShapeType, vedo::DOShapeAttributes> Node2DOShapeTypeAndAttributes(boost::property_tree::ptree*)" << std::endl
				<< "        Note: parameter of \"QuasiCylinder\" is wrong!!" << std::endl;
			exit(0);
		}
	}
	else if (pt.get_child_optional("Ellipsoid"))
	{
		pt2 = &(pt.get_child("Ellipsoid"));
		st = vedo::Ellipsoid;
		sa.ellipsoid.xlength = pt2->get<vedo::vedo_float_t>("<xmlattr>.XLength", 0.0);
		sa.ellipsoid.ylength = pt2->get<vedo::vedo_float_t>("<xmlattr>.YLength", 0.0);
		sa.ellipsoid.zlength = pt2->get<vedo::vedo_float_t>("<xmlattr>.ZLength", 0.0);
		if ((sa.ellipsoid.xlength <= 0.0) || (sa.ellipsoid.ylength <= 0.0) || (sa.ellipsoid.zlength <= 0.0))
		{
			std::cout
				<< "Error!! Code: DOWorld_ReadXML.cpp: std::pair<vedo::DOShapeType, vedo::DOShapeAttributes> Node2DOShapeTypeAndAttributes(boost::property_tree::ptree*)" << std::endl
				<< "        Note: parameter of \"Ellipsoid\" is wrong!!" << std::endl;
			exit(0);
		}
	}
	else if (pt.get_child_optional("Polyhedra"))
	{
		pt2 = &(pt.get_child("Polyhedra"));
		st = vedo::Polyhedra;
	}
	else if(pt.get_child_optional("DMSphere"))
	{
		pt2 = &(pt.get_child("DMSphere"));
		st = vedo::DMSphere;
	}
	else if(pt.get_child_optional("polyhedrabrep"))
	{
		pt2 = &(pt.get_child("polyhedrabrep"));
		st = vedo::PolyhedraBRep;
	}
	else
	{
		st = vedo::NoType;
	}

	return std::make_pair(st, sa);
}

static vedo::SystemParameter* Node2SystemParameter(boost::property_tree::ptree& pt)
{
	vedo::Constants* vedo_cp = vedo::Constants::Instance();

	std::string sName;
	boost::property_tree::ptree* pt2;
	vedo::vedo_float_t dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent;
	njr::Vector3d vFieldAcceleration, vLowerBoundaryZOI, vUpperBoundaryZOI, vLowerBoundaryPBC, vUpperBoundaryPBC;

	if (pt.get_child_optional("SimConstant"))
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("SimConstant"))
		{
			sName = v.second.get<std::string>("<xmlattr>.Name");
			if (sName == "SafetyFactor")
			{
				vedo_cp->SetSafetyFactor
					((vedo::vedo_float_t)(std::max(1.0, (double)(v.second.get<vedo::vedo_float_t>("<xmlattr>.Value")))));
			}
			else if (sName == "NumUDDDOStatus")
			{
				// Aries: We have not finished it.
				//vedo::uNumUDDDOStatus
				//	= v.second.get<vedo::vedo_float_t>("<xmlattr>.Value");
			}
			else if (sName == "NumUDDIactStatus")
			{
				// Aries: We have not finished it.
				//vedo::uNumUDDImpactStatus
				//	= v.second.get<vedo::vedo_float_t>("<xmlattr>.Value");
			}
			else if (sName == "MaxIDofDOStatus")
			{
				// Aries: We have not finished it.
			}
			else
			{
				#ifdef _VEDO_DEBUG
					std::cout << "SimConstant \"" << sName << "\"in <SimConstant> not used!!" << std::endl;
				#endif   // _VEDO_DEBUG
			}
		}
	}

	vFieldAcceleration = Node2Vector3d(pt.get_child("FieldAcceleration"));

	pt2 = &(pt.get_child("TimeControl.<xmlattr>"));
	dTimeStart    = pt2->get<vedo::vedo_float_t>("Start"   );
	dTimeStop     = pt2->get<vedo::vedo_float_t>("Stop"    );
	dTimeInterval = pt2->get<vedo::vedo_float_t>("Interval");
	dTimeCurrent  = pt2->get<vedo::vedo_float_t>("Current" );

	if (pt.get_child_optional("ZOI"))
	{
		pt2 = &(pt.get_child("ZOI.<xmlattr>"));
		vedo::vedo_float_t dBV[3];
		dBV[0] = pt2->get<vedo::vedo_float_t>("XMin", 0.0);
		dBV[1] = pt2->get<vedo::vedo_float_t>("YMin", 0.0);
		dBV[2] = pt2->get<vedo::vedo_float_t>("ZMin", 0.0);
		vLowerBoundaryZOI.Set(dBV[0], dBV[1], dBV[2]);
		dBV[0] = pt2->get<vedo::vedo_float_t>("XMax", 0.0);
		dBV[1] = pt2->get<vedo::vedo_float_t>("YMax", 0.0);
		dBV[2] = pt2->get<vedo::vedo_float_t>("ZMax", 0.0);
		vUpperBoundaryZOI.Set(dBV[0], dBV[1], dBV[2]);
	}
	else
	{
        vLowerBoundaryZOI.Set(0.0, 0.0, 0.0);
        vUpperBoundaryZOI.Set(0.0, 0.0, 0.0);
	}

	if (pt.get_child_optional("PBC"))
	{
		pt2 = &(pt.get_child("PBC.<xmlattr>"));
		vedo::vedo_float_t dBV[3];
		dBV[0] = pt2->get<vedo::vedo_float_t>("XMin", 0.0);
		dBV[1] = pt2->get<vedo::vedo_float_t>("YMin", 0.0);
		dBV[2] = pt2->get<vedo::vedo_float_t>("ZMin", 0.0);
		vLowerBoundaryPBC.Set(dBV[0], dBV[1], dBV[2]);
		dBV[0] = pt2->get<vedo::vedo_float_t>("XMax", 0.0);
		dBV[1] = pt2->get<vedo::vedo_float_t>("YMax", 0.0);
		dBV[2] = pt2->get<vedo::vedo_float_t>("ZMax", 0.0);
		vUpperBoundaryPBC.Set(dBV[0], dBV[1], dBV[2]);
	}
	else
	{
        vLowerBoundaryPBC.Set(0.0, 0.0, 0.0);
        vUpperBoundaryPBC.Set(0.0, 0.0, 0.0);
	}

	return
		new vedo::SystemParameter
			(dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent,
			 vFieldAcceleration,
			 vedo::Boundary(vLowerBoundaryZOI, vUpperBoundaryZOI),
			 vedo::Boundary(vLowerBoundaryPBC, vUpperBoundaryPBC) );
}

vedo::DOBehaviorType String2DOBehaviorType(const std::string& s)
{
	if (s == "constrained")
	{
		return vedo::constrained;
	}
	else if (s == "fixed")
	{
		return vedo::fixed;
	}
	else if (s == "mobile")
	{
		return vedo::mobile;
	}
	else if (s == "orbital")
	{
		return vedo::orbital;
	}
	else
	{
		std::cout
			<< "Error!! Code: DOWorld_ReadXML.cpp: vedo::DOBehaviorType String2DOBehavior(const std::string&)" << std::endl
			<< "        Note: Type of \"Behavior\" is illegal!!" << std::endl;
		exit(0);
	}
}

vedo::DOShapeColor String2DOShapeColor(const std::string& s)
{
	if (s =="red")
	{
		return njrdxf::red;
	}
	else if (s == "yellow")
	{
		return njrdxf::yellow;
	}
	else if (s == "green")
	{
		return njrdxf::green;
	}
	else if (s == "cyan")
	{
		return njrdxf::cyan;
	}
	else if (s == "blue")
	{
		return njrdxf::blue;
	}
	else if (s == "magenta")
	{
		return njrdxf::magenta;
	}
	else if (s == "bylayer")
	{
		return njrdxf::bylayer;
	}
	else
	{
		return njrdxf::bylayer;
	}
}

vedo::DOScopeType String2ScopeType(const std::string& s, const vedo::DOBehaviorType& b)
{
	if (s == "local")
	{
		return vedo::local;
	}
	else if (s == "global")
	{
		return vedo::global;
	}
	else if (s == "undefined")
	{
		if(b == vedo::mobile)
		{
			return vedo::local;
		}
		else
		{
			return vedo::global;
		}
	}
	else
	{
		std::cout
			<< "Error!! Code: DOWorld_ReadXML.cpp: vedo::DOScopeType String2ScopeType(const std::string&, vedo::DOBehaviorType&)" << std::endl
			<< "        Note: Type of \"Scope\" is illegal!!" << std::endl;
		exit(0);
	}
}

static vedo::DOModel* Node2DOModel(boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree* pt2 = &pt.get_child("<xmlattr>");

	std::string          sDOName        = pt2->get<std::string>("DOName");
	std::string          sDOGroup       = pt2->get<std::string>("DOGroup");
	vedo::vedo_float_t   dDensity       = pt2->get<vedo::vedo_float_t>("Density");
	vedo::vedo_float_t   dDensityFactor = pt2->get<vedo::vedo_float_t>("DensityFactor", 1.0);
	vedo::DOBehaviorType eBehavior      = String2DOBehaviorType(pt2->get<std::string>("Behavior"));
	vedo::DOShapeColor   cColor         = String2DOShapeColor(pt2->get<std::string>("Color", "bylayer"));
	vedo::DOScopeType    eScope         = String2ScopeType(pt2->get<std::string>("Scope", "undefined"), eBehavior);

	njr::Vector3d vExternalForce;
	if (pt.get_child_optional("ExternalForce"))
	{
		vExternalForce = Node2Vector3d(pt.get_child("ExternalForce"));
	}

	vedo::DOShapeType       st;
	vedo::DOShapeAttributes sa;
	if (pt.get_child_optional("Shape"))
	{
		std::pair<vedo::DOShapeType, vedo::DOShapeAttributes> psa = Node2DOShapeTypeAndAttributes(pt.get_child("Shape"));
		st = psa.first;
		sa = psa.second;
	}
	else
	{
		std::cout
			<< "Error!! Code: DOWorld_ReadXML.cpp: void Node2DOModel(boost::property_tree::ptree*)" << std::endl
			<< "        Note: Cannot find tag \"Shape\" in DOModel!!" << std::endl;
		exit(0);
	}

	std::vector<vedo::DOMaterialAttribute> cMatOpt(0);
	if (pt.get_child_optional("MaterialOption"))
	{
		vedo::DOMaterialAttribute DOMatOpt;
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("MaterialOption"))
		{
			DOMatOpt.Name  = v.second.get<std::string>("<xmlattr>.Name");
			DOMatOpt.Value = v.second.get<vedo::vedo_float_t>("<xmlattr>.Value");
			cMatOpt.push_back(DOMatOpt);
		}
	}

	if(st == vedo::Polyhedra)
	{
		njr::NJRpolyhedra polyhedra;
		if (pt.get_child_optional("Shape.Polyhedra"))
		{
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("Shape.Polyhedra"))
			{
				polyhedra.AddConstrain(Node2HalfSpace(v.second));
			}
		}
		return new vedo::DOModel
			(sDOName, sDOGroup, eBehavior, eScope, dDensity, dDensityFactor, vExternalForce, polyhedra, cColor, cMatOpt);
	}

	return new vedo::DOModel
		(sDOName, sDOGroup, eBehavior, eScope, dDensity, dDensityFactor, vExternalForce, st, sa, cColor, cMatOpt);
}

static vedo::IactModel* Node2IactModel(boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree* pt2 = &pt.get_child("<xmlattr>");

	std::string sMasterGroup  = pt2->get<std::string>("MasterGroup");
	std::string sSlaveGroup   = pt2->get<std::string>("SlaveGroup");
	std::string sEquationType = pt2->get<std::string>("EquationType");

	std::vector<vedo::IactMechanism> svIactMechanisms(0);
	vedo::IactMechanism              iactmechanism;

	bool bFirstData = true;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt)
	{
		if(bFirstData)
		{
			bFirstData = false;
			// First sub-node is "<xmlattr>", so skip it.
		}
		else
		{
			iactmechanism.Name  = v.second.get<std::string>("<xmlattr>.Name");
			iactmechanism.Value = v.second.get<vedo::vedo_float_t>("<xmlattr>.Value");
			svIactMechanisms.push_back(iactmechanism);
		}
	}

	return new vedo::IactModel(sMasterGroup, sSlaveGroup, sEquationType, svIactMechanisms);
}

static vedo::DOStatus* Node2DOStatus(boost::property_tree::ptree& pt)
{
	std::string sDOName = pt.get<std::string>("<xmlattr>.DOName");
	njr::Vector3d vPosition        = Node2Vector3d(pt.get_child("Position"       ));
	njr::Vector3d vVelocity        = Node2Vector3d(pt.get_child("Velocity"       ));
	njr::Vector3d vOrientationX    = Node2Vector3d(pt.get_child("OrientationX"   ));
	njr::Vector3d vOrientationZ    = Node2Vector3d(pt.get_child("OrientationZ"   ));
	njr::Vector3d vAngularVelocity = Node2Vector3d(pt.get_child("AngularVelocity"));

	njr::Vector3d vImpact;
	if (pt.get_child_optional("Impact"))
	{
		vImpact = Node2Vector3d(pt.get_child("Impact"));
	}

	njr::Vector3d vAngularImpact;
	if (pt.get_child_optional("AngularImpact"))
	{
		vAngularImpact = Node2Vector3d(pt.get_child("AngularImpact"));
	}

	return new vedo::DOStatus
		(sDOName, vPosition, vVelocity, vOrientationX, vOrientationZ, vAngularVelocity, vImpact, vAngularImpact);
}

static std::pair<std::pair<vedo::vedo_uint_t, vedo::vedo_uint_t>, vedo::ImpactStatus*>
	Node2IactStatus(boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree* pt2 = &(pt.get_child("<xmlattr>"));

	vedo::vedo_uint_t ulMaster = pt2->get<vedo::vedo_uint_t>("MasterDOStatusSN");
	vedo::vedo_uint_t ulSlave  = pt2->get<vedo::vedo_uint_t>("SlaveDOStatusSN" );

	vedo::vedo_uint_t uContact = pt2->get<vedo::vedo_uint_t>("Contact");
	bool bContact(true);
	if (uContact == 0)
	{
		bContact = false;
	}

	vedo::vedo_uint_t uBond = pt2->get<vedo::vedo_uint_t>("Bond");
	bool bBond(true);
	if (uBond == 0)
	{
		bBond = false;
	}

	vedo::vedo_float_t dNormalStiffness       = pt2->get<vedo::vedo_float_t>("NormalStiffness");
	vedo::vedo_float_t dInitialVelocity       = pt2->get<vedo::vedo_float_t>("InitialVelocity");
	vedo::vedo_float_t dOverlap               = pt2->get<vedo::vedo_float_t>("Overlap"        );

	njr::Vector3d      vShearForce            = Node2Vector3d(pt.get_child("ShearForce"           ));
	njr::Vector3d      vImpactPoint           = Node2Vector3d(pt.get_child("ImpactPoint"          ));
	njr::Vector3d      vImpactDirection       = Node2Vector3d(pt.get_child("ImpactDirection"      ));
	njr::Vector3d      vImpactToMaster        = Node2Vector3d(pt.get_child("ImpactToMaster"       ));
	njr::Vector3d      vAngularImpactToMaster = Node2Vector3d(pt.get_child("AngularImpactToMaster"));

	if (vedo::uNumUDDImpactStatus != 0)
	{
		vedo::vedo_float_t* dpudv = new vedo::vedo_float_t[4*vedo::uNumUDDImpactStatus];
		for (vedo::vedo_uint_t u=vedo::uNumUDDImpactStatus; u<3*vedo::uNumUDDImpactStatus; u++)
		{
			*(dpudv+u) = 0.0;
		}

		if (pt.get_child_optional("AccumulativeUserDefinedValue"))
		{
			vedo::vedo_uint_t uLocation = 0;
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("AccumulativeUserDefinedValue"))
			{
				*(dpudv+uLocation) = v.second.get<vedo::vedo_float_t>("<xmlattr>.Value");
				uLocation++;
			}
		}

		if(pt.get_child_optional("UserDefinedValue"))
		{
			dpudv += 2*vedo::uNumUDDImpactStatus;
			vedo::vedo_uint_t uLocation = 0;
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("UserDefinedValue"))
			{
				*(dpudv+uLocation) = v.second.get<vedo::vedo_float_t>("<xmlattr>.Value");
				uLocation++;
			}
		}

		return
			std::make_pair
				(std::make_pair(ulMaster, ulSlave),
				 new vedo::ImpactStatus
					(bContact, bBond, dNormalStiffness, dInitialVelocity,
                     vShearForce, vImpactPoint, vImpactDirection,
                     vImpactToMaster, vAngularImpactToMaster, dOverlap, dpudv));
	}
	else
	{
		return
			std::make_pair
				(std::make_pair(ulMaster, ulSlave),
				 new vedo::ImpactStatus
					(bContact, bBond, dNormalStiffness, dInitialVelocity,
                     vShearForce, vImpactPoint, vImpactDirection,
                     vImpactToMaster, vAngularImpactToMaster, dOverlap   ));
	}
}



namespace vedo
{

bool DOWorld::ReadXML(const std::string xmlFile, IactRecordTab* irtp)
{
	DOWorld::Clear();

	boost::property_tree::ptree bPTree;
	boost::property_tree::xml_parser::read_xml(xmlFile, bPTree);
	boost::property_tree::ptree* pt = &bPTree.get_child("DOWorld");

	pSystemParameter = Node2SystemParameter(pt->get_child("SimParameter"));

	if (pt->get_child_optional("DOModelTab"))
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt->get_child("DOModelTab"))
		{
			cDOModel.push_back(Node2DOModel(v.second));
		}
	}
	else
	{
		std::cout
			<< "Error!! Code: bool DOWorld::ReadXML(const char*, IactRecordTab*)" << std::endl
			<< "        Note: Cannot find tag \"DOModelTab\"!!"                   << std::endl;
		exit(0);
	}

	if(pt->get_child_optional("IactModelTab"))
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt->get_child("IactModelTab"))
		{
			cIactModel.push_back(Node2IactModel(v.second));
		}
	}
	else
	{
		std::cout
			<< "Error!! Code: bool DOWorld::ReadXML(const char*, IactRecordTab*)" << std::endl
			<< "        Note: Cannot find tag \"IactModelTab\"!!"                 << std::endl;
		exit(0);
	}

	if(pt->get_child_optional("DOStatusTab"))
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt->get_child("DOStatusTab"))
		{
			cDOStatus.push_back(Node2DOStatus(v.second));
		}
		pSystemParameter->SetDONumber(cDOStatus.size());
	}
	else
	{
		std::cout
			<< "Error!! Code: bool DOWorld::ReadXML(const char*, IactRecordTab*)" << std::endl
			<< "        Note: Cannot find tag \"DOStatusTab\"!!"                  << std::endl;
		exit(0);
	}

	if (irtp != 0)
	{
		std::pair<std::pair<vedo::vedo_uint_t, vedo::vedo_uint_t>, vedo::ImpactStatus*> ululis;
		if (pt->get_child_optional("IactStatusTab"))
		{
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt->get_child("IactStatusTab"))
			{
				ululis = Node2IactStatus(v.second);
				irtp->PushRecord(ululis.first.first, ululis.first.second, *(ululis.second));
			}
			vedo::vedo_uint_t ulSize = irtp->GetTabSize();
			pSystemParameter->SetIactNumber(ulSize);
		}
	}

	return DOWorld::Check();
}

bool DOWorld::ReadXML(const std::string xmlFile)
{
	DOWorld::Clear();

	boost::property_tree::ptree bPTree;
	boost::property_tree::xml_parser::read_xml(xmlFile, bPTree);
	boost::property_tree::ptree* pt = &bPTree.get_child("DOWorld");

	pSystemParameter = Node2SystemParameter(pt->get_child("SimParameter"));

	if (pt->get_child_optional("DOModelTab"))
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt->get_child("DOModelTab"))
		{
			cDOModel.push_back(Node2DOModel(v.second));
		}
	}
	else
	{
		std::cout
			<< "Error!! Code: bool DOWorld::ReadXML(const char*, IactRecordTab*)" << std::endl
			<< "        Note: Cannot find tag \"DOModelTab\"!!"                   << std::endl;
		exit(0);
	}

	if(pt->get_child_optional("IactModelTab"))
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt->get_child("IactModelTab"))
		{
			cIactModel.push_back(Node2IactModel(v.second));
		}
	}
	else
	{
		std::cout
			<< "Error!! Code: bool DOWorld::ReadXML(const char*, IactRecordTab*)" << std::endl
			<< "        Note: Cannot find tag \"IactModelTab\"!!"                 << std::endl;
		exit(0);
	}

	if(pt->get_child_optional("DOStatusTab"))
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt->get_child("DOStatusTab"))
		{
			cDOStatus.push_back(Node2DOStatus(v.second));
		}
		pSystemParameter->SetDONumber(cDOStatus.size());
	}
	else
	{
		std::cout
			<< "Error!! Code: bool DOWorld::ReadXML(const char*, IactRecordTab*)" << std::endl
			<< "        Note: Cannot find tag \"DOStatusTab\"!!"                  << std::endl;
		exit(0);
	}

	return DOWorld::Check();
}

}   // namespace vedo
