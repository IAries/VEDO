/******************************************************************************
 * acadxf.h  header file for AutoCAD DXF file
 *
 * version 1.2
 *
 * History:
 *    Ver 1.0 Created 2001 Aug  by  C.T. Jitin Yang
 *    Ver 1.1 Modified 2003 Feb by  C.T. Jitin Yang
 *    Ver 1.2 Modifined 2003 July by C.T. Jitin Yang
 *    Ver 1.3 Modifined 2004 Jan 25 by C.T. Jitin Yang
 *       For CAE summer traning course
 *       (C++ Programming Langurage)
 ******************************************************************************/

#ifndef _NJRACADXF_H
#define _NJRACADXF_H

#include <NJR/Interfaces/vector3d.h>
#include <fstream>
#include <iostream>
#include <list>

namespace NJRDXF
{

enum Color
{
	bylayer	= 256,
	byblock	=   0,
	red	    =   1,
	yellow	=   2,
	green   =   3,
	cyan    =   4,
	blue    =   5,
	magenta =   6
};

enum Code
{
	type     =  0,
	layer    =  8,
	color    = 62,
	handle   =  5,
	keyword  =  0,
	secname  =  2,
	pheight  = 40,
	pangle   = 50,
	ptext    =  1,
	px0      = 10,
	py0      = 20,
	pz0      = 30,
	px1      = 11,
	py1      = 21,
	pz1      = 31,
	px2      = 12,
	py2      = 22,
	pz2      = 32,
	px3      = 13,
	py3      = 23,
	pz3      = 33,
	submaker = 100
};

class Group;
class Entitity;
class Line;
class Face;
class Text;
class ofstream;
class CodeEQ;

}; // NJRDXF namespace

/******************************************************************************
 * NJRDXFGroup is used to hold AutoCAD Group of binary DXF file AutoDXF Group
 * includes code and values.
 *
 * Reference: AutoCAD R13 DXF help file
 ******************************************************************************/

class NJRDXF::Group
{

public:

	// User must give this NJRDXFCode that its own Group_code
	Group(const NJRDXF::Code& code);

	// Destructor
	~Group() ;

	// Retrieves _length
	inline unsigned int length() const
	{
		return _length;
	};

	// Retrieves _value
	inline const char* data() const
	{
		return _value;
	};

	// Retrieve its own GRoup_code of thie DXFGroup
	inline NJRDXF::Code code() const
	{
		return _code;
	};

	/**************************************************************************
	 * 1. Retrieves false if the Group_code outs of range
	 * 2. else set the value of this DXFGroup and Retrieve true
	 **************************************************************************/
	bool SetValue(const void* value);

private:

	// These member functions) are not allowable to access
	Group(const Group& group);

	const Group& operator = (const Group&);

	// Code of this DXFGroup
	const NJRDXF::Code _code;

	// The pointer points to the binary data of this DXFGroup
	char* _value;

	// The length (bites) of the binary data of this DXFGroup
	unsigned int _length;

};



// NJRDXFEntitity is base class of all concrete classes of DXF Entitity
class NJRDXF::Entitity
{

public:

	/**************************************************************************
	 * Adds essential common DXFGroups of this entitity.
	 * e.g. type, layer, color, handle
	 **************************************************************************/
	Entitity();

	// Cleans all DXFGroups in (the container of DXFGRoups)
	~Entitity();

	// Sets layer of this Entitity
	void SetHandle(const long& value);

	// Sets layer of this Entitity
	void SetColor(const NJRDXF::Color& value);

	// Sets layer of this Entitity
	void SetLayer(const char* value);

	// Retrieves the Container of DXFGroups
	const std::list<NJRDXF::Group *>& GetGroupContainer() const;

protected:

