#include <vedo/framework/Boundary.h>
#include <vedo/framework/DOWorld.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>

namespace vedo
{

void DOWorld::WriteVPF(const std::string filename) const
{
    _float_t Radius, Height, Width, Length, HoleRadius, HoleXOffset, HoleYOffset;
    const DOModel* pdoml;
	std::vector<DOStatus *>::const_iterator idos;
	njr::NJRpolyhedra polyhedra;
	std::vector<njr::NJRpolygon> faces;
	aries::Vector3df Vt;

	std::ofstream ofVPF(filename.c_str(), std::ios::out);
	if (!ofVPF)
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteVPF(const char*)" << std::endl
			<< "        Note: DOWorld cannot write vpf file" << std::endl;
		exit(-1);
	}

	ofVPF
		<< "<Header>"                                  << std::endl
		<< '\t' << pSystemParameter->GetTimeStart()    << " "
		        << pSystemParameter->GetTimeStop()     << " "
		        << pSystemParameter->GetTimeInterval() << " "
		        << pSystemParameter->GetTimeCurrent()  << std::endl
		<< "</Header>"                                 << std::endl
		<< "<DiscreteObject>"                          << std::endl;

/*
	Boundary ZoneOfInterest = DOWorld::GetSystemParameter()->GetZoneOfInterest();
	if(ZoneOfInterest.Active())
	{
		aries::Vector3df	vLowerPoint = ZoneOfInterest.GetLowerPoint();
		aries::Vector3df	vUpperPoint = ZoneOfInterest.GetUpperPoint();
		aries::Vector3df	vCenter     = 0.5 * (vUpperPoint + vLowerPoint);
		dX                      = vUpperPoint.x() - vLowerPoint.x();
		dY                      = vUpperPoint.y() - vLowerPoint.y();
		dZ                      = vUpperPoint.z() - vLowerPoint.z();
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "ZoneOfInterest",
			 vCenter.x() + 0.5*dX, vCenter.y(), vCenter.z(),
			 0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   dZ, dY, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "ZoneOfInterest",
			 vCenter.x() - 0.5*dX, vCenter.y(), vCenter.z(),
			 0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   dZ, dY, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "ZoneOfInterest",
			 vCenter.x(), vCenter.y() + 0.5*dY, vCenter.z(),
			 1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   dZ, dX, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "ZoneOfInterest",
			 vCenter.x(), vCenter.y() - 0.5*dY, vCenter.z(),
			 1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   dZ, dX, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "ZoneOfInterest",
			 vCenter.x(), vCenter.y(), vCenter.z() + 0.5*dZ,
			 1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   dY, dX, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "ZoneOfInterest",
			 vCenter.x(), vCenter.y(), vCenter.z() - 0.5*dZ,
			 1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   dY, dX, thickness       );
	}

	Boundary PeriodicBoundaryConditions
		= DOWorld::GetSystemParameter()->GetPeriodicBoundaryConditions();

	if(PeriodicBoundaryConditions.Active())
	{
		aries::Vector3df	vLowerPoint = PeriodicBoundaryConditions.GetLowerPoint();
		aries::Vector3df	vUpperPoint = PeriodicBoundaryConditions.GetUpperPoint();
		aries::Vector3df	vCenter     = 0.5 * (vUpperPoint + vLowerPoint);
		dX                      = vUpperPoint.x() - vLowerPoint.x();
		dY                      = vUpperPoint.y() - vLowerPoint.y();
		dZ                      = vUpperPoint.z() - vLowerPoint.z();
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "PeriodicBoundaryConditions",
			 vCenter.x() + 0.5*dX, vCenter.y(), vCenter.z(),
			 0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   dZ, dY, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "PeriodicBoundaryConditions",
			 vCenter.x() - 0.5*dX, vCenter.y(), vCenter.z(),
			 0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   dZ, dY, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "PeriodicBoundaryConditions",
			 vCenter.x(), vCenter.y() + 0.5*dY, vCenter.z(),
			 1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   dZ, dX, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "PeriodicBoundaryConditions",
			 vCenter.x(), vCenter.y() - 0.5*dY, vCenter.z(),
			 1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   dZ, dX, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "PeriodicBoundaryConditions",
			 vCenter.x(), vCenter.y(), vCenter.z() + 0.5*dZ,
			 1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   dY, dX, thickness       );
		fprintf
			(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
			 "plate", "PeriodicBoundaryConditions",
			 vCenter.x(), vCenter.y(), vCenter.z() - 0.5*dZ,
			 1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   dY, dX, thickness       );
	}
*/

	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		switch (GetDOModel((*idos)->GetDOName())->GetShapeType())
		{
			case Sphere:
				ofVPF
					<< "\tsphere "
					<< (*idos)->GetDOName()              << " "
					<< (*idos)->GetPosition().x()        << " "
					<< (*idos)->GetPosition().y()        << " "
					<< (*idos)->GetPosition().z()        << " "
					<< (*idos)->GetVelocity().x()        << " "
					<< (*idos)->GetVelocity().y()        << " "
					<< (*idos)->GetVelocity().z()        << " "
                    << (*idos)->GetAngularVelocity().x() << " "
                    << (*idos)->GetAngularVelocity().y() << " "
                    << (*idos)->GetAngularVelocity().z() << " "
					<< DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().sphere.radius << std::endl;
				break;
			case Polyhedra:
				pdoml     = DOWorld::GetDOModel((*idos)->GetDOName());
				polyhedra = pdoml->GetPolyhedra();
				faces     = polyhedra.faces();
				ofVPF
					<< "\t<polyhedra>"                           << std::endl
					<< '\t' << (*idos)->GetDOName()              << " "
					        << faces.size()                      << " "
					        << (*idos)->GetPosition().x()        << " "
					        << (*idos)->GetPosition().y()        << " "
					        << (*idos)->GetPosition().z()        << " "
					        << (*idos)->GetOrientationX().x()    << " "
					        << (*idos)->GetOrientationX().y()    << " "
					        << (*idos)->GetOrientationX().z()    << " "
					        << (*idos)->GetOrientationZ().x()    << " "
					        << (*idos)->GetOrientationZ().y()    << " "
					        << (*idos)->GetOrientationZ().z()    << " "
					        << (*idos)->GetVelocity().x()        << " "
					        << (*idos)->GetVelocity().y()        << " "
					        << (*idos)->GetVelocity().z()        << " "
					        << (*idos)->GetAngularVelocity().x() << " "
					        << (*idos)->GetAngularVelocity().y() << " "
					        << (*idos)->GetAngularVelocity().z() << std::endl;

				for	(_uint_t i=0; i<polyhedra.constrains().size();	++i)
				{
					ofVPF
						<< "\t\t<constrain>"                 << std::endl
						<< (*idos)->GetDOName()              << " "
						<< i                                 << " "
						<< faces[i].vertexes().size()        << " "
						<< polyhedra.constrains()[i].a()     << " "
						<< polyhedra.constrains()[i].b()     << " "
						<< polyhedra.constrains()[i].c()     << " "
						<< polyhedra.constrains()[i].sense() << " "
						<< polyhedra.constrains()[i].d()     << std::endl
						<< "\t\t</constrain>"                << std::endl;
					for	(_uint_t j=0; j<faces[i].vertexes().size(); ++j)
					{
						ofVPF
							<< "\t\t<vertex>"             << std::endl
							<< (*idos)->GetDOName()       << " "
							<< faces[i].vertexes()[j].x() << " "
							<< faces[i].vertexes()[j].y() << " "
							<< faces[i].vertexes()[j].z() << std::endl
							<< "\t\t</vertex>"            << std::endl;
					}
				}
				ofVPF << "\t</polyhedra>" << std::endl;
				break;
			case QuasiPlate:
				Height = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplate.height;
				Width  = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplate.width;
				Length = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplate.length;
				ofVPF
					<< "\tplate "
					<< (*idos)->GetDOName()           << " "
					<< (*idos)->GetPosition().x()     << " "
					<< (*idos)->GetPosition().y()     << " "
					<< (*idos)->GetPosition().z()     << " "
					<< (*idos)->GetOrientationX().x() << " "
					<< (*idos)->GetOrientationX().y() << " "
					<< (*idos)->GetOrientationX().z() << " "
					<< (*idos)->GetOrientationZ().x() << " "
					<< (*idos)->GetOrientationZ().y() << " "
					<< (*idos)->GetOrientationZ().z() << " "
					<< Length                         << " "
					<< Width                          << " "
					<< Height                         << std::endl;
				break;
			case QuasiPlateWithCircularHole:
				Height      = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.height;
				Width       = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.width;
				Length      = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.length;
				HoleRadius  = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.holeradius;
				HoleXOffset = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.holexoffset;
				HoleYOffset = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.holeyoffset;
				ofVPF
					<< "\tplate "
					<< (*idos)->GetDOName()           << " "
					<< (*idos)->GetPosition().x()     << " "
					<< (*idos)->GetPosition().y()     << " "
					<< (*idos)->GetPosition().z()     << " "
					<< (*idos)->GetOrientationX().x() << " "
					<< (*idos)->GetOrientationX().y() << " "
					<< (*idos)->GetOrientationX().z() << " "
					<< (*idos)->GetOrientationZ().x() << " "
					<< (*idos)->GetOrientationZ().y() << " "
					<< (*idos)->GetOrientationZ().z() << " "
					<< Length                         << " "
					<< Width                          << " "
					<< Height                         << " "
					<< HoleRadius                     << " "
					<< HoleXOffset                    << " "
					<< HoleYOffset                    << std::endl;
				break;
			case QuasiCylinder:
				Height = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasicylinder.height;
				Radius = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasicylinder.radius;
				Vt     = (*idos)->GetPosition() - ((*idos)->GetOrientationZ()*(0.5*Height));
				ofVPF
					<< "\tcylinder "
					<< (*idos)->GetDOName()           << " "
					<< Vt.x()                         << " "
					<< Vt.y()                         << " "
					<< Vt.z()                         << " "
					<< Radius                         << " "
					<< Height                         << " "
					<< (*idos)->GetOrientationZ().x() << " "
					<< (*idos)->GetOrientationZ().y() << " "
					<< (*idos)->GetOrientationZ().z() << std::endl;
				break;
			default:
				std::cerr
					<< "Error!! Code: DOWorld::WriteVPF(const char*)" << std::endl
					<< "        Note: Unknown VPF shape"              << std::endl;
				exit(-1);
		}
	}
	ofVPF << "</DiscreteObject>" << std::endl;
	ofVPF.close();
}

