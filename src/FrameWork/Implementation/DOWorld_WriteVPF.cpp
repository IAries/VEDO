#include <FrameWork/Interfaces/Boundary.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

void DOWorld::WriteVPF (const char* filename) const
{
	FILE *fpvpf;
    double Radius, Height, Width, Length;
    double dX, dY, dZ, thickness = 0.001;
    const DOModel* pdoml;
	vector<DOStatus *>::const_iterator idos;
	NJRpolyhedra polyhedra;
	vector<NJRpolygon> faces;
	NJRvector3d Vt;

	if ((fpvpf = fopen(filename, "w"))== NULL )
	{
		cerr << "DOWorld write vpf failed" << endl;
		exit (0);
	};

	fprintf(fpvpf,"<Header>\n");
	fprintf
		(fpvpf,
		"%g %g %g %g\n",
		pSystemParameter->GetTimeStart(),
		pSystemParameter->GetTimeStop(),
		pSystemParameter->GetTimeInterval(),
		pSystemParameter->GetTimeCurrent());

	fprintf(fpvpf, "</Header>\n");
	fprintf(fpvpf,"<DiscreteObject>\n");

/*
	Boundary ZoneOfInterest = DOWorld::GetSystemParameter()->GetZoneOfInterest();
	if(ZoneOfInterest.Active())
	{
		NJRvector3d	vLowerPoint = ZoneOfInterest.GetLowerPoint();
		NJRvector3d	vUpperPoint = ZoneOfInterest.GetUpperPoint();
		NJRvector3d	vCenter     = 0.5 * (vUpperPoint + vLowerPoint);
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
	};

	Boundary PeriodicBoundaryConditions
		= DOWorld::GetSystemParameter()->GetPeriodicBoundaryConditions();

	if(PeriodicBoundaryConditions.Active())
	{
		NJRvector3d	vLowerPoint = PeriodicBoundaryConditions.GetLowerPoint();
		NJRvector3d	vUpperPoint = PeriodicBoundaryConditions.GetUpperPoint();
		NJRvector3d	vCenter     = 0.5 * (vUpperPoint + vLowerPoint);
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
	};
*/

	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		switch(GetDOModel((*idos)->GetDOName())->GetShapeType())
		{
			case Sphere:
				Radius
					= DOWorld::GetDOModel
						((*idos)->GetDOName())
							->GetShapeAttributes().sphere.radius;
				fprintf
					(fpvpf,
//					"%s %s %g %g %g %g %g %g %g %g %g %g %g %g\n", "sphere",
					"%s %s %g %g %g %g %g %g %g %g %g %g\n", "sphere",
					(*idos)->GetDOName().c_str(),
					(*idos)->GetPosition().x(),
					(*idos)->GetPosition().y(),
					(*idos)->GetPosition().z(),
					(*idos)->GetVelocity().x(),
					(*idos)->GetVelocity().y(),
					(*idos)->GetVelocity().z(),
                    (*idos)->GetAngularVelocity().x(),
                    (*idos)->GetAngularVelocity().y(),
                    (*idos)->GetAngularVelocity().z(),
					Radius);
//					Radius,
//					(*idos)->GetGranularTemperatureV(),
//					(*idos)->GetGranularTemperatureAV());
				break;
			case Polyhedra:
				pdoml     = DOWorld::GetDOModel((*idos)->GetDOName());
				polyhedra = pdoml->GetPolyhedra();
				faces     = polyhedra.faces();
				fprintf
					(fpvpf,
//					"<polyhedra> %s %u %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
					"<polyhedra> %s %u %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
					(*idos)->GetDOName().c_str(),
					faces.size(),
					(*idos)->GetPosition().x(),
					(*idos)->GetPosition().y(),
					(*idos)->GetPosition().z(),
					(*idos)->GetOrientationX().x(),
					(*idos)->GetOrientationX().y(),
					(*idos)->GetOrientationX().z(),
					(*idos)->GetOrientationZ().x(),
					(*idos)->GetOrientationZ().y(),
					(*idos)->GetOrientationZ().z(),
					(*idos)->GetVelocity().x(),
					(*idos)->GetVelocity().y(),
					(*idos)->GetVelocity().z(),
                    (*idos)->GetAngularVelocity().x(),
                    (*idos)->GetAngularVelocity().y(),
                    (*idos)->GetAngularVelocity().z());
//                    (*idos)->GetAngularVelocity().z(),
//					(*idos)->GetGranularTemperatureV(),
//					(*idos)->GetGranularTemperatureAV());

				for	(unsigned int i=0; i<polyhedra.constrains().size();	++i)
				{
					fprintf
						(fpvpf,
						"<constrain> %s %d %d %g %g %g [%d] %g </constrain>\n",
						(*idos)->GetDOName().c_str(),
						i,
						faces[i].vertexes().size(),
						polyhedra.constrains()[i].a(),
						polyhedra.constrains()[i].b(),
						polyhedra.constrains()[i].c(),
						(int) polyhedra.constrains()[i].sense(),
						polyhedra.constrains()[i].d()	        );
					for	(unsigned int j=0; j<faces[i].vertexes().size(); ++j)
					{
						fprintf
							(fpvpf,
							"<vertex> %s %g %g %g </vertex>\n",
							(*idos)->GetDOName().c_str(),
							faces[i].vertexes()[j].x(),
							faces[i].vertexes()[j].y(),
							faces[i].vertexes()[j].z()    );
					}
				}
				fprintf(fpvpf, "</polyhedra>\n");
				break;
			case QuasiPlate:
				Height
					= DOWorld::GetDOModel
						((*idos)->GetDOName())
							->GetShapeAttributes().quasiplate.height;
				Width
					= DOWorld::GetDOModel
						((*idos)->GetDOName())
							->GetShapeAttributes().quasiplate.width;
				Length
					= DOWorld::GetDOModel
						((*idos)->GetDOName())
							->GetShapeAttributes().quasiplate.length;
				fprintf
					(fpvpf,
//					"%s %s %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
					"%s %s %g %g %g %g %g %g %g %g %g %g %g %g\n",
					"plate",
					(*idos)->GetDOName().c_str(),
					(*idos)->GetPosition().x(),
					(*idos)->GetPosition().y(),
					(*idos)->GetPosition().z(),
					(*idos)->GetOrientationX().x(),
					(*idos)->GetOrientationX().y(),
					(*idos)->GetOrientationX().z(),
					(*idos)->GetOrientationZ().x(),
					(*idos)->GetOrientationZ().y(),
					(*idos)->GetOrientationZ().z(),
                    Length,
					Width,
					Height);
//					Height,
//					(*idos)->GetGranularTemperatureV(),
//					(*idos)->GetGranularTemperatureAV());
				break;
			case QuasiCylinder:
				Height
					= DOWorld::GetDOModel
						((*idos)->GetDOName())
							->GetShapeAttributes().quasicylinder.height;
				Radius
					= DOWorld::GetDOModel
						((*idos)->GetDOName())
							->GetShapeAttributes().quasicylinder.radius;

				Vt
					= (*idos)->GetPosition()
					- ((*idos)->GetOrientationZ()*(0.5*Height));

				fprintf
					(fpvpf,
//					"%s %s %g %g %g %g %g %g %g %g %g %g\n",
					"%s %s %g %g %g %g %g %g %g %g\n",
					"cylinder",
					(*idos)->GetDOName().c_str(),
					Vt.x(),
					Vt.y(),
					Vt.z(),
					Radius,
					Height,
					(*idos)->GetOrientationZ().x(),
					(*idos)->GetOrientationZ().y(),
					(*idos)->GetOrientationZ().z());
//					(*idos)->GetOrientationZ().z(),
//					(*idos)->GetGranularTemperatureV(),
//					(*idos)->GetGranularTemperatureAV());
				break;
			default:
				cerr << "Unknown vpf shape" << endl;
		}
	}
	fprintf (fpvpf, "</DiscreteObject>\n");
	fclose (fpvpf);
};

