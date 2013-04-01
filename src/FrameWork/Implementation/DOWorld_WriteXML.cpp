#include <FrameWork/Interfaces/Boundary.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <FrameWork/Interfaces/IactRecordTab.h>
#include <NJR/Interfaces/HalfSpace.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>

namespace vedo
{

void DOWorld::WriteXML(const char* filename) const
{
	FILE *fpxml;
	std::list<DOModel*>::const_iterator     idoml;
	std::list<IactModel*>::const_iterator   iactml;
	std::vector<DOStatus *>::const_iterator idos;

	if ((fpxml = fopen (filename,"w")) == NULL )
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteXML(const char*)" << std::endl
			<< "        Note: DOWorld cannot write XML file"  << std::endl;
		exit(-1);
	};

	fprintf
		(fpxml,
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<!-- created by DOWorld -->\n"
		"<DOWorld xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
		"\t\txsi:noNamespaceSchemaLocation=\"DOWorldSchema.xsd\">\n"
		"\t<Header>\n\t\t<Publish>%s</Publish>\n"
		"\t\t<Title>%s</Title>\n"
		"\t\t<Note>%s</Note>\n"
		"\t</Header>\n"
		"\t<SimParameter>\n"
		"\t\t<TimeControl Start=\"%g\" Stop=\"%g\" Interval=\"%g\" Current=\"%g\"/>\n"
		"\t\t<NumObject>%d</NumObject>\n"
		"\t\t<SimConstant ContactDetectSafetyFactor=\"%g\" NumUDDOStatus=\"%d\" NumUDIactStatus=\"%d\"/>\n"
		"\t\t<FieldAcceleration x=\"%g\" y=\"%g\" z=\"%g\"/>\n",
		pSystemParameter->GetPublish().c_str(),
		pSystemParameter->GetTitle().c_str(),
		pSystemParameter->GetNote().c_str(),
		pSystemParameter->GetTimeStart(),
		pSystemParameter->GetTimeStop(),
		pSystemParameter->GetTimeInterval(),
		pSystemParameter->GetTimeCurrent(),
		pSystemParameter->GetDONumber(),
        vedo::dSafetyFactor,
        vedo::uNumUDDDOStatus,
        vedo::uNumUDDImpactStatus,
		pSystemParameter->GetFieldAcceleration().x(),
		pSystemParameter->GetFieldAcceleration().y(),
		pSystemParameter->GetFieldAcceleration().z()  );

	Boundary bZOI = pSystemParameter->GetZoneOfInterest();
	if(bZOI.Active())
	{
		fprintf(fpxml, "\t\t<ZOI");
		if(bZOI.GetSwitch(0))
		{
			fprintf
				(fpxml,
				 " XMin=\"%g\" XMax=\"%g\"",
				 bZOI.GetLowerPoint().x(),
				 bZOI.GetUpperPoint().x()   );
		};
		if(bZOI.GetSwitch(1))
		{
			fprintf
				(fpxml,
				 " YMin=\"%g\" YMax=\"%g\"",
				 bZOI.GetLowerPoint().y(),
				 bZOI.GetUpperPoint().y()   );
		};
		if(bZOI.GetSwitch(2))
		{
			fprintf
				(fpxml,
				 " ZMin=\"%g\" ZMax=\"%g\"",
				 bZOI.GetLowerPoint().z(),
				 bZOI.GetUpperPoint().z()   );
		};
		fprintf(fpxml, "/>\n");
	}
	else
	{
		fprintf(fpxml, "\t\t<ZOI />\n");
	};

	Boundary bPBC = pSystemParameter->GetPeriodicBoundaryConditions();
	if(bPBC.Active())
	{
		fprintf(fpxml, "\t\t<PBC");
		if(bPBC.GetSwitch(0))
		{
			fprintf
				(fpxml,
				 " XMin=\"%g\" XMax=\"%g\"",
				 bPBC.GetLowerPoint().x(),
				 bPBC.GetUpperPoint().x()   );
		};
		if(bPBC.GetSwitch(1))
		{
			fprintf
				(fpxml,
				 " YMin=\"%g\" YMax=\"%g\"",
				 bPBC.GetLowerPoint().y(),
				 bPBC.GetUpperPoint().y()   );
		};
		if(bPBC.GetSwitch(2))
		{
			fprintf
				(fpxml,
				 " ZMin=\"%g\" ZMax=\"%g\"",
				 bPBC.GetLowerPoint().z(),
				 bPBC.GetUpperPoint().z()   );
		};
		fprintf(fpxml, "/>\n");
	}
	else
	{
		fprintf(fpxml, "\t\t<PBC />\n");
	};

