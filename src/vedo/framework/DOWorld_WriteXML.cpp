#include <njr/HalfSpace.h>
#include <vedo/framework/Boundary.h>
#include <vedo/framework/DOModel.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/IactRecordTab.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

namespace vedo
{

void DOWorld::WriteXMLPrefix(const std::string filename) const
{
	std::ofstream ofXML(filename.c_str(), std::ios::out);

	std::list  <DOModel  *>::const_iterator idoml;
	std::list  <IactModel*>::const_iterator iactml;
	std::vector<DOStatus *>::const_iterator idos;

	if(!ofXML)
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteXML(const char*)" << std::endl
			<< "        Note: DOWorld cannot access XML file" << std::endl;
		exit(-1);
	}

	aries::Constants* aries_cp = aries::Constants::Instance();

	ofXML
		<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl
		<< "<DOWorld xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"DOWorldSchema.xsd\">" << std::endl
		<< "\t<Version>" << aries_cp->Version() << "</Version>" << std::endl
		<< "\t<SimParameter>" << std::endl
		<< "\t\t<SimConstant>" << std::endl
		<< "\t\t\t<Constant Name=\"ContactDetectSafetyFactor\" Value=\"" << aries_cp->SafetyFactor() << "\"/>" << std::endl
		<< "\t\t\t<Constant Name=\"NumUDDDOStatus\" Value=\"" << aries_cp->NumUDDDOStatus() << "\"/>" << std::endl
		<< "\t\t\t<Constant Name=\"uNumUDDImpactStatus\" Value=\"" << aries_cp->NumUDDImpactStatus() << "\"/>" << std::endl
		<< "\t\t\t<Constant Name=\"MaxIDofDOStatus\" Value=\"" << pSystemParameter->GetMaxIDofDO() << "\"/>" << std::endl
		<< "\t\t</SimConstant>" << std::endl
		<< "\t\t<TimeControl Start=\""    << pSystemParameter->GetTimeStart()
		<<               "\" Stop=\""     << pSystemParameter->GetTimeStop()
		<<               "\" Interval=\"" << pSystemParameter->GetTimeInterval()
		<<               "\" Current=\""  << pSystemParameter->GetTimeCurrent() << "\"/>" << std::endl
		<< "\t\t<FieldAcceleration x=\""  << pSystemParameter->GetFieldAcceleration().x()
		<<                     "\" y=\""  << pSystemParameter->GetFieldAcceleration().y()
		<<                     "\" z=\""  << pSystemParameter->GetFieldAcceleration().z() << "\"/>" << std::endl;

	Boundary bc = pSystemParameter->GetZoneOfInterest();
	if (bc.Active())
	{
		ofXML << "\t\t<ZOI";
		if (bc.GetSwitch(0))
		{
			ofXML << " XMin=\"" << bc.GetLowerPoint().x() << "\" XMax=\"" << bc.GetUpperPoint().x() << "\"";
		}

		if (bc.GetSwitch(1))
		{
			ofXML << " YMin=\"" << bc.GetLowerPoint().y() << "\" YMax=\"" << bc.GetUpperPoint().y() << "\"";
		}

		if (bc.GetSwitch(2))
		{
			ofXML << " ZMin=\"" << bc.GetLowerPoint().z() << "\" ZMax=\"" << bc.GetUpperPoint().z() << "\"";
		}

		ofXML << "/>" << std::endl;
	}

	bc = pSystemParameter->GetPeriodicBoundaryConditions();
	if (bc.Active())
	{
		ofXML << "\t\t<PBC";
		if (bc.GetSwitch(0))
		{
			ofXML << " XMin=\"" << bc.GetLowerPoint().x() << "\" XMax=\"" << bc.GetUpperPoint().x() << "\"";
		}

		if (bc.GetSwitch(1))
		{
			ofXML << " YMin=\"" << bc.GetLowerPoint().y() << "\" YMax=\"" << bc.GetUpperPoint().y() << "\"";
		}

		if (bc.GetSwitch(2))
		{
			ofXML << " ZMin=\"" << bc.GetLowerPoint().z() << "\" ZMax=\"" << bc.GetUpperPoint().z() << "\"";
		}

		ofXML << "/>" << std::endl;
	}

	ofXML << "\t</SimParameter>" << std::endl;

