#include <aries/utility/BinaryIO.h>
#include <aries/utility/Constants.h>
#include <vedo/framework/DOModel.h>
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
	_float_t r, w, h, l, hr, hxo, hyo, dmmi, x, y, z;
	switch (eType)
	{
		case Sphere:
			r                        = uAttributes.sphere.radius;
			dRange                   = r;
			dVolume                  = aries::math::_FourthThirdsPI * r * r * r;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			dmmi                     = 0.4 * dMass * r * r;
			vMassMomentInertia       = aries::Vector3df(dmmi, dmmi, dmmi);
			break;
		case Ellipsoid:
			x                        = uAttributes.ellipsoid.xlength;
			y                        = uAttributes.ellipsoid.ylength;
			z                        = uAttributes.ellipsoid.zlength;
			dRange                   = 0.5 * std::max(std::max(x, y), z);
			dVolume		             = aries::math::_OneSixthPI * x * y * z;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			vMassMomentInertia       = 0.05 * dMass * aries::Vector3df(y * y + z * z, z * z + x * x, x * x + y * y);
			break;
		case QuasiCylinder:
			r                        = uAttributes.quasicylinder.radius;
			h                        = uAttributes.quasicylinder.height;

			if(eScope == local)
                dRange               = std::max(r, (_float_t)0.5 * h);
            else
                dRange               = r;

			dVolume                  = aries::math::_PI * r * r * h;
			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			dmmi                     = dMass * (3.0 * r * r + h * h) / 12.0;
			vMassMomentInertia       = aries::Vector3df(dmmi, dmmi, 0.5 * dMass * r * r);
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
			vMassMomentInertia       = dMass / 12.0 * aries::Vector3df(h * h + l * l, w * w + l * l, h * h + w * w);
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
                dVolume              = w * h * l - hr * hr * aries::math::_PI * h;
            }
            else
            {
                std::cerr << "Error!! Code: DOModel::Analysis()" << std::endl;
                exit(-1);
            }

			dMass                    = dDensity * dVolume;
			dSudoMass                = dDensityFactor * dMass;
			vMassMomentInertia       = aries::Vector3df();   // Need to be modified.
			break;
		case Polyhedra:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = aries::Vector3df();   // Need to be modified.
			break;
		case DMSphere:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = aries::Vector3df();   // Need to be modified.
			break;
		case PolyhedraBRep:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = aries::Vector3df();   // Need to be modified.
			break;
		default:
			dRange                   = 0.0;
			dVolume                  = 0.0;
			dMass                    = 0.0;
			dSudoMass                = 0.0;
			vMassMomentInertia       = aries::Vector3df();
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
	 const _float_t& Density, const _float_t& DensityFactor, const aries::Vector3df& ExternalForce,
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
	 const _float_t& Density, const _float_t& DensityFactor, const aries::Vector3df& ExternalForce,
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
	 const _float_t& Density, const _float_t& DensityFactor, const aries::Vector3df& ExternalForce,
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
	 const _float_t& Density, const _float_t& DensityFactor, const aries::Vector3df& ExternalForce,
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
	_uint_t MatOptSize,i;
	DOMaterialAttribute DMO;

	aries::WriteString(sDOName , idof);
	aries::WriteString(sDOGroup, idof);
	idof.write((const char*) &eBehavior     , sizeof(_uint_t      ));
	idof.write((const char*) &eScope        , sizeof(_uint_t      ));
	idof.write((const char*) &dDensity,       sizeof(_float_t     ));
	idof.write((const char*) &dDensityFactor, sizeof(_float_t     ));
	idof.write((const char*) &vExternalForce, sizeof(aries::Vector3df    ));
	idof.write((const char*) &eColor        , sizeof(_uint_t      ));
	idof.write((const char*) &eType         , sizeof(_uint_t      ));
	idof.write((const char*) &uAttributes   , sizeof(DOShapeAttributes));

	MatOptSize = (_uint_t) matAttributes.size();
	idof.write((const char*) &MatOptSize    , sizeof(_uint_t      ));
	for (i=0; i<MatOptSize; i++)
	{
		DMO=matAttributes[i];
		aries::WriteString(DMO.Name,idof);
		idof.write((const char*) &(DMO.Value), sizeof(_float_t    ));
	}

	if (eType == Polyhedra)
	{
		_uint_t numhf = (_uint_t)(polyhedra.constrains().size());
		std::vector<njr::HalfSpace> constrains = polyhedra.constrains();

		idof.write ((char*) &numhf, sizeof(_uint_t));

		for (i=0; i<numhf; i++)
		{
			idof.write((char*) & (constrains[i]), sizeof(njr::HalfSpace));
		}
	}

	return idof;
}