	fprintf(fpxml, "\t</SimParameter>\n");

	char* color[]
		= {"bylayer", "red", "yellow", "green", "cyan", "blue", "magenta"};

	char sa[256];

	std::string buffer1;

	fprintf(fpxml, "\t<DOModelTab>\n");

	for (idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
	{
		switch ((*idoml)->GetShapeType())
		{
			case Sphere:
				sprintf
					(sa,
					"<Sphere Radius=\"%g\"/>\0",
					(*idoml)->GetShapeAttributes().sphere.radius);
				buffer1 = sa;
				break;
			case QuasiPlate:
				sprintf
					(sa,
					"<QuasiPlate Width=\"%g\" Length=\"%g\" Height=\"%g\"/>\0",
					(*idoml)->GetShapeAttributes().quasiplate.width,
					(*idoml)->GetShapeAttributes().quasiplate.length,
					(*idoml)->GetShapeAttributes().quasiplate.height );
				 buffer1 = sa;
				 break;
			case QuasiCylinder:
				sprintf
					(sa,
					"<QuasiCylinder Radius=\"%g\" Height=\"%g\"/>\0",
					(*idoml)->GetShapeAttributes().quasicylinder.radius,
					(*idoml)->GetShapeAttributes().quasicylinder.height );
				buffer1 = sa;
				break;
			case Ellipsoid:
				sprintf
					(sa,
					"<Ellipsoid XLength=\"%g\" YLength=\"%g\" ZLength=\"%g\"/>\0",
					(*idoml)->GetShapeAttributes().ellipsoid.xlength,
					(*idoml)->GetShapeAttributes().ellipsoid.ylength,
					(*idoml)->GetShapeAttributes().ellipsoid.zlength );
				buffer1 = sa;
			case Polyhedra:
				njr::NJRpolyhedra polyhedra = (*idoml)->GetPolyhedra();
				std::vector<njr::HalfSpace> faces = polyhedra.constrains();
				sprintf(sa,"<Polyhedra>\n");
				buffer1 = sa;
				for (unsigned int i=0; i<faces.size(); ++i)
				{
					char sense;
					switch (faces[i].sense())
					{
						case L:
							sense ='L';
							break;
						case G:
							sense ='G';
							break;
						case E:
							sense ='E';
							break;
					}
					sprintf
						(sa,
						"<HalfSpace a=\"%g\" b=\"%g\" c=\"%g\" sense=\"%c\" d=\"%g\"/>",
						faces[i].a(),
						faces[i].b(),
						faces[i].c(),
						sense,
						faces[i].d());
						buffer1.append(sa);
				}
				sprintf(sa, "</Polyhedra>\n\0");
				buffer1.append(sa);
				break;
		}
		if ( (((int)(*idoml)->GetShapeColor())>0)
			&& (((int)(*idoml)->GetShapeColor())<=6) )
		{
			strcpy(sa, color[(int)(*idoml)->GetShapeColor()]);
		}
		else
		{
			strcpy(sa, "bylayer");
		}

		fprintf
			(fpxml,
			"\t\t<DOModel DOName=\"%s\" DOGroup=\"%s\"\n"
			"\t\t\tColor=\"%s\" Behavior=\"%s\" Scope=\"%s\" Density=\"%g\" DensityFactor=\"%g\">\n"
			"\t\t\t<ExternalForce x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<Shape>%s</Shape>\n",
			(*idoml)->GetDOName().c_str(),
			(*idoml)->GetDOGroup().c_str(),
			sa,
			(*idoml)->GetBehavior().c_str(),
			(*idoml)->GetScope().c_str(),
			(*idoml)->GetDensity(),
			(*idoml)->GetDensityFactor(),
			(*idoml)->GetExternalForce().x(),
			(*idoml)->GetExternalForce().y(),
			(*idoml)->GetExternalForce().z(),
			buffer1.c_str()                  );

		const std::vector<DOMaterialAttribute>&
			cMatOpt = (*idoml)->GetMaterialAttributes();

		for (unsigned int i=0; i<cMatOpt.size(); i++)
		{
			fprintf
				(fpxml,
				"<MaterialOption Name=\"%s\" Value=\"%g\" />\n",
				cMatOpt[i].Name.c_str(),
				cMatOpt[i].Value        );
		}
		fprintf(fpxml,"\t\t</DOModel>\n");
	}
	fprintf(fpxml, "\t</DOModelTab>\n");
	fprintf(fpxml, "\t<IactModelTab>\n");

	for (iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
	{
		fprintf
			(fpxml,
			"\t\t<IactModel MasterGroup=\"%s\" SlaveGroup=\"%s\" EquationType=\"%s\">\n",
			(*iactml)->GetMasterDOGroup().c_str(),
			(*iactml)->GetSlaveDOGroup().c_str(),
			(*iactml)->GetEquationType().c_str()  );

		const std::vector<IactMechanism>&
			svIactMechanisms = (*iactml)->GetIactMechanisms();

		for (unsigned int i=0; i<svIactMechanisms.size(); i++)
		{
			fprintf
				(fpxml,
				"\t\t\t<Mechanism Name=\"%s\" Value=\"%g\"/>\n",
				svIactMechanisms[i].Name.c_str(),
				svIactMechanisms[i].Value);
		}
		fprintf(fpxml, "\t\t</IactModel>\n");
	}
	fprintf(fpxml, "\t</IactModelTab>\n");
	fprintf(fpxml, "\t<DOStatusTab>\n");
	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		fprintf (fpxml,
			"\t\t<DOStatus DOName=\"%s\">\n"
			"\t\t\t<Position x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<Velocity x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<OrientationX x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<OrientationZ x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<AngularVelocity x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t</DOStatus>\n",
			(*idos)->GetDOName().c_str(),
			(*idos)->GetPosition().x(),
			(*idos)->GetPosition().y(),
			(*idos)->GetPosition().z(),
			(*idos)->GetVelocity().x(),
			(*idos)->GetVelocity().y(),
			(*idos)->GetVelocity().z(),
			(*idos)->GetOrientationX().x(),
			(*idos)->GetOrientationX().y(),
			(*idos)->GetOrientationX().z(),
			(*idos)->GetOrientationZ().x(),
			(*idos)->GetOrientationZ().y(),
			(*idos)->GetOrientationZ().z(),
			(*idos)->GetAngularVelocity().x(),
			(*idos)->GetAngularVelocity().y(),
			(*idos)->GetAngularVelocity().z()  );
	}
	fprintf(fpxml, "\t</DOStatusTab>\n");

	fprintf(fpxml, "</DOWorld>");
	fclose(fpxml);
};

void DOWorld::WriteXML(const char* filename, const IactRecordTab* irtp) const
{
	FILE *fpxml;
	std::list<DOModel*>::const_iterator     idoml;
	std::list<IactModel*>::const_iterator   iactml;
	std::vector<DOStatus *>::const_iterator idos;

	if ((fpxml = fopen (filename,"w")) == NULL )
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteXML(const char*)" << std::endl
			<< "        Note: DOWorld cannot write XML file"  << std::endl;
		exit(-1);
	};

