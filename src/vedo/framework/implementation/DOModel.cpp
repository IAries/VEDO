#include <vedo/njr/interfaces/Constants.h>
#include <vedo/njr/interfaces/Utility.h>
#include <vedo/framework/interfaces/DOModel.h>
#include <cmath>
#include <cstdlib>

#include <stdint.h>

namespace vedo
{

bool DOModel::operator () (const DOModel* p) const
{
	return (sDOName == (p->sDOName));
}

void DOModel::Analysis()
{
	vedo_float_t r, w, h, l, hr, hxo, hyo, dmmi, x, y, z;
	switch (eType)
	{
		case Sphere:
			r                        = uAttributes.sphere.radius;
			dRange                   = r;
			dVolume                  = njr::dFourthThirdsPI * r * r * r;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			dmmi                     = 0.4 * dMass * r * r;
			vMassMomentInertia       = njr::Vector3d(dmmi, dmmi, dmmi);
			break;
		case Ellipsoid:
			x                        = uAttributes.ellipsoid.xlength;
			y                        = uAttributes.ellipsoid.ylength;
			z                        = uAttributes.ellipsoid.zlength;
			dRange                   = 0.5 * std::max(std::max(x, y), z);
			dVolume		             = njr::dOneSixthPI * x * y * z;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			vMassMomentInertia       = 0.05 * dMass * njr::Vector3d(y * y + z * z, z * z + x * x, x * x + y * y);
			break;
		case QuasiCylinder:
			r                        = uAttributes.quasicylinder.radius;
			h                        = uAttributes.quasicylinder.height;

			if(eScope == local)
                dRange               = std::max(r, (vedo_float_t)0.5 * h);
            else
                dRange               = r;

			dVolume                  = njr::dPI * r * r * h;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			dmmi                     = dMass * (3.0 * r * r + h * h) / 12.0;
			vMassMomentInertia       = njr::Vector3d(dmmi, dmmi, 0.5 * dMass * r * r);
			break;
		case QuasiPlate:
			w                        = uAttributes.quasiplate.width;
			h                        = uAttributes.quasiplate.height;
			l                        = uAttributes.quasiplate.length;

			if (eScope == local)
			{
				dRange               = 0.5 * std::max(std::max(w, h), l);
			}
			else
			{
				dRange               = 0.5 * h;
			}

			dVolume                  = w * h * l;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			vMassMomentInertia       = dMass / 12.0 * njr::Vector3d(h * h + l * l, w * w + l * l, h * h + w * w);
			break;
		case QuasiPlateWithCircularHole:
			w                        = uAttributes.quasiplatewithcircularhole.width;
			h                        = uAttributes.quasiplatewithcircularhole.height;
			l                        = uAttributes.quasiplatewithcircularhole.length;
			hr                       = uAttributes.quasiplatewithcircularhole.holeradius;
			hxo                      = uAttributes.quasiplatewithcircularhole.holexoffset;
			hyo                      = uAttributes.quasiplatewithcircularhole.holeyoffset;

			if (eScope == local)
			{
				dRange               = 0.5 * std::max(std::max(w, h), l);
			}
            else
            {
				dRange               = 0.5 * h;
			}

            if ((std::abs(hxo) <= (0.5 * w - hr)) && (std::abs(hyo) <= (0.5 * l - hr)))
            {
                dVolume              = w * h * l - hr * hr * njr::dPI * h;
            }
            else
            {
                std::cerr << "Error!! Code: DOModel::Analysis()" << std::endl;
                exit(-1);
            }

			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			vMassMomentInertia       = njr::Vector3d();   // Need to be modified.
			break;
		case Polyhedra:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = njr::Vector3d();   // Need to be modified.
			break;
		case DMSphere:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = njr::Vector3d();   // Need to be modified.
			break;
		case PolyhedraBRep:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = njr::Vector3d();   // Need to be modified.
			break;
		default:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = njr::Vector3d();
	}
}

DOModel::DOModel(std::ifstream& idof)
{
	*this << idof;
	Analysis();
}

DOModel::DOModel(const std::string& DOName) : matAttributes(0)
{
	sDOName = DOName;
}

DOModel::DOModel
	(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
	 const vedo_float_t& Density, const vedo_float_t& DensityFactor, const njr::Vector3d& ExternalForce,
	 const DOShapeType& type, const DOShapeAttributes& attributes, const DOShapeColor& color                         ):
		matAttributes(0)
{
	sDOName        = DOName;
	sDOGroup       = DOGroup;
	eBehavior      = Behavior;
	eScope         = Scope;
	dDensity       = Density;
	dDensityFactor = DensityFactor;
	vExternalForce = ExternalForce;
	eColor         = color;
	eType          = type;
	uAttributes    = attributes;
	Analysis();
}

DOModel::DOModel
	(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
	 const vedo_float_t& Density, const vedo_float_t& DensityFactor, const njr::Vector3d& ExternalForce,
	 const DOShapeType& type, const DOShapeAttributes& attributes, const DOShapeColor& color,
	 const std::vector<DOMaterialAttribute>& mats                                                                    ):
		matAttributes(0)
{
	sDOName        = DOName;
	sDOGroup       = DOGroup;
	eBehavior      = Behavior;
	eScope         = Scope;
	dDensity       = Density;
	dDensityFactor = DensityFactor;
	vExternalForce = ExternalForce;
	eColor         = color;
	eType          = type;
	uAttributes    = attributes;
	matAttributes  = mats;
	Analysis();
}

DOModel::DOModel
	(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
	 const vedo_float_t& Density, const vedo_float_t& DensityFactor, const njr::Vector3d& ExternalForce,
	 const njr::NJRpolyhedra& poly, const DOShapeColor& color                                                        ):
		matAttributes(0)
{
	sDOName        = DOName;
	sDOGroup       = DOGroup;
	eBehavior      = Behavior;
	eScope         = Scope;
	dDensity       = Density;
	dDensityFactor = DensityFactor;
	vExternalForce = ExternalForce;
	eColor         = color;
	eType          = Polyhedra;
	polyhedra      = poly;
	Analysis();
}

DOModel::DOModel
	(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
	 const vedo_float_t& Density, const vedo_float_t& DensityFactor, const njr::Vector3d& ExternalForce,
	 const njr::NJRpolyhedra& poly, const DOShapeColor& color, const std::vector<DOMaterialAttribute>& mats          ):
		matAttributes(0)
{
	sDOName        = DOName;
	sDOGroup       = DOGroup;
	eBehavior      = Behavior;
	eScope         = Scope;
	dDensity       = Density;
	dDensityFactor = DensityFactor;
	vExternalForce = ExternalForce;
	eColor         = color;
	eType          = Polyhedra;
	polyhedra      = poly;
	matAttributes  = mats;
	Analysis();
}

DOModel::DOModel(const DOModel& doml): matAttributes(0)
{
	*this = doml;
}

const DOModel& DOModel::operator = (const DOModel& doml)
{
	sDOName            = doml.sDOName;
	sDOGroup           = doml.sDOGroup;
	eBehavior          = doml.eBehavior;
	eScope             = doml.eScope;
	dDensity           = doml.dDensity;
	dDensityFactor     = doml.dDensityFactor;
	vExternalForce     = doml.GetExternalForce();
	dMass              = doml.dMass;
	dSudoMass          = doml.dSudoMass;
	dVolume            = doml.dVolume;
	vMassMomentInertia = doml.vMassMomentInertia;
	eColor             = doml.eColor;
	eType              = doml.eType;
	uAttributes        = doml.uAttributes;
	matAttributes      = doml.matAttributes;
	polyhedra          = doml.polyhedra;
	return *this;
}

std::ofstream& DOModel::operator >> (std::ofstream& idof) const
{
	vedo_uint_t MatOptSize,i;
	DOMaterialAttribute DMO;

	njr::WriteString(sDOName  , idof);
	njr::WriteString(sDOGroup , idof);
	idof.write((const char*) &eBehavior     , sizeof(vedo_uint_t      ));
	idof.write((const char*) &eScope        , sizeof(vedo_uint_t      ));
	idof.write((const char*) &dDensity,       sizeof(vedo_float_t     ));
	idof.write((const char*) &dDensityFactor, sizeof(vedo_float_t     ));
	idof.write((const char*) &vExternalForce, sizeof(njr::Vector3d    ));
	idof.write((const char*) &eColor        , sizeof(vedo_uint_t      ));
	idof.write((const char*) &eType         , sizeof(vedo_uint_t      ));
	idof.write((const char*) &uAttributes   , sizeof(DOShapeAttributes));

	MatOptSize = (vedo_uint_t) matAttributes.size();
	idof.write((const char*) &MatOptSize    , sizeof(vedo_uint_t      ));
	for (i=0; i<MatOptSize; i++)
	{
		DMO=matAttributes[i];
		njr::WriteString(DMO.Name,idof);
		idof.write((const char*) &(DMO.Value), sizeof(vedo_float_t    ));
	}

	if (eType == Polyhedra)
	{
		vedo_uint_t numhf = (vedo_uint_t)(polyhedra.constrains().size());
		std::vector<njr::HalfSpace> constrains = polyhedra.constrains();

		idof.write ((char*) &numhf, sizeof(vedo_uint_t));

		for (i=0; i<numhf; i++)
		{
			idof.write((char*) & (constrains[i]), sizeof(njr::HalfSpace));
		}
	}

	return idof;
}

std::ifstream& DOModel::operator << (std::ifstream &idof)
{
	vedo_uint_t MatOptSize, i;
	DOMaterialAttribute DMO;

	njr::ReadString(sDOName  , idof);
	njr::ReadString(sDOGroup , idof);
 	idof.read((char*) &eBehavior     , sizeof(vedo_uint_t      ));
 	idof.read((char*) &eScope        , sizeof(vedo_uint_t      ));
	idof.read((char*) &dDensity      , sizeof(vedo_float_t     ));
	idof.read((char*) &dDensityFactor, sizeof(vedo_float_t     ));
	idof.read((char*) &vExternalForce, sizeof(njr::Vector3d    ));
 	idof.read((char*) &eColor        , sizeof(vedo_uint_t      ));
	idof.read((char*) &eType         , sizeof(vedo_uint_t      ));
	idof.read((char*) &uAttributes   , sizeof(DOShapeAttributes));

	idof.read((char*) &MatOptSize    , sizeof(vedo_uint_t      ));
	for (i=0; i<MatOptSize; i++)
	{
		njr::ReadString(DMO.Name, idof);
		idof.read((char*) &(DMO.Value), sizeof(vedo_float_t));
		matAttributes.push_back(DMO);
	}

	if (eType == Polyhedra)
	{
		vedo_uint_t numhf;
		njr::HalfSpace hf;
		polyhedra.Clear();

		idof.read((char*) &numhf, sizeof (vedo_uint_t));

		for (i=0; i<numhf; i++)
		{
			idof.read((char*) &hf, sizeof(njr::HalfSpace));
			polyhedra.AddConstrain(hf);
		}
	}
	return idof;
}

vedo_float_t DOModel::GetMaterialAttribute(std::string Name) const
{
	for (vedo_uint_t i=0; i<matAttributes.size(); i++)
	{
		if (matAttributes[i].Name == Name)
		{
			return matAttributes[i].Value;
		}
	}

	std::cout << "Model " << sDOName.c_str() << "contains no such material option -- " << Name.c_str() << std::endl;

	return 0.0;
}

vedo_float_t DOModel::CrossAreaToSurface
	(const njr::Vector3d& vP, const vedo_float_t& a, const vedo_float_t& b, const vedo_float_t& c, const vedo_float_t& d) const
{
	vedo_float_t dSphere2Surface, r;
	njr::Vector3d vSurfaceNormal(a, b, c);   // Surface: ax+by+cz=d
	switch (eType)
	{
		case Sphere:
			r               = uAttributes.sphere.radius;
			dSphere2Surface = fabs(d - (vP%vSurfaceNormal)/vSurfaceNormal.length());
			if (dSphere2Surface >= r)
			{
				return 0.0;
			}
			else
			{
				return (r * r - dSphere2Surface * dSphere2Surface) * njr::dPI;
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2007/08/11)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return 0.0;
			break;
		default:
			return 0.0;
	}
}

void FixBoundaryForVolumeInsideBoundary
	(vedo_float_t* L, vedo_float_t* U, const vedo_float_t* P, const vedo_float_t* R, const vedo_float_t* MS)
{
	if (*L < (*P)-(*R))
	{
		*L += (vedo_float_t)(vedo::vedo_uint_t)((*P - *R - *L) / *MS) * (*MS);
		if (*U > *P + *R)
		{
			*U = *L + (vedo_float_t)((vedo::vedo_uint_t)((2.0 * (*R))/ *MS) + 1) * (*MS);
		}
	}
	else if (*L < *P + *R)
	{
		if (*U > *P + *R)
		{
			*U = *L + (vedo_float_t)((vedo::vedo_uint_t)((*P + *R - *L) / *MS) + 1) * (*MS);
		}
	}
	else
	{
		*U = *L;
	}
}

std::pair<vedo_float_t, njr::Vector3d> DOModel::VolumeInsideBoundary
	(const njr::Vector3d& vP, const Boundary* pBC, const vedo_float_t& dMeshSize) const
{
	vedo_float_t Lx = pBC->GetLowerPoint().x();
	vedo_float_t Ly = pBC->GetLowerPoint().y();
	vedo_float_t Lz = pBC->GetLowerPoint().z();
	vedo_float_t Ux = pBC->GetUpperPoint().x();
	vedo_float_t Uy = pBC->GetUpperPoint().y();
	vedo_float_t Uz = pBC->GetUpperPoint().z();
	vedo_float_t Px = vP.x();
	vedo_float_t Py = vP.y();
	vedo_float_t Pz = vP.z();
	vedo_float_t R;

	switch (eType)
	{
		case Sphere:
			R = uAttributes.sphere.radius;
			if ((Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R)) || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)))
			{
				return std::make_pair(0.0, njr::ZERO);
			}
			else if ((Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R)) && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)))
			{
				return std::make_pair(dVolume, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Lx, &Ux, &Px, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Ly, &Uy, &Py, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Lz, &Uz, &Pz, &R, &dMeshSize);

				njr::Vector3d vT;
				vedo_float_t dVolumeInsideBoundary = 0.0;
				vedo_float_t dMeshVolume  = pow(dMeshSize, 3.0);
				vedo_float_t dMassCenterX = 0.0;
				vedo_float_t dMassCenterY = 0.0;
				vedo_float_t dMassCenterZ = 0.0;

				for (vedo_float_t Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for (vedo_float_t Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
					{
						for (vedo_float_t Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
						{
							vT.Set(Tx, Ty, Tz);
							if ((vP-vT).length() < R)
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
				}
				return std::make_pair(dVolumeInsideBoundary, njr::Vector3d(dMassCenterX, dMassCenterY, dMassCenterZ));
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		default:
			return std::make_pair(0.0, njr::ZERO);
	}
}

std::pair<vedo_float_t, njr::Vector3d> DOModel::ProjectedAreaOnXYPlane
	(const njr::Vector3d& vP, const Boundary* pBC, const vedo_float_t& dMeshSize) const
{
	vedo_float_t Lx = pBC->GetLowerPoint().x();
	vedo_float_t Ly = pBC->GetLowerPoint().y();
	vedo_float_t Lz = pBC->GetLowerPoint().z();
	vedo_float_t Ux = pBC->GetUpperPoint().x();
	vedo_float_t Uy = pBC->GetUpperPoint().y();
	vedo_float_t Uz = pBC->GetUpperPoint().z();
	vedo_float_t Px = vP.x();
	vedo_float_t Py = vP.y();
	vedo_float_t Pz = vP.z();
	vedo_float_t R;

	switch (eType)
	{
		case Sphere:
			R = uAttributes.sphere.radius;
			if ((Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R)) || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)))
			{
				return std::make_pair(0.0, njr::ZERO);
			}
			else if ((Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R)) && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)))
			{
				return std::make_pair(R * R * njr::dPI, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Lx, &Ux, &Px, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Ly, &Uy, &Py, &R, &dMeshSize);
				njr::Vector3d vT;
				vedo_float_t dProjectedArea = 0.0;
				vedo_float_t dMeshArea      = dMeshSize * dMeshSize;
				vedo_float_t dCentroidX     = 0.0;
				vedo_float_t dCentroidY     = 0.0;

				for (vedo_float_t Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for (vedo_float_t Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
					{
						if (sqrt((Tx-Px)*(Tx-Px) + (Ty-Py)*(Ty-Py)) < R)
						{
							dProjectedArea +=      dMeshArea;
							dCentroidX     += Tx * dMeshArea;
							dCentroidY     += Ty * dMeshArea;
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
				}
				return std::make_pair(dProjectedArea, njr::Vector3d(dCentroidX, dCentroidY, vP.z()));
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		default:
			return std::make_pair(0.0, njr::ZERO);
	}
}

std::pair<vedo_float_t, njr::Vector3d> DOModel::ProjectedAreaOnYZPlane
	(const njr::Vector3d& vP, const Boundary* pBC, const vedo_float_t& dMeshSize) const
{
	vedo_float_t Lx = pBC->GetLowerPoint().x();
	vedo_float_t Ly = pBC->GetLowerPoint().y();
	vedo_float_t Lz = pBC->GetLowerPoint().z();
	vedo_float_t Ux = pBC->GetUpperPoint().x();
	vedo_float_t Uy = pBC->GetUpperPoint().y();
	vedo_float_t Uz = pBC->GetUpperPoint().z();
	vedo_float_t Px = vP.x();
	vedo_float_t Py = vP.y();
	vedo_float_t Pz = vP.z();
	vedo_float_t R;

	switch (eType)
	{
		case Sphere:
			R = uAttributes.sphere.radius;
			if ((Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R)) || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)))
			{
				return std::make_pair(0.0, njr::ZERO);
			}
			else if ((Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R)) && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)))
			{
				return std::make_pair(R * R * njr::dPI, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Ly, &Uy, &Py, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Lz, &Uz, &Pz, &R, &dMeshSize);

				njr::Vector3d vT;
				vedo_float_t  dProjectedArea = 0.0;
				vedo_float_t  dMeshArea      = dMeshSize * dMeshSize;
				vedo_float_t  dCentroidY     = 0.0;
				vedo_float_t  dCentroidZ     = 0.0;

				for (vedo_float_t Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
				{
					for (vedo_float_t Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
					{
						if (sqrt((Ty-Py)*(Ty-Py) + (Tz-Pz)*(Tz-Pz)) < R)
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
				}
				return std::make_pair(dProjectedArea, njr::Vector3d(vP.x(), dCentroidY, dCentroidZ));
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		default:
			return std::make_pair(0.0, njr::ZERO);
	}
}

std::pair<vedo_float_t, njr::Vector3d> DOModel::ProjectedAreaOnXZPlane
	(const njr::Vector3d& vP, const Boundary* pBC, const vedo_float_t& dMeshSize) const
{
	vedo_float_t Lx = pBC->GetLowerPoint().x();
	vedo_float_t Ly = pBC->GetLowerPoint().y();
	vedo_float_t Lz = pBC->GetLowerPoint().z();
	vedo_float_t Ux = pBC->GetUpperPoint().x();
	vedo_float_t Uy = pBC->GetUpperPoint().y();
	vedo_float_t Uz = pBC->GetUpperPoint().z();
	vedo_float_t Px = vP.x();
	vedo_float_t Py = vP.y();
	vedo_float_t Pz = vP.z();
	vedo_float_t R;

	switch (eType)
	{
		case Sphere:
			R = uAttributes.sphere.radius;
			if ((Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R)) || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)))
			{
				return std::make_pair(0.0, njr::ZERO);
			}
			else if ((Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R)) && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)))
			{
				return std::make_pair(R * R * njr::dPI, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Lx, &Ux, &Px, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Lz, &Uz, &Pz, &R, &dMeshSize);

				njr::Vector3d vT;
				vedo_float_t  dProjectedArea = 0.0;
				vedo_float_t  dMeshArea      = dMeshSize * dMeshSize;
				vedo_float_t  dCentroidX     = 0.0;
				vedo_float_t  dCentroidZ     = 0.0;

				for (vedo_float_t Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for (vedo_float_t Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
					{
						if (sqrt((Tx-Px)*(Tx-Px) + (Tz-Pz)*(Tz-Pz)) < R)
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
				}
				return std::make_pair(dProjectedArea, njr::Vector3d(dCentroidX, vP.y(), dCentroidZ));
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, njr::ZERO);
			break;
		default:
			return std::make_pair(0.0, njr::ZERO);
	}
}

}   // namespace vedo
