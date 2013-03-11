// DOModel.cpp: implementation of the DOModel class.

#include <FrameWork/Interfaces/DOModel.h>
#include <NJR/Interfaces/Utility.h>
#include <cmath>
#include <cstdlib>

using namespace std;

bool DOModel::operator () (const DOModel* p) const
{
	return (sDOName == (p->sDOName));
};

void DOModel::Analysis()
{
	double r, w, h, l, dmmi, x, y, z;
	//double dmmiX, dmmiY, dmmiZ;
	switch (shType)
	{
		case 1:
			r                        = shAttributes.sphere.radius;
			dRange                   = r;
			dVolume                  = 4.188790204786 * r * r * r;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			dmmi                     = 0.4 * dMass * r * r;
			vMassMomentInertia       = NJRvector3d(dmmi, dmmi, dmmi);
			break;
		case 2:
/******************************************************************************
 * Aries' Debug (2006/08/02)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			x                        = shAttributes.ellipsoid.xlength;
			y                        = shAttributes.ellipsoid.ylength;
			z                        = shAttributes.ellipsoid.zlength;
			dRange                   = 0.5 * max(max(x, y), z);
			dVolume		         = 0.523598775598 * x * y * z;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			vMassMomentInertia       = NJRvector3d();
			break;
		case 3:
			r                        = shAttributes.quasicylinder.radius;
			h                        = shAttributes.quasicylinder.height;
			dRange                   = max(r, 0.5 * h);
			dVolume                  = 3.141592654 * r * r * h;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
/******************************************************************************
 * Aries' Debug (2006/08/02)
 *
 *    Unfortunately, Mass moment of inertia depend on the direction of
 * coordinate system.
 *
			dmmi               = dMass * (3.0 * r * r + h * h) / 12.0;
			vMassMomentInertia = NJRvector3d(dmmi, dmmi, 0.5 * dMass * r * r);
 ******************************************************************************/
			vMassMomentInertia       = NJRvector3d();
			break;
		case 4:
			w                        = shAttributes.quasiplate.width;
			h                        = shAttributes.quasiplate.height;
			l                        = shAttributes.quasiplate.length;
			dRange                   = 0.5 * max(max(w, h), l);
			dVolume                  = w * h * l;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
/******************************************************************************
 * Aries' Debug (2006/08/02)
 *
 *    Unfortunately, Mass moment of inertia depend on the direction of
 * coordinate system.
 *
			dmmiX              = dMass * (h * h + l * l) / 12.0;
			dmmiY              = dMass * (w * w + l * l) / 12.0;
			dmmiZ              = dMass * (h * h + w * w) / 12.0;
			vMassMomentInertia = NJRvector3d(dmmiX, dmmiY, dmmiZ);
 ******************************************************************************/
			vMassMomentInertia       = NJRvector3d();
			break;
		case 5:
/******************************************************************************
 * Aries' Debug (2006/08/02)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = NJRvector3d();
			break;
		case 6:
/******************************************************************************
 * Aries' Debug (2006/08/02)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = NJRvector3d();
			break;
		case 7:
/******************************************************************************
 * Aries' Debug (2006/08/02)
 *
 *    Polyhedra (B-Rep), need to modify.
 ******************************************************************************/
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = NJRvector3d();
			break;
		default:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = NJRvector3d();
	}
};

DOModel::DOModel(ifstream& idof)
{
	*this << idof;
	Analysis();
};


DOModel::DOModel(ifstream& idof, unsigned int _version)
{
	int MatOptSize, i;
	DOMaterialAttribute DMO;

	NJR::ReadString(sDOName  , idof);
	NJR::ReadString(sDOGroup , idof);
	NJR::ReadString(sBehavior, idof);

	if(_version > 2011)
	{
		NJR::ReadString(sScope, idof);
	}
	else
	{
		if(sBehavior == "mobile")
			sScope = "local";
		else
			sScope = "global";
	}

	idof.read((char*) &dDensity, sizeof(double));
	if(_version > 2007)
		idof.read((char*) &dDensityFactor, sizeof(double));
	else
		dDensityFactor = 1.0;

	if(_version >= 2010)
		idof.read((char*) &vExternalForce, sizeof(NJRvector3d));
	else
		vExternalForce.Set(0.0, 0.0, 0.0);

	idof.read((char*) &shColor     , sizeof(DOShapeColor)     );
	idof.read((char*) &shType      , sizeof(DOShapeType)      );
	idof.read((char*) &shAttributes, sizeof(DOShapeAttributes));

	idof.read((char*) &MatOptSize  , sizeof (int));
	for (i=0; i<MatOptSize; i++)
	{
		NJR::ReadString(DMO.Name, idof);
		idof.read((char*) &(DMO.Value), sizeof(double));
		matAttributes.push_back(DMO);
	}

	if (shType == Polyhedra)
	{
		int numhf;
		NJRhalfspace hf;
		polyhedra.Clear();

		idof.read((char*) &numhf, sizeof (int));

		for (i=0; i<numhf; i++)
		{
			idof.read((char*) &hf, sizeof(NJRhalfspace));
			polyhedra.AddConstrain(hf);
		}
	}
	Analysis();
};