	fprintf
		(fpxml,
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<!-- created by DOWorld -->\n"
		"<DOWorld xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
		"\t\txsi:noNamespaceSchemaLocation=\"DOWorldSchema.xsd\">\n"
		"\t<Header>\n\t\t<Publish>%s</Publish>\n"
		"\t\t<Title>%s</Title>\n"
		"\t\t<Note>%s</Note>\n"
		"\t</Header>\n"
		"\t<SimParameter>\n"
		"\t\t<TimeControl Start=\"%g\" Stop=\"%g\" Interval=\"%g\" Current=\"%g\"/>\n"
		"\t\t<NumObject>%d</NumObject>\n"
		"\t\t<SimConstant ContactDetectSafetyFactor=\"%g\" NumUDDOStatus=\"%d\" NumUDIactStatus=\"%d\"/>\n"
		"\t\t<FieldAcceleration x=\"%g\" y=\"%g\" z=\"%g\"/>\n",
		pSystemParameter->GetPublish().c_str(),
		pSystemParameter->GetTitle().c_str(),
		pSystemParameter->GetNote().c_str(),
		pSystemParameter->GetTimeStart(),
		pSystemParameter->GetTimeStop(),
		pSystemParameter->GetTimeInterval(),
		pSystemParameter->GetTimeCurrent(),
		pSystemParameter->GetDONumber(),
        vedo::dSafetyFactor,
        vedo::uNumUDDDOStatus,
        vedo::uNumUDDImpactStatus,
		pSystemParameter->GetFieldAcceleration().x(),
		pSystemParameter->GetFieldAcceleration().y(),
		pSystemParameter->GetFieldAcceleration().z()  );

