#ifndef _DOMODEL_H
#define _DOMODEL_H

#include <vedo/njr/interfaces/ACADXF.h>
#include <vedo/njr/interfaces/Polyhedra.h>
#include <vedo/framework/interfaces/Boundary.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace vedo
{

// Data Type Class
// __DXFColor declared in <Utiltity/acadxf.h>
typedef enum njrdxf::Color DOShapeColor;



enum DOShapeType
{
	NoType,
	Sphere,
	Ellipsoid,
	QuasiCylinder,
	QuasiPlate,
	QuasiPlateWithCircularHole,
	Polyhedra,
	DMSphere,
	PolyhedraBRep
};



enum DOBehaviorType
{
	NoDOBehaviorType,
	constrained,
	fixed,
	mobile,
	orbital
};



enum DOScopeType
{
	NoDOScopeType,
	global,
	local
};



union DOShapeAttributes
{
	struct
	{
		double radius;
	} sphere;

	struct
	{
		double radius;
		double height;
	} quasicylinder;

	struct
	{
		double width;
		double height;
		double length;
	} quasiplate;

	struct
	{
		double width;
		double height;
		double length;
		double holeradius;
		double holexoffset;
		double holeyoffset;
	} quasiplatewithcircularhole;

	struct
	{
		double xlength;
		double ylength;
		double zlength;
	} ellipsoid;

	struct
	{
	} polyhedrabrep;
};



struct DOMaterialAttribute
{
	std::string Name;
	double Value;

	DOMaterialAttribute ()
	{
		Name  = "NoName";
		Value = 0.0;
	};

	explicit DOMaterialAttribute (std::string n, double v)
	{
		Name  = n;
		Value = v;
	};
};



class DOModel
{

public:

	// Function for algorithm std::find_if
	explicit DOModel(const std::string& DOname);

	DOModel
		(const std::string& DOName,
		const std::string& DOGroup,
		const DOBehaviorType& Behavior,
		const DOScopeType& Scope,
		const double& Density,
		const double& DensityFactor,
		const njr::Vector3d& ExternalForce,
		const DOShapeType&,
		const DOShapeAttributes&,
		const DOShapeColor&);

	DOModel
		(const std::string& DOName,
		const std::string& DOGroup,
		const DOBehaviorType& Behavior,
		const DOScopeType& Scope,
		const double& Density,
		const double& DensityFactor,
		const njr::Vector3d& ExternalForce,
		const DOShapeType&,
		const DOShapeAttributes&,
		const DOShapeColor&,
		const std::vector<DOMaterialAttribute>&);

	DOModel
		(const std::string& DOName,
		const std::string& DOGroup,
		const DOBehaviorType& Behavior,
		const DOScopeType& Scope,
		const double& Density,
		const double& DensityFactor,
		const njr::Vector3d& ExternalForce,
		const njr::NJRpolyhedra&,
		const DOShapeColor&);

	DOModel
		(const std::string& DOName,
		const std::string& DOGroup,
		const DOBehaviorType& Behavior,
		const DOScopeType& Scope,
		const double& Density,
		const double& DensityFactor,
		const njr::Vector3d& ExternalForce,
		const njr::NJRpolyhedra&,
		const DOShapeColor&,
		const std::vector<DOMaterialAttribute>&);

	DOModel(const DOModel& doml);

	DOModel(std::ifstream& idof);

	inline std::string GetDOName() const
	{
		return sDOName;
	};

	inline std::string GetDOGroup() const
	{
		return sDOGroup;
	};

	inline DOBehaviorType GetBehavior() const
	{
		return eBehavior;
	};

	inline DOScopeType GetScope() const
	{
		return eScope;
	};

	// Only used when DiscreteObject (DOSphere) Initialization
	inline double GetDensity() const
	{
		return dDensity;
	};

	// Only used when DiscreteObject (DOSphere) Initialization
	inline double GetDensityFactor() const
	{
		return dDensityFactor;
	};

	inline njr::Vector3d GetExternalForce() const
	{
		return vExternalForce;
	};

	// Only used when DiscreteObject (DOSphere) Initialization
	inline double GetVolume() const
	{
		return dVolume;
	};

	// Only used when DiscreteObject (DOSphere) Initialization
	inline double GetMass() const
	{
		return dMass;
	};

	// Only used when DiscreteObject (DOSphere) Initialization
	inline double GetSudoMass() const
	{
		return dSudoMass;
	};

	// Only used when DiscreteObject (DOSphere) Initialization
	inline njr::Vector3d GetMassMomentInertia() const
	{
		return vMassMomentInertia;
	};

	inline double GetRange() const
	{
		return dRange;
	};

	inline DOShapeColor GetShapeColor() const
	{
		return eColor;
	};

	inline DOShapeType GetShapeType() const
	{
		return eType;
	};

	inline DOShapeAttributes GetShapeAttributes() const
	{
		return uAttributes;
	};

	inline const std::vector<DOMaterialAttribute>& GetMaterialAttributes() const
	{
		return matAttributes;
	};

	inline const njr::NJRpolyhedra& GetPolyhedra() const
	{
		return polyhedra;
	};

	double GetMaterialAttribute(std::string Name) const;

	// Cross area to surface ax+by+cz=d;
	double CrossAreaToSurface
		(const njr::Vector3d& vP,
		 const double&      a,
		 const double&      b,
		 const double&      c,
		 const double&      d ) const;

	std::pair<double, njr::Vector3d> VolumeInsideBoundary
		(const njr::Vector3d& vP,
		 const Boundary*    pBC,
		 const double&      dMeshSize) const;

	std::pair<double, njr::Vector3d> ProjectedAreaOnXYPlane
		(const njr::Vector3d& vP,
		 const Boundary*    pBC,
		 const double&      dMeshSize) const;

	std::pair<double, njr::Vector3d> ProjectedAreaOnYZPlane
		(const njr::Vector3d& vP,
		 const Boundary*    pBC,
		 const double&      dMeshSize) const;

	std::pair<double, njr::Vector3d> ProjectedAreaOnXZPlane
		(const njr::Vector3d& vP,
		 const Boundary*    pBC,
		 const double&      dMeshSize) const;

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
	double                           dDensity;
	double                           dDensityFactor;
	njr::Vector3d                    vExternalForce;
	double                           dMass;
	double                           dSudoMass;
	double                           dVolume;
	double                           dRange;
	njr::Vector3d                    vMassMomentInertia;
	DOShapeColor                     eColor;
	DOShapeType	                     eType;
	DOShapeAttributes                uAttributes;
	std::vector<DOMaterialAttribute> matAttributes;
	njr::NJRpolyhedra                polyhedra;
};

};   // namespace vedo

#endif // _DOMODEL_H