DOModel::DOModel(const string& DOName) : matAttributes(0)
{
	sDOName = DOName;
};

DOModel::DOModel
	(const string&            DOName,
	 const string&            DOGroup,
	 const string&            Behavior,
	 const string&            Scope,
	 const double&            Density,
	 const double&            DensityFactor,
	 const NJRvector3d&       ExternalForce,
	 const DOShapeType&       type,
	 const DOShapeAttributes& attributes,
	 const DOShapeColor&      color         ): matAttributes(0)
{
	sDOName        = DOName;
	sDOGroup       = DOGroup;
	sBehavior      = Behavior;
	sScope         = Scope;
	dDensity       = Density;
	dDensityFactor = DensityFactor;
	vExternalForce = ExternalForce;
	shColor        = color;
	shType         = type;
	shAttributes   = attributes;
	Analysis();
};

DOModel::DOModel
	(const string&            DOName,
	 const string&            DOGroup,
	 const string&            Behavior,
	 const string&            Scope,
	 const double&            Density,
	 const double&            DensityFactor,
	 const NJRvector3d&       ExternalForce,
	 const DOShapeType&       type,
	 const DOShapeAttributes& attributes,
	 const DOShapeColor&      color,
	 const vector<DOMaterialAttribute>& mats): matAttributes(0)
{
	sDOName        = DOName;
	sDOGroup       = DOGroup;
	sBehavior      = Behavior;
	sScope         = Scope;
	dDensity       = Density;
	dDensityFactor = DensityFactor;
	vExternalForce = ExternalForce;
	shColor        = color;
	shType         = type;
	shAttributes   = attributes;
	matAttributes  = mats;
	Analysis();
};

DOModel::DOModel
	(const string&       DOName,
	 const string&       DOGroup,
	 const string&       Behavior,
	 const string&       Scope,
	 const double&       Density,
	 const double&       DensityFactor,
	 const NJRvector3d&  ExternalForce,
	 const NJRpolyhedra& poly,
	 const DOShapeColor& color         ): matAttributes(0)
{
	sDOName        = DOName;
	sDOGroup       = DOGroup;
	sBehavior      = Behavior;
	sScope         = Scope;
	dDensity       = Density;
	dDensityFactor = DensityFactor;
	vExternalForce = ExternalForce;
	shColor        = color;
	shType         = Polyhedra;
	polyhedra      = poly;
	Analysis();
};

DOModel::DOModel
	(const string&       DOName,
	 const string&       DOGroup,
	 const string&       Behavior,
	 const string&       Scope,
	 const double&       Density,
	 const double&       DensityFactor,
	 const NJRvector3d&  ExternalForce,
	 const NJRpolyhedra& poly,
	 const DOShapeColor& color,
	 const vector<DOMaterialAttribute>& mats): matAttributes(0)
{
	sDOName        = DOName;
	sDOGroup       = DOGroup;
	sBehavior      = Behavior;
	sScope         = Scope;
	dDensity       = Density;
	dDensityFactor = DensityFactor;
	vExternalForce = ExternalForce;
	shColor        = color;
	shType         = Polyhedra;
	polyhedra      = poly;
	matAttributes  = mats;
	Analysis();
};

DOModel::DOModel(const DOModel& doml) : matAttributes(0)
{
	*this = doml;
};

const DOModel& DOModel::operator = (const DOModel& doml)
{
	sDOName            = doml.sDOName;
	sDOGroup           = doml.sDOGroup;
	sBehavior          = doml.sBehavior;
	sScope             = doml.sScope;
	dDensity           = doml.dDensity;
	dDensityFactor     = doml.dDensityFactor;
	vExternalForce     = doml.GetExternalForce();
	dMass              = doml.dMass;
	dSudoMass          = doml.dSudoMass;
	dVolume            = doml.dVolume;
	vMassMomentInertia = doml.vMassMomentInertia;
	shColor            = doml.shColor;
	shType             = doml.shType;
	shAttributes       = doml.shAttributes;
	matAttributes      = doml.matAttributes;
	polyhedra          = doml.polyhedra;
	return *this;
};