	Boundary bZOI = pSystemParameter->GetZoneOfInterest();
	if(bZOI.Active())
	{
		fprintf(fpxml, "\t\t<ZOI");
		if(bZOI.GetSwitch(0))
		{
			fprintf
				(fpxml,
				 " XMin=\"%g\" XMax=\"%g\"",
				 bZOI.GetLowerPoint().x(),
				 bZOI.GetUpperPoint().x()   );
		};
		if(bZOI.GetSwitch(1))
		{
			fprintf
				(fpxml,
				 " YMin=\"%g\" YMax=\"%g\"",
				 bZOI.GetLowerPoint().y(),
				 bZOI.GetUpperPoint().y()   );
		};
		if(bZOI.GetSwitch(2))
		{
			fprintf
				(fpxml,
				 " ZMin=\"%g\" ZMax=\"%g\"",
				 bZOI.GetLowerPoint().z(),
				 bZOI.GetUpperPoint().z()   );
		};
		fprintf(fpxml, "/>\n");
	}
	else
	{
		fprintf(fpxml, "\t\t<ZOI />\n");
	};

	Boundary bPBC = pSystemParameter->GetPeriodicBoundaryConditions();
	if(bPBC.Active())
	{
		fprintf(fpxml, "\t\t<PBC");
		if(bPBC.GetSwitch(0))
		{
			fprintf
				(fpxml,
				 " XMin=\"%g\" XMax=\"%g\"",
				 bPBC.GetLowerPoint().x(),
				 bPBC.GetUpperPoint().x()   );
		};
		if(bPBC.GetSwitch(1))
		{
			fprintf
				(fpxml,
				 " YMin=\"%g\" YMax=\"%g\"",
				 bPBC.GetLowerPoint().y(),
				 bPBC.GetUpperPoint().y()   );
		};
		if(bPBC.GetSwitch(2))
		{
			fprintf
				(fpxml,
				 " ZMin=\"%g\" ZMax=\"%g\"",
				 bPBC.GetLowerPoint().z(),
				 bPBC.GetUpperPoint().z()   );
		};
		fprintf(fpxml, "/>\n");
	}
	else
	{
		fprintf(fpxml, "\t\t<PBC />\n");
	};

	fprintf(fpxml, "\t</SimParameter>\n");

	char* color[]
		= {"bylayer", "red", "yellow", "green", "cyan", "blue", "magenta"};

	char sa[256];

	std::string buffer1;

	fprintf(fpxml, "\t<DOModelTab>\n");

