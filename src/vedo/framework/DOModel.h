#ifndef _DOMODEL_H
#define _DOMODEL_H

#include <vedo/framework/Boundary.h>
#include <vedo/framework/DataType.h>
#include <njr/ACADXF.h>
#include <njr/Polyhedra.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <aries/utility/Constants.h>

namespace vedo
{

// Data Type Class
// __DXFColor declared in <Utiltity/acadxf.h>
typedef enum njrdxf::Color DOShapeColor;



#ifdef _STD_CPP_11
	enum DOShapeType   : _uint_t {NoShapeType, Sphere, DMSphere, Ellipsoid, Polyhedra, PolyhedraBRep, QuasiCylinder, QuasiPlate, QuasiPlateWithCircularHole, Triangle};
	enum DOBehaviorType: _uint_t {NoBehaviorType, constrained, fixed, mobile, orbital};
	enum DOScopeType   : _uint_t {NoScopeType, global, local};
#else
	enum DOShapeType    {NoShapeType, DMSphere, Ellipsoid, Polyhedra, PolyhedraBRep, QuasiCylinder, QuasiPlate, QuasiPlateWithCircularHole, Sphere, Triangle};
	enum DOBehaviorType {NeBehaviorType, constrained, fixed, mobile, orbital};
	enum DOScopeType    {NoScopeType, global, local};
#endif   // _STD_CPP_11



union DOShapeAttributes
{
	struct
	{
	} NoShapeType;

	struct
	{
		_float_t radius;
	} sphere;

	struct
	{
		_float_t radius;
		_float_t height;
	} quasicylinder;

	struct
	{
		_float_t width;
		_float_t height;
		_float_t length;
	} quasiplate;

	struct
	{
		_float_t width;
		_float_t height;
		_float_t length;
		_float_t holeradius;
		_float_t holexoffset;
		_float_t holeyoffset;
	} quasiplatewithcircularhole;

	struct
	{
		_float_t xlength;
		_float_t ylength;
		_float_t zlength;
	} ellipsoid;

	struct
	{
		_float_t range;
		_float_t point0x;
		_float_t point0y;
		_float_t point0z;
		_float_t point1x;
		_float_t point1y;
		_float_t point1z;
		_float_t point2x;
		_float_t point2y;
		_float_t point2z;
	} triangle;

	struct
	{
	} polyhedra;

	struct
	{
	} dmsphere;

	struct
	{
	} polyhedrabrep;
};



struct DOMaterialAttribute
{
	std::string Name;
	_float_t Value;

	DOMaterialAttribute ()
	{
		Name  = "NoName";
		Value = 0.0;
	}

	explicit DOMaterialAttribute (std::string n, _float_t v)
	{
		Name  = n;
		Value = v;
	}
};



class DOModel
{

public:

	// Function for algorithm std::find_if
	explicit DOModel(const std::string& DOname);

	DOModel
		(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
		 const _float_t& Density, const _float_t& DensityFactor, const Vector3df& ExternalForce,
		 const DOShapeType&, const DOShapeAttributes&, const DOShapeColor&                                               );

	DOModel
		(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
		 const _float_t& Density, const _float_t& DensityFactor, const Vector3df& ExternalForce,
		 const DOShapeType&, const DOShapeAttributes&, const DOShapeColor&, const std::vector<DOMaterialAttribute>&      );

	DOModel
		(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
		 const _float_t& Density, const _float_t& DensityFactor, const Vector3df& ExternalForce,
		 const njr::NJRpolyhedra&, const DOShapeColor&                                                                   );

	DOModel
		(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
		const _float_t& Density, const _float_t& DensityFactor, const Vector3df& ExternalForce,
		const njr::NJRpolyhedra&, const DOShapeColor&, const std::vector<DOMaterialAttribute>&                           );

	DOModel(const DOModel& doml);

	DOModel(std::ifstream& idof);

	inline std::string GetDOName() const
	{
		return sDOName;
	}

	inline std::string GetDOGroup() const
	{
		return sDOGroup;
	}

	inline DOBehaviorType GetBehavior() const
	{
		return eBehavior;
	}

	inline DOScopeType GetScope() const
	{
		return eScope;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline _float_t GetDensity() const
	{
		return dDensity;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline _float_t GetDensityFactor() const
	{
		return dDensityFactor;
	}

	inline Vector3df GetExternalForce() const
	{
		return vExternalForce;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline _float_t GetVolume() const
	{
		return dVolume;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline _float_t GetMass() const
	{
		return dMass;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline _float_t GetSudoMass() const
	{
		return dSudoMass;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline Vector3df GetMassMomentInertia() const
	{
		return vMassMomentInertia;
	}

	inline _float_t GetRange() const
	{
		return dRange;
	}

	inline DOShapeColor GetShapeColor() const
	{
		return eColor;
	}

	inline DOShapeType GetShapeType() const
	{
		return eType;
	}

	inline DOShapeAttributes GetShapeAttributes() const
	{
		return uAttributes;
	}

	inline const std::vector<DOMaterialAttribute>& GetMaterialAttributes() const
	{
		return matAttributes;
	}

	inline const njr::NJRpolyhedra& GetPolyhedra() const
	{
		return polyhedra;
	}

	_float_t GetMaterialAttribute(std::string Name) const;

	// cross area to surface ax+by+cz=d;
	_float_t CrossAreaToSurface
		(const Vector3df& vP,
		 const _float_t& a, const _float_t& b, const _float_t& c, const _float_t& d ) const;

	std::pair<_float_t, Vector3df> VolumeInsideBoundary
		(const Vector3df& vP, const Boundary* pBC, const _float_t& dMeshSize) const;

	std::pair<_float_t, Vector3df> ProjectedAreaOnXYPlane
		(const Vector3df& vP, const Boundary* pBC, const _float_t& dMeshSize) const;

	std::pair<_float_t, Vector3df> ProjectedAreaOnYZPlane
		(const Vector3df& vP, const Boundary* pBC, const _float_t& dMeshSize) const;

	std::pair<_float_t, Vector3df> ProjectedAreaOnXZPlane
		(const Vector3df& vP, const Boundary* pBC, const _float_t& dMeshSize) const;

	const DOModel& operator = (const DOModel& doml);

	bool operator () (const DOModel* p) const;

	// Binary output and input
	std::ofstream& operator >> (std::ofstream& idof) const;

	std::ifstream& operator << (std::ifstream& idof);

protected:

	void Analysis();

private:

	std::string	                     sDOName;
	std::string	                     sDOGroup;
	DOBehaviorType                   eBehavior;
	DOScopeType	                     eScope;
	_float_t                         dDensity;
	_float_t                         dDensityFactor;
	Vector3df                        vExternalForce;
	_float_t                         dMass;
	_float_t                         dSudoMass;
	_float_t                         dVolume;
	_float_t                         dRange;
	Vector3df                        vMassMomentInertia;
	DOShapeColor                     eColor;
	DOShapeType	                     eType;
	DOShapeAttributes                uAttributes;
	std::vector<DOMaterialAttribute> matAttributes;
	njr::NJRpolyhedra                polyhedra;
};

}   // namespace vedo

#endif // _DOMODEL_H
