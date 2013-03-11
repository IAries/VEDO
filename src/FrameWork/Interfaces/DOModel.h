#ifndef _DOMODEL_H
#define _DOMODEL_H

#include <NJR/Interfaces/acadxf.h>
#include <NJR/Interfaces/polyhedra.h>
#include <FrameWork/Interfaces/Boundary.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Data Type Class
// __DXFColor declared in <Utiltity/acadxf.h>
typedef enum NJRDXF::Color DOShapeColor;

enum DOShapeType
{
	NoType        = 0,
	Sphere        = 1,
	Ellipsoid     = 2,
	QuasiCylinder = 3,
	QuasiPlate    = 4,
	Polyhedra     = 5,
	DMSphere      = 6,
	PolyhedraBRep = 7
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
		const std::string& Behavior,
		const std::string& Scope,
		const double& Density,
		const double& DensityFactor,
		const NJRvector3d& ExternalForce,
		const DOShapeType&,
		const DOShapeAttributes&,
		const DOShapeColor&);

	DOModel
		(const std::string& DOName,
		const std::string& DOGroup,
		const std::string& Behavior,
		const std::string& Scope,
		const double& Density,
		const double& DensityFactor,
		const NJRvector3d& ExternalForce,
		const DOShapeType&,
		const DOShapeAttributes&,
		const DOShapeColor&,
		const std::vector<DOMaterialAttribute>&);

	DOModel
		(const std::string& DOName,
		const std::string& DOGroup,
		const std::string& Behavior,
		const std::string& Scope,
		const double& Density,
		const double& DensityFactor,
		const NJRvector3d& ExternalForce,
		const NJRpolyhedra&,
		const DOShapeColor&);

	DOModel
		(const std::string& DOName,
		const std::string& DOGroup,
		const std::string& Behavior,
		const std::string& Scope,
		const double& Density,
		const double& DensityFactor,
		const NJRvector3d& ExternalForce,
		const NJRpolyhedra&,
		const DOShapeColor&,
		const std::vector<DOMaterialAttribute>&);

	DOModel(const DOModel& doml);

	DOModel(std::ifstream& idof);

	DOModel(std::ifstream& idof, unsigned int _version);

	inline std::string GetDOName() const
	{
		return sDOName;
	};

	inline std::string GetDOGroup() const
	{
		return sDOGroup;
	};

	inline std::string GetBehavior() const
	{
		return sBehavior;
	};

	inline std::string GetScope() const
	{
		return sScope;
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

	inline NJRvector3d GetExternalForce() const
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
	inline NJRvector3d GetMassMomentInertia() const
	{
		return vMassMomentInertia;
	};

	inline double GetRange() const
	{
		return dRange;
	};

	inline DOShapeColor GetShapeColor() const
	{
		return shColor;
	};

	inline DOShapeType GetShapeType() const
	{
		return shType;
	};

	inline DOShapeAttributes GetShapeAttributes() const
	{
		return shAttributes;
	};

	inline const std::vector<DOMaterialAttribute>& GetMaterialAttributes() const
	{
		return matAttributes;
	};

	inline const NJRpolyhedra& GetPolyhedra() const
	{
		return polyhedra;
	};

	double GetMaterialAttribute(std::string Name) const;

	// Cross area to surface ax+by+cz=d;
	double CrossAreaToSurface
		(const NJRvector3d& vP,
		 const double&      a,
		 const double&      b,
		 const double&      c,
		 const double&      d ) const;

	std::pair<double, NJRvector3d> VolumeInsideBoundary
		(const NJRvector3d& vP,
		 const Boundary*    pBC,
		 const double&      dMeshSize) const;

	std::pair<double, NJRvector3d> ProjectedAreaOnXYPlane
		(const NJRvector3d& vP,
		 const Boundary*    pBC,
		 const double&      dMeshSize) const;

	std::pair<double, NJRvector3d> ProjectedAreaOnYZPlane
		(const NJRvector3d& vP,
		 const Boundary*    pBC,
		 const double&      dMeshSize) const;

	std::pair<double, NJRvector3d> ProjectedAreaOnXZPlane
		(const NJRvector3d& vP,
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

	std::string	                 sDOName;
	std::string	                 sDOGroup;
	std::string	                 sBehavior;
	std::string	                 sScope;
	double                           dDensity;
	double                           dDensityFactor;
	NJRvector3d                      vExternalForce;
	double                           dMass;
	double                           dSudoMass;
	double                           dVolume;
	double                           dRange;
	NJRvector3d                      vMassMomentInertia;
	DOShapeColor                     shColor;
	DOShapeType	                 shType;
	DOShapeAttributes                shAttributes;
	std::vector<DOMaterialAttribute> matAttributes;
	NJRpolyhedra                     polyhedra;
};

#endif // _DOMODEL_H
