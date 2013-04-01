#ifndef _GEOMETRIC_SHAPE_H
#define _GEOMETRIC_SHAPE_H

#include <NJR/Interfaces/Vector3d.h>
#include <FrameWork/Interfaces/DOStatus.h>
#include <list>
#include <string>

namespace vedo
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

	virtual bool Inside(const njr::Vector3d&) = 0;

protected:

	GeometricShape();
	std::string                _sType;
	std::string                _sName;
	std::list<GeometricShape*> _ListGS;
	DOStatus*                  _Status;
};

};   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GeometricShape& gs);

#endif // _GEOMETRIC_SPHERE_H