std::ifstream& DOModel::operator << (std::ifstream &idof)
{
	_uint_t MatOptSize, i;
	DOMaterialAttribute DMO;

	aries::ReadString(sDOName , idof);
	aries::ReadString(sDOGroup, idof);
 	idof.read((char*) &eBehavior     , sizeof(_uint_t      ));
 	idof.read((char*) &eScope        , sizeof(_uint_t      ));
	idof.read((char*) &dDensity      , sizeof(_float_t     ));
	idof.read((char*) &dDensityFactor, sizeof(_float_t     ));
	idof.read((char*) &vExternalForce, sizeof(aries::Vector3df    ));
 	idof.read((char*) &eColor        , sizeof(_uint_t      ));
	idof.read((char*) &eType         , sizeof(_uint_t      ));
	idof.read((char*) &uAttributes   , sizeof(DOShapeAttributes));

	idof.read((char*) &MatOptSize    , sizeof(_uint_t      ));
	for (i=0; i<MatOptSize; i++)
	{
		aries::ReadString(DMO.Name, idof);
		idof.read((char*) &(DMO.Value), sizeof(_float_t));
		matAttributes.push_back(DMO);
	}

	if (eType == Polyhedra)
	{
		_uint_t numhf;
		njr::HalfSpace hf;
		polyhedra.Clear();

		idof.read((char*) &numhf, sizeof (_uint_t));

		for (i=0; i<numhf; i++)
		{
			idof.read((char*) &hf, sizeof(njr::HalfSpace));
			polyhedra.AddConstrain(hf);
		}
	}
	return idof;
}

_float_t DOModel::GetMaterialAttribute(std::string Name) const
{
	for (_uint_t i=0; i<matAttributes.size(); i++)
	{
		if (matAttributes[i].Name == Name)
		{
			return matAttributes[i].Value;
		}
	}

	std::cout << "Model " << sDOName.c_str() << "contains no such material option -- " << Name.c_str() << std::endl;

	return 0.0;
}