std::vector<_uint_t> GetObjectTypeNumber(const DOWorld* DOW)
{
	std::vector<DOStatus*>                 DOS = DOW->GetDOStatus();
	std::vector<DOStatus*>::const_iterator DOSP;
	std::string LastDOName = (*(DOS.begin()))->GetDOName();
	std::vector<_uint_t> DataStructure;
	_uint_t DataNumber = 0;
	for (DOSP = DOS.begin(); DOSP != DOS.end(); ++DOSP, DataNumber++)
	{
		if ((*DOSP)->GetDOName() != LastDOName)
		{
			DataStructure.push_back(DataNumber);
			LastDOName = (*DOSP)->GetDOName();
			DataNumber = 0;
		}
	}
	DataStructure.push_back(DataNumber);
	return DataStructure;
}

void DOWorld::WriteVPF (const std::string filename, const DOWorld* opw) const
{
	// "opw" is the "DOWorld" status of original system
	std::vector<_uint_t> OriginalDataStructure = GetObjectTypeNumber(opw);
	std::vector<_uint_t> CurrentDataStructure  = GetObjectTypeNumber(this);

    _float_t Radius, Height, Width, Length, HoleRadius, HoleXOffset, HoleYOffset;
    const DOModel* pdoml;
	std::vector<DOStatus *>::const_iterator  idos;
	std::vector<DOStatus *> ocDOStatus = opw->GetDOStatus();
	std::vector<DOStatus *>::const_iterator oidos;
	njr::NJRpolyhedra polyhedra;
	std::vector<njr::NJRpolygon> faces;
	aries::Vector3df Vt;

	std::ofstream ofVPF(filename.c_str(), std::ios::out);
	if (!ofVPF)
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteVPF(const char*, const DOWorld*)" << std::endl
			<< "        Note: DOWorld cannot write vpf file"                  << std::endl;
		exit(-1);
	}

	ofVPF
		<< "<Header>"                                  << std::endl
		<< '\t' << pSystemParameter->GetTimeStart()    << " "
		        << pSystemParameter->GetTimeStop()     << " "
		        << pSystemParameter->GetTimeInterval() << " "
		        << pSystemParameter->GetTimeCurrent()  << std::endl
		<< "</Header>"                                 << std::endl
		<< "<DiscreteObject>"                          << std::endl;