ofstream& DOModel::operator >> (ofstream& idof) const
{
	unsigned int MatOptSize,i;
	DOMaterialAttribute DMO;

	NJR::WriteString(sDOName  , idof);
	NJR::WriteString(sDOGroup , idof);
	NJR::WriteString(sBehavior, idof);
	NJR::WriteString(sScope   , idof);
	idof.write((const char*) &dDensity,       sizeof(double)           );
	idof.write((const char*) &dDensityFactor, sizeof(double)           );
	idof.write((const char*) &vExternalForce, sizeof(NJRvector3d)      );
	idof.write((const char*) &shColor,        sizeof(DOShapeColor)     );
	idof.write((const char*) &shType,         sizeof(DOShapeType)      );
	idof.write((const char*) &shAttributes,   sizeof(DOShapeAttributes));

	MatOptSize = (unsigned int) matAttributes.size();
	idof.write((const char*) &MatOptSize, sizeof(int));
	for (i=0; i<MatOptSize; i++)
	{
		DMO=matAttributes[i];
		NJR::WriteString(DMO.Name,idof);
		idof.write((const char*) &(DMO.Value), sizeof(double));
	}

	if (shType == Polyhedra)
	{
		unsigned int numhf = (unsigned int)(polyhedra.constrains().size());
		vector<NJRhalfspace> constrains = polyhedra.constrains();

		idof.write ((char*) &numhf, sizeof(int));

		for (i=0; i<numhf; i++)
		{
			idof.write((char*) & (constrains[i]), sizeof(NJRhalfspace));
		}
	}

	return idof;
};

ifstream& DOModel::operator << (ifstream &idof)
{
	int MatOptSize, i;
	DOMaterialAttribute DMO;

	NJR::ReadString(sDOName  , idof);
	NJR::ReadString(sDOGroup , idof);
	NJR::ReadString(sBehavior, idof);
	NJR::ReadString(sScope   , idof);
	idof.read((char*) &dDensity      , sizeof(double)           );
	idof.read((char*) &dDensityFactor, sizeof(double)           );
	idof.read((char*) &vExternalForce, sizeof(NJRvector3d)      );
 	idof.read((char*) &shColor       , sizeof(DOShapeColor)     );
	idof.read((char*) &shType        , sizeof(DOShapeType)      );
	idof.read((char*) &shAttributes  , sizeof(DOShapeAttributes));

	idof.read((char*) &MatOptSize    , sizeof (int));
	for (i=0; i<MatOptSize; i++)
	{
		NJR::ReadString(DMO.Name, idof);
		idof.read((char*) &(DMO.Value), sizeof(double));
		matAttributes.push_back(DMO);
	}

	if (shType == Polyhedra)
	{
		int numhf;
		NJRhalfspace hf;
		polyhedra.Clear();

		idof.read((char*) &numhf, sizeof (int));

		for (i=0; i<numhf; i++)
		{
			idof.read((char*) &hf, sizeof(NJRhalfspace));
			polyhedra.AddConstrain(hf);
		}
	}
	return idof;
};

double DOModel::GetMaterialAttribute(string Name) const
{

	for (unsigned int i=0; i<matAttributes.size(); i++)
	{
		if (matAttributes[i].Name == Name)
		{
			return matAttributes[i].Value;
		}
	}

	cerr
		<< "Model "
		<< sDOName.c_str()
		<< "contains no such material option -- "
		<< Name.c_str()
		<< endl;

	return 0.0;
};

double DOModel::CrossAreaToSurface
	(const NJRvector3d& vP,
	 const double&      a,
	 const double&      b,
	 const double&      c,
	 const double&      d ) const
{
	double dSphere2Surface, r;
	NJRvector3d vSurfaceNormal(a, b, c);   // Surface: ax+by+cz=d
	switch (shType)
	{
		case 1:
			r   = shAttributes.sphere.radius;
			dSphere2Surface
				= fabs(d - (vP%vSurfaceNormal)/vSurfaceNormal.length());
			if(dSphere2Surface >= r)
			{
				return 0.0;
			}
			else
			{
				return (r * r - dSphere2Surface * dSphere2Surface) * 3.14159267;
			}
			break;
		case 2:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case 3:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case 4:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case 5:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case 6:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		default:
			return 0.0;
	}
};