	for (idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
	{
		switch ((*idoml)->GetShapeType())
		{
			case Sphere:
				sprintf
					(sa,
					"<Sphere Radius=\"%g\"/>\0",
					(*idoml)->GetShapeAttributes().sphere.radius);
				buffer1 = sa;
				break;
			case QuasiPlate:
				sprintf
					(sa,
					"<QuasiPlate Width=\"%g\" Length=\"%g\" Height=\"%g\"/>\0",
					(*idoml)->GetShapeAttributes().quasiplate.width,
					(*idoml)->GetShapeAttributes().quasiplate.length,
					(*idoml)->GetShapeAttributes().quasiplate.height );
				 buffer1 = sa;
				 break;
			case QuasiCylinder:
				sprintf
					(sa,
					"<QuasiCylinder Radius=\"%g\" Height=\"%g\"/>\0",
					(*idoml)->GetShapeAttributes().quasicylinder.radius,
					(*idoml)->GetShapeAttributes().quasicylinder.height );
				buffer1 = sa;
				break;
			case Ellipsoid:
				sprintf
					(sa,
					"<Ellipsoid XLength=\"%g\" YLength=\"%g\" ZLength=\"%g\"/>\0",
					(*idoml)->GetShapeAttributes().ellipsoid.xlength,
					(*idoml)->GetShapeAttributes().ellipsoid.ylength,
					(*idoml)->GetShapeAttributes().ellipsoid.zlength );
				buffer1 = sa;
			case Polyhedra:
				njr::NJRpolyhedra polyhedra = (*idoml)->GetPolyhedra();
				std::vector<njr::HalfSpace> faces = polyhedra.constrains();
				sprintf(sa,"<Polyhedra>\n");
				buffer1 = sa;
				for (unsigned int i=0; i<faces.size(); ++i)
				{
					char sense;
					switch (faces[i].sense())
					{
						case L:
							sense ='L';
							break;
						case G:
							sense ='G';
							break;
						case E:
							sense ='E';
							break;
					}
					sprintf
						(sa,
						"<HalfSpace a=\"%g\" b=\"%g\" c=\"%g\" sense=\"%c\" d=\"%g\"/>",
						faces[i].a(),
						faces[i].b(),
						faces[i].c(),
						sense,
						faces[i].d());
						buffer1.append(sa);
				}
				sprintf(sa, "</Polyhedra>\n\0");
				buffer1.append(sa);
				break;
		}
		if ( (((int)(*idoml)->GetShapeColor())>0)
			&& (((int)(*idoml)->GetShapeColor())<=6) )
		{
			strcpy(sa, color[(int)(*idoml)->GetShapeColor()]);
		}
		else
		{
			strcpy(sa, "bylayer");
		}

		fprintf
			(fpxml,
			"\t\t<DOModel DOName=\"%s\" DOGroup=\"%s\"\n"
			"\t\t\tColor=\"%s\" Behavior=\"%s\" Scope=\"%s\" Density=\"%g\" DensityFactor=\"%g\">\n"
			"\t\t\t<ExternalForce x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<Shape>%s</Shape>\n",
			(*idoml)->GetDOName().c_str(),
			(*idoml)->GetDOGroup().c_str(),
			sa,
			(*idoml)->GetBehavior().c_str(),
			(*idoml)->GetScope().c_str(),
			(*idoml)->GetDensity(),
			(*idoml)->GetDensityFactor(),
			(*idoml)->GetExternalForce().x(),
			(*idoml)->GetExternalForce().y(),
			(*idoml)->GetExternalForce().z(),
			buffer1.c_str()                  );

		const std::vector<DOMaterialAttribute>&
			cMatOpt = (*idoml)->GetMaterialAttributes();

		for (unsigned int i=0; i<cMatOpt.size(); i++)
		{
			fprintf
				(fpxml,
				"<MaterialOption Name=\"%s\" Value=\"%g\" />\n",
				cMatOpt[i].Name.c_str(),
				cMatOpt[i].Value        );
		}
		fprintf(fpxml,"\t\t</DOModel>\n");
	}
	fprintf(fpxml, "\t</DOModelTab>\n");
	fprintf(fpxml, "\t<IactModelTab>\n");

	for (iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
	{
		fprintf
			(fpxml,
			"\t\t<IactModel MasterGroup=\"%s\" SlaveGroup=\"%s\" EquationType=\"%s\">\n",
			(*iactml)->GetMasterDOGroup().c_str(),
			(*iactml)->GetSlaveDOGroup().c_str(),
			(*iactml)->GetEquationType().c_str()  );

		const std::vector<IactMechanism>&
			svIactMechanisms = (*iactml)->GetIactMechanisms();

		for (unsigned int i=0; i<svIactMechanisms.size(); i++)
		{
			fprintf
				(fpxml,
				"\t\t\t<Mechanism Name=\"%s\" Value=\"%g\"/>\n",
				svIactMechanisms[i].Name.c_str(),
				svIactMechanisms[i].Value);
		}
		fprintf(fpxml, "\t\t</IactModel>\n");
	}
	fprintf(fpxml, "\t</IactModelTab>\n");
	fprintf(fpxml, "\t<DOStatusTab>\n");
	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		fprintf (fpxml,
			"\t\t<DOStatus DOName=\"%s\">\n"
			"\t\t\t<Position x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<Velocity x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<OrientationX x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<OrientationZ x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t\t<AngularVelocity x=\"%g\" y=\"%g\" z=\"%g\"/>\n"
			"\t\t</DOStatus>\n",
			(*idos)->GetDOName().c_str(),
			(*idos)->GetPosition().x(),
			(*idos)->GetPosition().y(),
			(*idos)->GetPosition().z(),
			(*idos)->GetVelocity().x(),
			(*idos)->GetVelocity().y(),
			(*idos)->GetVelocity().z(),
			(*idos)->GetOrientationX().x(),
			(*idos)->GetOrientationX().y(),
			(*idos)->GetOrientationX().z(),
			(*idos)->GetOrientationZ().x(),
			(*idos)->GetOrientationZ().y(),
			(*idos)->GetOrientationZ().z(),
			(*idos)->GetAngularVelocity().x(),
			(*idos)->GetAngularVelocity().y(),
			(*idos)->GetAngularVelocity().z()  );
	}
	fprintf(fpxml, "\t</DOStatusTab>\n");

	fprintf(fpxml, "\t<IactStatusTab>\n");

    const std::map< std::pair<unsigned long, unsigned long>, ImpactStatus >* mapImStatusP = &(irtp->GetData());
    std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator mmapImStatus;
	unsigned long ulMaster, ulSlave;
	const ImpactStatus* isp;
	njr::Vector3d vShearForce;
	const double* dpUDV;
	for (mmapImStatus=(mapImStatusP->begin()); mmapImStatus!=(mapImStatusP->end()); mmapImStatus++)
	{
		ulMaster    = mmapImStatus->first.first;
		ulSlave     = mmapImStatus->first.second;
		isp         = &(mmapImStatus->second);
        dpUDV       = isp->RetrieveAllUserDefinedValue();
		vShearForce = isp->ShearForce();
		fprintf (fpxml,
			"\t\t<IactStatus MasterDOStatusID=\"%d\" SlaveDOStatusID=\"%d\"\n"
			"\t\t\tContact=\"%d\" Bond=\"%d\" NormalStiffness=\"%g\">\n"
			"\t\t\t<ShearForce x=\"%g\" y=\"%g\" z=\"%g\"/>\n",
			ulMaster, ulSlave, isp->Contact(), isp->Bond(), isp->Kn(),
			vShearForce.x(), vShearForce.y(), vShearForce.z()         );

        for (unsigned u=0; u<vedo::uNumUDDImpactStatus; u++)
            fprintf (fpxml,
                "\t\t\t<AccumulativeUserDefinedValue ID=\"%d\" Value=\"%g\"/>\n",
                u, *(dpUDV+u)                                        );

        for (unsigned u=0; u<vedo::uNumUDDImpactStatus; u++)
            fprintf (fpxml,
                "\t\t\t<UserDefinedValue ID=\"%d\" Value=\"%g\"/>\n",
                u, *(dpUDV+3*vedo::uNumUDDImpactStatus+u)                                        );

		fprintf (fpxml, "\t\t</IactStatus>\n");
	}

	fprintf(fpxml, "\t</IactStatusTab>\n");

	fprintf(fpxml, "</DOWorld>");
	fclose(fpxml);
};

};   // namespace vedo
