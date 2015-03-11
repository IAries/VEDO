#ifndef _DOMODEL_H
#define _DOMODEL_H

#include <vedo/njr/interfaces/ACADXF.h>
#include <vedo/njr/interfaces/Polyhedra.h>
#include <vedo/framework/interfaces/Boundary.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <vedo/constants/interfaces/Constants.h>

namespace vedo
{

// Data Type Class
// __DXFColor declared in <Utiltity/acadxf.h>
typedef enum njrdxf::Color DOShapeColor;



#ifdef _STD_CPP_11
	enum DOShapeType   : vedo_uint_t {NoType, Sphere, Ellipsoid, QuasiCylinder, QuasiPlate, QuasiPlateWithCircularHole, Polyhedra, DMSphere, PolyhedraBRep};
	enum DOBehaviorType: vedo_uint_t {NoDOBehaviorType, constrained, fixed, mobile, orbital};
	enum DOScopeType   : vedo_uint_t {NoDOScopeType, global, local};
#else
	enum DOShapeType    {NoType, Sphere, Ellipsoid, QuasiCylinder, QuasiPlate, QuasiPlateWithCircularHole, Polyhedra, DMSphere, PolyhedraBRep};
	enum DOBehaviorType {NoDOBehaviorType, constrained, fixed, mobile, orbital};
	enum DOScopeType    {NoDOScopeType, global, local};
#endif   // _STD_CPP_11



union DOShapeAttributes
{
	struct
	{
	} notype;

	struct
	{
		vedo_float_t radius;
	} sphere;

	struct
	{
		vedo_float_t radius;
		vedo_float_t height;
	} quasicylinder;

	struct
	{
		vedo_float_t width;
		vedo_float_t height;
		vedo_float_t length;
	} quasiplate;

	struct
	{
		vedo_float_t width;
		vedo_float_t height;
		vedo_float_t length;
		vedo_float_t holeradius;
		vedo_float_t holexoffset;
		vedo_float_t holeyoffset;
	} quasiplatewithcircularhole;

	struct
	{
		vedo_float_t xlength;
		vedo_float_t ylength;
		vedo_float_t zlength;
	} ellipsoid;

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
	vedo_float_t Value;

	DOMaterialAttribute ()
	{
		Name  = "NoName";
		Value = 0.0;
	}

	explicit DOMaterialAttribute (std::string n, vedo_float_t v)
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
		 const vedo_float_t& Density, const vedo_float_t& DensityFactor, const njr::Vector3d& ExternalForce,
		 const DOShapeType&, const DOShapeAttributes&, const DOShapeColor&                                               );

	DOModel
		(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
		 const vedo_float_t& Density, const vedo_float_t& DensityFactor, const njr::Vector3d& ExternalForce,
		 const DOShapeType&, const DOShapeAttributes&, const DOShapeColor&, const std::vector<DOMaterialAttribute>&      );

	DOModel
		(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
		 const vedo_float_t& Density, const vedo_float_t& DensityFactor, const njr::Vector3d& ExternalForce,
		 const njr::NJRpolyhedra&, const DOShapeColor&                                                                   );

	DOModel
		(const std::string& DOName, const std::string& DOGroup, const DOBehaviorType& Behavior, const DOScopeType& Scope,
		const vedo_float_t& Density, const vedo_float_t& DensityFactor, const njr::Vector3d& ExternalForce,
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
	inline vedo_float_t GetDensity() const
	{
		return dDensity;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline vedo_float_t GetDensityFactor() const
	{
		return dDensityFactor;
	}

	inline njr::Vector3d GetExternalForce() const
	{
		return vExternalForce;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline vedo_float_t GetVolume() const
	{
		return dVolume;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline vedo_float_t GetMass() const
	{
		return dMass;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline vedo_float_t GetSudoMass() const
	{
		return dSudoMass;
	}

	// Only used when DiscreteObject (DOSphere) Initialization
	inline njr::Vector3d GetMassMomentInertia() const
	{
		return vMassMomentInertia;
	}

	inline vedo_float_t GetRange() const
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

	vedo_float_t GetMaterialAttribute(std::string Name) const;

	// Cross area to surface ax+by+cz=d;
	vedo_float_t CrossAreaToSurface
		(const njr::Vector3d& vP,
		 const vedo_float_t& a, const vedo_float_t& b, const vedo_float_t& c, const vedo_float_t& d ) const;

	std::pair<vedo_float_t, njr::Vector3d> VolumeInsideBoundary
		(const njr::Vector3d& vP, const Boundary* pBC, const vedo_float_t& dMeshSize) const;

	std::pair<vedo_float_t, njr::Vector3d> ProjectedAreaOnXYPlane
		(const njr::Vector3d& vP, const Boundary* pBC, const vedo_float_t& dMeshSize) const;

	std::pair<vedo_float_t, njr::Vector3d> ProjectedAreaOnYZPlane
		(const njr::Vector3d& vP, const Boundary* pBC, const vedo_float_t& dMeshSize) const;

	std::pair<vedo_float_t, njr::Vector3d> ProjectedAreaOnXZPlane
		(const njr::Vector3d& vP, const Boundary* pBC, const vedo_float_t& dMeshSize) const;

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
	vedo_float_t                     dDensity;
	vedo_float_t                     dDensityFactor;
	njr::Vector3d                    vExternalForce;
	vedo_float_t                     dMass;
	vedo_float_t                     dSudoMass;
	vedo_float_t                     dVolume;
	vedo_float_t                     dRange;
	njr::Vector3d                    vMassMomentInertia;
	DOShapeColor                     eColor;
	DOShapeType	                     eType;
	DOShapeAttributes                uAttributes;
	std::vector<DOMaterialAttribute> matAttributes;
	njr::NJRpolyhedra                polyhedra;
};

}   // namespace vedo

#endif // _DOMODEL_H
