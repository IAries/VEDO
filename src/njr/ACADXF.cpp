#include <njr/ACADXF.h>
#include <aries/utility/Utility.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <functional>

namespace njrdxf
{

Group::Group(const Code& code): _code(code)
{
	_length = 0;
}

Group::~Group()
{
	if(_length != 0)
	{
		delete _value;
	}
}

bool Group::SetValue(const void* value)
{
	if (_length != 0)
	{
		delete _value;
	}

	if (_code >= 0 && _code <= 9)
	{
		_length = (_uint_t)(strlen((const char*)value)) + 2;
	}
	else if ( (_code >= 10) && (_code <= 59) )
	{
	   _length = 9 ;
	}
	else if ( (_code >= 60) && (_code <= 79) )
	{
	   _length = 3 ;
	}
	else if ( (_code >= 90) && (_code <= 99) )
	{
	   _length = 5 ;
	}
	else
	{
		return false;
	}

	_value    = new char[_length];
	_value[0] = _code;
	std::memcpy(_value+1, value, _length-1);

	return true;
}

// member function of class  Entitity
Entitity::Entitity(): lconGroup(0)
{
	Entitity::AddGroup(type);
	Entitity::AddGroup(layer);
	Entitity::AddGroup(color);
	Entitity::AddGroup(handle);
}

Entitity::~Entitity()
{
	std::list< Group *>::iterator iGroup;
	for	(iGroup=lconGroup.begin(); iGroup!=lconGroup.end(); ++iGroup)
	{
		delete *iGroup;
	}
}



class CodeEQ
{
public:

	explicit CodeEQ(const Code key): code(key)
	{
	}

	bool operator () (const  Group* group)
	{
		return (group->code() == code);
	}

private:

	const Code code;
};



bool Entitity::AddGroup(const Code& code)
{
	std::list< Group* >::iterator iGroup = std::find_if(lconGroup.begin(), lconGroup.end(), njrdxf::CodeEQ(code));

	if (iGroup != lconGroup.end())
	{
		return false;
	}

	lconGroup.push_back(new Group(code));

	return true;
}

bool  Entitity::SetValue(const Code& code, const void* value)
{
	std::list< Group* >::iterator iGroup = std::find_if(lconGroup.begin(), lconGroup.end(), njrdxf::CodeEQ(code));

	if (iGroup == lconGroup.end())
	{
		return false;
	}

	(*iGroup)->SetValue(value);

	return true;
}

void Entitity::SetHandle(const _uint_t& value)
{
	char hand[32];
	std::sprintf(hand, "%X", value);
	SetValue(handle, hand);
}

void Entitity::SetColor(const Color& value)
{
	_int_t val = (_int_t) value;
	SetValue(color,&val);
}

void Entitity::SetLayer(const char* value)
{
	SetValue(layer, value);
}

const std::list< Group* >& Entitity::GetGroupContainer() const
{
	return lconGroup;
}

Line:: Line()
{
	Entitity::SetValue(type, "line");
	Entitity::AddGroup(px0);
	Entitity::AddGroup(py0);
	Entitity::AddGroup(pz0);
	Entitity::AddGroup(px1);
	Entitity::AddGroup(py1);
	Entitity::AddGroup(pz1);
}

void Line::Set(const Vector3df& p1, const Vector3df& p2, const char* layer, const Color& color)
{
	Line::SetPoint1(p1);
	Line::SetPoint2(p2);
	Entitity::SetColor(color);
	Entitity::SetLayer(layer);
}

void Line::SetPoint1(const Vector3df& point)
{
	_float_t p[3] = { point.x(), point.y(), point.z() };
	Entitity::SetValue(px0, p);
	Entitity::SetValue(py0, p+1);
	Entitity::SetValue(pz0, p+2);
}

void Line::SetPoint2(const Vector3df& point)
{
	_float_t p[3] = { point.x(), point.y(), point.z() };
	Entitity::SetValue(px1, p);
	Entitity::SetValue(py1, p+1);
	Entitity::SetValue(pz1, p+2);
}

Face::Face()
{
	Entitity::SetValue(type, "3DFace");
	Entitity::AddGroup(px0);
	Entitity::AddGroup(py0);
	Entitity::AddGroup(pz0);
	Entitity::AddGroup(px1);
	Entitity::AddGroup(py1);
	Entitity::AddGroup(pz1);
	Entitity::AddGroup(px2);
	Entitity::AddGroup(py2);
	Entitity::AddGroup(pz2);
	Entitity::AddGroup(px3);
	Entitity::AddGroup(py3);
	Entitity::AddGroup(pz3);
}

void Face::Set
	(const Vector3df& p1, const Vector3df& p2, const Vector3df& p3, const Vector3df& p4, const char* layer, const Color& color)
{
	Face::SetPoint1(p1);
	Face::SetPoint2(p2);
	Face::SetPoint3(p3);
	Face::SetPoint4(p4);
	Entitity::SetColor(color);
	Entitity::SetLayer(layer);
}

void Face::SetPoint1(const Vector3df& point)
{
	_float_t p[3] = {point.x(), point.y(), point.z()};
	Entitity::SetValue(px0, p);
	Entitity::SetValue(py0, p+1);
	Entitity::SetValue(pz0, p+2);
}

void Face::SetPoint2(const Vector3df& point)
{
	_float_t p[3] = {point.x(), point.y(), point.z()};
	Entitity::SetValue(px1, p);
	Entitity::SetValue(py1, p+1);
	Entitity::SetValue(pz1, p+2);
}

void Face::SetPoint3(const Vector3df& point)
{
	_float_t p[3] = {point.x(), point.y(), point.z()};
	Entitity::SetValue(px2, p);
	Entitity::SetValue(py2, p+1);
	Entitity::SetValue(pz2, p+2);
}

void Face::SetPoint4(const Vector3df& point)
{
	_float_t p[3] = {point.x(), point.y(), point.z()};
	Entitity::SetValue(px3, p);
	Entitity::SetValue(py3, p+1);
	Entitity::SetValue(pz3, p+2);
}

Text:: Text()
{
	 Entitity::SetValue(type, "text");
	 Entitity::AddGroup(px0);
	 Entitity::AddGroup(py0);
	 Entitity::AddGroup(pz0);
	 Entitity::AddGroup(pheight);
	 Entitity::AddGroup(pangle);
	 Entitity::AddGroup(ptext);
}

void Text::Set(const Vector3df& p1, const char* text, const _float_t& dheight, const _float_t& dangle, const char* layer, const Color& color)
{
	 Text::SetPoint(p1);
	 Text::SetText(text, dheight, dangle);
	 Entitity::SetColor(color);
	 Entitity::SetLayer(layer);
}

void Text::SetPoint(const Vector3df& point)
{
	_float_t p[3] = { point.x(), point.y(), point.z() };
	Entitity::SetValue(px0, p);
	Entitity::SetValue(py0, p+1);
	Entitity::SetValue(pz0, p+2);
}

void Text::SetText(const char* t, const _float_t& h, const _float_t& a)
{
	_float_t p[2] = {h, a};
	Entitity::SetValue(pheight, p);
	Entitity::SetValue(pangle, p+1);
	Entitity::SetValue(ptext, t);
}

ofstream::ofstream(const char* filename)
{
	ofstream::open(filename);
}

void ofstream::open(const char* filename)
{
	bdxf.open(filename, std::ios::out | std::ios::binary);

	if (!bdxf.is_open())
	{
		std::cout
			<< "Error!! Code: acadxf.cpp" << std::endl
			<< "        Note: Open dxf file \'" << filename << "\' error" << std::endl;
		std::exit(0);
	}

	bdxf.write("AutoCAD Binary DXF\r\n\032\000", 22);
	bdxf.write("\000SECTION\000", 9);
	bdxf.write("\002ENTITIES\000", 10);
	hand = 0;
}

ofstream::~ofstream()
{
	if (bdxf.bad())
	{
		std::cout
			<< "Error!! Code: acadxf.cpp" << std::endl
			<< "        Note: Cannot write dxf file" << std::endl;
		std::exit(0);
	}
	else
	{
		bdxf.write("\000ENDSEC\000", 8);
		bdxf.write("\000EOF\000", 5);
	}
	bdxf.close();
}

ofstream& njrdxf::ofstream::operator << (Entitity* entitity)
{
	entitity->SetHandle(hand++);

	const std::list< Group*> lconGroup = entitity->GetGroupContainer();

	std::list< Group* >::const_iterator ciGroup;

	for (ciGroup=lconGroup.begin(); ciGroup!=lconGroup.end(); ++ciGroup)
	{
		if ((*ciGroup)->length() != 0)
		{
			bdxf.write ((char*)(*ciGroup)->data(), (*ciGroup)->length());
		}
		else
		{
			std::cout
				<< "Error!! Code: acadxf.cpp" << std::endl
				<< "        Note: You forgot to set attributes of entitity" << std::endl;
			std::exit(0);
		}
	}
	return *this;
}

} // njrdxf namespace