void FixBoundaryForVolumeInsideBoundary
	(double* L, double* U, const double* P, const double* R, const double* MS)
{
	if(*L < (*P)-(*R))
	{
		*L += (double)(unsigned long)((*P - *R - *L) / *MS) * (*MS);
		if(*U > *P + *R)
		{
			*U = *L + (double)((unsigned long)((2.0 * (*R))/ *MS) + 1) * (*MS);
		}
	}
	else if(*L < *P + *R)
	{
		if(*U > *P + *R)
		{
			*U
				= *L
				+ (double)((unsigned long)((*P + *R - *L) / *MS) + 1) * (*MS);
		}
	}
	else
	{
		*U = *L;
	};
};

pair<double, NJRvector3d> DOModel::VolumeInsideBoundary
	(const NJRvector3d& vP, const Boundary* pBC, const double& dMeshSize) const
{
	double Lx = pBC->GetLowerPoint().x();
	double Ly = pBC->GetLowerPoint().y();
	double Lz = pBC->GetLowerPoint().z();
	double Ux = pBC->GetUpperPoint().x();
	double Uy = pBC->GetUpperPoint().y();
	double Uz = pBC->GetUpperPoint().z();
	double Px = vP.x();
	double Py = vP.y();
	double Pz = vP.z();
	double R;

	switch (shType)
	{
		case 1:
			R = shAttributes.sphere.radius;
			if(   (Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R))
			   || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)) )
			{
				return make_pair(0.0, NJRvector3d(ZERO));
			}
			else if(   (Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R))
					&& (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)) )
			{
				return make_pair(dVolume, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Lx, &Ux, &Px, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Ly, &Uy, &Py, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Lz, &Uz, &Pz, &R, &dMeshSize);

				NJRvector3d vT;
				double dVolumeInsideBoundary = 0.0;
				double dMeshVolume  = pow(dMeshSize, 3.0);
				double dMassCenterX = 0.0;
				double dMassCenterY = 0.0;
				double dMassCenterZ = 0.0;

				for(double Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for(double Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
					{
						for(double Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
						{
							vT.Set(Tx, Ty, Tz);
							if( (vP-vT).length() < R)
							{
								dVolumeInsideBoundary += dMeshVolume;
								dMassCenterX          += Tx * dMeshVolume;
								dMassCenterY          += Ty * dMeshVolume;
								dMassCenterZ          += Tz * dMeshVolume;
							}
						}
					}
				}
				if (dVolumeInsideBoundary != 0.0)
				{
					dMassCenterX /= dVolumeInsideBoundary;
					dMassCenterY /= dVolumeInsideBoundary;
					dMassCenterZ /= dVolumeInsideBoundary;
				}
				else
				{
					dMassCenterX = dMassCenterY = dMassCenterZ = 0.0;
				};
				return make_pair
					(dVolumeInsideBoundary,
					 NJRvector3d(dMassCenterX, dMassCenterY, dMassCenterZ));
			};
			break;
		case 2:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 3:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 4:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 5:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 6:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		default:
			return make_pair(0.0, NJRvector3d(ZERO));
	};
};

pair<double, NJRvector3d> DOModel::ProjectedAreaOnXYPlane
	(const NJRvector3d& vP, const Boundary* pBC, const double& dMeshSize) const
{
	double Lx = pBC->GetLowerPoint().x();
	double Ly = pBC->GetLowerPoint().y();
	double Lz = pBC->GetLowerPoint().z();
	double Ux = pBC->GetUpperPoint().x();
	double Uy = pBC->GetUpperPoint().y();
	double Uz = pBC->GetUpperPoint().z();
	double Px = vP.x();
	double Py = vP.y();
	double Pz = vP.z();
	double R;

	switch (shType)
	{
		case 1:
			R = shAttributes.sphere.radius;
			if(   (Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R))
			   || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)) )
			{
				return make_pair(0.0, NJRvector3d(ZERO));
			}
			else if(   (Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R))
				    && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)) )
			{
				return make_pair(R * R * 3.141592654, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Lx, &Ux, &Px, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Ly, &Uy, &Py, &R, &dMeshSize);
				NJRvector3d vT;
				double dProjectedArea = 0.0;
				double dMeshArea      = dMeshSize * dMeshSize;
				double dCentroidX     = 0.0;
				double dCentroidY     = 0.0;

				for(double Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for(double Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
					{
						if(sqrt((Tx-Px)*(Tx-Px) + (Ty-Py)*(Ty-Py)) < R)
						{
							dProjectedArea += dMeshArea;
							dCentroidX += Tx * dMeshArea;
							dCentroidY += Ty * dMeshArea;
						}
					}
				}

				if (dProjectedArea != 0.0)
				{
					dCentroidX /= dProjectedArea;
					dCentroidY /= dProjectedArea;
				}
				else
				{
					dCentroidX = dCentroidY = 0.0;
				};
				return make_pair
					(dProjectedArea, NJRvector3d(dCentroidX, dCentroidY, vP.z()));
			};
			break;
		case 2:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 3:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 4:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 5:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 6:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		default:
			return make_pair(0.0, NJRvector3d(ZERO));
	};
};