vector<unsigned long> GetObjectTypeNumber(const DOWorld* DOW)
{
	vector<DOStatus*>                 DOS = DOW->GetDOStatus();
	vector<DOStatus*>::const_iterator DOSP;
	string LastDOName = (*(DOS.begin()))->GetDOName();
	vector<unsigned long> DataStructure;
	unsigned long DataNumber = 0;
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
};

void DOWorld::WriteVPF (const char* filename, const DOWorld* opw) const
{
	// "opw" is the "DOWorld" status of original system
	vector<unsigned long> OriginalDataStructure = GetObjectTypeNumber(opw);
	vector<unsigned long> CurrentDataStructure  = GetObjectTypeNumber(this);
/*
	for (unsigned long i=0; i!=OriginalDataStructure.size(); i++)
	{
		cout << OriginalDataStructure[i] << '\t';
	}
	cout << endl;
	for (unsigned long i=0; i!=CurrentDataStructure.size(); i++)
	{
		cout << CurrentDataStructure[i] << '\t';
	}
	cout << endl;
*/

	FILE *fpvpf;
    double Radius, Height, Width, Length;
    double dX, dY, dZ, thickness = 0.001;
    const DOModel* pdoml;
	vector<DOStatus *>::const_iterator  idos;
	vector<DOStatus *> ocDOStatus = opw->GetDOStatus();
	vector<DOStatus *>::const_iterator oidos;
	NJRpolyhedra polyhedra;
	vector<NJRpolygon> faces;
	NJRvector3d Vt;

	if ((fpvpf = fopen(filename, "w")) == NULL)
	{
		cerr << "DOWorld write vpf failed" << endl;
		exit (0);
	};

	fprintf(fpvpf,"<Header>\n");
	fprintf
		(fpvpf,
		"%g %g %g %g\n",
		pSystemParameter->GetTimeStart(),
		pSystemParameter->GetTimeStop(),
		pSystemParameter->GetTimeInterval(),
		pSystemParameter->GetTimeCurrent());

	fprintf(fpvpf, "</Header>\n");
	fprintf(fpvpf,"<DiscreteObject>\n");

/*
	Boundary ZoneOfInterest = DOWorld::GetSystemParameter()->GetZoneOfInterest();
	NJRvector3d	vLowerPoint = ZoneOfInterest.GetLowerPoint();
	NJRvector3d	vUpperPoint = ZoneOfInterest.GetUpperPoint();
	NJRvector3d	vCenter     = 0.5 * (vUpperPoint + vLowerPoint);
	NJRvector3d	vRedundant  = vCenter + NJRvector3d(0.0, dY, 0.0);
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

	unsigned long gap;
	idos  =  cDOStatus.begin();
	oidos = ocDOStatus.begin();
	for(unsigned uli=0; uli<OriginalDataStructure.size(); uli++)
	{
		gap = OriginalDataStructure[uli] - CurrentDataStructure[uli];
		for(unsigned ulj=0; ulj<CurrentDataStructure[uli]; ulj++)
		{
			switch(GetDOModel((*idos)->GetDOName())->GetShapeType())
			{
				case Sphere:
					Radius
						= DOWorld::GetDOModel
							((*idos)->GetDOName())
								->GetShapeAttributes().sphere.radius;
					fprintf
						(fpvpf,
//						"%s %s %g %g %g %g %g %g %g %g %g %g %g %g\n", "sphere",
						"%s %s %g %g %g %g %g %g %g %g %g %g\n", "sphere",
						(*idos)->GetDOName().c_str(),
						(*idos)->GetPosition().x(),
						(*idos)->GetPosition().y(),
						(*idos)->GetPosition().z(),
						(*idos)->GetVelocity().x(),
						(*idos)->GetVelocity().y(),
						(*idos)->GetVelocity().z(),
                        (*idos)->GetAngularVelocity().x(),
                        (*idos)->GetAngularVelocity().y(),
                        (*idos)->GetAngularVelocity().z(),
						Radius);
//						Radius,
//						(*idos)->GetGranularTemperatureV(),
//						(*idos)->GetGranularTemperatureAV());
					break;
				case Polyhedra:
					pdoml     = DOWorld::GetDOModel((*idos)->GetDOName());
					polyhedra = pdoml->GetPolyhedra();
					faces     = polyhedra.faces();
					fprintf
						(fpvpf,
//						"<polyhedra> %s %u %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
						"<polyhedra> %s %u %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
						(*idos)->GetDOName().c_str(),
						faces.size(),
						(*idos)->GetPosition().x(),
						(*idos)->GetPosition().y(),
						(*idos)->GetPosition().z(),
						(*idos)->GetOrientationX().x(),
						(*idos)->GetOrientationX().y(),
						(*idos)->GetOrientationX().z(),
						(*idos)->GetOrientationZ().x(),
						(*idos)->GetOrientationZ().y(),
						(*idos)->GetOrientationZ().z(),
						(*idos)->GetVelocity().x(),
						(*idos)->GetVelocity().y(),
						(*idos)->GetVelocity().z(),
                        (*idos)->GetAngularVelocity().x(),
                        (*idos)->GetAngularVelocity().y(),
                        (*idos)->GetAngularVelocity().z());
//                        (*idos)->GetAngularVelocity().z(),
//						(*idos)->GetGranularTemperatureV(),
//						(*idos)->GetGranularTemperatureAV());

					for	(unsigned int i=0; i<polyhedra.constrains().size();	++i)
					{
						fprintf
							(fpvpf,
							"<constrain> %s %d %d %g %g %g [%d] %g </constrain>\n",
							(*idos)->GetDOName().c_str(),
							i,
							faces[i].vertexes().size(),
							polyhedra.constrains()[i].a(),
							polyhedra.constrains()[i].b(),
							polyhedra.constrains()[i].c(),
							(int) polyhedra.constrains()[i].sense(),
							polyhedra.constrains()[i].d()	        );
						for	(unsigned int j=0; j<faces[i].vertexes().size(); ++j)
						{
							fprintf
								(fpvpf,
								"<vertex> %s %g %g %g </vertex>\n",
								(*idos)->GetDOName().c_str(),
								faces[i].vertexes()[j].x(),
								faces[i].vertexes()[j].y(),
								faces[i].vertexes()[j].z()    );
						}
					}
					fprintf(fpvpf, "</polyhedra>\n");
					break;
				case QuasiPlate:
					Height
						= DOWorld::GetDOModel
							((*idos)->GetDOName())
								->GetShapeAttributes().quasiplate.height;
					Width
						= DOWorld::GetDOModel
							((*idos)->GetDOName())
								->GetShapeAttributes().quasiplate.width;
					Length
						= DOWorld::GetDOModel
							((*idos)->GetDOName())
								->GetShapeAttributes().quasiplate.length;
					fprintf
						(fpvpf,
//						"%s %s %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
						"%s %s %g %g %g %g %g %g %g %g %g %g %g %g\n",
						"plate",
						(*idos)->GetDOName().c_str(),
						(*idos)->GetPosition().x(),
						(*idos)->GetPosition().y(),
						(*idos)->GetPosition().z(),
						(*idos)->GetOrientationX().x(),
						(*idos)->GetOrientationX().y(),
						(*idos)->GetOrientationX().z(),
						(*idos)->GetOrientationZ().x(),
						(*idos)->GetOrientationZ().y(),
						(*idos)->GetOrientationZ().z(),
						Length,
						Width,
						Height);
//						Height,
//						(*idos)->GetGranularTemperatureV(),
//						(*idos)->GetGranularTemperatureAV());
					break;
				case QuasiCylinder:
					Height
						= DOWorld::GetDOModel
							((*idos)->GetDOName())
								->GetShapeAttributes().quasicylinder.height;
					Radius
						= DOWorld::GetDOModel
							((*idos)->GetDOName())
								->GetShapeAttributes().quasicylinder.radius;

					Vt
						= (*idos)->GetPosition()
						- ((*idos)->GetOrientationZ()*(0.5*Height));

					fprintf
						(fpvpf,
//						"%s %s %g %g %g %g %g %g %g %g %g %g\n",
						"%s %s %g %g %g %g %g %g %g %g\n",
						"cylinder",
						(*idos)->GetDOName().c_str(),
						Vt.x(),
						Vt.y(),
						Vt.z(),
						Radius,
						Height,
						(*idos)->GetOrientationZ().x(),
						(*idos)->GetOrientationZ().y(),
						(*idos)->GetOrientationZ().z());
//						(*idos)->GetOrientationZ().z(),
//						(*idos)->GetGranularTemperatureV(),
//						(*idos)->GetGranularTemperatureAV());
					break;
				default:
					cerr << "Unknown vpf shape" << endl;
			}
			idos++;
			oidos++;
		}
		// Add redundant DiscreteObjects
		NJRvector3d	vRedundant(ZERO);
		if(gap != 0)
		{
			switch(GetDOModel((*oidos)->GetDOName())->GetShapeType())
			{
				case Sphere:
					for(unsigned ulj=0; ulj<gap; ulj++)
					{
						fprintf
							(fpvpf,
							"%s %s %g %g %g %g %g %g %g %g %g %g %g\n", "sphere",
							(*oidos)->GetDOName().c_str(),
							vRedundant.x(), vRedundant.y(), vRedundant.z(),
							0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0,
							0.0, 0.0                                       );
						oidos++;
					}
					break;
				case Polyhedra:
					pdoml     = DOWorld::GetDOModel((*oidos)->GetDOName());
					polyhedra = pdoml->GetPolyhedra();
					faces     = polyhedra.faces();
					for(unsigned ulj=0; ulj<gap; ulj++)
					{
						fprintf
							(fpvpf,
							"<polyhedra> %s %u %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
							(*oidos)->GetDOName().c_str(),
							faces.size(),
							vRedundant.x(), vRedundant.y(), vRedundant.z(),
							1.0, 0.0, 0.0,
							0.0, 0.0, 1.0,
							0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0,
							0.0                                            );
						for	(unsigned int i=0; i<polyhedra.constrains().size();	++i)
						{
							fprintf
								(fpvpf,
								"<constrain> %s %d %d %g %g %g [%d] %g </constrain>\n",
								(*oidos)->GetDOName().c_str(),
								i,
								faces[i].vertexes().size(),
								polyhedra.constrains()[i].a(),
								polyhedra.constrains()[i].b(),
								polyhedra.constrains()[i].c(),
								(int) polyhedra.constrains()[i].sense(),
								polyhedra.constrains()[i].d()	        );
							for	(unsigned int j=0; j<faces[i].vertexes().size(); ++j)
							{
								fprintf
									(fpvpf,
									"<vertex> %s %g %g %g </vertex>\n",
									(*oidos)->GetDOName().c_str(),
									faces[i].vertexes()[j].x(),
									faces[i].vertexes()[j].y(),
									faces[i].vertexes()[j].z()    );
							}
						}
						fprintf(fpvpf, "</polyhedra>\n");
						oidos++;
					}
					break;
				case QuasiPlate:
					for(unsigned ulj=0; ulj<gap; ulj++)
					{
						fprintf
							(fpvpf,
							"%s %s %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
							"plate",
							(*oidos)->GetDOName().c_str(),
							vRedundant.x(), vRedundant.y(), vRedundant.z(),
							1.0, 0.0, 0.0,
							0.0, 0.0, 1.0,
							0.001, 0.001, 0.001,
							0.0                                            );
						oidos++;
					}
					break;
				case QuasiCylinder:
					for(unsigned ulj=0; ulj<gap; ulj++)
					{
						fprintf
							(fpvpf,
							"%s %s %g %g %g %g %g %g %g %g %g\n",
							"cylinder",
							(*oidos)->GetDOName().c_str(),
							vRedundant.x(), vRedundant.y(), vRedundant.z(),
							0.001, 0.001,
							0.0, 0.0, 0.1,
							0.0                                            );
						oidos++;
					}
					break;
				default:
					cerr << "Unknown vpf shape" << endl;
			}
		}
	}
	fprintf (fpvpf, "</DiscreteObject>\n");
	fclose (fpvpf);
};