	/**************************************************************************
	 * Adds one essential DXFGroup of this entitity whoes Group_code equals to
	 * vc
	 *
	 * 1.  if the new Group_code can't find in the lconGroup (container of
	 *     DXFGroupS)
	 * 1.a Adds one essential DXFGroup of this entitity
	 * 1.b whoes Group_code equals to vc
	 * 1.c Retrieves true
	 * 2.  Retrieves false if the new Gruop_cone has existed
	 **************************************************************************/
	bool AddGroup(const NJRDXF::Code& vc);

	// Sets the value of the DXFGroup whoes Group_code equals to vc
	bool SetValue(const NJRDXF::Code& vc, const void* value);

private:

	// (These member functions) are not allowable to access
	Entitity(const Entitity&);

	const Entitity& operator = (const Entitity&);

	// (Container of DXFGroup) is used to hold all DXFGroups of this entitity
	std::list<NJRDXF::Group *> lconGroup;

};



// NJRDXFLine is used to hold all essential DXFGroups of AutoCAD line Entitity
class NJRDXF::Line : public NJRDXF::Entitity
{

public:

	/**************************************************************************
	 * Adds essential DXFGroups of line Entitity to lconGroup (member of base
	 * class)
	 **************************************************************************/
	Line() ;

	// Sets attributes of this Entitity
	void Set
		(const NJRvector3d& p1,
		const NJRvector3d& p2,
		const char* layer = "NJRDXFdefault",
		const NJRDXF::Color& color = NJRDXF::bylayer);

	// Set two endpoints of this line
	void SetPoint1(const NJRvector3d& point);

	void SetPoint2(const NJRvector3d& point);

private:

	// (These member functions) are not allowable to access
	Line(const Line&);

	const Line& operator = (const Line&);

};



// NJRDXFFace is used to hold all essential DXFGroups of AutoCAD Face Entitity
class NJRDXF::Face: public NJRDXF::Entitity
{

public:

	/**************************************************************************
	 * Adds essential DXFGroups of Face Entitity to lconGroup (member of base
	 * class)
	 **************************************************************************/
	Face() ;

	// Sets attributes of this Entititty
	void Set
		(const NJRvector3d& p1,
		const NJRvector3d& p2,
		const NJRvector3d& p3,
		const NJRvector3d& p4,
		const char* layer = "NJRDXFdefault",
		const NJRDXF::Color& color = NJRDXF::bylayer);

	// Sets four corners of this 3d face
	void SetPoint1(const NJRvector3d &point);

	void SetPoint2(const NJRvector3d &point);

	void SetPoint3(const NJRvector3d &point);

	void SetPoint4(const NJRvector3d &point);

private:

	// These member functions are not allowable to access
	Face(const Face&);

	const Face& operator = (const Face&);

};

// NJRDXFText is used to hold all essential DXFGroups of AutoCAD Text Entitity
class NJRDXF::Text: public NJRDXF::Entitity
{

public:

	/**************************************************************************
	 * Adds essential DXFGroups of Text Entitity to lconGroup (member of base
	 * class)
	 **************************************************************************/
	Text() ;

	// Sets attributes of this Entitity
	void Set
		(const NJRvector3d& p1,
		const char* text,
		const double& dheight = 2.0,
		const double& dangle=0.0,
		const char* layer = "NJRDXFdefault",
		const NJRDXF::Color& color = NJRDXF::bylayer);

	// Sets the position of this Text
	void SetPoint(const NJRvector3d& point);

	// Sets the size , angle, and content of this Text
	void SetText
		(const char* text,
		const double& dheight = 2.0,
		const double& dangle = 0.0) ;

private:

	// These member functions that are not allowable to access
	Text (const Text&);

	const Text& operator = (const Text&);

};

// ofstream is AutoCAD Binary DXF File stream

class NJRDXF::ofstream
{

public:

	ofstream()
	{
	};

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
	ofstream& operator << (NJRDXF::Entitity* entitity);

private:

	// ofsteram of AutoCAD dxf file
	std::ofstream bdxf;

	// The number of entitity that has been output to this dxf file
	unsigned long hand;

};

#endif // _NJRACADXF_H