_float_t DOModel::CrossAreaToSurface
	(const aries::Vector3df& vP, const _float_t& a, const _float_t& b, const _float_t& c, const _float_t& d) const
{
	_float_t dSphere2Surface, r;
	aries::Vector3df vSurfaceNormal(a, b, c);   // Surface: ax+by+cz=d
	switch (eType)
	{
		case Sphere:
			r               = uAttributes.sphere.radius;
			dSphere2Surface = fabs(d - (vP.dot(vSurfaceNormal))/vSurfaceNormal.length());
			if (dSphere2Surface >= r)
			{
				return 0.0;
			}
			else
			{
				return (r * r - dSphere2Surface * dSphere2Surface) * aries::math::_PI;
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
	(_float_t* L, _float_t* U, const _float_t* P, const _float_t* R, const _float_t* MS)
{
	if (*L < (*P)-(*R))
	{
		*L += (_float_t)(_uint_t)((*P - *R - *L) / *MS) * (*MS);
		if (*U > *P + *R)
		{
			*U = *L + (_float_t)((_uint_t)((2.0 * (*R))/ *MS) + 1) * (*MS);
		}
	}
	else if (*L < *P + *R)
	{
		if (*U > *P + *R)
		{
			*U = *L + (_float_t)((_uint_t)((*P + *R - *L) / *MS) + 1) * (*MS);
		}
	}
	else
	{
		*U = *L;
	}
}

std::pair<_float_t, aries::Vector3df> DOModel::VolumeInsideBoundary
	(const aries::Vector3df& vP, const Boundary* pBC, const _float_t& dMeshSize) const
{
	_float_t Lx = pBC->GetLowerPoint().x();
	_float_t Ly = pBC->GetLowerPoint().y();
	_float_t Lz = pBC->GetLowerPoint().z();
	_float_t Ux = pBC->GetUpperPoint().x();
	_float_t Uy = pBC->GetUpperPoint().y();
	_float_t Uz = pBC->GetUpperPoint().z();
	_float_t Px = vP.x();
	_float_t Py = vP.y();
	_float_t Pz = vP.z();
	_float_t R;

	switch (eType)
	{
		case Sphere:
			R = uAttributes.sphere.radius;
			if ((Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R)) || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)))
			{
				return std::make_pair(0.0, aries::Vector3df());
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

				aries::Vector3df vT;
				_float_t dVolumeInsideBoundary = 0.0;
				_float_t dMeshVolume  = pow(dMeshSize, 3.0);
				_float_t dMassCenterX = 0.0;
				_float_t dMassCenterY = 0.0;
				_float_t dMassCenterZ = 0.0;

				for (_float_t Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for (_float_t Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
					{
						for (_float_t Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
						{
							vT.set(Tx, Ty, Tz);
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
				return std::make_pair(dVolumeInsideBoundary, aries::Vector3df(dMassCenterX, dMassCenterY, dMassCenterZ));
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		default:
			return std::make_pair(0.0, aries::Vector3df());
	}
}

std::pair<_float_t, aries::Vector3df> DOModel::ProjectedAreaOnXYPlane
	(const aries::Vector3df& vP, const Boundary* pBC, const _float_t& dMeshSize) const
{
	_float_t Lx = pBC->GetLowerPoint().x();
	_float_t Ly = pBC->GetLowerPoint().y();
	_float_t Lz = pBC->GetLowerPoint().z();
	_float_t Ux = pBC->GetUpperPoint().x();
	_float_t Uy = pBC->GetUpperPoint().y();
	_float_t Uz = pBC->GetUpperPoint().z();
	_float_t Px = vP.x();
	_float_t Py = vP.y();
	_float_t Pz = vP.z();
	_float_t R;

	switch (eType)
	{
		case Sphere:
			R = uAttributes.sphere.radius;
			if ((Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R)) || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)))
			{
				return std::make_pair(0.0, aries::Vector3df());
			}
			else if ((Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R)) && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)))
			{
				return std::make_pair(R * R * aries::math::_PI, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Lx, &Ux, &Px, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Ly, &Uy, &Py, &R, &dMeshSize);
				aries::Vector3df vT;
				_float_t dProjectedArea = 0.0;
				_float_t dMeshArea      = dMeshSize * dMeshSize;
				_float_t dCentroidX     = 0.0;
				_float_t dCentroidY     = 0.0;

				for (_float_t Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for (_float_t Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
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
				return std::make_pair(dProjectedArea, aries::Vector3df(dCentroidX, dCentroidY, vP.z()));
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		default:
			return std::make_pair(0.0, aries::Vector3df());
	}
}

std::pair<_float_t, aries::Vector3df> DOModel::ProjectedAreaOnYZPlane
	(const aries::Vector3df& vP, const Boundary* pBC, const _float_t& dMeshSize) const
{
	_float_t Lx = pBC->GetLowerPoint().x();
	_float_t Ly = pBC->GetLowerPoint().y();
	_float_t Lz = pBC->GetLowerPoint().z();
	_float_t Ux = pBC->GetUpperPoint().x();
	_float_t Uy = pBC->GetUpperPoint().y();
	_float_t Uz = pBC->GetUpperPoint().z();
	_float_t Px = vP.x();
	_float_t Py = vP.y();
	_float_t Pz = vP.z();
	_float_t R;

	switch (eType)
	{
		case Sphere:
			R = uAttributes.sphere.radius;
			if ((Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R)) || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)))
			{
				return std::make_pair(0.0, aries::Vector3df());
			}
			else if ((Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R)) && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)))
			{
				return std::make_pair(R * R * aries::math::_PI, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Ly, &Uy, &Py, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Lz, &Uz, &Pz, &R, &dMeshSize);

				aries::Vector3df vT;
				_float_t  dProjectedArea = 0.0;
				_float_t  dMeshArea      = dMeshSize * dMeshSize;
				_float_t  dCentroidY     = 0.0;
				_float_t  dCentroidZ     = 0.0;

				for (_float_t Ty=Ly+0.5*dMeshSize; Ty<Uy; Ty+=dMeshSize)
				{
					for (_float_t Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
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
				return std::make_pair(dProjectedArea, aries::Vector3df(vP.x(), dCentroidY, dCentroidZ));
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		default:
			return std::make_pair(0.0, aries::Vector3df());
	}
}

std::pair<_float_t, aries::Vector3df> DOModel::ProjectedAreaOnXZPlane
	(const aries::Vector3df& vP, const Boundary* pBC, const _float_t& dMeshSize) const
{
	_float_t Lx = pBC->GetLowerPoint().x();
	_float_t Ly = pBC->GetLowerPoint().y();
	_float_t Lz = pBC->GetLowerPoint().z();
	_float_t Ux = pBC->GetUpperPoint().x();
	_float_t Uy = pBC->GetUpperPoint().y();
	_float_t Uz = pBC->GetUpperPoint().z();
	_float_t Px = vP.x();
	_float_t Py = vP.y();
	_float_t Pz = vP.z();
	_float_t R;

	switch (eType)
	{
		case Sphere:
			R = uAttributes.sphere.radius;
			if ((Lx>(Px+R)) || (Ly>(Py+R)) || (Lz>(Pz+R)) || (Ux<(Px-R)) || (Uy<(Py-R)) || (Uz<(Pz-R)))
			{
				return std::make_pair(0.0, aries::Vector3df());
			}
			else if ((Lx<=(Px-R)) && (Ly<=(Py-R)) && (Lz<=(Pz-R)) && (Ux>=(Px+R)) && (Uy>=(Py+R)) && (Uz>=(Pz+R)))
			{
				return std::make_pair(R * R * aries::math::_PI, vP);
			}
			else
			{
				FixBoundaryForVolumeInsideBoundary(&Lx, &Ux, &Px, &R, &dMeshSize);
				FixBoundaryForVolumeInsideBoundary(&Lz, &Uz, &Pz, &R, &dMeshSize);

				aries::Vector3df vT;
				_float_t  dProjectedArea = 0.0;
				_float_t  dMeshArea      = dMeshSize * dMeshSize;
				_float_t  dCentroidX     = 0.0;
				_float_t  dCentroidZ     = 0.0;

				for (_float_t Tx=Lx+0.5*dMeshSize; Tx<Ux; Tx+=dMeshSize)
				{
					for (_float_t Tz=Lz+0.5*dMeshSize; Tz<Uz; Tz+=dMeshSize)
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
				return std::make_pair(dProjectedArea, aries::Vector3df(dCentroidX, vP.y(), dCentroidZ));
			}
			break;
		case Ellipsoid:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Ellipsoid, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiCylinder:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiCylinder, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiPlate:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlate, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case QuasiPlateWithCircularHole:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    QuasiPlateWithCircularHole, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case Polyhedra:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    Polyhedra, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case DMSphere:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    DMSphere, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		case PolyhedraBRep:
/******************************************************************************
 * Aries' Debug (2008/08/20)
 *
 *    PolyhedraBRep, need to modify.
 ******************************************************************************/
			return std::make_pair(0.0, aries::Vector3df());
			break;
		default:
			return std::make_pair(0.0, aries::Vector3df());
	}
}

}   // namespace vedo
