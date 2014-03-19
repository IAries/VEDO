#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
{

GeometricShape::GeometricShape(): _sType("Geometric Shape"), _sName("Null")
{
	_Status = new DOStatus("No Name");
};

GeometricShape::GeometricShape(const std::string& type, const std::string& name):
	_sType(type), _sName(name)
{
	_Status = new DOStatus("No Name");
};

void GeometricShape::operator = (const GeometricShape& gs)
{
	_sType   = gs.GetType();
	_sName   = gs.GetName();
	_ListGS  = gs.GetListGS();
	*_Status = *(gs.GetStatus());
};

GeometricShape::GeometricShape(const GeometricShape& gs)
{
	*this = gs;
};


GeometricShape::~GeometricShape()
{
	std::list<GeometricShape*>::iterator _lgsp;
	for (_lgsp=_ListGS.begin(); _lgsp!=_ListGS.end(); _lgsp++)
	{
		delete *_lgsp;
	}
	delete _Status;
};

void GeometricShape::Add(GeometricShape* gs)
{
	_ListGS.push_back(gs);
};

void GeometricShape::Remove(GeometricShape* gs)
{
	_ListGS.remove(gs);
};

GeometricShape* GeometricShape::GetGeometricShape(std::string& st)
{
	std::list<GeometricShape*>::iterator _lgsp;
	for (_lgsp=_ListGS.begin(); _lgsp!=_ListGS.end(); _lgsp++)
	{
		if ((*_lgsp)->GetName() == st)
		{
			return *_lgsp;
		};
	}
	return 0;
};


};   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GeometricShape& gs)
{
	std::cout << "Type: " << gs.GetType() << std::endl << gs.GetStatus() << std::endl;
	return os;
};