	std::string buffer1;
	ofXML << "\t<DOModelTab>" << std::endl;
	for (idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
	{
		ofXML
			<< "\t\t<DOModel DOName=\"" << (*idoml)->GetDOName()
			<< "\" DOGroup=\"" << (*idoml)->GetDOGroup()
			<< "\" Behavior=\"";
		switch ((*idoml)->GetBehavior())
		{
			case constrained:
				ofXML << "constrained";
				break;
			case fixed:
				ofXML << "fixed";
				break;
			case mobile:
				ofXML << "mobile";
				break;
			case orbital:
				ofXML << "orbital";
				break;
			default:
				std::cerr
					<< "Error!! Code: void DOWorld::WriteXML(const char*) const" << std::endl
					<< "        Note: \"Behavior\" in DOModel wrong!!" << std::endl;
				exit(-1);
		}
		ofXML << "\" Scope=\"";
		switch ((*idoml)->GetScope())
		{
			case local:
				ofXML << "local";
				break;
			case global:
				ofXML << "global";
				break;
			default:
				std::cerr
					<< "Error!! Code: void DOWorld::WriteXML(const char*) const" << std::endl
					<< "        Note: \"Scope\" in DOModel wrong!!" << std::endl;
				exit(-1);
		}
		ofXML << "\" Color=\"";

		#ifdef _STD_CPP_11
			switch ((*idoml)->GetShapeColor())
			{
				case njrdxf::Color::bylayer:
					ofXML << "bylayer";
					break;
				case njrdxf::Color::red:
					ofXML << "red";
					break;
				case njrdxf::Color::yellow:
					ofXML << "yellow";
					break;
				case njrdxf::Color::green:
					ofXML << "green";
					break;
				case njrdxf::Color::cyan:
					ofXML << "cyan";
					break;
				case njrdxf::Color::blue:
					ofXML << "blue";
					break;
				case njrdxf::Color::magenta:
					ofXML << "magenta";
					break;
				default:
					ofXML << "bylayer";
			}
		#else
			switch ((*idoml)->GetShapeColor())
			{
				case njrdxf::bylayer:
					ofXML << "bylayer";
					break;
				case njrdxf::red:
					ofXML << "red";
					break;
				case njrdxf::yellow:
					ofXML << "yellow";
					break;
				case njrdxf::green:
					ofXML << "green";
					break;
				case njrdxf::cyan:
					ofXML << "cyan";
					break;
				case njrdxf::blue:
					ofXML << "blue";
					break;
				case njrdxf::magenta:
					ofXML << "magenta";
					break;
				default:
					ofXML << "bylayer";
			}
		#endif

		ofXML
			<< "\" Density=\"" << (*idoml)->GetDensity()
			<< "\" DensityFactor=\"" << (*idoml)->GetDensityFactor() << "\">" << std::endl
			<< "\t\t\t<ExternalForce x=\"" << (*idoml)->GetExternalForce().x()
			<<                   "\" y=\"" << (*idoml)->GetExternalForce().y()
			<<                   "\" z=\"" << (*idoml)->GetExternalForce().z() << "\"/>" << std::endl
			<< "\t\t\t<Shape>";

		switch ((*idoml)->GetShapeType())
		{
			case Sphere:
				ofXML
					<< "<Sphere"
					<< " Radius=\"" << (*idoml)->GetShapeAttributes().sphere.radius << "\""
					<< "/>";
				break;
			case QuasiPlate:
				ofXML
					<< "<QuasiPlate"
					<< " Width=\""  << (*idoml)->GetShapeAttributes().quasiplate.width  << "\""
					<< " Length=\"" << (*idoml)->GetShapeAttributes().quasiplate.length << "\""
					<< " Height=\"" << (*idoml)->GetShapeAttributes().quasiplate.height << "\""
					<< "/>";
				break;
			case QuasiPlateWithCircularHole:
				ofXML
					<< "<QuasiPlateWithCircularHole"
					<< " Width=\""       << (*idoml)->GetShapeAttributes().quasiplatewithcircularhole.width       << "\""
					<< " Length=\""      << (*idoml)->GetShapeAttributes().quasiplatewithcircularhole.length      << "\""
					<< " Height=\""      << (*idoml)->GetShapeAttributes().quasiplatewithcircularhole.height      << "\""
					<< " HoleRadius=\""  << (*idoml)->GetShapeAttributes().quasiplatewithcircularhole.holeradius  << "\""
					<< " HoleXOffset=\"" << (*idoml)->GetShapeAttributes().quasiplatewithcircularhole.holexoffset << "\""
					<< " HoleYOffset=\"" << (*idoml)->GetShapeAttributes().quasiplatewithcircularhole.holeyoffset << "\""
					<< "/>";
				break;
			case QuasiCylinder:
				ofXML
					<< "<QuasiCylinder"
					<< " Radius=\"" << (*idoml)->GetShapeAttributes().quasicylinder.radius << "\""
					<< " Height=\"" << (*idoml)->GetShapeAttributes().quasicylinder.height << "\""
					<< "/>";
				break;
			case Ellipsoid:
				ofXML
					<< "<Ellipsoid"
					<< " XLength=\"" << (*idoml)->GetShapeAttributes().ellipsoid.xlength << "\""
					<< " YLength=\"" << (*idoml)->GetShapeAttributes().ellipsoid.ylength << "\""
					<< " YLength=\"" << (*idoml)->GetShapeAttributes().ellipsoid.zlength << "\""
					<< "/>";
				break;
			case Polyhedra:
				ofXML << std::endl << "\t\t\t\t<Polyhedra>" << std::endl;
				njr::NJRpolyhedra polyhedra = (*idoml)->GetPolyhedra();
				std::vector<njr::HalfSpace> faces = polyhedra.constrains();
				for (_uint_t i=0; i<faces.size(); ++i)
				{
					ofXML
						<< "\t\t\t\t\t<HalfSpace a=\"" << faces[i].a()
						<<           " b=\"" << faces[i].b()
						<<           " c=\"" << faces[i].c()
						<<           " d=\"" << faces[i].d()
						<<           " sense=\"";
					switch (faces[i].sense())
					{
						case njr::Sense::L:
							ofXML << "L";
							break;
						case njr::Sense::G:
							ofXML << "G";
							break;
						case njr::Sense::E:
							ofXML << "E";
							break;
						default:
							std::cerr
								<< "Error!! Code: void DOWorld::WriteXML(const char*) const" << std::endl
								<< "        Note: \"Sense\" of \"Polyhedra\" wrong!!"        << std::endl;
							exit(-1);
					}
					ofXML << "\"/>" << std::endl;
				}
				ofXML << "\t\t\t\t</Polyhedra>" << std::endl << "\t\t\t";
				break;
//			case PolyhedraBRep:
				// This part is still under testing
				//break;
//			default:
//				std::cerr
//					<< "Error!! Code: void DOWorld::WriteXML(const char*) const" << std::endl
//					<< "        Note: \"ShapeType\" wrong!!"                     << std::endl;
//				exit(-1);
		}
		ofXML << "</Shape>" << std::endl;

		const std::vector<DOMaterialAttribute>& cMatOpt = (*idoml)->GetMaterialAttributes();
		if (cMatOpt.size() != 0)
		{
			ofXML << "\t\t\t<MaterialOption>" << std::endl;
			for (_uint_t i=0; i<cMatOpt.size(); i++)
			{
				ofXML
					<< "\t\t\t\t<Option Name=\""  << cMatOpt[i].Name
					<<              "\" Value=\"" << cMatOpt[i].Value << "\"/>" << std::endl;
			}
			ofXML << "\t\t\t</MaterialOption>" << std::endl;
		}

		ofXML << "\t\t</DOModel>" << std::endl;
	}
	ofXML << "\t</DOModelTab>" << std::endl;

	ofXML << "\t<IactModelTab>" << std::endl;
	for (iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
	{
		ofXML
			<< "\t\t<IactModel MasterGroup=\""  << (*iactml)->GetMasterDOGroup()
			<<             "\" SlaveGroup=\""   << (*iactml)->GetSlaveDOGroup()
			<<             "\" EquationType=\"" << (*iactml)->GetEquationType() << "\">" << std::endl;
		const std::vector<IactMechanism>& svIactMechanisms = (*iactml)->GetIactMechanisms();
		for (_uint_t i=0; i<svIactMechanisms.size(); i++)
		{
			ofXML
				<< "\t\t\t<Mechanism Name=\""  << svIactMechanisms[i].Name
				<<               "\" Value=\"" << svIactMechanisms[i].Value << "\"/>" << std::endl;
		}
		ofXML << "\t\t</IactModel>" << std::endl;
	}
	ofXML << "\t</IactModelTab>" << std::endl;

	ofXML << "\t<DOStatusTab>" << std::endl;
	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		ofXML
			<< "\t\t<DOStatus DOName=\"" << (*idos)->GetDOName() << "\">" << std::endl
			<< std::setiosflags(std::ios::scientific) << std::setprecision(6)
			<< "\t\t\t<Position        x=\"" << (*idos)->GetPosition()       .x()
			<<                     "\" y=\"" << (*idos)->GetPosition()       .y()
			<<                     "\" z=\"" << (*idos)->GetPosition()       .z() << "\"/>" << std::endl
			<< "\t\t\t<Velocity        x=\"" << (*idos)->GetVelocity()       .x()
			<<                     "\" y=\"" << (*idos)->GetVelocity()       .y()
			<<                     "\" z=\"" << (*idos)->GetVelocity()       .z() << "\"/>" << std::endl
			<< std::resetiosflags(std::ios::scientific) << std::setiosflags(std::ios::fixed) << std::setprecision(6)
			<< "\t\t\t<OrientationX    x=\"" << (*idos)->GetOrientationX()   .x()
			<<                     "\" y=\"" << (*idos)->GetOrientationX()   .y()
			<<                     "\" z=\"" << (*idos)->GetOrientationX()   .z() << "\"/>" << std::endl
			<< "\t\t\t<OrientationZ    x=\"" << (*idos)->GetOrientationZ()   .x()
			<<                     "\" y=\"" << (*idos)->GetOrientationZ()   .y()
			<<                     "\" z=\"" << (*idos)->GetOrientationZ()   .z() << "\"/>" << std::endl
			<< std::resetiosflags(std::ios::fixed) << std::setiosflags(std::ios::scientific) << std::setprecision(6)
			<< "\t\t\t<AngularVelocity x=\"" << (*idos)->GetAngularVelocity().x()
			<<                     "\" y=\"" << (*idos)->GetAngularVelocity().y()
			<<                     "\" z=\"" << (*idos)->GetAngularVelocity().z() << "\"/>" << std::endl
			<< "\t\t\t<Impact          x=\"" << (*idos)->GetImpact()         .x()
			<<                     "\" y=\"" << (*idos)->GetImpact()         .y()
			<<                     "\" z=\"" << (*idos)->GetImpact()         .z() << "\"/>" << std::endl
			<< "\t\t\t<AngularImpact   x=\"" << (*idos)->GetAngularImpact()  .x()
			<<                     "\" y=\"" << (*idos)->GetAngularImpact()  .y()
			<<                     "\" z=\"" << (*idos)->GetAngularImpact()  .z() << "\"/>" << std::endl
			<< "\t\t</DOStatus>"                                                            << std::endl
			<< std::resetiosflags(std::ios::scientific);
	}
	ofXML << "\t</DOStatusTab>" << std::endl;
	ofXML.close();
}

void DOWorld::WriteXMLPostfix(const std::string filename) const
{
	std::ofstream ofXML(filename.c_str(), std::ios::app);

	if (!ofXML)
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteXML(const char*)" << std::endl
			<< "        Note: DOWorld cannot access XML file" << std::endl;
		exit(-1);
	}