pair<double, NJRvector3d> DOModel::ProjectedAreaOnYZPlane
	(const NJRvector3d& vP, const Boundary* pBC, const double& dMeshSize) const
{
	double Lx = pBC->GetLowerPoint().x();
	double Ly = pBC->GetLowerPoint().y();
	double Lz = pBC->GetLowerPoint().z();
	double Ux = pBC->GetUpperPoint().x();
	double Uy = pBC->GetUpperPoint().y();
	double Uz = pBC->GetUpperPoint().z();
	double Px = vP.x();
	double Py = vP.y();
	double Pz = vP.z();
	double R;

	switch (shType)
	{
		case 1:
			R = shAttributes.sphere.radius;
			if(   (Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R))
			   || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)) )
			{
				return make_pair(0.0, NJRvector3d(ZERO));
			}
			else if(   (Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R))
				    && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)) )
			{
				return make_pair(R * R * 3.141592654, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Ly, &Uy, &Py, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Lz, &Uz, &Pz, &R, &dMeshSize);

				NJRvector3d vT;
				double dProjectedArea = 0.0;
				double dMeshArea      = dMeshSize * dMeshSize;
				double dCentroidY     = 0.0;
				double dCentroidZ     = 0.0;

				for(double Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
				{
					for(double Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
					{
						if(sqrt((Ty-Py)*(Ty-Py) + (Tz-Pz)*(Tz-Pz)) < R)
						{
							dProjectedArea += dMeshArea;
							dCentroidY += Ty * dMeshArea;
							dCentroidZ += Tz * dMeshArea;
						}
					}
				}

				if (dProjectedArea != 0.0)
				{
					dCentroidY /= dProjectedArea;
					dCentroidZ /= dProjectedArea;
				}
				else
				{
					dCentroidY = dCentroidZ = 0.0;
				};
				return make_pair
					(dProjectedArea, NJRvector3d(vP.x(), dCentroidY, dCentroidZ));
			};
			break;
		case 2:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 3:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 4:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 5:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 6:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		default:
			return make_pair(0.0, NJRvector3d(ZERO));
	};
};

pair<double, NJRvector3d> DOModel::ProjectedAreaOnXZPlane
	(const NJRvector3d& vP, const Boundary* pBC, const double& dMeshSize) const
{
	double Lx = pBC->GetLowerPoint().x();
	double Ly = pBC->GetLowerPoint().y();
	double Lz = pBC->GetLowerPoint().z();
	double Ux = pBC->GetUpperPoint().x();
	double Uy = pBC->GetUpperPoint().y();
	double Uz = pBC->GetUpperPoint().z();
	double Px = vP.x();
	double Py = vP.y();
	double Pz = vP.z();
	double R;

	switch (shType)
	{
		case 1:
			R = shAttributes.sphere.radius;
			if(   (Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R))
			   || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)) )
			{
				return make_pair(0.0, NJRvector3d(ZERO));
			}
			else if(   (Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R))
				    && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)) )
			{
				return make_pair(R * R * 3.141592654, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Lx, &Ux, &Px, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Lz, &Uz, &Pz, &R, &dMeshSize);

				NJRvector3d vT;
				double dProjectedArea = 0.0;
				double dMeshArea      = dMeshSize * dMeshSize;
				double dCentroidX     = 0.0;
				double dCentroidZ     = 0.0;

				for(double Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for(double Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
					{
						if(sqrt((Tx-Px)*(Tx-Px) + (Tz-Pz)*(Tz-Pz)) < R)
						{
							dProjectedArea += dMeshArea;
							dCentroidX += Tx * dMeshArea;
							dCentroidZ += Tz * dMeshArea;
						}
					}
				}

				if (dProjectedArea != 0.0)
				{
					dCentroidX /= dProjectedArea;
					dCentroidZ /= dProjectedArea;
				}
				else
				{
					dCentroidX = dCentroidZ = 0.0;
				};
				return make_pair
					(dProjectedArea, NJRvector3d(dCentroidX, vP.y(), dCentroidZ));
			};
			break;
		case 2:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 3:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 4:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 5:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		case 6:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return make_pair(0.0, NJRvector3d(ZERO));
			break;
		default:
			return make_pair(0.0, NJRvector3d(ZERO));
	};
};