/*
	Boundary ZoneOfInterest = DOWorld::GetSystemParameter()->GetZoneOfInterest();
	aries::Vector3df	vLowerPoint = ZoneOfInterest.GetLowerPoint();
	aries::Vector3df	vUpperPoint = ZoneOfInterest.GetUpperPoint();
	aries::Vector3df	vCenter     = 0.5 * (vUpperPoint + vLowerPoint);
	aries::Vector3df	vRedundant  = vCenter + aries::Vector3df(0.0, dY, 0.0);
	dX                      = vUpperPoint.x() - vLowerPoint.x();
	dY                      = vUpperPoint.y() - vLowerPoint.y();
	dZ                      = vUpperPoint.z() - vLowerPoint.z();
	fprintf
		(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
		"plate", "ZoneOfInterest",
		vCenter.x() + 0.5*dX, vCenter.y(), vCenter.z(),
		0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   dZ, dY, thickness);
	fprintf
		(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
		"plate", "ZoneOfInterest",
		vCenter.x() - 0.5*dX, vCenter.y(), vCenter.z(),
		0.0, 1.0, 0.0,   1.0, 0.0, 0.0,   dZ, dY, thickness);
	fprintf
		(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
		"plate", "ZoneOfInterest",
		vCenter.x(), vCenter.y() + 0.5*dY, vCenter.z(),
		1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   dZ, dX, thickness);
	fprintf
		(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
		"plate", "ZoneOfInterest",
		vCenter.x(), vCenter.y() - 0.5*dY, vCenter.z(),
		1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   dZ, dX, thickness);
	fprintf
		(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
		"plate", "ZoneOfInterest",
		vCenter.x(), vCenter.y(), vCenter.z() + 0.5*dZ,
		1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   dY, dX, thickness);
	fprintf
		(fpvpf,  "%s %s %g %g %g %g   %g %g %g %g %g   %g %g %g\n",
		"plate", "ZoneOfInterest",
		vCenter.x(), vCenter.y(), vCenter.z() - 0.5*dZ,
		1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   dY, dX, thickness);
*/

	_uint_t gap;
	idos  =  cDOStatus.begin();
	oidos = ocDOStatus.begin();
	for (_uint_t uli=0; uli<OriginalDataStructure.size(); uli++)
	{
		gap = OriginalDataStructure[uli] - CurrentDataStructure[uli];
		for (_uint_t ulj=0; ulj<CurrentDataStructure[uli]; ulj++)
		{
			switch (GetDOModel((*idos)->GetDOName())->GetShapeType())
			{
				case Sphere:
					ofVPF
						<< "\tsphere "
						<< (*idos)->GetDOName()              << " "
						<< (*idos)->GetPosition().x()        << " "
						<< (*idos)->GetPosition().y()        << " "
						<< (*idos)->GetPosition().z()        << " "
						<< (*idos)->GetVelocity().x()        << " "
						<< (*idos)->GetVelocity().y()        << " "
						<< (*idos)->GetVelocity().z()        << " "
						<< (*idos)->GetAngularVelocity().x() << " "
						<< (*idos)->GetAngularVelocity().y() << " "
						<< (*idos)->GetAngularVelocity().z() << " "
						<< DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().sphere.radius << std::endl;
					break;
				case Polyhedra:
					pdoml     = DOWorld::GetDOModel((*idos)->GetDOName());
					polyhedra = pdoml->GetPolyhedra();
					faces     = polyhedra.faces();
					ofVPF
						<< "\t<polyhedra>"                           << std::endl
						<< '\t' << (*idos)->GetDOName()              << " "
								<< faces.size()                      << " "
								<< (*idos)->GetPosition().x()        << " "
								<< (*idos)->GetPosition().y()        << " "
								<< (*idos)->GetPosition().z()        << " "
								<< (*idos)->GetOrientationX().x()    << " "
								<< (*idos)->GetOrientationX().y()    << " "
								<< (*idos)->GetOrientationX().z()    << " "
								<< (*idos)->GetOrientationZ().x()    << " "
								<< (*idos)->GetOrientationZ().y()    << " "
								<< (*idos)->GetOrientationZ().z()    << " "
								<< (*idos)->GetVelocity().x()        << " "
								<< (*idos)->GetVelocity().y()        << " "
								<< (*idos)->GetVelocity().z()        << " "
								<< (*idos)->GetAngularVelocity().x() << " "
								<< (*idos)->GetAngularVelocity().y() << " "
								<< (*idos)->GetAngularVelocity().z() << std::endl;

					for	(_uint_t i=0; i<polyhedra.constrains().size();	++i)
					{
						ofVPF
							<< "\t\t<constrain>"                 << std::endl
							<< (*idos)->GetDOName()              << " "
							<< i                                 << " "
							<< faces[i].vertexes().size()        << " "
							<< polyhedra.constrains()[i].a()     << " "
							<< polyhedra.constrains()[i].b()     << " "
							<< polyhedra.constrains()[i].c()     << " "
							<< polyhedra.constrains()[i].sense() << " "
							<< polyhedra.constrains()[i].d()     << std::endl
							<< "\t\t</constrain>"                << std::endl;
						for	(_uint_t j=0; j<faces[i].vertexes().size(); ++j)
						{
							ofVPF
								<< "\t\t<vertex>"             << std::endl
								<< (*idos)->GetDOName()       << " "
								<< faces[i].vertexes()[j].x() << " "
								<< faces[i].vertexes()[j].y() << " "
								<< faces[i].vertexes()[j].z() << std::endl
								<< "\t\t</vertex>"            << std::endl;
						}
					}
					ofVPF << "\t</polyhedra>" << std::endl;
					break;
				case QuasiPlate:
					Height = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplate.height;
					Width  = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplate.width;
					Length = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplate.length;
					ofVPF
						<< "\tplate "
						<< (*idos)->GetDOName()           << " "
						<< (*idos)->GetPosition().x()     << " "
						<< (*idos)->GetPosition().y()     << " "
						<< (*idos)->GetPosition().z()     << " "
						<< (*idos)->GetOrientationX().x() << " "
						<< (*idos)->GetOrientationX().y() << " "
						<< (*idos)->GetOrientationX().z() << " "
						<< (*idos)->GetOrientationZ().x() << " "
						<< (*idos)->GetOrientationZ().y() << " "
						<< (*idos)->GetOrientationZ().z() << " "
						<< Length                         << " "
						<< Width                          << " "
						<< Height                         << std::endl;
					break;
				case QuasiPlateWithCircularHole:
					Height      = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.height;
					Width       = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.width;
					Length      = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.length;
					HoleRadius  = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.holeradius;
					HoleXOffset = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.holexoffset;
					HoleYOffset = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasiplatewithcircularhole.holeyoffset;
					ofVPF
						<< "\tplate "
						<< (*idos)->GetDOName()           << " "
						<< (*idos)->GetPosition().x()     << " "
						<< (*idos)->GetPosition().y()     << " "
						<< (*idos)->GetPosition().z()     << " "
						<< (*idos)->GetOrientationX().x() << " "
						<< (*idos)->GetOrientationX().y() << " "
						<< (*idos)->GetOrientationX().z() << " "
						<< (*idos)->GetOrientationZ().x() << " "
						<< (*idos)->GetOrientationZ().y() << " "
						<< (*idos)->GetOrientationZ().z() << " "
						<< Length                         << " "
						<< Width                          << " "
						<< Height                         << " "
						<< HoleRadius                     << " "
						<< HoleXOffset                    << " "
						<< HoleYOffset                    << std::endl;
                    break;
				case QuasiCylinder:
					Height = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasicylinder.height;
					Radius = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().quasicylinder.radius;
					Vt     = (*idos)->GetPosition() - ((*idos)->GetOrientationZ()*(0.5*Height));
					ofVPF
						<< "\tcylinder "
						<< (*idos)->GetDOName()           << " "
						<< Vt.x()                         << " "
						<< Vt.y()                         << " "
						<< Vt.z()                         << " "
						<< Radius                         << " "
						<< Height                         << " "
						<< (*idos)->GetOrientationZ().x() << " "
						<< (*idos)->GetOrientationZ().y() << " "
						<< (*idos)->GetOrientationZ().z() << std::endl;
					break;
				default:
					std::cerr
						<< "Error!! Code: DOWorld::WriteVPF(const char*, const DOWorld*)" << std::endl
						<< "        Note: Unknown VPF shape"                              << std::endl;
					exit(-1);
			}
			idos++;
			oidos++;
		}
		// Add redundant DiscreteObjects
		aries::Vector3df vRedundant;
		if (gap != 0)
		{
			switch (GetDOModel((*oidos)->GetDOName())->GetShapeType())
			{
				case Sphere:
					for (_uint_t ulj=0; ulj<gap; ulj++)
					{
						ofVPF
							<< "\tsphere " << (*oidos)->GetDOName() << " "
							<< vRedundant.x() << " " << vRedundant.y() << " " << vRedundant.z()
							<< " 0 0 0 0 0 0 0" << std::endl;
						oidos++;
					}
					break;
				case Polyhedra:
					pdoml     = DOWorld::GetDOModel((*oidos)->GetDOName());
					polyhedra = pdoml->GetPolyhedra();
					faces     = polyhedra.faces();
					for (_uint_t ulj=0; ulj<gap; ulj++)
					{
						ofVPF
							<< "\t<polyhedra>" << std::endl
							<< '\t' << (*oidos)->GetDOName() << " " << faces.size() << " "
									<< vRedundant.x() << " " << vRedundant.y() << " " << vRedundant.z()
									<< " 1 0 0 0 0 1 0 0 0 0 0 0" << std::endl;
						for	(_uint_t i=0; i<polyhedra.constrains().size();	++i)
						{
							ofVPF
								<< "\t\t<constrain>"                 << std::endl
								<< (*oidos)->GetDOName()             << " "
								<< i                                 << " "
								<< faces[i].vertexes().size()        << " "
								<< polyhedra.constrains()[i].a()     << " "
								<< polyhedra.constrains()[i].b()     << " "
								<< polyhedra.constrains()[i].c()     << " "
								<< polyhedra.constrains()[i].sense() << " "
								<< polyhedra.constrains()[i].d()     << std::endl
								<< "\t\t</constrain>"                << std::endl;
							for	(_uint_t j=0; j<faces[i].vertexes().size(); ++j)
							{
								ofVPF
									<< "\t\t<vertex>"             << std::endl
									<< (*oidos)->GetDOName()      << " "
									<< faces[i].vertexes()[j].x() << " "
									<< faces[i].vertexes()[j].y() << " "
									<< faces[i].vertexes()[j].z() << std::endl
									<< "\t\t</vertex>"            << std::endl;
							}
						}
						ofVPF << "\t</polyhedra>" << std::endl;
						oidos++;
					}
					break;
				case QuasiPlate:
					for (_uint_t ulj=0; ulj<gap; ulj++)
					{
						ofVPF
							<< "\tplate " << (*oidos)->GetDOName() << " "
							<< vRedundant.x() << " " << vRedundant.y() << " " << vRedundant.z()
							<< " 1 0 0 0 0 1 0.001 0.001 0.001" << std::endl;
						oidos++;
					}
					break;
				case QuasiPlateWithCircularHole:
					for (_uint_t ulj=0; ulj<gap; ulj++)
					{
						ofVPF
							<< "\tplate " << (*oidos)->GetDOName() << " "
							<< vRedundant.x() << " " << vRedundant.y() << " " << vRedundant.z()
							<< " 1 0 0 0 0 1 0.001 0.001 0.001 0 0 0" << std::endl;
						oidos++;
					}
					break;
				case QuasiCylinder:
					for (_uint_t ulj=0; ulj<gap; ulj++)
					{
						ofVPF
							<< "\tcylinder " << (*oidos)->GetDOName() << " "
							<< vRedundant.x() << " " << vRedundant.y() << " " << vRedundant.z()
							<< " 0.001 0.001 0 0 1" << std::endl;
						oidos++;
					}
					break;
				default:
					std::cerr
						<< "Error!! Code: DOWorld::WriteVPF(const char*, const DOWorld*)" << std::endl
						<< "        Note: Unknown VPF shape"                              << std::endl;
					exit(-1);
			}
		}
	}
	ofVPF << "</DiscreteObject>" << std::endl;
	ofVPF.close();
}

}   // namespace vedo