	ofXML << "</DOWorld>" << std::endl;
	ofXML.close();
}

void DOWorld::WriteXMLIactRecordTab(const std::string filename, const IactRecordTab* irtp) const
{
	std::ofstream ofXML(filename.c_str(), std::ios::app);

	if (!ofXML)
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteXML(const char*)" << std::endl
			<< "        Note: DOWorld cannot access XML file" << std::endl;
		exit(-1);
	}

	aries::Constants* aries_cp = aries::Constants::Instance();

    const std::map< std::pair<_uint_t, _uint_t>, ImpactStatus >* mapImStatusP = &(irtp->GetData());
	if (mapImStatusP->size() != 0)
	{
		ofXML << "\t<IactStatusTab>" << std::endl;
		std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::const_iterator mmapImStatus;
		const ImpactStatus* isp;
		aries::Vector3df vShearForce, vImpactPoint, vImpactDirection, vImpactToMaster, vAngularImpactToMaster;
		const _float_t* dpUDV;
		for (mmapImStatus=(mapImStatusP->begin()); mmapImStatus!=(mapImStatusP->end()); mmapImStatus++)
		{
			isp                    = &(mmapImStatus->second);
			vShearForce            = isp->ShearForce();
			vImpactPoint           = isp->ImpactPoint();
			vImpactDirection       = isp->ImpactDirection();
			vImpactToMaster        = isp->ImpactToMaster();
			vAngularImpactToMaster = isp->AngularImpactToMaster();
			ofXML
				<< "\t\t<IactStatus MasterDOStatusSN=\"" << mmapImStatus->first.first
				<<              "\" SlaveDOStatusSN=\""  << mmapImStatus->first.second
				<<              "\" Contact=\""          << isp->Contact()
				<<              "\" Bond=\""             << isp->Bond()                << "\""   << std::endl
				<< std::setiosflags(std::ios::scientific) << std::setprecision(6)
				<< "\t\t            NormalStiffness=\""  << isp->Kn()
				<<              "\" InitialVelocity=\""  << isp->InitialVelocity()
				<<              "\" Overlap=\""          << isp->Overlap()             << "\">"  << std::endl
				<< "\t\t\t<ShearForce            x=\""   << vShearForce.x()
				<<                           "\" y=\""   << vShearForce.y()
				<<                           "\" z=\""   << vShearForce.z()            << "\"/>" << std::endl
				<< "\t\t\t<ImpactPoint           x=\""   << vImpactPoint.x()
				<<                           "\" y=\""   << vImpactPoint.y()
				<<                           "\" z=\""   << vImpactPoint.z()           << "\"/>" << std::endl
				<< std::resetiosflags(std::ios::scientific) << std::setiosflags(std::ios::fixed) << std::setprecision(6)
				<< "\t\t\t<ImpactDirection       x=\""   << vImpactDirection.x()
				<<                           "\" y=\""   << vImpactDirection.y()
				<<                           "\" z=\""   << vImpactDirection.z()       << "\"/>" << std::endl
				<< std::resetiosflags(std::ios::fixed) << std::setiosflags(std::ios::scientific) << std::setprecision(6)
				<< "\t\t\t<ImpactToMaster        x=\""   << vImpactToMaster.x()
				<<                           "\" y=\""   << vImpactToMaster.y()
				<<                           "\" z=\""   << vImpactToMaster.z()        << "\"/>" << std::endl
				<< "\t\t\t<AngularImpactToMaster x=\""   << vAngularImpactToMaster.x()
				<<                           "\" y=\""   << vAngularImpactToMaster.y()
				<<                           "\" z=\""   << vAngularImpactToMaster.z() << "\"/>" << std::endl
				<< std::resetiosflags(std::ios::scientific);

			if (aries_cp->NumUDDImpactStatus() != 0)
			{
				dpUDV = isp->RetrieveAllUserDefinedValue();
				ofXML << "\t\t\t<AccumulativeUserDefinedValue>" << std::endl;
				for (_uint_t u=0; u<(aries_cp->NumUDDImpactStatus()); u++)
				{
					ofXML
						<< "\t\t\t\t<AUDV SN=\"" << u
						<<            "\" Value=\"" << *(dpUDV+u) << "\"/>" << std::endl;
				}
				ofXML << "\t\t\t</AccumulativeUserDefinedValue>" << std::endl;

				ofXML << "\t\t\t<UserDefinedValue>" << std::endl;
				for (_uint_t u=0; u<(aries_cp->NumUDDImpactStatus()); u++)
				{
					ofXML
						<< "\t\t\t\t<UDV SN=\"" << u
						<<           "\" Value=\"" << *(dpUDV+3*(aries_cp->NumUDDImpactStatus())+u) << "\"/>" << std::endl;
				}
				ofXML << "\t\t\t</UserDefinedValue>" << std::endl;
			}

			ofXML << "\t\t</IactStatus>" << std::endl;
		}
		ofXML << "\t</IactStatusTab>" << std::endl;
	}

	ofXML.close();
}

void DOWorld::WriteXML(const std::string filename) const
{
	WriteXMLPrefix(filename);
	WriteXMLPostfix(filename);
}

void DOWorld::WriteXML(const std::string filename, const IactRecordTab* irtp) const
{
	WriteXMLPrefix(filename);
	WriteXMLIactRecordTab(filename, irtp);
	WriteXMLPostfix(filename);
}

}   // namespace vedo
