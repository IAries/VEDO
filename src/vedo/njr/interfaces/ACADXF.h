// Header file for AutoCAD DXF file (2001-2004 by C.T. Jitin Yang)

#ifndef _NJR_ACADXF_H
#define _NJR_ACADXF_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/constants/interfaces/Constants.h>
#include <fstream>
#include <iostream>
#include <list>

namespace njrdxf
{

// NJRDXFGroup is used to hold AutoCAD Group of binary DXF file AutoDXF Group includes code and values.
// Reference: AutoCAD R13 DXF help file
class Group
{

public:

	// User must give this NJRDXFCode that its own Group_code
	Group(const Code& code);

	// Destructor
	~Group() ;

	// Retrieves _length
	inline vedo::vedo_uint_t length() const
	{
		return _length;
	}

	// Retrieves _value
	inline const char* data() const
	{
		return _value;
	}

	// Retrieve its own GRoup_code of thie DXFGroup
	inline Code code() const
	{
		return _code;
	}

	// 1. Retrieves false if the Group_code outs of range
	// 2. else set the value of this DXFGroup and Retrieve true
	bool SetValue(const void* value);

private:

	// These member functions) are not allowable to access
	Group(const Group& group);

	const Group& operator = (const Group&);

	// Code of this DXFGroup
	const Code _code;

	// The pointer points to the binary data of this DXFGroup
	char* _value;

	// The length (bites) of the binary data of this DXFGroup
	vedo::vedo_uint_t _length;

};



// NJRDXFEntitity is base class of all concrete classes of DXF Entitity
class Entitity
{

public:

	// Adds essential common DXFGroups of this entitity. e.g. type, layer, color, handle
	Entitity();

	// Cleans all DXFGroups in (the container of DXFGRoups)
	~Entitity();

	// Sets layer of this Entitity
	void SetHandle(const vedo::vedo_uint_t& value);

	// Sets layer of this Entitity
	void SetColor(const Color& value);

	// Sets layer of this Entitity
	void SetLayer(const char* value);

	// Retrieves the Container of DXFGroups
	const std::list<Group *>& GetGroupContainer() const;

protected:

	/********************************************************************************
	 * Adds one essential DXFGroup of this entitity whoes Group_code equals to vc
	 *
	 * 1.  if the new Group_code can't find in the lconGroup (container of DXFGroupS)
	 * 1.a Adds one essential DXFGroup of this entitity
	 * 1.b whoes Group_code equals to vc
	 * 1.c Retrieves true
	 * 2.  Retrieves false if the new Gruop_cone has existed
	 ********************************************************************************/
	bool AddGroup(const Code& vc);

	// Sets the value of the DXFGroup whoes Group_code equals to vc
	bool SetValue(const Code& vc, const void* value);

private:

	// (These member functions) are not allowable to access
	Entitity(const Entitity&);

	const Entitity& operator = (const Entitity&);

	// (Container of DXFGroup) is used to hold all DXFGroups of this entitity
	std::list<Group *> lconGroup;

};



// NJRDXFLine is used to hold all essential DXFGroups of AutoCAD line Entitity
class Line: public Entitity
{

public:

	// Adds essential DXFGroups of line Entitity to lconGroup (member of base class)
	Line() ;

	// Sets attributes of this Entitity
	void Set
		(const njr::Vector3d& p1, const njr::Vector3d& p2, const char* layer = "NJRDXFdefault", const Color& color = bylayer);

	// Set two endpoints of this line
	void SetPoint1(const njr::Vector3d& point);

	void SetPoint2(const njr::Vector3d& point);

private:

	// (These member functions) are not allowable to access
	Line(const Line&);

	const Line& operator = (const Line&);

};



// NJRDXFFace is used to hold all essential DXFGroups of AutoCAD Face Entitity
class Face: public Entitity
{

public:

	/**************************************************************************
	 * Adds essential DXFGroups of Face Entitity to lconGroup (member of base
	 * class)
	 **************************************************************************/
	Face() ;

	// Sets attributes of this Entititty
	void Set
		(const njr::Vector3d& p1, const njr::Vector3d& p2, const njr::Vector3d& p3, const njr::Vector3d& p4,
		 const char* layer = "NJRDXFdefault", const Color& color = bylayer);

	// Sets four corners of this 3d face
	void SetPoint1(const njr::Vector3d &point);

	void SetPoint2(const njr::Vector3d &point);

	void SetPoint3(const njr::Vector3d &point);

	void SetPoint4(const njr::Vector3d &point);

private:

	// These member functions are not allowable to access
	Face(const Face&);

	const Face& operator = (const Face&);

};



// NJRDXFText is used to hold all essential DXFGroups of AutoCAD Text Entitity
class Text: public Entitity
{

public:

	/**************************************************************************
	 * Adds essential DXFGroups of Text Entitity to lconGroup (member of base
	 * class)
	 **************************************************************************/
	Text() ;

	// Sets attributes of this Entitity
	void Set
		(const njr::Vector3d& p1, const char* text, const vedo::vedo_float_t& dheight=2.0, const vedo::vedo_float_t& dangle=0.0,
		 const char* layer = "NJRDXFdefault", const Color& color = bylayer                                                      );

	// Sets the position of this Text
	void SetPoint(const njr::Vector3d& point);

	// Sets the size , angle, and content of this Text
	void SetText(const char* text, const vedo::vedo_float_t& dheight = 2.0, const vedo::vedo_float_t& dangle = 0.0);

private:

	// These member functions that are not allowable to access
	Text (const Text&);

	const Text& operator = (const Text&);

};



// ofstream is AutoCAD Binary DXF File stream
class ofstream
{

public:

	ofstream()
	{
	}

	/**************************************************************************
	 * Constructor
	 * 1. Sets hand be 0
	 * 2. Opens this dxf file stream
	 * 3. Output the header of autocad dxf file to this file stream
     **************************************************************************/
	ofstream(const char* filename);

	/**************************************************************************
	 * Destructor
	 * 1. Outputs the end of autocad dxf file to this file stream
	 * 2. Closes file stream
     **************************************************************************/
	virtual ~ofstream() ;

	/**************************************************************************
	 * 1. Sets hand be 0
	 * 2. Opens this dxf file stream
	 * 3. Output the header of DXF file to this file
     **************************************************************************/
	void open(const char* filename);

	/**************************************************************************
	 * 1. Sets the handle (handle = hand++) of entitity
	 * 2. Outputs all DXFGroup belongs to the input entitity to this autoCAD
	 *    dxf file
     **************************************************************************/
	ofstream& operator << (Entitity* entitity);

private:

	// ofsteram of AutoCAD dxf file
	std::ofstream bdxf;

	// The number of entitity that has been output to this dxf file
	vedo::vedo_uint_t hand;

};

} // namespace njrdxf

#endif // _NJR_ACADXF_H
