#ifndef _GEOMETRIC_SHAPE_H
#define _GEOMETRIC_SHAPE_H

#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/DOStatus.h>
#include <list>
#include <string>

namespace VEDO
{

class GeometricShape
{

public:

	virtual ~GeometricShape();

	GeometricShape(const std::string& type, const std::string& name);

	GeometricShape(const GeometricShape&);

	void operator = (const GeometricShape&);

	inline std::string GetType() const
	{
		return _sType;
	};

	inline std::string GetName() const
	{
		return _sName;
	};

	inline std::list<GeometricShape*> GetListGS() const
	{
		return _ListGS;
	};

	inline DOStatus* GetStatus() const
	{
		return _Status;
	};

	inline void SetStatus(DOStatus* s)
	{
		*_Status = *s;
	};

	virtual void Add(GeometricShape*);

	virtual void Remove(GeometricShape*);

	virtual GeometricShape* GetGeometricShape(std::string&);

	virtual bool Inside(const NJR::NJRvector3d&) = 0;

protected:

	GeometricShape();
	std::string                _sType;
	std::string                _sName;
	std::list<GeometricShape*> _ListGS;
	DOStatus*                  _Status;
};

};   // namespace VEDO



std::ostream& operator << (std::ostream& os, VEDO::GeometricShape& gs);

#endif // _GEOMETRIC_SPHERE_H
